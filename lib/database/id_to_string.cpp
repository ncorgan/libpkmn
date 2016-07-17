/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "id_to_string.hpp"

// TODO: database sptr

namespace pkmn { namespace database {

    int get_ability_id(
        const std::string &ability_name
    ) {
        (void)ability_name;
        return 0;
    }

    std::string get_ability_name(
        int ability_id,
        int language_id
    ) {
        (void)ability_id;
        (void)language_id;
        return "";
    }

    int get_ball_id(
        const std::string &ball_name
    ) {
        (void)ball_name;
        return 0;
    }

    std::string get_ball_name(
        int ball_id
    ) {
        (void)ball_id;
        return "";
    }

    int get_egg_group_id(
        const std::string &egg_group_name
    ) {
        (void)egg_group_name;
        return 0;
    }

    std::string get_egg_group_name(
        int egg_group_id,
        int language_id
    ) {
        (void)egg_group_id;
        (void)language_id;
        return "";
    }

    int get_game_id(
        const std::string &game_name
    ) {
        (void)game_name;
        return 0;
    }

    std::string get_game_name(
        int game_id,
        int language_id
    ) {
        (void)game_id;
        (void)language_id;
        return "";
    }

    int get_item_id(
        const std::string &item_name
    ) {
        (void)item_name;
        return 0;
    }

    std::string get_item_name(
        int item_id,
        int language_id
    ) {
        (void)item_id;
        (void)language_id;
        return "";
    }

    int get_item_list_id(
        const std::string &item_list_name,
        int version_group_id
    ) {
        (void)item_list_name;
        (void)version_group_id;
        return 0;
    }

    std::string get_item_list_name(
        int item_list_id,
        int version_group_id
    ) {
        (void)item_list_id;
        (void)version_group_id;
        return "";
    }

    int get_location_id(
        const std::string &location_name
    ) {
        (void)location_name;
        return 0;
    }

    std::string get_location_name(
        int location_id,
        int language_id
    ) {
        (void)location_id;
        (void)language_id;
        return "";
    }

    int get_move_id(
        const std::string &move_name
    ) {
        (void)move_name;
        return 0;
    }

    std::string get_move_name(
        int move_id,
        int generation,
        int language_id
    ) {
        (void)move_id;
        (void)generation;
        (void)language_id;
        return "";
    }

    int get_nature_id(
        const std::string &nature_name
    ) {
        (void)nature_name;
        return 0;
    }

    std::string get_nature_name(
        int nature_id,
        int language_id
    ) {
        (void)nature_id;
        (void)language_id;
        return "";
    }

    int get_species_id(
        const std::string &species_name
    ) {
        (void)species_name;
        return 0;
    }

    std::string get_species_name(
        int species_id,
        int language_id
    ) {
        (void)species_id;
        (void)language_id;
        return "";
    }

    int get_type_id(
        const std::string &type_name
    ) {
        (void)type_name;
        return 0;
    }

    std::string get_type_name(
        int type_id,
        int language_id
    ) {
        (void)type_id;
        (void)language_id;
        return "";
    }

}}
