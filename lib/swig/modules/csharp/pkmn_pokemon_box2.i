/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokemon_box2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_box2::pokemon_box2();
%ignore pkmn::swig::pokemon_box2::pokemon_box2(const pkmn::pokemon_box::sptr&);
%ignore pkmn::swig::pokemon_box2::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon_box2::cptr() "private";

// Underlying functions for indexing.
%csmethodmodifiers pkmn::swig::pokemon_box2::get_pokemon(int) "private";
%csmethodmodifiers pkmn::swig::pokemon_box2::set_pokemon(int, const pkmn::swig::pokemon2&) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon_box2, std::string, Name, get_name, set_name);
%attributestring(pkmn::swig::pokemon_box2, std::string, Game, get_game);
%attribute(pkmn::swig::pokemon_box2, int, NumPokemon, get_num_pokemon);
%attribute(pkmn::swig::pokemon_box2, int, Length, get_capacity);

%typemap(cscode) pkmn::swig::pokemon_box2
%{
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
