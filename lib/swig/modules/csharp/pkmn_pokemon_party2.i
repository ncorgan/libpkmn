/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_party.hpp"
%}

%typemap(csimports) pkmn::swig::pokemon_party2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_party2::pokemon_party2();
%ignore pkmn::swig::pokemon_party2::pokemon_party2(const pkmn::pokemon_party::sptr&);
%ignore pkmn::swig::pokemon_party2::as_vector;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::pokemon_party2::get_game() "private";
%csmethodmodifiers pkmn::swig::pokemon_party2::get_num_pokemon() "private";
%csmethodmodifiers pkmn::swig::pokemon_party2::get_pokemon() "private";
%csmethodmodifiers pkmn::swig::pokemon_party2::set_pokemon(int, const pkmn::swig::pokemon2&) "private";
%csmethodmodifiers pkmn::swig::pokemon_party2::cptr() "private";

%typemap(cscode) pkmn::swig::pokemon_party2
%{
    public string Game
    {
        get { return GetGame(); }
    }

    public int NumPokemon
    {
        get { return GetNumPokemon(); }
    }

    public int Length
    {
        get { return 6; }
    }

    public Pokemon2 this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(PokemonParty2 rhs)
    {
        if(rhs == null)
        {
            return false;
        }
        else if(this == rhs)
        {
            return true;
        }
        else
        {
            return (this.Cptr() == rhs.Cptr());
        }
    }

    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        PokemonParty2 rhsAsPokemonParty2 = rhs as PokemonParty2;
        if(rhsAsPokemonParty2 == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsPokemonParty2);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_party.hpp"
