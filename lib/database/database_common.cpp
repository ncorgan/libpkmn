/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "../misc_common.hpp"

#include <pkmn/utils/paths.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/config.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

#define PKMN_COMPAT_NUM 4

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

    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_GS  (int game_id) {return (game_id == 4 or game_id == 5);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_RS  (int game_id) {return (game_id == 7 or game_id == 8);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_E   (int game_id) {return (game_id == 9);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_DP  (int game_id) {return (game_id == 12 or game_id == 13);}
    static PKMN_CONSTEXPR_OR_INLINE bool GAME_IS_B2W2(int game_id) {return (game_id == 21 or game_id == 22);}

    std::string alternate_location_string(
        const std::string &original_string,
        const int location_id,
        const int game_id,
        bool whole_generation,
        bool* different_found,
        bool* different_applies
    ) {
        *different_found = true;

        if(location_id == 210) {
            *different_applies = GAME_IS_DP(game_id);
            return "Cafe";
        } else if(location_id == 378) {
            *different_applies = GAME_IS_B2W2(game_id);
            return "PWT";
        } else if(location_id == 486) {
            *different_applies = (game_id == 8);
            return "Aqua Hideout";
        } else if(location_id == 586) {
            *different_applies = GAME_IS_E(game_id);
            return "Battle Frontier";
        } else if(location_id == 10030) {
            *different_applies = (GAME_IS_GS(game_id) and not whole_generation);
            return "";
        } else if(location_id == 10343) {
            *different_applies = (GAME_IS_RS(game_id) or GAME_IS_E(game_id));
            return "Ferry";
        } else if(location_id == 10345) {
            *different_applies = GAME_IS_E(game_id);
            return "Aqua Hideout";
        } else {
            *different_found = false;
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
        int version_group_id = game_is_gamecube(game_id)
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
                SQLite::Statement stmt((*_db), query);
                stmt.bind(1, generation);
                stmt.bind(2, version_group_item_index_bounds[version_group_id][i][0]);
                stmt.bind(3, version_group_item_index_bounds[version_group_id][i][1]);
                if(not all_pockets) {
                    stmt.bind(4, version_group_id);
                    stmt.bind(5, list_id);
                }

                while(stmt.executeStep()) {
                    /*
                     * Some items' names changed in Generation IV (TODO: confirm this or V).
                     * If the function is called for an older game, substitute in old names
                     * as needed.
                     */
                    BOOST_STATIC_CONSTEXPR int XY = 25;
                    if(game_id <= XY) {
                        std::string old_name;
                        static BOOST_CONSTEXPR const char* old_name_query = \
                            "SELECT name FROM old_item_names WHERE local_language_id=9 AND "
                            "item_id=? AND latest_version_group>=? ORDER BY latest_version_group";
                        if(pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                               _db, old_name_query, old_name,
                               int(stmt.getColumn(0)), version_group_id
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
            gcn_stmt.bind(1, (colosseum ? 1 : 0));
            if(not all_pockets) {
                gcn_stmt.bind(2, list_id);
            }

            while(gcn_stmt.executeStep()) {
                ret.push_back((const char*)gcn_stmt.getColumn(0));
            }
        }

        // Sort alphabetically
        std::sort(ret.begin(), ret.end(), boost::algorithm::is_less());
    }

}}
