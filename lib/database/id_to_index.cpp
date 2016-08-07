/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_index.hpp"

#include <boost/config.hpp>
#include <boost/format.hpp>

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

        int generation = pkmn::database::game_id_to_generation(game_id);

        /*
         * In any case, check the main item indices table. If this
         * fails for a Gamecube game, check that table. If it fails
         * for any other game, it failed overall.
         */
        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT game_index FROM item_game_indices WHERE item_id=? "
            "AND generation_id=?";

        int ret;
        if(pkmn::database::maybe_query_db_bind2<int, int, int>(
               _db, main_query, ret, item_id, generation
           ))
        {
            /*
             * We know the item existed in this generation, but we need to
             * confirm that it existed in this specific game.
             */

            int version_group_id = pkmn::database::game_id_to_version_group(game_id);
            if(item_index_valid(ret, version_group_id)) {
                return ret;
            } else {
                throw std::invalid_argument("This item did not exist in this game.");
            }
        } else {
            if(game_is_gamecube(game_id)) {
                bool colosseum = (game_id == 19);

                static BOOST_CONSTEXPR const char* gcn_query = \
                    "SELECT game_index FROM gamecube_item_game_indices "
                    "WHERE item_id=? AND colosseum=?";

                return pkmn::database::query_db_bind2<int, int, int>(
                           _db, gcn_query, item_id, (colosseum ? 1 : 0)
                       );
            }
        }

        throw std::invalid_argument(
                  str(boost::format("Invalid SQLite query: %s") % main_query)
              );
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
