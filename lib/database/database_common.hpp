/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_DATABASE_COMMON_HPP
#define PKMN_DATABASE_DATABASE_COMMON_HPP

#include "SQLiteCpp/SQLiteCpp.h"

#include <pkmn/config.hpp>

#include <boost/config.hpp>

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace pkmn { namespace database {

    typedef std::shared_ptr<SQLite::Database> sptr;

    void initialize_connection();
    SQLite::Database* get_connection();

    /*
     * Templated query functions
     */

    template <typename ret_type>
    ret_type query_db(
        const char* query,
        const std::string& error_message
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        if(stmt.executeStep())
        {
            return (ret_type)stmt.getColumn(0);
        }
        else
        {
            if(error_message.empty())
            {
                std::ostringstream stream;
                stream << "Internal error:" << std::endl
                       << "Invalid SQLite query: \"" << query << "\"";
                throw std::invalid_argument(stream.str());
            }
            else
            {
                throw std::invalid_argument(error_message);
            }
        }
    }

    template <typename ret_type, typename bind1_type>
    ret_type query_db_bind1(
        const char* query,
        bind1_type bind1,
        const std::string& error_message = ""
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        if(stmt.executeStep())
        {
            return (ret_type)stmt.getColumn(0);
        }
        else
        {
            if(error_message.empty())
            {
                std::ostringstream stream;
                stream << "Internal error:" << std::endl
                       << "Invalid SQLite query: \"" << query << "\"" << std::endl
                       << " * Value 1 = " << bind1;
                throw std::invalid_argument(stream.str());
            }
            else
            {
                throw std::invalid_argument(error_message);
            }
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    ret_type query_db_bind2(
        const char* query,
        bind1_type bind1,
        bind2_type bind2,
        const std::string& error_message = ""
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        if(stmt.executeStep())
        {
            return (ret_type)stmt.getColumn(0);
        }
        else
        {
            if(error_message.empty())
            {
                std::ostringstream stream;
                stream << "Internal error:" << std::endl
                       << "Invalid SQLite query: \"" << query << "\"" << std::endl
                       << " * Value 1 = " << bind1 << std::endl
                       << " * Value 2 = " << bind2;
                throw std::invalid_argument(stream.str());
            }
            else
            {
                throw std::invalid_argument(error_message);
            }
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    ret_type query_db_bind3(
        const char* query,
        bind1_type bind1,
        bind2_type bind2,
        bind3_type bind3,
        const std::string& error_message = ""
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        if(stmt.executeStep())
        {
            return (ret_type)stmt.getColumn(0);
        }
        else
        {
            if(error_message.empty())
            {
                std::ostringstream stream;
                stream << "Internal error:" << std::endl
                       << "Invalid SQLite query: \"" << query << "\"" << std::endl
                       << " * Value 1 = " << bind1 << std::endl
                       << " * Value 2 = " << bind2 << std::endl
                       << " * Value 3 = " << bind3 << std::endl;
                throw std::invalid_argument(stream.str());
            }
            else
            {
                throw std::invalid_argument(error_message);
            }
        }
    }

    /*
     * Templated query functions (queries may fail)
     */

    template <typename ret_type>
    bool maybe_query_db(
        const char* query,
        ret_type& out
    )
    {
        bool ret = false;
        SQLite::Statement stmt(get_connection(), query);
        if(stmt.executeStep())
        {
            out = (ret_type)stmt.getColumn(0);
            ret = true;
        }

        return ret;
    }

    template <typename ret_type, typename bind1_type>
    bool maybe_query_db_bind1(
        const char* query,
        ret_type& out,
        bind1_type bind1
    )
    {
        bool ret = false;
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        if(stmt.executeStep())
        {
            out = (ret_type)stmt.getColumn(0);
            ret = true;
        }

        return ret;
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    bool maybe_query_db_bind2(
        const char* query,
        ret_type& out,
        bind1_type bind1,
        bind2_type bind2
    )
    {
        bool ret = false;
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        if(stmt.executeStep())
        {
            out = (ret_type)stmt.getColumn(0);
            ret = true;
        }

        return ret;
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    bool maybe_query_db_bind2(
        const char* query,
        ret_type& out,
        bind1_type bind1,
        bind2_type bind2,
        bind3_type bind3
    )
    {
        bool ret = false;
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        if(stmt.executeStep())
        {
            out = (ret_type)stmt.getColumn(0);
            ret = true;
        }

        return ret;
    }

    /*
     * Templated query functions that form lists
     */

    template <typename ret_type>
    static void query_db_list(
        const char* query,
        std::vector<ret_type> &ret_vec
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        while(stmt.executeStep())
        {
            ret_vec.emplace_back((ret_type)stmt.getColumn(0));
        }
    }

    template <typename ret_type, typename bind1_type>
    static void query_db_list_bind1(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        while(stmt.executeStep())
        {
            ret_vec.emplace_back((ret_type)stmt.getColumn(0));
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type>
    static void query_db_list_bind2(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind2_type bind2
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        while(stmt.executeStep())
        {
            ret_vec.emplace_back((ret_type)stmt.getColumn(0));
        }
    }

    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    static void query_db_list_bind3(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind1_type bind2,
        bind1_type bind3
    )
    {
        SQLite::Statement stmt(get_connection(), query);
        stmt.bind(1, (bind1_type)bind1);
        stmt.bind(2, (bind2_type)bind2);
        stmt.bind(3, (bind3_type)bind3);
        while(stmt.executeStep())
        {
            ret_vec.emplace_back((ret_type)stmt.getColumn(0));
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

    std::string alternate_location_string(
        const std::string &original_string,
        int location_id,
        int game_id,
        bool whole_generation,
        bool* different_found,
        bool* different_applies
    );

    void _get_item_list(
        std::vector<std::string> &ret,
        int list_id, int game_id
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
        {{1,376},{0,0},{0,0},{0,0}}, // Emerald
        {{1,374},{0,0},{0,0},{0,0}}, // FR/LG
        {{1,111},{135,464},{0,0},{0,0}}, // D/P
        {{1,467},{0,0},{0,0},{0,0}}, // Platinum
        {{1,427},{429,536},{0,0},{0,0}}, // HG/SS
        {{1,626},{0,0},{0,0},{0,0}}, // B/W
        {{1,120},{133,174},{179,258},{289,338}}, // Colosseum
        {{1,120},{133,174},{179,258},{289,338}}, // XD
        {{1,638},{0,0},{0,0},{0,0}}, // B2/W2
        {{1,717},{0,0},{0,0},{0,0}}, // X/Y
        {{1,775},{0,0},{0,0},{0,0}} // OR/AS
    };

    PKMN_CONSTEXPR_OR_INLINE bool item_index_in_bounds(
        int item_id,
        int version_group_id,
        int range
    ) {
        return (item_id >= version_group_item_index_bounds[version_group_id][range][0]) and
               (item_id <= version_group_item_index_bounds[version_group_id][range][1]);
    }

    PKMN_CONSTEXPR_OR_INLINE bool item_range_empty(
        int version_group_id,
        int range
    ) {
        return (version_group_item_index_bounds[version_group_id][range][0] == 0) and
               (version_group_item_index_bounds[version_group_id][range][1] == 0);
    }

    inline bool item_index_valid(
        int item_index,
        int version_group_id
    ) {
        for(int i = 0; i < 4; ++i) {
            if(item_index_in_bounds(item_index, version_group_id, i)) {
                return true;
            }
        }

        return false;
    }
}}

#endif /* PKMN_DATABASE_DATABASE_COMMON_HPP */
