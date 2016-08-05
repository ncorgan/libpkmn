/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_DATABASE_COMMON_HPP
#define PKMN_DATABASE_DATABASE_COMMON_HPP

#include "SQLiteCpp/SQLiteCpp.h"

#include <pkmn/config.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <boost/config.hpp>
#include <boost/format.hpp>
#include <boost/lockfree/detail/branch_hints.hpp>

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef PKMN_SQLITE_DEBUG
#include <iostream>
#endif

namespace pkmn { namespace database {

    typedef pkmn::shared_ptr<SQLite::Database> sptr;

    sptr _get_connection();

    PKMN_INLINE void get_connection(
        sptr &db
    ) {
        if(boost::lockfree::detail::unlikely(!db)) {
            db = _get_connection();
        }
    }

    /*
     * Templated query functions
     */

    template <typename ret_type>
    ret_type query_db(
        sptr db,
        const char* query
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl;
        SQLite::Column col = db->execAndGet(query);
        if(not col.isBlob()) {
            std::cout << " * Result: " << (ret_type)col << std::endl;
        }
        return (ret_type)col;
#else
        return (ret_type)db->execAndGet(query);
#endif
    }

    template <typename ret_type, typename bind1_type>
    ret_type query_db_bind1(
        sptr db,
        const char* query,
        bind1_type bind1
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not stmt.getColumn(0).isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            return (ret_type)col;
#endif
            return (ret_type)stmt.getColumn(0);
        } else {
            throw std::runtime_error(
                      str(boost::format("Invalid SQLite query: \"%s\")") % query)
                  );
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    ret_type query_db_bind2(
        sptr db,
        const char* query,
        bind1_type bind1,
        bind2_type bind2
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not stmt.getColumn(0).isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            return (ret_type)col;
#endif
            return (ret_type)stmt.getColumn(0);
        } else {
            throw std::runtime_error(
                      str(boost::format("Invalid SQLite query: \"%s\")") % query)
                  );
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    ret_type query_db_bind3(
        sptr db,
        const char* query,
        bind1_type bind1,
        bind2_type bind2,
        bind3_type bind3
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl
                  << " * Bind " << bind3 << " to 3" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not stmt.getColumn(0).isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            return (ret_type)col;
#endif
            return (ret_type)stmt.getColumn(0);
        } else {
            throw std::runtime_error(
                      str(boost::format("Invalid SQLite query: \"%s\")") % query)
                  );
        }
    }

    /*
     * Templated query functions (queries may fail)
     */

    template <typename ret_type>
    bool maybe_query_db(
        sptr db,
        const char* query,
        ret_type &out
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not col.isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            out = (ret_type)col;
#else
            out = (ret_type)stmt.getColumn(0);
#endif
            return true;
        } else {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << " * Query failed." << std::endl;
#endif
            return false;
        }
    }

