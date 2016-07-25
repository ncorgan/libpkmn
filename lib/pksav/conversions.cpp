/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "conversions.hpp"
#include "../database/database_common.hpp"

#include <pksav/math/endian.h>

#include <boost/config.hpp>

#include <cstring>

namespace pksav {

    pkmn::database::sptr _db;

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

    static int pokemon_index_to_base_happiness(
        int pokemon_index,
        int game_id
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT base_happiness FROM pokemon_species WHERE id="
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
        to->species = pksav_bigendian16(
                          uint16_t(convert_pokemon_game_index(
                              pksav_bigendian16(from->species),
                              1, 4
                          ))
                      );
        to->held_item = from->catch_rate;
        // The rest of the Gen I PC fields are identically laid out in Gen II
        std::memcpy(to->moves, from->moves, 25);
        to->friendship = uint8_t(pokemon_index_to_base_happiness(
                             pksav_bigendian16(to->species), 4
                         ));
        // Leave Pokérus field at 0

        /*
         * Leave location caught at 0, Crystal's Poké Seer can't tell where
         * traded Pokémon were caught.
         *
         * TODO: time of day, fix PKSav mask names
         */
        to->caught_data = (to->caught_data & ~GEN2_LEVEL_CAUGHT_MASK)
                        | (from->level << GEN2_LEVEL_CAUGHT_OFFSET);

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
        to->party_data.status = from->pc.status;
        // Leave unused field at 0
        to->party_data.current_hp = from->pc.current_hp;
        // The next four fields are identical
        memcpy(&to->party_data.max_hp, &from->party_data.max_hp, 12);
        // TODO: how does Special translate to Sp.Atk, Sp.Def?
    }

}
