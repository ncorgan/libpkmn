/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/daycare.hpp"
%}

%import <lua/pkmn_daycare_helpers.i>

%include <attribute.i>

%ignore pkmn::swig::daycare::daycare(const pkmn::daycare::sptr&);

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::swig::daycare, std::string, game, get_game);
%attribute(pkmn::swig::daycare, bool, can_breed_pokemon, can_breed_pokemon);
%attributeval(pkmn::swig::daycare, pkmn::swig::daycare_levelup_pokemon, levelup_pokemon, get_levelup_pokemon);
%attributeval(pkmn::swig::daycare, pkmn::swig::daycare_breeding_pokemon, breeding_pokemon, get_breeding_pokemon);

%include "cpp_wrappers/daycare.hpp"
