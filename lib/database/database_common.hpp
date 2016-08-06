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
#endif
        SQLite::Statement stmt((*db), query);
        if(stmt.executeStep()) {
#ifdef PKMN_SQLITE_DEBUG
            SQLite::Column col = db->execAndGet(query);
            if(not col.isBlob()) {
                std::cout << " * Result: " << (ret_type)col << std::endl;
            }
            return (ret_type)col;
#else
            return (ret_type)stmt.getColumn(0);
#endif
        } else {
            throw std::invalid_argument(
                      str(boost::format("Invalid SQLite query: \"%s\")") % query)
                  );
        }
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
            throw std::invalid_argument(
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
            throw std::invalid_argument(
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
            throw std::invalid_argument(
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
