/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%import <csharp/stl_macros.i>

%{
    #include <pkmn/move_slot.hpp>
%}

%typemap(csimports) pkmn::move_slot "
using System;
using System.Runtime.InteropServices;
using Database;"

%include <pkmn/move_slot.hpp>
PKMN_CSHARP_VECTOR(pkmn::move_slot, MoveSlot, MoveSlotList)
