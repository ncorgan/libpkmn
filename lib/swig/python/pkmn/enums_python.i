/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%{
    // Conveniently brings in every enum header
    #include <pkmn/enums/enum_to_string.hpp>

    // TODO: remove once in enum_to_string
    #include <pkmn/enums/marking.hpp>
%}

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
%include <pkmn/enums/move.hpp>
%include <pkmn/enums/move_damage_class.hpp>
%include <pkmn/enums/move_target.hpp>
%include <pkmn/enums/nature.hpp>
%include <pkmn/enums/species.hpp>
%include <pkmn/enums/stat.hpp>
%include <pkmn/enums/type.hpp>
