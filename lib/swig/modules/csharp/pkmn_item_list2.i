/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_list.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::item_list2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_list2::item_list2();
%ignore pkmn::swig::item_list2::item_list2(const pkmn::item_list::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::item_list2::cptr() "private";

// Underlying function for indexing.
%csmethodmodifiers pkmn::swig::item_list2::at(int) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::item_list2, std::string, Name, get_name);
%attributestring(pkmn::swig::item_list2, std::string, Game, get_game);
%attribute(pkmn::swig::item_list2, int, Length, get_capacity);
%attribute(pkmn::swig::item_list2, int, NumItems, get_num_items);
%attributeval(pkmn::swig::item_list2, %arg(std::vector<std::string>), ValidItems, get_valid_items);

%typemap(cscode) pkmn::swig::item_list2
%{
    public ItemSlot2 this[int index]
    {
        get { return At(index); }
    }

    public bool Equals(ItemList2 other)
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

        ItemList2 otherAsItemList = other as ItemList2;
        if(otherAsItemList == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsItemList);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/item_list.hpp"
