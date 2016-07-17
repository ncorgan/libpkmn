/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_DATABASE_COMMON_HPP
#define PKMN_DATABASE_DATABASE_COMMON_HPP

#include <sstream>
#include <stdexcept>
#include <string>

/*
 * TODO:
 *
 * Get valid database connection


#define GET_DATABASE_CONNECTION(db) \
    if(!db) { \
        db = pkmn::database::get_database_connection(); \
    }
*/

/*
 * Use this when a valid query, given proper inputs, should
 * always succeeds.
 *
 * TODO: change these to two templated functions
 */

#define DATABASE_DOUBLE_QUERY(db, var, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.executeStep(); \
        var = float((double)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_DOUBLE_QUERY1(db, var, val1, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.executeStep(); \
        var = float((double)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_DOUBLE_QUERY2(db, var, val1, val2, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.bind(2, val2); \
        query.executeStep(); \
        var = float((double)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT_QUERY(db, var, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.executeStep(); \
        var = (int)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT_QUERY1(db, var, val1, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.executeStep(); \
        var = (int)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT_QUERY2(db, var, val1, val2, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.bind(2, val2); \
        query.executeStep(); \
        var = (int)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_UINT32_QUERY(db, var, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.executeStep(); \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_UINT32_QUERY1(db, var, val1, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.executeStep(); \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_UINT32_QUERY2(db, var, val1, val2, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.bind(2, val2); \
        query.executeStep(); \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT64_QUERY(db, var, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.executeStep(); \
        var = (int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT64_QUERY1(db, var, val1, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.executeStep(); \
        var = (int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_INT64_QUERY2(db, var, val1, val2, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.bind(2, val2); \
        query.executeStep(); \
        var = (int64_t)query.getColumn(0); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_STRING_QUERY(db, var, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.executeStep(); \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_STRING_QUERY1(db, var, val1, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.executeStep(); \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

#define DATABASE_STRING_QUERY2(db, var, val1, val2, ...) \
    try { \
        SQLite::Statement query(db, __VA_ARGS__); \
        query.bind(1, val1); \
        query.bind(2, val2); \
        query.executeStep(); \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } catch (const SQLite::Exception &e) { \
        std::stringstream err_msg; \
        err_msg << "SQLite threw error: " << e.what(); \
        throw std::runtime_error(err_msg.str()); \
    }

/*
 * Use this when a query may or may not succeed.
 */

#define DATABASE_DOUBLE_QUERY_OR_NONE(db, var, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    if(query.executeStep()) { \
        var = float((double)query.getColumn(0)); \
    } else { \
        var = 0.0; \
    }

#define DATABASE_DOUBLE_QUERY1_OR_NONE(db, var, val1, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    if(query.executeStep()) { \
        var = float((double)query.getColumn(0)); \
    } else { \
        var = 0.0; \
    }

#define DATABASE_DOUBLE_QUERY2_OR_NONE(db, var, val1, val2, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    query.bind(2, val2); \
    if(query.executeStep()) { \
        var = float((double)query.getColumn(0)); \
    } else { \
        var = 0.0; \
    }

#define DATABASE_INT_QUERY_OR_NONE(db, var, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    if(query.executeStep()) { \
        var = (int)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_INT_QUERY1_OR_NONE(db, var, val1, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    if(query.executeStep()) { \
        var = (int)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_INT_QUERY2_OR_NONE(db, var, val1, val2, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    query.bind(2, val2); \
    if(query.executeStep()) { \
        var = (int)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_UINT32_QUERY_OR_NONE(db, var, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    if(query.executeStep()) { \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_UINT32_QUERY1_OR_NONE(db, var, val1, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    if(query.executeStep()) { \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_UINT32_QUERY2_OR_NONE(db, var, val1, val2, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    query.bind(2, val2); \
    if(query.executeStep()) { \
        var = (uint32_t)(int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_INT64_QUERY_OR_NONE(db, var, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    if(query.executeStep()) { \
        var = (int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_INT64_QUERY1_OR_NONE(db, var, val1, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    if(query.executeStep()) { \
        var = (int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_INT64_QUERY2_OR_NONE(db, var, val1, val2, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    query.bind(2, val2); \
    if(query.executeStep()) { \
        var = (int64_t)query.getColumn(0); \
    } else { \
        var = 0; \
    }

#define DATABASE_STRING_QUERY_OR_NONE(db, var, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    if(query.executeStep()) { \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } else { \
        var = ""; \
    }

#define DATABASE_STRING_QUERY1_OR_NONE(db, var, val1, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    if(query.executeStep()) { \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } else { \
        var = ""; \
    }

#define DATABASE_STRING_QUERY2_OR_NONE(db, var, val1, val2, ...) \
    SQLite::Statement query(db, __VA_ARGS__); \
    query.bind(1, val1); \
    query.bind(2, val2); \
    if(query.executeStep()) { \
        var = pkmn::database::fix_veekun_whitespace((const char*)query.getColumn(0)); \
    } else { \
        var = ""; \
    }

namespace pkmn { namespace database {

    /*
     * TODO:
     *  * get_database_connection
     *  * Templated functions for querying
     */

    /*
     * Workarounds for Veekun database oddities
     */

    std::string fix_veekun_whitespace(
        const std::string &input
    );

    std::string fix_location_string(
        const std::string &original_string,
        const int location_id,
        const int game_id,
        bool whole_generation
    );

}}

#endif /* PKMN_DATABASE_DATABASE_COMMON_HPP */
