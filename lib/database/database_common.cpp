/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "../utils/misc.hpp"

#include <pkmn/utils/paths.hpp>

#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/config.hpp>
#include <boost/algorithm/string/compare.hpp>
#include <boost/thread/lock_guard.hpp>

#include <algorithm>
#include <atomic>
#include <sstream>
#include <stdexcept>

#define PKMN_COMPAT_NUM 19

// TODO: check for more internal errors that shouldn't be

namespace pkmn { namespace database {

    static std::unique_ptr<SQLite::Database> _database_uptr;

    void initialize_connection()
    {
        static std::atomic_bool atomic_is_db_initialized(false);

        if(!atomic_is_db_initialized)
        {
            BOOST_ASSERT(!_database_uptr);
            std::string database_path = pkmn::get_database_path();
            _database_uptr.reset(new SQLite::Database(database_path));

            // Make sure our Veekun commit matches the database's.
            int compat_num = _database_uptr->execAndGet("SELECT compat_num FROM compat_num");
            if(compat_num != PKMN_COMPAT_NUM)
            {
                throw std::runtime_error("This database is incompatible with this version of LibPKMN.");
            }

            atomic_is_db_initialized = true;
        }
    }

    SQLite::Database* get_connection()
    {
        initialize_connection();
        return _database_uptr.get();
    }

    /*
     * Workarounds for Veekun database oddities
     */

    int game_id_to_generation(
        int game_id
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT generation_id FROM version_groups WHERE id="
            "(SELECT version_group_id FROM versions WHERE id=?)";

        return pkmn::database::query_db_bind1<int, int>(query, game_id);
    }

    int game_id_to_version_group(
        int game_id
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT version_group_id FROM versions WHERE id=?";

        return pkmn::database::query_db_bind1<int, int>(query, game_id);
    }

    int game_name_to_generation(
        const std::string& game_name
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT generation_id FROM version_groups WHERE id="
            "(SELECT version_group_id FROM versions WHERE id="
            "(SELECT version_id FROM version_names WHERE name=?))";

        std::string error_message = "Invalid game: ";
        error_message += game_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query, game_name, error_message
               );
    }

