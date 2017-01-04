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
    #include <pkmn/pokemon.hpp>
%}

%rename(pokemon_base) pkmn::pokemon;

%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__cptr "
private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__sptr_eq "
private";

%typemap(cscode) pkmn::shared_ptr<pkmn::pokemon> %{
    /// <summary>Compares two Pokemon instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">Pokemon with which to compare self</param>
    /// <returns>Whether or not Pokemon instances are equal</returns>
    /// </remarks>
    public bool Equals(Pokemon rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of Pokemon to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not Pokemon and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        Pokemon rhsSptr = rhs as Pokemon;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given Pokemon.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }

    public static readonly uint LIBPKMN_OT_ID = 2105214279;
    public static readonly string LIBPKMN_OT_NAME = "LibPKMN";
%}

%ignore LIBPKMN_OT_ID;
%ignore LIBPKMN_OT_NAME;
%ignore from_file;
%ignore get_native_pc_data;
%ignore get_native_party_data;
%include <pkmn/pokemon.hpp>
%template(Pokemon) pkmn::shared_ptr<pkmn::pokemon>;

%import <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::pokemon::sptr, Pokemon, PokemonList)
