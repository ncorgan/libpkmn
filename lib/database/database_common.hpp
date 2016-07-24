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

#include <boost/format.hpp>
#include <boost/lockfree/detail/branch_hints.hpp>

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

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
        return (ret_type)db->execAndGet(query);
    }

    template <typename ret_type, typename bind1_type>
    ret_type query_db_bind1(
        sptr db,
        const char* query,
        bind1_type bind1
    ) {
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        if(stmt.executeStep()) {
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
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        if(stmt.executeStep()) {
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
        SQLite::Statement stmt((*db), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        if(stmt.executeStep()) {
            return (ret_type)stmt.getColumn(0);
        } else {
            throw std::runtime_error(
                      str(boost::format("Invalid SQLite query: \"%s\")") % query)
                  );
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
     * Workarounds for Veekun database oddities
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

}}

#endif /* PKMN_DATABASE_DATABASE_COMMON_HPP */
