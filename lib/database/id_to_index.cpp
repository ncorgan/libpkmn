/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_index.hpp"

#include <boost/config.hpp>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    int game_id_to_index(
        int game_id
    ) {
        if(game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM version_game_indices WHERE version_id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, game_id
               );
    }

    int game_index_to_id(
        int game_index
    ) {
        if(game_index == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT version_id FROM version_game_indices WHERE game_index=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, game_index
               );
    }

    int item_id_to_index(
        int item_id,
        int game_id
    ) {
        if(item_id == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        (void)item_id;
        (void)game_id;
        return 0;
    }

    int item_index_to_id(
        int item_index,
        int game_id
    ) {
        if(item_index == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        (void)item_index;
        (void)game_id;
        return 0;
    }

    int location_id_to_index(
        int location_id,
        int game_id
    ) {
        if(location_id == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        (void)location_id;
        (void)game_id;
        return 0;
    }

    int location_index_to_id(
        int location_index,
        int game_id
    ) {
        if(location_index == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        (void)location_index;
        (void)game_id;
        return 0;
    }

    int nature_id_to_index(
        int nature_id
    ) {
        if(nature_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM natures WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, nature_id
               );
    }

    int nature_index_to_id(
        int nature_index
    ) {
        if(nature_index == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT id FROM natures WHERE game_index=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, nature_index
               );
    }

    int pokemon_id_to_index(
        int pokemon_id,
        int game_id
    ) {
        if(pokemon_id == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT game_index FROM pokemon_game_indices WHERE "
            "pokemon_id=? AND version_id=?";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, pokemon_id, game_id
               );
    }

    int pokemon_index_to_id(
        int pokemon_index,
        int game_id
    ) {
        if(pokemon_index == 0 or game_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "game_index=? AND version_id=?";

        return pkmn::database::query_db_bind2<int, int, int>(
                   _db, query, pokemon_index, game_id
               );
    }

}}
