/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_bag.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::item_bag2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_bag2::item_bag2();
%ignore pkmn::swig::item_bag2::item_bag2(const pkmn::item_bag::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::item_bag2::cptr() "private";

// Underlying function for indexing.
%csmethodmodifiers pkmn::swig::item_bag2::get_pocket(const std::string&) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::item_bag2, std::string, Game, get_game);
%attributeval(pkmn::swig::item_bag2, %arg(std::vector<std::string>), PocketNames, get_pocket_names);

%typemap(cscode) pkmn::swig::item_bag2
%{
    public ItemList2 this[string pocketName]
    {
        get { return GetPocket(pocketName); }
    }

    public int Count
    {
        get { return PocketNames.Count; }
    }

    public bool Equals(ItemBag2 other)
    {
        if(other == null)
        {
            return false;
        }
        else if(this == other)
        {
            return true;
        }
        else
        {
            return (this.Cptr() == other.Cptr());
        }
    }

    public override bool Equals(System.Object other)
    {
        if(other == null)
        {
            return false;
        }

        ItemBag2 otherAsItemBag = other as ItemBag2;
        if(otherAsItemBag == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsItemBag);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/item_bag.hpp"
