/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_bag.hpp"
%}

%typemap(csimports) pkmn::swig::item_bag2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_bag2::item_bag2();
%ignore pkmn::swig::item_bag2::item_bag2(const pkmn::item_bag::sptr&);

// Make C++ methods private, replace with properties for more idiomatic
// C#.

%csmethodmodifiers pkmn::swig::item_bag2::get_game() "private";
%csmethodmodifiers pkmn::swig::item_bag2::get_pocket(const std::string&) "private";
%csmethodmodifiers pkmn::swig::item_bag2::get_pocket_names() "private";
%csmethodmodifiers pkmn::swig::item_bag2::cptr() "private";

%typemap(cscode) pkmn::swig::item_bag2
%{
    public string Game
    {
        get { return GetGame(); }
    }

    public ItemList2 this[string pocketName]
    {
        get { return GetPocket(pocketName); }
    }

    public int Count
    {
        get { return PocketNames.Count; }
    }

    public StringList PocketNames
    {
        get { return GetPocketNames(); }
    }

    public bool Equals(ItemBag2 rhs)
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

        ItemBag2 rhsAsItemBag = rhs as ItemBag2;
        if(rhsAsItemBag == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsItemBag);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/item_bag.hpp"
