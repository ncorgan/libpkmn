/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "conversions.hpp"
#include "pksav_call.hpp"

#include "database/database_common.hpp"
#include "database/id_to_index.hpp"
#include "database/id_to_string.hpp"
#include "types/datetime_internal.hpp"

#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/stats.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/types/datetime.hpp>

#include <pksav/common/stats.h>
#include <pksav/gba/text.h>
#include <pksav/gen4/text.h>
#include <pksav/math/endian.h>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <stdexcept>

namespace pksav {

    pkmn::database::sptr _db;

    static int convert_item_game_index(
        int item_index,
        int from_generation,
        int to_generation
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM item_game_indices WHERE generation_id=? AND "
            "item_id=(SELECT item_id FROM item_game_indices WHERE game_index=? "
            "AND generation_id=?";

        return pkmn::database::query_db_bind3<int, int, int, int>(
                   _db, query, to_generation, item_index, from_generation
               );
    }

    static int convert_pokemon_game_index(
        int pokemon_index,
        int from_game_id,
        int to_game_id
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM pokemon_game_indices WHERE version_id=? "
            "AND pokemon_id=(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "game_index=? AND version_id=?)";

        return pkmn::database::query_db_bind3<int, int, int, int>(
                   _db, query, to_game_id, pokemon_index, from_game_id
               );
    }

    static int pokemon_index_to_base_friendship(
        int pokemon_index,
        int game_id
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT base_friendship FROM pokemon_species WHERE id="
            "(SELECT species_id FROM pokemon WHERE id="
            "(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "version_id=? AND game_index=?))";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, game_id, pokemon_index
               );
    }

    void gen1_pc_pokemon_to_gen2(
        const pksav_gen1_pc_pokemon_t* from,
        pksav_gen2_pc_pokemon_t* to
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        std::memset(to, 0, sizeof(*to));
        to->species = uint8_t(convert_pokemon_game_index(
                          pksav_bigendian16(from->species),
                          1, 4
                      ));
        /*
         * The rest of the Gen I fields are laid out identically.
         * Gen I's catch rate corresponds to Gen II's held item.
         */
        std::memcpy(&to->held_item, &from->catch_rate, 26);
        to->friendship = uint8_t(pokemon_index_to_base_friendship(
                             pksav_bigendian16(to->species), 4
                         ));
        // Leave Pokérus field at 0

        /*
         * Leave location caught at 0, Crystal's Poké Seer can't tell where
         * traded Pokémon were caught.
         *
         * TODO: time of day, fix PKSav mask names
         */
        to->caught_data = (to->caught_data & ~PKSAV_GEN2_LEVEL_CAUGHT_MASK)
                        | (from->level << PKSAV_GEN2_LEVEL_CAUGHT_OFFSET);

        to->level = from->level;
    }

    void gen1_party_pokemon_to_gen2(
        const pksav_gen1_party_pokemon_t* from,
        pksav_gen2_party_pokemon_t* to
    ) {
        // PC data
        gen1_pc_pokemon_to_gen2(
            &from->pc,
            &to->pc
        );

        // Party data
        std::memset(&to->party_data, 0, sizeof(to->party_data));
        to->party_data.condition = from->pc.condition;
        // Leave unused field at 0
        to->party_data.current_hp = from->pc.current_hp;
        // The next four fields are identical
        memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 12);
        // TODO: how does Special translate to Sp.Atk, Sp.Def?
    }

    void gen2_pc_pokemon_to_gen1(
        const pksav_gen2_pc_pokemon_t* from,
        pksav_gen1_pc_pokemon_t* to
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        std::memset(to, 0, sizeof(*to));
        to->species = uint8_t(convert_pokemon_game_index(
                          pksav_bigendian16(from->species),
                          4, 1
                      ));

        // Current HP

        static BOOST_CONSTEXPR const char* hp_stat_query = \
            "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
            "(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "game_index=? AND version_id=1) AND stat_id=1";

        int hp_stat = pkmn::database::query_db_bind1<int, int>(
                          _db, hp_stat_query, from->species
                      );

        uint8_t IV = 0;
        pksav_get_gb_IV(
           const_cast<uint16_t*>(&from->iv_data), // PKSav TODO: make this param a const ptr
           PKSAV_STAT_HP,
           &IV
        );
        to->current_hp = pksav_bigendian16(
                             pkmn::calculations::get_gb_stat(
                                 "HP", from->level, hp_stat,
                                 pksav_bigendian16(from->ev_hp), IV
                             )
                         );

        // Keep status field at 0

        // TODO: make sure we use Generation I types

        static BOOST_CONSTEXPR const char* type1_query = \
            "SELECT game_index FROM type_game_indices WHERE "
            "generation_id=1 AND type_id=(SELECT type_id FROM "
            "pokemon_types WHERE pokemon_id=(SELECT pokemon_id "
            "FROM pokemon_game_indices WHERE version_id=4 AND "
            "game_index=?) AND slot=1)";

        to->types[0] = uint8_t(pkmn::database::query_db_bind1<int, int>(
                                   _db, type1_query,
                                   pksav_bigendian16(from->species)
                              ));

        static BOOST_CONSTEXPR const char* type2_query = \
            "SELECT game_index FROM type_game_indices WHERE "
            "generation_id=1 AND type_id=(SELECT type_id FROM "
            "pokemon_types WHERE pokemon_id=(SELECT pokemon_id "
            "FROM pokemon_game_indices WHERE version_id=4 AND "
            "game_index=?) AND slot=2)";

        int dummy_type2 = 0;
        if(pkmn::database::maybe_query_db_bind1<int, int>(
               _db, type2_query, dummy_type2,
               pksav_bigendian16(from->species)
           ))
        {
            to->types[1] = uint8_t(dummy_type2);
        } else {
            // If a Pokémon has one type, both indices match
            to->types[1] = to->types[0];
        }

        /*
         * The rest of the Gen I fields are laid out identically.
         * Gen I's catch rate corresponds to Gen II's held item.
         */
        std::memcpy(&to->catch_rate, &from->held_item, 26);
    }

    void gen2_party_pokemon_to_gen1(
        const pksav_gen2_party_pokemon_t* from,
        pksav_gen1_party_pokemon_t* to
    ) {
        // PC data
        gen2_pc_pokemon_to_gen1(
            &from->pc,
            &to->pc
        );
        // TODO: party data
    }

    BOOST_STATIC_CONSTEXPR int FIRERED = 10;
    BOOST_STATIC_CONSTEXPR int LEAFGREEN = 11;

    BOOST_STATIC_CONSTEXPR int PLATINUM = 14;
    BOOST_STATIC_CONSTEXPR int HEARTGOLD = 15;
    BOOST_STATIC_CONSTEXPR int SOULSILVER = 16;

    static const uint16_t GEN4_KANTO = pksav_littleendian16(0x07D3);
    static const uint16_t GEN4_HOENN = pksav_littleendian16(0x07D5);

    void gba_pc_pokemon_to_gen4(
        const pksav_gba_pc_pokemon_t* from,
        pksav_nds_pc_pokemon_t* to,
        int from_game_id,
        int to_game_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        pkmn::database::pokemon_entry gba_entry(
            pksav_littleendian16(uint16_t(from->personality)),
            from_game_id
        );
        pkmn::datetime now = pkmn::current_datetime();

        std::memset(to, 0, sizeof(*to));

        const pksav_gba_pokemon_growth_t* gba_growth = &from->blocks.growth;
        const pksav_gba_pokemon_attacks_t* gba_attacks = &from->blocks.attacks;
        const pksav_gba_pokemon_effort_t* gba_effort = &from->blocks.effort;
        const pksav_gba_pokemon_misc_t* gba_misc = &from->blocks.misc;

        pksav_nds_pokemon_blockA_t* nds_blockA = &to->blocks.blockA;
        pksav_nds_pokemon_blockB_t* nds_blockB = &to->blocks.blockB;
        pksav_nds_pokemon_blockC_t* nds_blockC = &to->blocks.blockC;
        pksav_nds_pokemon_blockD_t* nds_blockD = &to->blocks.blockD;

        uint16_t gba_origin_game = (gba_misc->origin_info & PKSAV_GBA_ORIGIN_GAME_MASK) >> PKSAV_GBA_ORIGIN_GAME_OFFSET;
        uint16_t gba_ball = (gba_misc->origin_info & PKSAV_GBA_BALL_MASK) >> PKSAV_GBA_BALL_OFFSET;
        uint16_t gba_level_met = gba_misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK;

        to->personality = from->personality;
        to->isdecrypted_isegg |= PKSAV_NDS_PC_DATA_DECRYPTED_MASK;
        if(gba_misc->iv_egg_ability & PKSAV_GBA_EGG_MASK) {
            to->isdecrypted_isegg |= PKSAV_NDS_IS_EGG_MASK;
        }

        nds_blockA->species = pksav_littleendian16(uint16_t(convert_pokemon_game_index(
                                  pksav_littleendian16(gba_growth->species),
                                  pkmn::database::game_index_to_id(gba_origin_game),
                                  to_game_id
                              )));
        nds_blockA->held_item = pksav_littleendian16(uint16_t(convert_item_game_index(
                                    pksav_littleendian16(gba_growth->held_item),
                                    3, 4
                                )));
        nds_blockA->exp = gba_growth->exp;
        nds_blockA->ot_id = from->ot_id;
        nds_blockA->friendship = gba_growth->friendship;

        std::pair<std::string, std::string> abilities = gba_entry.get_abilities();
        if(abilities.second == "None") {
            nds_blockA->ability = uint8_t(pkmn::database::ability_name_to_id(abilities.first));
        } else {
            if(pksav_littleendian16(to->personality) % 2) {
                nds_blockA->ability = uint8_t(pkmn::database::ability_name_to_id(abilities.second));
            } else {
                nds_blockA->ability = uint8_t(pkmn::database::ability_name_to_id(abilities.first));
            }
        }

        nds_blockA->markings = from->markings;

        // Korea was stored differently between generations.
        if(pksav_littleendian16(from->language) == 0x0206) {
            nds_blockA->country = 0x8;
        } else {
            nds_blockA->country = uint8_t(pksav_littleendian16(from->language) - 0x0200);
        }

        // EVs and contest stats are stored with the same layout, so just copy them all at once.
        std::memcpy(&nds_blockA->ev_hp, gba_effort, sizeof(*gba_effort));

        // Moves and move PPs are stored with the same layout, so just copy them all at once.
        std::memcpy(&nds_blockB->moves, gba_attacks, sizeof(*gba_attacks));

        // PP Up usage is stored in a bitfield in GBA but in an array in NDS games.
        for(size_t i = 0; i < 4; ++i) {
            nds_blockB->move_pp_ups[i] = (gba_growth->pp_up & (0x2 << (2*i))) >> (0x2 >> (2*i));
        }

        /*
         * Bring over IVs, remove the GBA masks, and apply what NDS masks we can. We'll revisit this
         * for the nickname field.
         */
        nds_blockB->iv_isegg_isnicknamed = gba_misc->iv_egg_ability;
        nds_blockB->iv_isegg_isnicknamed &= ~PKSAV_GBA_ABILITY_MASK;
        if(nds_blockB->iv_isegg_isnicknamed & PKSAV_GBA_EGG_MASK) {
            nds_blockB->iv_isegg_isnicknamed &= ~PKSAV_GBA_EGG_MASK;
            nds_blockB->iv_isegg_isnicknamed |= PKSAV_NDS_ISEGG_MASK;
        }

        nds_blockB->hoenn_ribbons = gba_misc->ribbons_obedience;

        std::string gender = pkmn::calculations::modern_pokemon_gender(
                                 gba_entry.get_species(),
                                 pksav_littleendian32(to->personality)
                             );
        if(gender == "Female") {
            nds_blockB->form_encounterinfo |= PKSAV_NDS_FEMALE_MASK;
        } else if(gender == "Genderless") {
            nds_blockB->form_encounterinfo |= PKSAV_NDS_GENDERLESS_MASK;
        }

        if(to_game_id == PLATINUM) {
            if(from_game_id == FIRERED or from_game_id == LEAFGREEN) {
                nds_blockB->eggmet_plat = GEN4_KANTO;
                nds_blockB->met_plat = GEN4_KANTO;
            } else {
                nds_blockB->eggmet_plat = GEN4_HOENN;
                nds_blockB->met_plat = GEN4_HOENN;
            }
        }

        char nickname[10] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                from->nickname,
                nickname,
                10
            )
        );
        PKSAV_CALL(
            pksav_text_to_gen4(
                nickname,
                nds_blockC->nickname,
                10
            )
        );

        nds_blockC->hometown = uint8_t(gba_origin_game);

        char otname[7] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                from->otname,
                otname,
                7
            )
        );
        PKSAV_CALL(
            pksav_text_to_gen4(
                otname,
                nds_blockD->otname,
                7
            )
        );

        libpkmn_datetime_to_pksav_date(
            now,
            &nds_blockD->met_date
        );
        libpkmn_datetime_to_pksav_date(
            now,
            &nds_blockD->eggmet_date
        );

        if(to_game_id != PLATINUM) {
            if(from_game_id == FIRERED or from_game_id == LEAFGREEN) {
                nds_blockD->eggmet_dp = GEN4_KANTO;
                nds_blockD->met_dp = GEN4_KANTO;
            } else {
                nds_blockD->eggmet_dp = GEN4_HOENN;
                nds_blockD->met_dp = GEN4_HOENN;
            }
        }

        if(to_game_id == HEARTGOLD or to_game_id == SOULSILVER) {
            nds_blockD->ball_hgss = uint8_t(gba_ball);
        } else {
            nds_blockD->ball = uint8_t(gba_ball);
        }

        nds_blockD->metlevel_otgender = uint8_t(gba_level_met);
    }

    void gba_party_pokemon_to_gen4(
        const pksav_gba_party_pokemon_t* from,
        pksav_nds_party_pokemon_t* to,
        int from_game_id,
        int to_game_id
    ) {
        gba_pc_pokemon_to_gen4(
            &from->pc,
            &to->pc,
            from_game_id,
            to_game_id
        );

        to->party_data.level = from->party_data.level;
        std::memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 6*sizeof(uint16_t));
        to->party_data.current_hp = to->party_data.max_hp;
    }
}
