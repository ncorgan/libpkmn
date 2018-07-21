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

%typemap(csimports) pkmn::swig::item_list "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::item_list::item_list();
%ignore pkmn::swig::item_list::item_list(const pkmn::item_list::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::item_list::cptr() "private";

// Underlying function for indexing.
%csmethodmodifiers pkmn::swig::item_list::at(int) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::item_list, std::string, Name, get_name);
%attribute(pkmn::swig::item_list, pkmn::e_game, Game, get_game);
%attribute(pkmn::swig::item_list, int, Length, get_capacity);
%attribute(pkmn::swig::item_list, int, NumItems, get_num_items);
%attributeval(pkmn::swig::item_list, %arg(std::vector<pkmn::e_item>), ValidItems, get_valid_items);
%attributeval(pkmn::swig::item_list, %arg(std::vector<std::string>), ValidItemNames, get_valid_item_names);

%typemap(cscode) pkmn::swig::item_list
%{
    public ItemSlot this[int index]
    {
        get { return At(index); }
    }

    public bool Equals(ItemList other)
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

        ItemList otherAsItemList = other as ItemList;
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
