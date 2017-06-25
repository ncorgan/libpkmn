/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>
%}

%typemap(csimports) pkmn::item_list "
using System;
using System.Runtime.InteropServices;
using Database;"

%typemap(csimports) pkmn::shared_ptr<pkmn::item_list> "
using System;
using System.Runtime.InteropServices;
using Database;"

%rename(item_list_base) pkmn::item_list;
%rename(AsList) as_vector;

%csmethodmodifiers pkmn::item_list::get_name() "private";
%csmethodmodifiers pkmn::item_list::get_game() "private";
%csmethodmodifiers pkmn::item_list::get_capacity() "private";
%csmethodmodifiers pkmn::item_list::get_num_items() "private";
%csmethodmodifiers pkmn::item_list::at(int) "private";
%csmethodmodifiers pkmn::item_list::set_item(int, pkmn::database::item_entry const&) "private";
%csmethodmodifiers pkmn::item_list::get_valid_items() "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::item_list>::__cptr "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::item_list>::__sptr_eq "private";

%typemap(cscode) pkmn::shared_ptr<pkmn::item_list> %{
    public string Name {
        get { return GetName(); }
    }

    public string Game {
        get { return GetGame(); }
    }

    public int Capacity {
        get { return GetCapacity(); }
    }

    public int NumItems {
        get { return GetNumItems(); }
    }

    public StringList ValidItems {
        get { return GetValidItems(); }
    }

    /// <summary>Gets the item slot at the given index.</summary>
    /// <exception cref="System.SystemException">If index is invalid</exception>
    /// <param name="index">Index</param>
    public ItemSlot this[int index] {
        get {
            return this.At(index);
        }
        set {
            this.SetItem(index, value);
        }
    }

    /// <summary>Compares two ItemList instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">ItemList with which to compare self</param>
    /// <returns>Whether or not ItemList instances are equal</returns>
    /// </remarks>
    public bool Equals(ItemList rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of ItemList to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not ItemList and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        ItemList rhsSptr = rhs as ItemList;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given ItemList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%include <pkmn/item_list.hpp>
%template(ItemList) pkmn::shared_ptr<pkmn::item_list>;
