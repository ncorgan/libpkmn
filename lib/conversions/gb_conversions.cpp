/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "conversions_common.hpp"
#include "gb_conversions.hpp"
#include "database/database_common.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/calculations/stats.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <pksav/common/stats.h>
#include <pksav/gen2/time.h>
#include <pksav/math/endian.h>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>

namespace pkmn { namespace conversions {

    /*
     * Between Generations I-II, the Special stat was separated into
     * Special Attack and Special Defense. This vector tracks if the
     * Special stat matches the Special Attack stat.
     */
    static const std::vector<int> gen1_to_gen2_with_spcl_as_spatk =
    {
        1,2,3, // Bulbasaur-Venusaur
        7,8,9, // Squirtle-Blastoise
        10,11,12, // Caterpie-Butterfree
        13,14, // Weedle-Kakuna
        16,17,18, // Pidgey-Pidgeot
        19,20, // Rattata-Raticate
        21,22, // Spearow-Fearow
        23,24, // Ekans-Arbok
        25,26, // Pikachu-Raichu
        29,30,31, // Nidoran♀-Nidoqueen
        32,33, // Nidoran♂-Nidorino
        35,36, // Clefairy-Clefable
        43,44,45, // Oddish-Vileplume
        48,49, // Venonat-Venomoth
        52,53, // Meowth-Persian
        56,57, // Mankey-Primeape
        60,61,62, // Poliwag-Poliwrath
        63,64,65, // Abra-Alakazam
        66,67,68, // Machop-Machamp
        69,70,71, // Bellsprout-Victreebel
        74,75,76, // Geodude-Golem
        77,78, // Ponyta-Rapidash
        81,82, // Magnemite-Magneton
        83, // Farfetch'd
        84,85, // Doduo-Dodrio
        88,89, // Grimer-Muk
        90,91, // Shellder-Cloyster
        92,93,94, // Gastly-Gengar
        95, // Onix
        98,99, // Krabby-Kingler
        100,101, // Voltorb-Electrode
        102,103, // Exeggcute-Exeggutor
        104,105, // Cubone-Marowak
        106, // Hitmonlee
        107, // Hitmonchan
        108, // Lickitung
        109,110, // Koffing-Weezing
        111,112, // Rhyhorn-Rhydon
        114, // Tangela
        115, // Kangaskhan
        116,117, // Horsea-Seadra
        120,121, // Staryu-Starmie
        122, // Mr. Mime
        123, // Scyther
        127, // Pinsir
        132, // Ditto
        134,135, // Vaporeon,Jolteon
        138,139, // Omanyte,Omastar
        142, // Aerodactyl
        143, // Snorlax
        145, // Zapdos
        146, // Moltres
        147,148,149, // Dratini-Dragonite
        150, // Mewtwo
        151 // Mew
    };

    static inline bool does_spcl_match_spatk(int species_id)
    {
        return (std::find(
                    gen1_to_gen2_with_spcl_as_spatk.begin(),
                    gen1_to_gen2_with_spcl_as_spatk.end(),
                    species_id
               ) == gen1_to_gen2_with_spcl_as_spatk.end());
    }

    void gen1_pc_pokemon_to_gen2(
        const struct pksav_gen1_pc_pokemon* from,
        struct pksav_gen2_pc_pokemon* to
    )
    {
        std::memset(to, 0, sizeof(*to));
        to->species = uint8_t(convert_pokemon_game_index(
                          from->species,
                          1, 4
                      ));
        /*
         * The rest of the Gen I fields are laid out identically.
         * Gen I's catch rate corresponds to Gen II's held item.
         */
        std::memcpy(&to->held_item, &from->catch_rate, 26);
        to->friendship = uint8_t(pokemon_index_to_base_friendship(
                             to->species, 4
                         ));
        // Leave Pokérus field at 0

        /*
         * Leave location caught at 0, Crystal's Poké Seer can't tell where
         * traded Pokémon were caught.
         */
        to->caught_data = (to->caught_data & ~PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_MASK)
                        | (uint16_t(from->level) << PKSAV_GEN2_POKEMON_LEVEL_CAUGHT_OFFSET);

        time_t now = std::time(0);
        PKSAV_CALL(
            pksav_gen2_set_caught_data_time_field(
                &now,
                &to->caught_data
            )
        )

        to->level = from->level;
    }

