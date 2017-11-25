/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "database_common.hpp"
#include "id_to_index.hpp"

#include <boost/config.hpp>

#include <stdexcept>

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

        int ret = 0;
        if(pkmn::database::maybe_query_db_bind2<int, int, int>(
               _db, main_query, ret, item_id, generation
           ))
        {
            /*
             * We know the item existed in this generation, but we need to
             * confirm that it existed in this specific game.
             */
            BOOST_STATIC_CONSTEXPR int RS = 5;
            int version_group_id = pkmn::database::game_id_to_version_group(game_id);
            if(item_id < 10000 and (version_group_id == 12 or version_group_id == 13)) {
                version_group_id = RS;
            }

            if(not item_index_valid(ret, version_group_id)) {
                if(game_is_gamecube(game_id)) {
                    // This may share a name but be in the Gamecube indices.
                    bool colosseum = (game_id == 19);

                    static BOOST_CONSTEXPR const char* gcn_query = \
                        "SELECT game_index FROM gamecube_item_game_indices "
                        "WHERE item_id=? AND colosseum=?";

                    ret = pkmn::database::query_db_bind2<int, int, int>(
                              _db, gcn_query, item_id, (colosseum ? 1 : 0)
                          );
                } else {
                    throw std::invalid_argument("This item did not exist in this game.");
                }
            }
        } else {
            if(game_is_gamecube(game_id)) {
                bool colosseum = (game_id == 19);

                static BOOST_CONSTEXPR const char* gcn_query = \
                    "SELECT game_index FROM gamecube_item_game_indices "
                    "WHERE item_id=? AND colosseum=?";

                ret = pkmn::database::query_db_bind2<int, int, int>(
                          _db, gcn_query, item_id, (colosseum ? 1 : 0)
                      );
            } else {
                throw std::invalid_argument("This item did not exist in this game.");
            }
        }

        return ret;
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

        int version_group_id = pkmn::database::game_id_to_version_group(game_id);

        /*
         * Make sure the item index is valid before attempting to query the
         * database. This check may not succeed for Gamecube games.
         */
        if(not item_index_valid(item_index, version_group_id) and
           not game_is_gamecube(game_id)
        ) {
            throw std::invalid_argument("Invalid item index.");
        }

        /*
         * In any case, check the main item indices table. If this
         * fails for a Gamecube game, check that table. If it fails
         * for any other game, it failed overall.
         */
        int generation = pkmn::database::game_id_to_generation(game_id);

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT item_id FROM item_game_indices WHERE game_index=? "
            "AND generation_id=?";

        int ret = 0;
        if(not pkmn::database::maybe_query_db_bind2<int, int, int>(
               _db, main_query, ret, item_index, generation
           )
        ) {
            if(game_is_gamecube(game_id)) {
                bool colosseum = (game_id == 19);

                static BOOST_CONSTEXPR const char* gcn_query = \
                    "SELECT item_id FROM gamecube_item_game_indices "
                    "WHERE game_index=? AND colosseum=?";

                ret = pkmn::database::query_db_bind2<int, int, int>(
                          _db, gcn_query, item_index, (colosseum ? 1 : 0)
                      );
            } else {
                throw std::invalid_argument("Invalid item index.");
            }
        }

        return ret;
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
