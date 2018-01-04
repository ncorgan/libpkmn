/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_party.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokemon_party2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_party2::pokemon_party2();
%ignore pkmn::swig::pokemon_party2::pokemon_party2(const pkmn::pokemon_party::sptr&);
%ignore pkmn::swig::pokemon_party2::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon_party2::cptr() "private";

// Underlying functions for indexing.
%csmethodmodifiers pkmn::swig::pokemon_party2::get_pokemon(int) "private";
%csmethodmodifiers pkmn::swig::pokemon_party2::set_pokemon(int, const pkmn::swig::pokemon2&) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon_party2, std::string, Game, get_game);
%attribute(pkmn::swig::pokemon_party2, int, NumPokemon, get_num_pokemon);

%typemap(cscode) pkmn::swig::pokemon_party2
%{
    public int Length
    {
        get { return 6; }
    }

    public Pokemon2 this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(PokemonParty2 other)
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

        PokemonParty2 otherAsPokemonParty = other as PokemonParty2;
        if(otherAsPokemonParty == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsPokemonParty);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_party.hpp"
