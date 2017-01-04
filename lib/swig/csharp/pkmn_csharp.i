/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <database_csharp.i>
%import <stl_csharp.i>

%{
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_slot.hpp>
    #include <pkmn/item_bag.hpp>

    #include <pkmn/utils/paths.hpp>
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_csharp_docs.i>

// For all sptrs, for some reason didn't get ignored from pkmn.i
%ignore make;
%ignore get_native;

// Bring in our wrapped make functions, don't expose them
%csmethodmodifiers detect_game_save_type "
internal";
%csmethodmodifiers make_game_save "
internal";
%csmethodmodifiers make_item_bag "
internal";
%csmethodmodifiers make_item_list "
internal";
%include <pkmn_statics.i>

// Item Slot
%include <pkmn/item_slot.hpp>
PKMN_CSHARP_VECTOR(pkmn::item_slot, ItemSlot, ItemSlotList)

// Item List
%include <csharp/pkmn_item_list.i>
PKMN_CSHARP_MAP(std::string, pkmn::item_list::sptr, String, ItemList, ItemPockets);

// Item Bag
%include <csharp/pkmn_item_bag.i>

// Game Save
%include <csharp/pkmn_game_save.i>
