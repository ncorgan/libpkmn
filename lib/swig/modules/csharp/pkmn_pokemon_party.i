/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
    #include <pkmn/pokemon_party.hpp>
%}

%rename(pokemon_party_base) pkmn::pokemon_party;
%rename(AsList) as_vector;

%csmethodmodifiers pkmn::pokemon_party::get_game "private";
%csmethodmodifiers pkmn::pokemon_party::get_num_pokemon "private";
%csmethodmodifiers pkmn::pokemon_party::get_pokemon(int) "private";
%csmethodmodifiers pkmn::pokemon_party::set_pokemon(int, pkmn::pokemon::sptr) "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon_party>::__cptr "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokemon_party>::__sptr_eq "private";

%typemap(cscode) pkmn::shared_ptr<pkmn::pokemon_party> %{
    public string Game {
        get {
            return GetGame();
        }
    }

    public int NumPokemon {
        get {
            return GetNumPokemon();
        }
    }

    public int Count {
        get {
            return 6;
        }
    }

    /// <summary>Gets the Pokémon at the given index.</summary>
    /// <exception cref="System.SystemException">If index is invalid</exception>
    /// <param name="index">Index</param>
    public Pokemon this[int index] {
        get {
            return this.GetPokemon(index);
        }
        set {
            this.SetPokemon(index, value);
        }
    }

    /// <summary>Compares two PokemonParty instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">PokemonParty with which to compare self</param>
    /// <returns>Whether or not PokemonParty instances are equal</returns>
    /// </remarks>
    public bool Equals(PokemonParty rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of PokemonParty to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not PokemonParty and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        PokemonParty rhsSptr = rhs as PokemonParty;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given PokemonParty.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%include <pkmn/pokemon_party.hpp>
%template(PokemonParty) pkmn::shared_ptr<pkmn::pokemon_party>;
