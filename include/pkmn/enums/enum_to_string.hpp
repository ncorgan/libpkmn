/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_ENUM_TO_STRING_HPP
#define PKMN_ENUMS_ENUM_TO_STRING_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/ability.hpp>
#include <pkmn/enums/ball.hpp>
#include <pkmn/enums/condition.hpp>
#include <pkmn/enums/egg_group.hpp>
#include <pkmn/enums/game.hpp>
#include <pkmn/enums/gender.hpp>
#include <pkmn/enums/item.hpp>
#include <pkmn/enums/language.hpp>
#include <pkmn/enums/move_damage_class.hpp>
#include <pkmn/enums/move.hpp>
#include <pkmn/enums/nature.hpp>
#include <pkmn/enums/species.hpp>
#include <pkmn/enums/stat.hpp>
#include <pkmn/enums/type.hpp>

#include <string>

namespace pkmn
{
    PKMN_API std::string ability_to_string(
        pkmn::e_ability ability
    );

    PKMN_API pkmn::e_ability string_to_ability(
        const std::string& ability_name
    );

    PKMN_API std::string ball_to_string(
        pkmn::e_ball ball
    );

    PKMN_API pkmn::e_ball string_to_ball(
        const std::string& ball_name
    );

    PKMN_API std::string condition_to_string(
        pkmn::e_condition condition
    );

    PKMN_API pkmn::e_condition string_to_condition(
        const std::string& condition_name
    );

    PKMN_API std::string egg_group_to_string(
        pkmn::e_egg_group egg_group
    );

    PKMN_API pkmn::e_egg_group string_to_egg_group(
        const std::string& egg_group_name
    );

    PKMN_API std::string game_to_string(
        pkmn::e_game game
    );

    PKMN_API pkmn::e_game string_to_game(
        const std::string& game_name
    );

    PKMN_API std::string gender_to_string(
        pkmn::e_gender gender
    );

    PKMN_API pkmn::e_gender string_to_gender(
        const std::string& gender_name
    );

    PKMN_API std::string item_to_string(
        pkmn::e_item item
    );

    PKMN_API pkmn::e_item string_to_item(
        const std::string& item_name
    );

    PKMN_API std::string language_to_string(
        pkmn::e_language language
    );

    PKMN_API pkmn::e_language string_to_language(
        const std::string& language_name
    );

    PKMN_API std::string move_damage_class_to_string(
        pkmn::e_move_damage_class move_damage_class
    );

    PKMN_API pkmn::e_move_damage_class string_to_move_damage_class(
        const std::string& move_damage_class_name
    );

    PKMN_API std::string move_to_string(
        pkmn::e_move move
    );

    PKMN_API pkmn::e_move string_to_move(
        const std::string& move_name
    );

    PKMN_API std::string nature_to_string(
        pkmn::e_nature nature
    );

    PKMN_API pkmn::e_nature string_to_nature(
        const std::string& nature_name
    );

    PKMN_API std::string species_to_string(
        pkmn::e_species species
    );

    PKMN_API pkmn::e_species string_to_species(
        const std::string& species_name
    );

    PKMN_API std::string stat_to_string(
        pkmn::e_stat stat
    );

    PKMN_API pkmn::e_stat string_to_stat(
        const std::string& stat_name
    );

    PKMN_API std::string type_to_string(
        pkmn::e_type type
    );

    PKMN_API pkmn::e_type string_to_type(
        const std::string& type_name
    );
}

#endif /* PKMN_ENUMS_ENUM_TO_STRING_HPP */
