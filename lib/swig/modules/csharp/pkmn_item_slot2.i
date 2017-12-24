/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

// TODO: equality, hash code

%{
    #include "cpp_wrappers/item_slot.hpp"
%}

%typemap(csimports) pkmn::swig::item_slot2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_slot2::item_slot2();
%ignore pkmn::swig::item_slot2::item_slot2(const pkmn::item_list::sptr&, int);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::item_slot2::get_item() "private";
%csmethodmodifiers pkmn::swig::item_slot2::set_item(const std::string&) "private";
%csmethodmodifiers pkmn::swig::item_slot2::get_amount() "private";
%csmethodmodifiers pkmn::swig::item_slot2::set_amount(int) "private";

%typemap(cscode) pkmn::swig::item_slot2
%{
    public string Item
    {
        get { return GetItem(); }
        set { SetItem(value); }
    }

    public int Amount
    {
        get { return GetAmount(); }
        set { SetAmount(value); }
    }
%}

%include "cpp_wrappers/item_slot.hpp"
