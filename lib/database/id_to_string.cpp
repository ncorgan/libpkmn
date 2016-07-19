/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <boost/config.hpp>

// TODO: database sptr

namespace pkmn { namespace database {

    std::string ability_id_to_name(
        int ability_id,
        int language_id
    ) {
        (void)ability_id;
        (void)language_id;
        return "";
    }

    int ability_name_to_id(
        const std::string &ability_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT ability_id FROM ability_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, ability_name
               );
    }

    std::string ball_id_to_name(
        int ball_id
    ) {
        (void)ball_id;
        return "";
    }

    int ball_name_to_id(
        const std::string &ball_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT id FROM balls WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, ball_name
               );
    }

    std::string egg_group_id_to_name(
        int egg_group_id,
        int language_id
    ) {
        (void)egg_group_id;
        (void)language_id;
        return "";
    }

    int egg_group_name_to_id(
        const std::string &egg_group_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT egg_group_id FROM egg_group_prose WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, egg_group_name
               );
    }

    std::string game_id_to_name(
        int game_id,
        int language_id
    ) {
        (void)game_id;
        (void)language_id;
        return "";
    }

    int game_name_to_id(
        const std::string &game_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT version_id FROM version_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, game_name
               );
    }

    std::string item_id_to_name(
        int item_id,
        int language_id
    ) {
        (void)item_id;
        (void)language_id;
        return "";
    }

    int item_name_to_id(
        const std::string &item_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT item_id FROM item_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, item_name
               );
    }

    std::string item_list_id_to_name(
        int item_list_id,
        int version_group_id
    ) {
        (void)item_list_id;
        (void)version_group_id;
        return "";
    }

    int item_list_name_to_id(
        const std::string &item_list_name,
        int version_group_id
    ) {
        (void)item_list_name;
        (void)version_group_id;
        return 0;
    }

    std::string location_id_to_name(
        int location_id,
        int language_id
    ) {
        (void)location_id;
        (void)language_id;
        return "";
    }

    int location_name_to_id(
        const std::string &location_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT location_id FROM location_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, location_name
               );
    }

    std::string move_id_to_name(
        int move_id,
        int generation,
        int language_id
    ) {
        (void)move_id;
        (void)generation;
        (void)language_id;
        return "";
    }

    int move_name_to_id(
        const std::string &move_name
    ) {
        (void)move_name;
        return 0;
    }

    std::string nature_id_to_name(
        int nature_id,
        int language_id
    ) {
        (void)nature_id;
        (void)language_id;
        return "";
    }

    int nature_name_to_id(
        const std::string &nature_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT nature_id FROM nature_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, nature_name
               );
    }

    std::string species_id_to_name(
        int species_id,
        int language_id
    ) {
        (void)species_id;
        (void)language_id;
        return "";
    }

    int species_name_to_id(
        const std::string &species_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT pokemon_species_id FROM pokemon_species_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, species_name
               );
    }

    std::string type_id_to_name(
        int type_id,
        int language_id
    ) {
        (void)type_id;
        (void)language_id;
        return "";
    }

    int type_name_to_id(
        const std::string &type_name
    ) {
        static BOOST_CONSTEXPR const char* query = \
            "SELECT type_id FROM type_names WHERE name=?";

        return pkmn_db_query_bind1<int, const std::string&>(
                   query, type_name
               );
    }

}}
