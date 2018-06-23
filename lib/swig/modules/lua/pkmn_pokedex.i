/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokedex.hpp"
%}

%import <lua/pkmn_pokedex_helpers.i>

%include <attribute.i>

%ignore pkmn::swig::pokedex::pokedex();
%ignore pkmn::swig::pokedex::pokedex(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex::pokedex(const pkmn::swig::pokedex&);

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attribute(pkmn::swig::pokedex, pkmn::e_game, game, get_game);
%attribute(pkmn::swig::pokedex, int, num_seen, get_num_seen);
%attributeval(pkmn::swig::pokedex, pkmn::swig::pokedex_has_seen_helper, seen_pokemon_map, get_has_seen);
%attributeval(pkmn::swig::pokedex, %arg(std::vector<std::string>), all_seen, get_all_seen);
%attribute(pkmn::swig::pokedex, int, num_caught, get_num_caught);
%attributeval(pkmn::swig::pokedex, pkmn::swig::pokedex_has_caught_helper, caught_pokemon_map, get_has_caught);
%attributeval(pkmn::swig::pokedex, %arg(std::vector<std::string>), all_caught, get_all_caught);

%include "cpp_wrappers/pokedex.hpp"
