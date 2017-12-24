/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_list.hpp"
%}

%typemap(csimports) pkmn::swig::item_list2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_list2::item_list2();
%ignore pkmn::swig::item_list2::item_list2(const pkmn::item_list::sptr&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::item_list2::get_name() "private";
%csmethodmodifiers pkmn::swig::item_list2::get_game() "private";
%csmethodmodifiers pkmn::swig::item_list2::get_capacity() "private";
%csmethodmodifiers pkmn::swig::item_list2::get_num_items() "private";
%csmethodmodifiers pkmn::swig::item_list2::at(int) "private";
%csmethodmodifiers pkmn::swig::item_list2::get_valid_items() "private";
%csmethodmodifiers pkmn::swig::item_list2::cptr() "private";

%typemap(cscode) pkmn::swig::item_list2
%{
    public string Name
    {
        get { return GetName(); }
    }

    public string Game
    {
        get { return GetGame(); }
    }

    public int Length
    {
        get { return GetCapacity(); }
    }

    public int NumItems
    {
        get { return GetNumItems(); }
    }

    public ItemSlot2 this[int index]
    {
        get { return At(index); }
    }

    public StringList ValidItems
    {
        get { return GetValidItems(); }
    }

    public bool Equals(ItemList2 rhs)
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
            return (this.Cptr() == rhs.Cptr());
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        ItemList2 rhsAsItemList2 = rhs as ItemList2;
        if(rhsAsItemList2 == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsItemList2);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/item_list.hpp"
