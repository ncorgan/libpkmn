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

    std::string ability_id_to_name(
        int ability_id
    );

    int ability_name_to_id(
        const std::string &ability_name
    );

    /*
     * Poké Balls
     */

    std::string ball_id_to_name(
        int ball_id
    );

    int ball_name_to_id(
        const std::string &ball_name
    );

    /*
     * Egg Groups
     */

    std::string egg_group_id_to_name(
        int egg_group_id
    );

    int egg_group_name_to_id(
        const std::string &egg_group_name
    );

    /*
     * Games
     */

    std::string game_id_to_name(
        int game_id
    );

    int game_name_to_id(
        const std::string &game_name
    );

    /*
     * Items
     */

    std::string item_id_to_name(
        int item_id
    );

    int item_name_to_id(
        const std::string &item_name
    );

    /*
     * Item Lists
     */

    std::string item_list_id_to_name(
        int item_list_id,
        int version_group_id
    );

    int item_list_name_to_id(
        const std::string &item_list_name,
        int version_group_id
    );

    /*
     * Locations
     */

    std::string location_id_to_name(
        int location_id
    );

    int location_name_to_id(
        const std::string &location_name
    );

    /*
     * Moves
     *
     * Move names have changed over the years, so take an extra parameter
     * for generation.
     */

    std::string move_id_to_name(
        int move_id,
        int generation
    );

    int move_name_to_id(
        const std::string &move_name
    );

    /*
     * Natures
     */

    std::string nature_id_to_name(
        int nature_id
    );

    int nature_name_to_id(
        const std::string &nature_name
    );

    /*
     * Species
     */

    std::string species_id_to_name(
        int species_id
    );

    int species_name_to_id(
        const std::string &species_name
    );

    /*
     * Types
     */

    std::string type_id_to_name(
        int type_id
    );

    int type_name_to_id(
        const std::string &type_name
    );

}}

#endif /* PKMN_DATABASE_ID_TO_STRING_HPP */
