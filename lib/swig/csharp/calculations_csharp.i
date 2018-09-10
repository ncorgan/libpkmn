/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module(directors="0") Calculations

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <stdint.i>
%include <std_string.i>

%import <stl_csharp.i>

%{
    #include <pkmn/calculations/form.hpp>
    #include <pkmn/calculations/gender.hpp>
    #include <pkmn/calculations/nature.hpp>
    #include <pkmn/calculations/personality.hpp>
    #include <pkmn/calculations/shininess.hpp>
    #include <pkmn/calculations/size.hpp>
    #include <pkmn/calculations/spinda_spots.hpp>
    #include <pkmn/calculations/stats.hpp>

    #include <pkmn/calculations/moves/critical_hit.hpp>
    #include <pkmn/calculations/moves/damage.hpp>
    #include <pkmn/calculations/moves/hidden_power.hpp>
    #include <pkmn/calculations/moves/modifiers.hpp>
    #include <pkmn/calculations/moves/natural_gift.hpp>
    #include <pkmn/calculations/moves/power.hpp>
%}

// Convert Doxygen docs to C# docstrings
%include <pkmn_csharp_docs.i>

%include <pkmn/enums/game.hpp>
%include <pkmn/enums/gender.hpp>
%include <pkmn/enums/stat.hpp>

%rename(STABModifier) STAB_MODIFIER;
%include <pkmn/calculations/moves/critical_hit.hpp>
%include <pkmn/calculations/moves/damage.hpp>
%include <pkmn/calculations/moves/hidden_power.hpp>
%include <pkmn/calculations/moves/modifiers.hpp>
%include <pkmn/calculations/moves/natural_gift.hpp>
%include <pkmn/calculations/moves/power.hpp>

%include <pkmn/calculations/form.hpp>
%include <pkmn/calculations/gender.hpp>
%include <pkmn/calculations/nature.hpp>
%include <pkmn/calculations/personality.hpp>
%include <pkmn/calculations/shininess.hpp>
%include <pkmn/calculations/size.hpp>
%include <pkmn/calculations/spinda_spots.hpp>
%include <pkmn/calculations/stats.hpp>
