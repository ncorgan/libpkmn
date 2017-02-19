/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT

%{
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>
%}

%import <database_java.i>

// Include but hide static makes
%javamethodmodifiers make_item_bag "";
%javamethodmodifiers make_item_list "";
%include <pkmn_statics.i>

// Item Slot
%include <java/pkmn_item_slot.i>

// For all sptrs, for some reason didn't get ignored from pkmn.i
%ignore make;
%ignore get_native;

// Item List
PKMN_JAVA_INCLUDE_AND_SPTR(item_list, ItemList);
PKMN_JAVA_MAP(std::string, pkmn::shared_ptr<pkmn::item_list>, String, ItemList, StringVector, StringItemListMap);

// Item Bag
PKMN_JAVA_INCLUDE_AND_SPTR(item_bag, ItemBag);
