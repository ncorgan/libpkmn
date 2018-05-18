/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_slot.hpp"
%}

%include <attribute.i>

%attribute(pkmn::swig::item_slot, std::string, item, get_item, set_item);
%attribute(pkmn::swig::item_slot, int, amount, get_amount, set_amount);

%include "cpp_wrappers/item_slot.hpp"
