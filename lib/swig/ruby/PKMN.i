/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN"

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%include <pkmn_statics.i>

%import <Database.i>
%import <STL.i>

%{
    #include <pkmn/item_slot.hpp>
%}

%include <pkmn/item_slot.hpp>
PKMN_RUBY_VECTOR(pkmn::item_slot, ItemSlotList);

%include <ruby/pkmn_item_list.i>
