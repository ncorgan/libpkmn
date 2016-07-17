/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ID_TO_STRING_HPP
#define PKMN_DATABASE_ID_TO_STRING_HPP

#include <string>

/*
 * These functions convert between database ID's and their corresponding
 * strings.
 *
 * TODO: language enum
 */

namespace pkmn { namespace database {

    /*
     * Abilities
     */

    int get_ability_id(
        const std::string &ability_name
    );

    std::string get_ability_name(
        int ability_id,
        int language_id = 9
    );

    /*
     * Poké Balls
     *
     * Currently only in English.
     */

    int get_ball_id(
        const std::string &ball_name
    );

    std::string get_ball_name(
        int ball_id
    );

    /*
     * Egg Groups
     */

    int get_egg_group_id(
        const std::string &egg_group_name
    );

    std::string get_egg_group_name(
        int egg_group_id,
        int language_id = 9
    );

    /*
     * Games
     */

    int get_game_id(
        const std::string &game_name
    );

    std::string get_game_name(
        int game_id,
        int language_id = 9
    );

    /*
     * Items
     */

    int get_item_id(
        const std::string &item_name
    );

    std::string get_item_name(
        int item_id,
        int language_id = 9
    );

    /*
     * Item Lists
     *
     * Currently only in English.
     */

    int get_item_list_id(
        const std::string &item_list_name,
        int version_group_id
    );

    std::string get_item_list_name(
        int item_list_id,
        int version_group_id
    );

    /*
     * Locations
     */

    int get_location_id(
        const std::string &location_name
    );

    std::string get_location_name(
        int location_id,
        int language_id = 9
    );

    /*
     * Moves
     *
     * Move names have changed over the years, so take an extra parameter
     * for generation. Some names are only in English.
     */

    int get_move_id(
        const std::string &move_name
    );

    std::string get_move_name(
        int move_id,
        int generation,
        int language_id = 9
    );

    /*
     * Natures
     */

    int get_nature_id(
        const std::string &nature_name
    );

    std::string get_nature_name(
        int nature_id,
        int language_id = 9
    );

    /*
     * Species
     */

    int get_species_id(
        const std::string &species_name
    );

    std::string get_species_name(
        int species_id,
        int language_id = 9
    );

    /*
     * Types
     */

    int get_type_id(
        const std::string &type_name
    );

    std::string get_type_name(
        int type_id,
        int language_id = 9
    );
}}

#endif /* PKMN_DATABASE_ID_TO_STRING_HPP */
