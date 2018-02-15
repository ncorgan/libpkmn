/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
    #include <pkmn/pokemon_box.hpp>
%}

%rename(pokemon_box_base) pkmn::pokemon_box;
%rename(AsList) as_vector;

%csmethodmodifiers pkmn::pokemon_box::get_name "private";
%csmethodmodifiers pkmn::pokemon_box::set_name(const std::string&) "private";
%csmethodmodifiers pkmn::pokemon_box::get_game "private";
%csmethodmodifiers pkmn::pokemon_box::get_num_pokemon "private";
%csmethodmodifiers pkmn::pokemon_box::get_capacity "private";
%csmethodmodifiers pkmn::pokemon_box::get_pokemon(int) "private";
%csmethodmodifiers pkmn::pokemon_box::set_pokemon(int, pkmn::pokemon::sptr) "private";
%csmethodmodifiers std::shared_ptr<pkmn::pokemon_box>::__cptr "private";
%csmethodmodifiers std::shared_ptr<pkmn::pokemon_box>::__sptr_eq "private";

%typemap(cscode) std::shared_ptr<pkmn::pokemon_box> %{

    public string Name {
        get {
            return GetName();
        }
        set {
            SetName(value);
        }
    }

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
            return GetCapacity();
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

    /// <summary>Compares two PokemonBox instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">PokemonBox with which to compare self</param>
    /// <returns>Whether or not PokemonBox instances are equal</returns>
    /// </remarks>
    public bool Equals(PokemonBox rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of PokemonBox to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not PokemonBox and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        PokemonBox rhsSptr = rhs as PokemonBox;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given PokemonBox.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%include <pkmn/pokemon_box.hpp>
%template(PokemonBox) std::shared_ptr<pkmn::pokemon_box>;

%import <csharp/stl_macros.i>
PKMN_CSHARP_VECTOR(pkmn::pokemon_box::sptr, PokemonBox, PokemonBoxList)
