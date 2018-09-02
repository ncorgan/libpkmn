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

%typemap(csimports) pkmn::swig::item_bag "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_bag::item_bag();
%ignore pkmn::swig::item_bag::item_bag(const pkmn::item_bag::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::item_bag::cptr() "private";

// Underlying function for indexing.
%csmethodmodifiers pkmn::swig::item_bag::get_pocket(const std::string&) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attribute(pkmn::swig::item_bag, pkmn::e_game, Game, get_game);
%attributeval(pkmn::swig::item_bag, %arg(std::vector<std::string>), PocketNames, get_pocket_names);

%typemap(cscode) pkmn::swig::item_bag
%{
    public ItemList this[string pocketName]
    {
        get { return GetPocket(pocketName); }
    }

    public int Count
    {
        get { return PocketNames.Count; }
    }

    public bool Equals(ItemBag other)
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

        ItemBag otherAsItemBag = other as ItemBag;
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
