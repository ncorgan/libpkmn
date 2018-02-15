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
    #include <pkmn/pokemon_pc.hpp>
%}

%rename(pokemon_pc_base) pkmn::pokemon_pc;
%rename(AsList) as_vector;

%csmethodmodifiers pkmn::pokemon_pc::get_game "private";
%csmethodmodifiers pkmn::pokemon_pc::get_num_boxes "private";
%csmethodmodifiers pkmn::pokemon_pc::get_box "private";
%csmethodmodifiers pkmn::pokemon_pc::get_box_names "private";
%csmethodmodifiers std::shared_ptr<pkmn::pokemon_pc>::__cptr "private";
%csmethodmodifiers std::shared_ptr<pkmn::pokemon_pc>::__sptr_eq "private";

%typemap(cscode) std::shared_ptr<pkmn::pokemon_pc> %{
    public string Game {
        get {
            return GetGame();
        }
    }

    public int Count {
        get {
            return GetNumBoxes();
        }
    }

    public StringList BoxNames {
        get {
            return GetBoxNames();
        }
    }

    /// <summary>Gets the box at the given index.</summary>
    /// <exception cref="System.SystemException">If index is invalid</exception>
    /// <param name="index">Index</param>
    public PokemonBox this[int index] {
        get {
            return this.GetBox(index);
        }
    }

    /// <summary>Compares two PokemonPC instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">PokemonPC with which to compare self</param>
    /// <returns>Whether or not PokemonPC instances are equal</returns>
    /// </remarks>
    public bool Equals(PokemonPC rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of PokemonPC to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not PokemonPC and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        PokemonPC rhsSptr = rhs as PokemonPC;
        if(rhsSptr == null) {
            return false;
        } else {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given PokemonPC.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                                       .ToHashCode();
    }
%}

%include <pkmn/pokemon_pc.hpp>
%template(PokemonPC) std::shared_ptr<pkmn::pokemon_pc>;
