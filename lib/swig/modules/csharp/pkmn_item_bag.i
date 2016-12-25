/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Given that this is supposed to be a list, we'll add the []
 * operator to treat it as such. But we can't use the cscode typemap
 * twice, so we can't use the usual SPTR macro.
 */

%{
    #include <pkmn/item_bag.hpp>
%}

%rename(item_bag_base) pkmn::item_bag;
%rename(AsList) as_vector;

%csmethodmodifiers pkmn::shared_ptr<pkmn::item_bag>::__cptr "
private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::item_bag>::__sptr_eq "
private";

%typemap(cscode) pkmn::shared_ptr<pkmn::item_bag> %{
    /// <summary>Gets the pocket with the given name.</summary>
    /// <exception cref="System.SystemException">If index is invalid</exception>
    /// <param name="key">Key</param>
    public ItemList this[string key] {
        get {
            return this.GetPocket(key);
        }
    }

    /// <summary>Compares two ItemBag instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">ItemBag with which to compare self</param>
    /// <returns>Whether or not ItemBag instances are equal</returns>
    /// </remarks>
    public bool Equals(ItemBag rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of ItemBag to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not ItemBag and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        ItemBag rhsSptr = rhs as ItemBag;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given ItemBag.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        ulong cptr = __cptr();
        return new HashCodeBuilder<ItemBag>(this)
                   .With(m => cptr)
                   .HashCode;
    }
%}

%include <pkmn/item_bag.hpp>
%template(ItemBag) pkmn::shared_ptr<pkmn::item_bag>;
