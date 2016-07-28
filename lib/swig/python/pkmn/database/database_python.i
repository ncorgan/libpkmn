/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>
%include <std_vector.i>

%{
    #include <pkmn/database/item_entry.hpp>
    #include <pkmn/database/lists.hpp>
    #include <pkmn/database/move_entry.hpp>
    #include <pkmn/database/pokemon_entry.hpp>
%}

%include <python/pkmn_levelup_move.i>

%include <pkmn/database/item_entry.hpp>
%include <pkmn/database/lists.hpp>
%include <pkmn/database/move_entry.hpp>
%include <pkmn/database/pokemon_entry.hpp>
