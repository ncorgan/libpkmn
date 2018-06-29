/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <boost/config.hpp>

namespace pkmn { namespace database {

    std::string ability_id_to_name(
        int ability_id
    )
    {
        if(ability_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM ability_names WHERE ability_id=? AND "
            "local_language_id=9";

        const std::string error_message = "Invalid ability: " + std::to_string(ability_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   ability_id,
                   error_message
               );
    }

    int ability_name_to_id(
        const std::string& ability_name
    )
    {
        if(ability_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT ability_id FROM ability_names WHERE name=?";

        const std::string error_message = "Invalid ability: " + ability_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   ability_name,
                   error_message
               );
    }

    std::string ball_id_to_name(
        int ball_id
    )
    {
        if(ball_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM balls WHERE id=?";

        const std::string error_message = "Invalid ball: " + std::to_string(ball_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   ball_id,
                   error_message
               );
    }

    int ball_name_to_id(
        const std::string& ball_name
    )
    {
        if(ball_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT id FROM balls WHERE name=?";

        const std::string error_message = "Invalid ball: " + ball_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   ball_name,
                   error_message
               );
    }

    std::string egg_group_id_to_name(
        int egg_group_id
    )
    {
        if(egg_group_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM egg_group_prose WHERE egg_group_id=? AND "
            "local_language_id=9";

        const std::string error_message = "Invalid egg group: " + std::to_string(egg_group_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   egg_group_id,
                   error_message
               );
    }

    int egg_group_name_to_id(
        const std::string& egg_group_name
    )
    {
        if(egg_group_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT egg_group_id FROM egg_group_prose WHERE name=?";

        const std::string error_message = "Invalid egg group: " + egg_group_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   egg_group_name,
                   error_message
               );
    }

    std::string game_id_to_name(
        int game_id
    )
    {
        if(game_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM version_names WHERE version_id=? AND "
            "local_language_id=9";

        const std::string error_message = "Invalid game: " + std::to_string(game_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   game_id,
                   error_message
               );
    }

    int game_name_to_id(
        const std::string& game_name
    )
    {
        if(game_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT version_id FROM version_names WHERE name=?";

        const std::string error_message = "Invalid game: " + game_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   game_name,
                   error_message
               );
    }

    std::string item_id_to_name(
        int item_id,
        int version_group_id
    )
    {
        if((item_id == 0) or (version_group_id == 0))
        {
            return "None";
        }

        /*
         * If the version group isn't Generation VI, check to see if
         * the given version group had a different name for this item.
         * If not, fall through to the default query.
         *
         * For Gamecube games, use Ruby/Sapphire to check for valid
         * items since the indices are the same, and the database
         * doesn't know those items are in the Gamecube games.
         */
        BOOST_STATIC_CONSTEXPR int RS   = 5;
        BOOST_STATIC_CONSTEXPR int XY   = 25;
        BOOST_STATIC_CONSTEXPR int ORAS = 26;
        if(version_group_id != XY and version_group_id != ORAS)
        {
            static BOOST_CONSTEXPR const char* old_name_query =
                "SELECT name FROM old_item_names WHERE item_id=? AND "
                "latest_version_group>=? AND local_language_id=9 "
                "ORDER BY latest_version_group";

            int actual_version_group_id = version_group_id;
            if(item_id < 10000 and (version_group_id == 12 or version_group_id == 13)) {
                actual_version_group_id = RS;
            }

            std::string old_ret;
            if(pkmn::database::maybe_query_db_bind2<std::string, int, int>(
                   old_name_query, old_ret, item_id,
                   actual_version_group_id
               ))
            {
                return old_ret;
            }
        }

        static BOOST_CONSTEXPR const char* main_query =
            "SELECT name FROM item_names WHERE item_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   main_query, item_id
               );
    }

    int item_name_to_id(
        const std::string& item_name
    )
    {
        if(item_name == "None")
        {
            return 0;
        }

        static BOOST_CONSTEXPR const char* old_name_query =
            "SELECT item_id FROM old_item_names WHERE name=?";

        int old_ret = 0;
        if(pkmn::database::maybe_query_db_bind1<int, const std::string&>(
               old_name_query, old_ret, item_name
           ))
        {
            return old_ret;
        }

        static BOOST_CONSTEXPR const char* main_query =
            "SELECT item_id FROM item_names WHERE name=?";

        std::string error_message = "Invalid item: ";
        error_message += item_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   main_query, item_name, error_message
               );
    }

    std::string item_list_id_to_name(
        int item_list_id
    )
    {
        if(item_list_id == 0)
        {
            return "None";
        }

        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM libpkmn_item_lists WHERE id=?";

        return pkmn::database::query_db_bind1<std::string, int>(
                   query, item_list_id
               );
    }

    int item_list_name_to_id(
        const std::string& item_list_name
    )
    {
        if(item_list_name == "None")
        {
            return 0;
        }

        static BOOST_CONSTEXPR const char* query =
            "SELECT id FROM libpkmn_item_lists WHERE name=?";

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query, item_list_name
               );
    }

    std::string location_id_to_name(
        int location_id
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT name FROM location_names WHERE location_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   query, location_id
               );
    }

