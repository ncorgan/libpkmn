/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "conversions_common.hpp"
#include "gb_conversions.hpp"
#include "../database/database_common.hpp"

#include <pkmn/calculations/stats.hpp>

#include <pksav/common/stats.h>
#include <pksav/math/endian.h>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <stdexcept>

namespace pkmn { namespace conversions {

    static pkmn::database::sptr _db;

    void gen1_pc_pokemon_to_gen2(
        const pksav_gen1_pc_pokemon_t* from,
        pksav_gen2_pc_pokemon_t* to
    )
    {
        // Connect to database
        pkmn::database::get_connection(_db);

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
    )
    {
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
    )
    {
        // Connect to database
        pkmn::database::get_connection(_db);

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
               _db, type2_query, dummy_type2,
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
        const pksav_gen2_party_pokemon_t* from,
        pksav_gen1_party_pokemon_t* to
    )
    {
        // PC data
        gen2_pc_pokemon_to_gen1(
            &from->pc,
            &to->pc
        );

        // Party data
        std::memset(&to->party_data, 0, sizeof(to->party_data));
        to->party_data.level = from->pc.level;
        to->pc.condition = from->party_data.condition;
        // Leave unused field at 0
        to->pc.current_hp = from->party_data.current_hp;
        // The next four fields are identical
        memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 12);
        // TODO: how does Sp.Atk,Sp.Def translate to Special?
    }

}}