    template <typename ret_type, typename bind1_type>
    bool maybe_query_db_bind1(
        sptr db,
        const char* query,
        ret_type &out,
        bind1_type bind1
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not col.isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            out = (ret_type)col;
#else
            out = (ret_type)stmt.getColumn(0);
#endif
            return true;
        } else {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << " * Query failed." << std::endl;
#endif
            return false;
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    bool maybe_query_db_bind2(
        sptr db,
        const char* query,
        ret_type &out,
        bind1_type bind1,
        bind2_type bind2
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not col.isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            out = (ret_type)col;
#else
            out = (ret_type)stmt.getColumn(0);
#endif
            return true;
        } else {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << " * Query failed." << std::endl;
#endif
            return false;
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    bool maybe_query_db_bind2(
        sptr db,
        const char* query,
        ret_type &out,
        bind1_type bind1,
        bind2_type bind2,
        bind3_type bind3
    ) {
#ifdef PKMN_SQLITE_DEBUG
        std::cout << "Query: \"" << query << "\"" << std::endl
                  << " * Bind " << bind1 << " to 1" << std::endl
                  << " * Bind " << bind2 << " to 2" << std::endl
                  << " * Bind " << bind3 << " to 3" << std::endl;
#endif
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = stmt.getColumn(0);
            if(not col.isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            out = (ret_type)col;
#else
            out = (ret_type)stmt.getColumn(0);
#endif
            return true;
        } else {
#ifdef PKMN_SQLITE_DEBUG
            std::cout << " * Query failed." << std::endl;
#endif
            return false;
        }
    }

    /*
     * Common functions that don't belong elsewhere
     */
    int game_id_to_generation(
        int game_id
    );

    int game_id_to_version_group(
        int game_id
    );

    int game_name_to_generation(
        const std::string &game_name
    );

    int game_name_to_version_group(
        const std::string &game_name
    );

    /*
     * Workarounds for Veekun database string oddities
     */

    std::string fix_veekun_whitespace(
        const std::string &input
    );

    std::string fix_location_string(
        const std::string &original_string,
        int location_id,
        int game_id,
        bool whole_generation
    );

    /*
     * Veekun's database stores item game indices by generation, but
     * version groups would have been more appropriate. To work around this,
     * these values determine the bounds of valid item indices for a given
     * version group. The functions below check them.
     */
    BOOST_STATIC_CONSTEXPR int version_group_item_index_bounds[][4][2] = {
        {{0,0},{0,0},{0,0},{0,0}}, // None
        {{1,255},{0,0},{0,0},{0,0}}, // Red/Blue
        {{1,255},{0,0},{0,0},{0,0}}, // Yellow
        {{1,69},{71,114},{117,128},{130,249}}, // Gold/Silver
        {{1,249},{0,0},{0,0},{0,0}}, // Crystal
        {{1,348},{0,0},{0,0},{0,0}}, // Ruby/Sapphire
        {{1,348},{375,376},{0,0},{0,0}}, // Emerald
        {{1,374},{0,0},{0,0},{0,0}}, // FR/LG
        {{1,111},{135,464},{0,0},{0,0}}, // D/P
        {{1,467},{0,0},{0,0},{0,0}}, // Platinum
        {{1,427},{429,536},{0,0},{0,0}}, // HG/SS
        {{1,626},{0,0},{0,0},{0,0}}, // B/W
        {{0,0},{0,0},{0,0},{0,0}}, // Colosseum
        {{0,0},{0,0},{0,0},{0,0}}, // XD
        {{1,638},{0,0},{0,0},{0,0}}, // B2/W2
        {{1,717},{0,0},{0,0},{0,0}}, // X/Y
        {{1,775},{0,0},{0,0},{0,0}} // OR/AS
    };

    // TODO: switch to PKMN_CONSTEXPR_OR_INLINE when cherrypicked over
    BOOST_CONSTEXPR PKMN_INLINE bool item_index_in_bounds(
        int item_id,
        int version_group_id,
        int range
    ) {
        return (item_id >= version_group_item_index_bounds[version_group_id][range][0]) and \
               (item_id >= version_group_item_index_bounds[version_group_id][range][1]);
    }

    // TODO: switch to PKMN_CONSTEXPR_OR_INLINE when cherrypicked over
    BOOST_CONSTEXPR PKMN_INLINE bool item_range_empty(
        int version_group_id,
        int range
    ) {
        return (version_group_item_index_bounds[version_group_id][range][0] == 0) and \
               (version_group_item_index_bounds[version_group_id][range][1] == 0);
    }

    PKMN_INLINE bool item_index_valid(
        int item_id,
        int version_group_id
    ) {
        for(int i = 0; i < 4; ++i) {
            if(not item_index_in_bounds(item_id, version_group_id, i)) {
                return false;
            }
        }

        return true;
    }

    void _get_item_list(
        std::vector<std::string> &ret,
        int list_id, int game_id
    );

    bool string_compare(
        const std::string &left,
        const std::string &right
    );
}}

#endif /* PKMN_DATABASE_DATABASE_COMMON_HPP */
