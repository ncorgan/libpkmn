/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%{
    #include <pkmn/enums/enum_to_string.hpp>

    // TODO: remove once in enum_to_string
    #include <pkmn/enums/marking.hpp>
%}

/*
 * This file contains STL containers with either pure STL types or with LibPKMN
 * enums. For convenience, the enums will be here as well.
 */

%include <pkmn/enums/stat.hpp>
%include <pkmn/enums/ability.hpp>
%include <pkmn/enums/ball.hpp>
%include <pkmn/enums/condition.hpp>
%include <pkmn/enums/contest_stat.hpp>
%include <pkmn/enums/egg_group.hpp>
%include <pkmn/enums/game.hpp>
%include <pkmn/enums/gender.hpp>
%include <pkmn/enums/item.hpp>
%include <pkmn/enums/language.hpp>
%include <pkmn/enums/marking.hpp>

%rename(SING) SING_MOVE;
%include <pkmn/enums/move.hpp>

%include <pkmn/enums/move_damage_class.hpp>
%include <pkmn/enums/move_target.hpp>
%include <pkmn/enums/nature.hpp>
%include <pkmn/enums/species.hpp>
%include <pkmn/enums/stat.hpp>
%include <pkmn/enums/type.hpp>

%include <std_string.i>

// std::map
PKMN_PYTHON_MAP(pkmn::e_stat, int, stat_dict);

// std::pair
PKMN_PYTHON_PAIR(int, int, int_pair);
PKMN_PYTHON_PAIR(std::string, std::string, string_pair);
PKMN_PYTHON_PAIR(pkmn::e_ability, pkmn::e_ability, ability_enum_pair);
PKMN_PYTHON_PAIR(pkmn::e_egg_group, pkmn::e_egg_group, egg_group_enum_pair);
PKMN_PYTHON_PAIR(pkmn::e_type, pkmn::e_type, type_enum_pair);

// std::vector
PKMN_PYTHON_VECTOR(int,          int_list)
PKMN_PYTHON_VECTOR(std::string,  string_list)
PKMN_PYTHON_VECTOR(pkmn::e_ability, ability_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_ball, ball_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_condition, condition_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_contest_stat, contest_stat_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_egg_group, egg_group_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_game, game_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_gender, gender_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_item, item_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_language, language_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_marking, marking_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_move, move_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_move_damage_class, move_damage_class_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_move_target, move_target_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_nature, nature_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_species, species_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_stat, stat_enum_list);
PKMN_PYTHON_VECTOR(pkmn::e_type, type_enum_list);
