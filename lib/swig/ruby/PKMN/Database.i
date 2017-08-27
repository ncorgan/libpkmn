/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::Database"

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%include <std_string.i>

%import <STL.i>

%{
    #include <pkmn/database/levelup_move.hpp>
    #include <pkmn/database/lists.hpp>
%}

%include <pkmn/database/lists.hpp>
%include <ruby/pkmn_item_entry.i>
%include <ruby/pkmn_move_entry.i>

%include <pkmn/database/levelup_move.hpp>
PKMN_RUBY_VECTOR(pkmn::database::levelup_move, LevelupMoveList)

%include <ruby/pkmn_pokemon_entry.i>
