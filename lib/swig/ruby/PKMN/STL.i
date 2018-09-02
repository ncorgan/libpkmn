/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::STL"

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%{
    // Conveniently brings in every enum header
    #include <pkmn/enums/enum_to_string.hpp>

    // TODO: remove once in enum_to_string
    #include <pkmn/enums/marking.hpp>
%}

/*
 * This file contains STL containers with either pure STL types or with LibPKMN
 * enums.
 */
%import <Enums.i>

%include <std_string.i>

// std::map
PKMN_RUBY_MAP(pkmn::e_stat, int, StatHash);

// std::pair
PKMN_RUBY_PAIR(int, int, IntPair);
PKMN_RUBY_PAIR(std::string, std::string, StringPair);
PKMN_RUBY_PAIR(pkmn::e_ability, pkmn::e_ability, AbilityEnumPair);
PKMN_RUBY_PAIR(pkmn::e_egg_group, pkmn::e_egg_group, EggGroupEnumPair);
PKMN_RUBY_PAIR(pkmn::e_type, pkmn::e_type, TypeEnumPair);

// std::vector
PKMN_RUBY_VECTOR(int,          IntList)
PKMN_RUBY_VECTOR(std::string,  StringList)
PKMN_RUBY_VECTOR(pkmn::e_ability, AbilityEnumList);
PKMN_RUBY_VECTOR(pkmn::e_ball, BallEnumList);
PKMN_RUBY_VECTOR(pkmn::e_condition, ConditionEnumList);
PKMN_RUBY_VECTOR(pkmn::e_contest_stat, ContestStatEnumList);
PKMN_RUBY_VECTOR(pkmn::e_egg_group, EggGroupEnumList);
PKMN_RUBY_VECTOR(pkmn::e_game, GameEnumList);
PKMN_RUBY_VECTOR(pkmn::e_gender, GenderEnumList);
PKMN_RUBY_VECTOR(pkmn::e_item, ItemEnumList);
PKMN_RUBY_VECTOR(pkmn::e_language, LanguageEnumList);
PKMN_RUBY_VECTOR(pkmn::e_marking, MarkingEnumList);
PKMN_RUBY_VECTOR(pkmn::e_move, MoveEnumList);
PKMN_RUBY_VECTOR(pkmn::e_move_damage_class, MoveDamageClassEnumList);
PKMN_RUBY_VECTOR(pkmn::e_move_target, MoveTargetEnumList);
PKMN_RUBY_VECTOR(pkmn::e_nature, NatureEnumList);
PKMN_RUBY_VECTOR(pkmn::e_species, SpeciesEnumList);
PKMN_RUBY_VECTOR(pkmn::e_stat, StatEnumList);
PKMN_RUBY_VECTOR(pkmn::e_type, TypeEnumList);
