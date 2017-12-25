/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%typemap(csimports) pkmn::swig::pokemon_box2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_box2::pokemon_box2();
%ignore pkmn::swig::pokemon_box2::pokemon_box2(const pkmn::pokemon_box::sptr&);
%ignore pkmn::swig::pokemon_box2::as_vector;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::pokemon_box2::get_name() "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::set_name(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::get_game() "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::get_num_pokemon() "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::get_capacity() "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::get_pokemon() "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::set_pokemon(int, const pkmn::swig::pokemon2&) "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::cptr() "private";

%typemap(cscode) pkmn::swig::pokemon_box2
%{
    public string Name
    {
        get { return GetName(); }
        set { SetName(value); }
    }

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
        get { return GetCapacity(); }
    }

    public Pokemon2 this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(PokemonBox2 rhs)
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

        PokemonBox2 rhsAsPokemonBox2 = rhs as PokemonBox2;
        if(rhsAsPokemonBox2 == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsPokemonBox2);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_box.hpp"
