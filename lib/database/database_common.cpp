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

}}
