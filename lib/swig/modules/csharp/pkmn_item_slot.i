/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%import <csharp/stl_macros.i>

%{
    #include <pkmn/item_slot.hpp>
%}

%typemap(csimports) pkmn::item_slot "
using System;
using System.Runtime.InteropServices;
using Database;"

%include <pkmn/item_slot.hpp>
PKMN_CSHARP_VECTOR(pkmn::item_slot, ItemSlot, ItemSlotList)
