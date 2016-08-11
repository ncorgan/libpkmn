/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"

#include <pkmn/utils/paths.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/config.hpp>
#include <boost/algorithm/string/compare.hpp>

#ifdef PKMN_SQLITE_DEBUG
#include <iostream>
#endif

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

#define PKMN_COMPAT_NUM 1

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    pkmn::database::sptr _get_connection() {
        if(!_db) {
            std::string database_path = pkmn::get_database_path();
            _db = pkmn::make_shared<SQLite::Database>(database_path.c_str());

            // Make sure our Veekun commit matches the database's
            int compat_num = _db->execAndGet("SELECT compat_num FROM compat_num");
            if(compat_num != PKMN_COMPAT_NUM) {
                throw std::runtime_error("This database is incompatible with this version of LibPKMN.");
            }
        }

        return _db;
    }

    /*
     * Workarounds for Veekun database oddities
     */

    int game_id_to_generation(
        int game_id
    ) {
        // Connect to database
        (void)_get_connection();

        static BOOST_CONSTEXPR const char* query = \
            "SELECT generation_id FROM version_groups WHERE id="
            "(SELECT version_group_id FROM versions WHERE id=?)";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, game_id
               );
    }

    int game_id_to_version_group(
        int game_id
    ) {
        // Connect to database
        (void)_get_connection();

        static BOOST_CONSTEXPR const char* query = \
            "SELECT version_group_id FROM versions WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(
                   _db, query, game_id
               );
    }

    int game_name_to_generation(
        const std::string &game_name
    ) {
        // Connect to database
        (void)_get_connection();

        static BOOST_CONSTEXPR const char* query = \
            "SELECT generation_id FROM version_groups WHERE id="
            "(SELECT version_group_id FROM versions WHERE id="
            "(SELECT version_id FROM version_names WHERE name=?))";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, game_name
               );
    }

    int game_name_to_version_group(
        const std::string &game_name
    ) {
        // Connect to database
        (void)_get_connection();

        static BOOST_CONSTEXPR const char* query = \
            "SELECT version_group_id FROM versions WHERE id="
            "(SELECT version_id FROM version_names WHERE name=?)";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, game_name
               );
    }

    std::string fix_veekun_whitespace(
        const std::string &input
    ) {
        std::string intermediate, s;
        std::istringstream iss(input);
        while(iss >> s) {
            if(intermediate.size() > 0) {
                intermediate += " " + s;
            } else {
                intermediate = s;
            }
        }

        return intermediate;
    }

    #define GAME_IS_RB   (game_id == 1 or game_id == 2)
    #define GAME_IS_Y    (game_id == 3)
    #define GAME_IS_GS   (game_id == 4 or game_id == 5)
    #define GAME_IS_C    (game_id == 6)
    #define GAME_IS_RS   (game_id == 7 or game_id == 8)
    #define GAME_IS_E    (game_id == 9)
    #define GAME_IS_FRLG (game_id == 10 or game_id == 11)
    #define GAME_IS_DP   (game_id == 12 or game_id == 13)
    #define GAME_IS_P    (game_id == 14)
    #define GAME_IS_HGSS (game_id == 15 or game_id == 16)
    #define GAME_IS_BW   (game_id == 17 or game_id == 18)
    #define GAME_IS_B2W2 (game_id == 21 or game_id == 22)
    #define GAME_IS_XY   (game_id == 23 or game_id == 24)
    #define GAME_IS_ORAS (game_id == 25 or game_id == 26)

    std::string fix_location_string(
        const std::string &original_string,
        int location_id,
        int game_id,
        bool whole_generation
    ) {
        if((location_id == 210) and GAME_IS_DP) {
            return "Cafe";
        } else if((location_id == 378) and GAME_IS_B2W2) {
            return "PWT";
        } else if((location_id == 586) and GAME_IS_E) {
            return "Battle Frontier";
        } else if((location_id == 486) and (game_id == 8)) {
            return "Aqua Hideout";
        } else if((location_id == 10030) and GAME_IS_GS and not whole_generation) {
            return "";
        } else if((location_id == 10343) and (GAME_IS_RS or GAME_IS_E)) {
            return "Ferry";
        } else if((location_id == 10345) and GAME_IS_E) {
            return "Aqua Hideout";
        } else {
            return original_string;
        }
    }

    void _get_item_list(
        std::vector<std::string> &ret,
        int list_id, int game_id
    ) {
        // Connect to database
        (void)_get_connection();

        int generation = pkmn::database::game_id_to_generation(game_id);

        /*
         * Gamecube games have the same item ranges as Ruby/Sapphire for
         * non-Gamecube exclusive items.
         */
        int version_group_id = pkmn::database::game_is_gamecube(game_id)
                                   ? 5
                                   : pkmn::database::game_id_to_version_group(game_id);

        static BOOST_CONSTEXPR const char* all_pockets_query = \
            "SELECT item_id,name FROM item_names WHERE local_language_id=9 "
            "AND item_id IN (SELECT items.id FROM items "
            "INNER JOIN item_game_indices ON "
            "(items.id=item_game_indices.item_id) WHERE "
            "item_game_indices.generation_id=? AND "
            "item_game_indices.game_index>=? AND item_game_indices.game_index<=?)";

        static BOOST_CONSTEXPR const char* single_pocket_query = \
            "SELECT item_id,name FROM item_names WHERE local_language_id=9 "
            "AND item_id IN (SELECT items.id FROM items "
            "INNER JOIN item_game_indices ON "
            "(items.id=item_game_indices.item_id) WHERE "
            "item_game_indices.generation_id=? AND "
            "item_game_indices.game_index>=? AND item_game_indices.game_index<=?) "
            "AND item_id IN (SELECT items.id FROM items WHERE id IN (SELECT "
            "items.id FROM items INNER JOIN item_categories ON "
            "(items.category_id=item_categories.id) WHERE item_categories.pocket_id "
            "IN (SELECT item_categories.pocket_id FROM item_categories INNER JOIN "
            "veekun_pocket_to_libpkmn_list ON "
            "(item_categories.pocket_id=veekun_pocket_to_libpkmn_list.veekun_pocket_id) "
            "WHERE version_group_id=? AND veekun_pocket_to_libpkmn_list.libpkmn_list_id=?)))";

        // TODO: can we make the statement just once?
        bool all_pockets = (list_id == -1);
        const char* query = (all_pockets ? all_pockets_query : single_pocket_query);
        for(int i = 0; i < 4; ++i) {
            if(not item_range_empty(version_group_id, i)) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << generation << " to 1" << std::endl
                  << " * Bind " << version_group_item_index_bounds[version_group_id][i][0] << " to 2" << std::endl
                  << " * Bind " << version_group_item_index_bounds[version_group_id][i][1] << " to 3" << std::endl;
#endif
                SQLite::Statement stmt((*_db), query);
                stmt.bind(1, generation);
                stmt.bind(2, version_group_item_index_bounds[version_group_id][i][0]);
                stmt.bind(3, version_group_item_index_bounds[version_group_id][i][1]);
                if(not all_pockets) {
#ifdef PKMN_SQLITE_DEBUG
                    std::cout << " * Bind " << version_group_id << " to 4" << std::endl
                              << " * Bind " << list_id << " to 5" << std::endl;
#endif
                    stmt.bind(4, version_group_id);
                    stmt.bind(5, list_id);
                }
#ifdef PKMN_SQLITE_DEBUG
                std::cout << "Results: " << std::endl;
#endif

                while(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
                    std::cout << (const char*)stmt.getColumn(0) << std::endl;
#endif
                    /*
                     * Some items' names changed in Generation IV (TODO: confirm this or V).
                     * If the function is called for an older game, substitute in old names
                     * as needed.
                     */
                    BOOST_STATIC_CONSTEXPR int LG = 11;
                    if(game_id <= LG) {
                        std::string old_name;
                        static BOOST_CONSTEXPR const char* old_name_query = \
                            "SELECT name FROM old_item_names WHERE local_language_id=9 AND "
                            "item_id=? AND latest_version_group>=? ORDER BY latest_version_group";
                        if(boost::lockfree::detail::unlikely(
                               pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                                   _db, old_name_query, old_name,
                                   int(stmt.getColumn(0)), version_group_id
                               )
                           ))
                        {
                            ret.push_back(old_name);
                        } else {
                            ret.push_back((const char*)stmt.getColumn(1));
                        }
                    } else {
                        ret.push_back((const char*)stmt.getColumn(1));
                    }
                }
            }
        }

        // Since Gamecube indices are stored separately, we need to do this over again
        if(game_is_gamecube(game_id)) {
            static BOOST_CONSTEXPR const char* gcn_all_pockets_query = \
                "SELECT name FROM item_names WHERE local_language_id=9 "
                "AND item_id IN (SELECT DISTINCT items.id FROM items "
                "INNER JOIN gamecube_item_game_indices ON "
                "(items.id=gamecube_item_game_indices.item_id) WHERE "
                "gamecube_item_game_indices.colosseum=?)";

            static BOOST_CONSTEXPR const char* gcn_single_pocket_query = \
                "SELECT name FROM item_names WHERE local_language_id=9 "
                "AND item_id IN (SELECT DISTINCT items.id FROM items "
                "INNER JOIN gamecube_item_game_indices ON "
                "(items.id=gamecube_item_game_indices.item_id) WHERE "
                "gamecube_item_game_indices.colosseum=?) "
                "AND item_id IN (SELECT items.id FROM items WHERE id IN (SELECT "
                "items.id FROM items INNER JOIN item_categories ON "
                "(items.category_id=item_categories.id) WHERE item_categories.pocket_id "
                "IN (SELECT item_categories.pocket_id FROM item_categories INNER JOIN "
                "veekun_pocket_to_libpkmn_list ON "
                "(item_categories.pocket_id=veekun_pocket_to_libpkmn_list.veekun_pocket_id) "
                "WHERE version_group_id=? AND veekun_pocket_to_libpkmn_list.libpkmn_list_id=?)))";

            bool colosseum = (game_id == 19);
            const char* gcn_query = (all_pockets ? gcn_all_pockets_query
                                                 : gcn_single_pocket_query);
            SQLite::Statement gcn_stmt((*_db), gcn_query);
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << gcn_query << "\"" << std::endl
                  << " * Bind " << (colosseum ? 1 : 0) << " to 1" << std::endl;
#endif
            gcn_stmt.bind(1, (colosseum ? 1 : 0));
            if(not all_pockets) {
#ifdef PKMN_SQLITE_DEBUG
                std::cout << " * Bind " << list_id << " to 2" << std::endl;
#endif
                gcn_stmt.bind(2, list_id);
            }
#ifdef PKMN_SQLITE_DEBUG
            std::cout << "Results: " << std::flush;
#endif

            while(gcn_stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
                std::cout << (const char*)gcn_stmt.getColumn(0) << ", " << std::flush;
#endif
                ret.push_back((const char*)gcn_stmt.getColumn(0));
            }
#ifdef PKMN_SQLITE_DEBUG
            std::cout << "\b\b " << std::endl;
#endif
        }

        // Sort alphabetically
        std::sort(ret.begin(), ret.end(), boost::algorithm::is_less());
    }

}}
