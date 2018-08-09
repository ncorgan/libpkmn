/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%{
    #include <pkmn/enums/enum_to_string.hpp>

    // TODO: remove once in enum_to_string
    #include <pkmn/enums/marking.hpp>
%}

/*
 * This file contains STL containers with either pure STL types or with LibPKMN
 * enums.
 */

%include <std_string.i>

// std::map
PKMN_LUA_MAP(pkmn::e_stat, int, stat_dict);

// std::pair
PKMN_LUA_PAIR(int, int, int_pair);
PKMN_LUA_PAIR(std::string, std::string, string_pair);
PKMN_LUA_PAIR(pkmn::e_ability, pkmn::e_ability, ability_enum_pair);
PKMN_LUA_PAIR(pkmn::e_egg_group, pkmn::e_egg_group, egg_group_enum_pair);
PKMN_LUA_PAIR(pkmn::e_type, pkmn::e_type, type_enum_pair);

// std::vector
PKMN_LUA_VECTOR(int,          int_list)
PKMN_LUA_VECTOR(std::string,  string_list)
PKMN_LUA_VECTOR(pkmn::e_ability, ability_enum_list);
PKMN_LUA_VECTOR(pkmn::e_ball, ball_enum_list);
PKMN_LUA_VECTOR(pkmn::e_condition, condition_enum_list);
PKMN_LUA_VECTOR(pkmn::e_contest_stat, contest_stat_enum_list);
PKMN_LUA_VECTOR(pkmn::e_egg_group, egg_group_enum_list);
PKMN_LUA_VECTOR(pkmn::e_game, game_enum_list);
PKMN_LUA_VECTOR(pkmn::e_gender, gender_enum_list);
PKMN_LUA_VECTOR(pkmn::e_item, item_enum_list);
PKMN_LUA_VECTOR(pkmn::e_language, language_enum_list);
PKMN_LUA_VECTOR(pkmn::e_marking, marking_enum_list);
PKMN_LUA_VECTOR(pkmn::e_move, move_enum_list);
PKMN_LUA_VECTOR(pkmn::e_move_damage_class, move_damage_class_enum_list);
PKMN_LUA_VECTOR(pkmn::e_move_target, move_target_enum_list);
PKMN_LUA_VECTOR(pkmn::e_nature, nature_enum_list);
PKMN_LUA_VECTOR(pkmn::e_species, species_enum_list);
PKMN_LUA_VECTOR(pkmn::e_stat, stat_enum_list);
PKMN_LUA_VECTOR(pkmn::e_type, type_enum_list);
