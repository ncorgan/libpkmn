/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

// TODO: equality, hash code

%{
    #include "cpp_wrappers/item_slot.hpp"
%}

%typemap(csimports) pkmn::swig::item_slot "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_slot::item_slot();
%ignore pkmn::swig::item_slot::item_slot(const pkmn::item_list::sptr&, int);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokedex_has_seen_helper::cptr() "private";

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::item_slot::get_item() "private";
%csmethodmodifiers pkmn::swig::item_slot::set_item(const std::string&) "private";
%csmethodmodifiers pkmn::swig::item_slot::get_amount() "private";
%csmethodmodifiers pkmn::swig::item_slot::set_amount(int) "private";
%csmethodmodifiers pkmn::swig::item_slot::index() "private";

%typemap(cscode) pkmn::swig::item_slot
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

    public bool Equals(ItemSlot rhs)
    {
        if(rhs == null)
        {
            return false;
        }
        else if(this == rhs)
        {
            return true;
        }
        else
        {
            return (this.Cptr() == rhs.Cptr()) &&
                   (this.Index() == rhs.Index());
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        ItemSlot rhsItemSlot = rhs as ItemSlot;
        if(rhsItemSlot == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsItemSlot);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<int>(this.Index())
                                       .AddValue<string>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/item_slot.hpp"
