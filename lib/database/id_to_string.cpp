/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <boost/config.hpp>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    std::string ability_id_to_name(
        int ability_id
    ) {
        if(ability_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM ability_names WHERE ability_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, ability_id
               );
    }

    int ability_name_to_id(
        const std::string &ability_name
    ) {
        if(ability_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT ability_id FROM ability_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, ability_name
               );
    }

    std::string ball_id_to_name(
        int ball_id
    ) {
        if(ball_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM balls WHERE id=?";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, ball_id
               );
    }

    int ball_name_to_id(
        const std::string &ball_name
    ) {
        if(ball_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT id FROM balls WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, ball_name
               );
    }

    std::string egg_group_id_to_name(
        int egg_group_id
    ) {
        if(egg_group_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM egg_group_prose WHERE egg_group_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, egg_group_id
               );
    }

    int egg_group_name_to_id(
        const std::string &egg_group_name
    ) {
        if(egg_group_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT egg_group_id FROM egg_group_prose WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, egg_group_name
               );
    }

    std::string game_id_to_name(
        int game_id
    ) {
        if(game_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM version_names WHERE version_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, game_id
               );
    }

    int game_name_to_id(
        const std::string &game_name
    ) {
        if(game_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT version_id FROM version_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, game_name
               );
    }

    std::string item_id_to_name(
        int item_id,
        int version_group_id
    ) {
        if(item_id == 0 or version_group_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        /*
         * If the version group isn't Generation VI, check to see if
         * the given version group had a different name for this item.
         * If not, fall through to the default query.
         */
        BOOST_STATIC_CONSTEXPR int XY   = 25;
        BOOST_STATIC_CONSTEXPR int ORAS = 26;
        if(version_group_id != XY and version_group_id != ORAS) {
            static BOOST_CONSTEXPR const char* old_name_query = \
                "SELECT name FROM old_item_names WHERE item_id=? AND "
                "latest_version_group>=? AND local_language_id=9 "
                "ORDER BY latest_version_group";

            std::string old_ret;
            if(pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                   _db, old_name_query, old_ret, item_id, version_group_id
               ))
            {
                return old_ret;
            }
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT name FROM item_names WHERE item_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, main_query, item_id
               );
    }

    int item_name_to_id(
        const std::string &item_name,
        int version_group_id
    ) {
        if(item_name == "None" or version_group_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        /*
         * If the version group isn't Generation VI, check to see if
         * the given version group had a different name for this item.
         * If not, fall through to the default query.
         */
        BOOST_STATIC_CONSTEXPR int XY   = 25;
        BOOST_STATIC_CONSTEXPR int ORAS = 26;
        if(version_group_id != XY and version_group_id != ORAS) {
            static BOOST_CONSTEXPR const char* old_name_query = \
                "SELECT item_id FROM old_item_names WHERE name=? AND "
                "latest_version_group>=? ORDER BY latest_version_group";

            int old_ret = 0;
            if(pkmn::database::maybe_query_db_bind2<int, const std::string&, int>(
                   _db, old_name_query, old_ret, item_name, version_group_id
               ))
            {
                return old_ret;
            }
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT item_id FROM item_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, main_query, item_name
               );
    }

    std::string item_list_id_to_name(
        int item_list_id,
        int version_group_id
    ) {
        if(item_list_id == 0 or version_group_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM libpkmn_item_lists WHERE id=? AND "
            "version_group_id=?";

        return pkmn::database::query_db_bind2<std::string, int, int>(
                   _db, query, item_list_id, version_group_id
               );
    }

    int item_list_name_to_id(
        const std::string &item_list_name,
        int version_group_id
    ) {
        if(item_list_name == "None" or version_group_id == 0) {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT id FROM libpkmn_item_lists WHERE name=? AND "
            "version_group_id=?";

        return pkmn::database::query_db_bind2<int, const std::string&, int>(
                   _db, query, item_list_name, version_group_id
               );
    }

    std::string location_id_to_name(
        int location_id
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM location_names WHERE location_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, location_id
               );
    }

    int location_name_to_id(
        const std::string &location_name
    ) {
        if(location_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT location_id FROM location_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, location_name
               );
    }

    std::string move_id_to_name(
        int move_id,
        int generation
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)move_id;
        (void)generation;
        return "";
    }

    int move_name_to_id(
        const std::string &move_name
    ) {
        // Connect to database
        pkmn::database::get_connection(_db);

        (void)move_name;
        return 0;
    }

    std::string nature_id_to_name(
        int nature_id
    ) {
        if(nature_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM nature_names WHERE nature_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, nature_id
               );
    }

    int nature_name_to_id(
        const std::string &nature_name
    ) {
        if(nature_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT nature_id FROM nature_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, nature_name
               );
    }

    std::string species_id_to_name(
        int species_id
    ) {
        if(species_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM pokemon_species_names WHERE "
            "pokemon_species_id=? AND local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, species_id
               );
    }

    int species_name_to_id(
        const std::string &species_name
    ) {
        if(species_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT pokemon_species_id FROM pokemon_species_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, species_name
               );
    }

    std::string type_id_to_name(
        int type_id
    ) {
        if(type_id == 0) {
            return "None";
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM type_names WHERE type_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, type_id
               );
    }

    int type_name_to_id(
        const std::string &type_name
    ) {
        if(type_name == "None") {
            return 0;
        }

        // Connect to database
        pkmn::database::get_connection(_db);

        static BOOST_CONSTEXPR const char* query = \
            "SELECT type_id FROM type_names WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   _db, query, type_name
               );
    }

}}
