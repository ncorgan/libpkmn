/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>

%import <database/database_python.i>
%import <stl_python.i>

%{
    #include <pkmn/build_info.hpp>

    #include <pkmn/game_save.hpp>
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>

    #include <pkmn/utils/paths.hpp>
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%include <pkmn/build_info.hpp>
%include <pkmn/utils/paths.hpp>

// Item Slot
%include <python/pkmn_item_slot.i>

// For all sptrs, for some reason didn't get ignored from pkmn.i
%ignore make;
%ignore get_native;

// Item List
%include <python/pkmn_item_list.i>

// Item Bag
%include <python/pkmn_item_bag.i>

// Game Save
%ignore detect_type;
%include <pkmn/game_save.hpp>
PKMN_PYTHON_SPTR(game_save)

/*
 * Remove access to bases of sptr'd classes, but don't give access to our
 * thin make wrapper.
 */
%rename(__make_game_save)   make_game_save;
%rename(__make_item_list)   make_item_list;
%rename(__make_item_bag)    make_item_bag;
%include <pkmn_statics.i>
%pythoncode %{
    game_save     = __make_game_save;
    item_list     = __make_item_list;
    item_bag      = __make_item_bag;
%}