    void gen1_party_pokemon_to_gen2(
        const struct pksav_gen1_party_pokemon* from,
        struct pksav_gen2_party_pokemon* to
    )
    {
        // PC data
        gen1_pc_pokemon_to_gen2(
            &from->pc_data,
            &to->pc_data
        );

        // Party data
        std::memset(&to->party_data, 0, sizeof(to->party_data));
        to->party_data.condition = from->pc_data.condition;

        // Leave unused field at 0

        to->party_data.current_hp = from->pc_data.current_hp;

        // The next four fields are identical
        std::memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 12);

        // In Generation II, the game index matches the species ID.
        if(does_spcl_match_spatk(to->pc_data.species))
        {
            to->party_data.spatk = from->party_data.spcl;
        }
        else
        {
            to->party_data.spdef = from->party_data.spcl;
        }
    }

    void gen2_pc_pokemon_to_gen1(
        const struct pksav_gen2_pc_pokemon* from,
        struct pksav_gen1_pc_pokemon* to
    )
    {
        const int RED = 1;

        std::memset(to, 0, sizeof(*to));
        to->species = uint8_t(convert_pokemon_game_index(
                          from->species,
                          4, 1
                      ));

        // Current HP

        static BOOST_CONSTEXPR const char* hp_stat_query = \
            "SELECT base_stat FROM pokemon_stats WHERE pokemon_id="
            "(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "game_index=? AND version_id=1) AND stat_id=1";

        int hp_stat = pkmn::database::query_db_bind1<int, int>(
                          hp_stat_query, from->species
                      );

        uint8_t IVs[PKSAV_NUM_GB_IVS] = {0};
        pksav_get_gb_IVs(
           &from->iv_data,
           IVs,
           sizeof(IVs)
        );
        to->current_hp = pksav_bigendian16(
                             pkmn::calculations::get_gb_stat(
                                 "HP", from->level, hp_stat,
                                 pksav_bigendian16(from->ev_hp),
                                 IVs[PKSAV_GB_IV_HP]
                             )
                         );

        // Keep status field at 0

        std::pair<std::string, std::string> types = pkmn::database::pokemon_entry(
                                                        to->species, RED
                                                    ).get_types();
        to->types[0] = uint8_t(pksav::get_gen1_type_bimap().left.at(types.first));
        if(types.second == "None")
        {
            to->types[1] = to->types[0];
        }
        else
        {
            to->types[1] = uint8_t(pksav::get_gen1_type_bimap().left.at(types.second));
        }

        static BOOST_CONSTEXPR const char* type1_query = \
            "SELECT game_index FROM type_game_indices WHERE "
            "generation_id=1 AND type_id=(SELECT type_id FROM "
            "pokemon_types WHERE pokemon_id=(SELECT pokemon_id "
            "FROM pokemon_game_indices WHERE version_id=4 AND "
            "game_index=?) AND slot=1)";

        to->types[0] = uint8_t(pkmn::database::query_db_bind1<int, int>(
                                   type1_query,
                                   from->species
                              ));

        static BOOST_CONSTEXPR const char* type2_query = \
            "SELECT game_index FROM type_game_indices WHERE "
            "generation_id=1 AND type_id=(SELECT type_id FROM "
            "pokemon_types WHERE pokemon_id=(SELECT pokemon_id "
            "FROM pokemon_game_indices WHERE version_id=4 AND "
            "game_index=?) AND slot=2)";

        int dummy_type2 = 0;
        if(pkmn::database::maybe_query_db_bind1<int, int>(
               type2_query, dummy_type2,
               from->species
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
        const struct pksav_gen2_party_pokemon* from,
        struct pksav_gen1_party_pokemon* to
    )
    {
        // PC data
        gen2_pc_pokemon_to_gen1(
            &from->pc_data,
            &to->pc_data
        );

        // Party data
        std::memset(&to->party_data, 0, sizeof(to->party_data));
        to->party_data.level = from->pc_data.level;
        to->pc_data.condition = from->party_data.condition;

        // Leave unused field at 0

        to->pc_data.current_hp = from->party_data.current_hp;

        // The next four fields are identical
        std::memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 12);

        // In Generation II, the game index matches the species ID.
        if(does_spcl_match_spatk(from->pc_data.species))
        {
            to->party_data.spcl = from->party_data.spatk;
        }
        else
        {
            to->party_data.spcl = from->party_data.spdef;
        }
    }

}}