    int location_name_to_id(
        const std::string& location_name
    )
    {
        if(location_name == "None")
        {
            return 0;
        }

        static BOOST_CONSTEXPR const char* query =
            "SELECT location_id FROM location_names WHERE name=?";

        std::string error_message = "Invalid location: ";
        error_message += location_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query, location_name, error_message
               );
    }

    std::string move_id_to_name(
        int move_id,
        int generation
    )
    {
        if(move_id == 0)
        {
            return "None";
        }

        if(generation < 6)
        {
            static BOOST_CONSTEXPR const char* old_name_query =
                "SELECT name FROM old_move_names WHERE move_id=?";

            std::string old_ret;
            if(pkmn::database::maybe_query_db_bind1<std::string, int>(
                   old_name_query, old_ret, move_id
            ))
            {
                return old_ret;
            }
        }

        static BOOST_CONSTEXPR const char* main_query =
            "SELECT name FROM move_names WHERE move_id=? AND "
            "local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   main_query, move_id
               );
    }

    int move_name_to_id(
        const std::string& move_name
    )
    {
        if(move_name == "None")
        {
            return 0;
        }

        static BOOST_CONSTEXPR const char* old_name_query =
            "SELECT move_id FROM old_move_names WHERE name=?";

        int old_ret = 0;
        if(pkmn::database::maybe_query_db_bind1<int, const std::string&>(
               old_name_query, old_ret, move_name
        ))
        {
            return old_ret;
        }

        static BOOST_CONSTEXPR const char* main_query =
            "SELECT move_id FROM move_names WHERE name=?";

        std::string error_message = "Invalid move: ";
        error_message += move_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   main_query, move_name, error_message
               );
    }

    std::string nature_id_to_name(
        int nature_id
    )
    {
        if(nature_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM nature_names WHERE nature_id=? AND "
            "local_language_id=9";

        const std::string error_message = "Invalid nature: " + std::to_string(nature_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   nature_id,
                   error_message
               );
    }

    int nature_name_to_id(
        const std::string& nature_name
    )
    {
        if(nature_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT nature_id FROM nature_names WHERE name=?";

        const std::string error_message = "Invalid nature: " + nature_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   nature_name,
                   query
               );
    }

    std::string species_id_to_name(
        int species_id
    )
    {
        if(species_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM pokemon_species_names WHERE "
            "pokemon_species_id=? AND local_language_id=9";

        const std::string error_message = "Invalid species: " + std::to_string(species_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   species_id,
                   error_message
               );
    }

    int species_name_to_id(
        const std::string& species_name
    )
    {
        if(species_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT pokemon_species_id FROM pokemon_species_names WHERE name=?";

        std::string error_message = "Invalid species: ";
        error_message += species_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(), species_name, error_message
               );
    }

    std::string type_id_to_name(
        int type_id
    )
    {
        if(type_id == 0)
        {
            return "None";
        }

        static const std::string query =
            "SELECT name FROM type_names WHERE type_id=? AND "
            "local_language_id=9";

        const std::string error_message = "Invalid type: " + std::to_string(type_id);

        return pkmn::database::query_db_bind1<std::string, int>(
                   query.c_str(),
                   type_id,
                   error_message
               );
    }

    int type_name_to_id(
        const std::string& type_name
    )
    {
        if(type_name == "None")
        {
            return 0;
        }

        static const std::string query =
            "SELECT type_id FROM type_names WHERE name=?";

        const std::string error_message = "Invalid type: " + type_name;

        return pkmn::database::query_db_bind1<int, const std::string&>(
                   query.c_str(),
                   type_name,
                   error_message
               );
    }

}}
