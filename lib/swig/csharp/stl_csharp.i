/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

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
PKMN_CSHARP_MAP(pkmn::e_stat, int, Stat, int, StatDict);

// std::pair
PKMN_CSHARP_PAIR(int,         int,         int,    int,    IntPair);
PKMN_CSHARP_PAIR(std::string, std::string, string, string, StringPair);
PKMN_CSHARP_PAIR(pkmn::e_ability, pkmn::e_ability, PKMN.Ability, PKMN.Ability, AbilityEnumPair);
PKMN_CSHARP_PAIR(pkmn::e_egg_group, pkmn::e_egg_group, PKMN.EggGroup, PKMN.EggGroup, EggGroupEnumPair);
PKMN_CSHARP_PAIR(pkmn::e_type, pkmn::e_type, PKMN.Type, PKMN.Type, TypeEnumPair);

// std::vector
PKMN_CSHARP_VECTOR(int,                       int,                  IntList)
PKMN_CSHARP_VECTOR(std::string,               string,               StringList)
PKMN_CSHARP_VECTOR(pkmn::e_ability,           PKMN.Ability,         AbilityEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_ball,              PKMN.Ball,            BallEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_condition,         PKMN.Condition,       ConditionEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_contest_stat,      PKMN.ContestStat,     ContestStatEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_egg_group,         PKMN.EggGroup,        EggGroupEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_game,              PKMN.Game,            GameEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_gender,            PKMN.Gender,          GenderEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_item,              PKMN.Item,            ItemEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_language,          PKMN.Language,        LanguageEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_move,              PKMN.Move,            MoveEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_marking,           PKMN.Marking,         MarkingEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_move_damage_class, PKMN.MoveDamageClass, MoveDamageClassEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_move_target,       PKMN.MoveTarget,      MoveTargetEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_nature,            PKMN.Nature,          NatureEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_species,           PKMN.Species,         SpeciesEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_stat,              PKMN.Stat,            StatEnumList);
PKMN_CSHARP_VECTOR(pkmn::e_type,              PKMN.Type,            TypeEnumList);