    int game_name_to_version_group(
        const std::string& game_name
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT version_group_id FROM versions WHERE id="
            "(SELECT version_id FROM version_names WHERE name=?)";

        std::string error_message = "Invalid game: ";
        error_message += game_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query, game_name, error_message
               );
    }

    std::string fix_veekun_whitespace(
        const std::string& input
    )
    {
        std::string intermediate, s;
        std::istringstream iss(input);
        while(iss >> s)
        {
            if(intermediate.size() > 0)
            {
                intermediate += " " + s;
            }
            else
            {
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
        const std::string& original_string,
        const int location_id,
        const int game_id,
        bool whole_generation,
        bool* different_found_ptr,
        bool* different_applies_ptr
    )
    {
        BOOST_ASSERT(different_found_ptr);
        BOOST_ASSERT(different_applies_ptr);

        *different_found_ptr = true;

        std::string ret;

        if(location_id == 210)
        {
            *different_applies_ptr = GAME_IS_DP(game_id);
            ret = "Cafe";
        }
        else if(location_id == 378)
        {
            *different_applies_ptr = GAME_IS_B2W2(game_id);
            ret = "PWT";
        }
        else if(location_id == 486)
        {
            *different_applies_ptr = (game_id == 8);
            ret = "Aqua Hideout";
        }
        else if(location_id == 586)
        {
            *different_applies_ptr = GAME_IS_E(game_id);
            ret = "Battle Frontier";
        }
        else if(location_id == 10030)
        {
            *different_applies_ptr = (GAME_IS_GS(game_id) and not whole_generation);
            ret = "";
        }
        else if(location_id == 10343)
        {
            *different_applies_ptr = (GAME_IS_RS(game_id) or GAME_IS_E(game_id));
            ret = "Ferry";
        }
        else if(location_id == 10345)
        {
            *different_applies_ptr = GAME_IS_E(game_id);
            ret = "Aqua Hideout";
        }
        else
        {
            *different_found_ptr = false;
            ret = original_string;
        }

        return ret;
    }

    static void _append_gamecube_item_lists(
        std::vector<pkmn::e_item>& r_items,
        std::vector<std::string>& r_item_names,
        int list_id,
        bool colosseum,
        bool should_populate_enum_list,
        bool should_populate_string_list
    )
    {
        int version_group_id = colosseum ? 12 : 13;
        bool all_pockets = (list_id == -1);

        static BOOST_CONSTEXPR const char* gcn_all_pockets_query =
            "SELECT item_id,name FROM item_names WHERE local_language_id=9 "
            "AND item_id IN (SELECT DISTINCT items.id FROM items "
            "INNER JOIN gamecube_item_game_indices ON "
            "(items.id=gamecube_item_game_indices.item_id) WHERE "
            "gamecube_item_game_indices.colosseum=?)";

        static BOOST_CONSTEXPR const char* gcn_single_pocket_query =
            "SELECT item_id,name FROM item_names WHERE local_language_id=9 "
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

        const char* gcn_query = (all_pockets ? gcn_all_pockets_query
                                             : gcn_single_pocket_query);
        SQLite::Statement gcn_stmt(get_connection(), gcn_query);
        gcn_stmt.bind(1, (colosseum ? 1 : 0));
        if(not all_pockets)
        {
            gcn_stmt.bind(2, version_group_id);
            gcn_stmt.bind(3, list_id);
        }

        while(gcn_stmt.executeStep())
        {
            if(should_populate_enum_list)
            {
                r_items.emplace_back(
                    static_cast<pkmn::e_item>(int(gcn_stmt.getColumn(0)))
                );
            }
            if(should_populate_string_list)
            {
                r_item_names.emplace_back(gcn_stmt.getColumn(1));
            }
        }
    }

    void _get_item_lists(
        std::vector<pkmn::e_item>& r_items,
        std::vector<std::string>& r_item_names,
        int list_id,
        int game_id,
        bool should_populate_enum_list,
        bool should_populate_string_list
    )
    {
        int generation = pkmn::database::game_id_to_generation(game_id);
        int version_group_id = pkmn::database::game_id_to_version_group(game_id);
        bool all_pockets = (list_id == -1);

        static BOOST_CONSTEXPR const char* all_pockets_query =
            "SELECT item_id,name FROM item_names WHERE local_language_id=9 "
            "AND item_id IN (SELECT items.id FROM items "
            "INNER JOIN item_game_indices ON "
            "(items.id=item_game_indices.item_id) WHERE "
            "item_game_indices.generation_id=? AND "
            "item_game_indices.game_index>=? AND item_game_indices.game_index<=?)";

        static BOOST_CONSTEXPR const char* single_pocket_query =
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

        const char* query = (all_pockets ? all_pockets_query : single_pocket_query);
        for(int i = 0; i < 4; ++i)
        {
            if(!item_range_empty(version_group_id, i))
            {
                SQLite::Statement stmt(get_connection(), query);
                stmt.bind(1, generation);
                stmt.bind(2, version_group_item_index_bounds[version_group_id][i][0]);
                stmt.bind(3, version_group_item_index_bounds[version_group_id][i][1]);
                if(!all_pockets)
                {
                    stmt.bind(4, version_group_id);
                    stmt.bind(5, list_id);
                }

                while(stmt.executeStep())
                {
                    if(should_populate_enum_list)
                    {
                        r_items.emplace_back(
                            static_cast<pkmn::e_item>(int(stmt.getColumn(0)))
                        );
                    }
                    if(should_populate_string_list)
                    {
                        /*
                         * Some items' names changed in Generation IV (TODO: confirm this or V).
                         * If the function is called for an older game, substitute in old names
                         * as needed.
                         */
                        BOOST_STATIC_CONSTEXPR int XY = 25;
                        if(game_id <= XY)
                        {
                            std::string old_name;
                            static BOOST_CONSTEXPR const char* old_name_query =
                                "SELECT name FROM old_item_names WHERE local_language_id=9 AND "
                                "item_id=? AND latest_version_group>=? ORDER BY latest_version_group";
                            if(pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                                   old_name_query, old_name,
                                   int(stmt.getColumn(0)), version_group_id
                               ))
                            {
                                r_item_names.emplace_back(old_name);
                            }
                            else
                            {
                                r_item_names.emplace_back(stmt.getColumn(1));
                            }
                        }
                        else
                        {
                            r_item_names.emplace_back(stmt.getColumn(1));
                        }
                    }
                }
            }
        }

        if(game_is_gamecube(game_id))
        {
            _append_gamecube_item_lists(
                r_items,
                r_item_names,
                list_id,
                (game_id == 19),
                should_populate_enum_list,
                should_populate_string_list
            );
        }

        static const std::vector<int> ITEM_POCKET_IDS =
        {
            5,  // Gold/Silver
            10, // Crystal
            15, // Ruby/Sapphire
            21, // Emerald
            27, // FireRed/LeafGreen
            33, // Diamond/Pearl
            41, // Platinum
            49, // HeartGold/SoulSilver
            57, // Black/White
            62, // Colosseum
            69, // XD
            76, // Black 2/White 2
            81, // X/Y
            86  // Omega Ruby/Alpha Sapphire
        };
        static const std::vector<int> KEY_ITEM_POCKET_IDS =
        {
            7,  // Gold/Silver
            12, // Crystal
        };

        if(!all_pockets)
        {
            if(pkmn::does_vector_contain_value(ITEM_POCKET_IDS, list_id))
            {
                // Veekun's database places all Berries in the Items pocket, which isn't the case.
                // Fix that here.
                r_items.erase(
                    std::remove_if(
                        r_items.begin(),
                        r_items.end(),
                        is_item_enum_berry
                    ),
                    r_items.end()
                );
                r_item_names.erase(
                    std::remove_if(
                        r_item_names.begin(),
                        r_item_names.end(),
                        [](const std::string& item)
                        {
                            return (item.find("Berry") != std::string::npos) &&
                                   (item != "Berry Juice");
                        }
                    ),
                    r_item_names.end()
                );
            }
            if(generation == 2)
            {
                // In Generation II, Apricorns and RageCandyBars were placed in
                // the "Items" pocket.
                //
                // TODO: this removes Apricorns from the KeyItems pocket, but where
                // is it added to the Items pocket?
                if(pkmn::does_vector_contain_value(KEY_ITEM_POCKET_IDS, list_id))
                {
                    r_items.erase(
                        std::remove_if(
                            r_items.begin(),
                            r_items.end(),
                            [](pkmn::e_item item)
                            {
                                return ((item >= pkmn::e_item::RED_APRICORN) &&
                                        (item <= pkmn::e_item::BLACK_APRICORN)) ||
                                       (item == pkmn::e_item::RAGE_CANDY_BAR);
                            }
                        ),
                        r_items.end()
                    );
                    r_item_names.erase(
                        std::remove_if(
                            r_item_names.begin(),
                            r_item_names.end(),
                            [](const std::string& item)
                            {
                                return (item.find("Apricorn") != std::string::npos) ||
                                       (item == "RAGECANDYBAR");
                            }
                        ),
                        r_item_names.end()
                    );
                }
            }
        }
    }

}}
