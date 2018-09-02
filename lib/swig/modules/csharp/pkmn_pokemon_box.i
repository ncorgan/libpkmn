/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokemon_box "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_box::pokemon_box();
%ignore pkmn::swig::pokemon_box::pokemon_box(const pkmn::pokemon_box::sptr&);
%ignore pkmn::swig::pokemon_box::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon_box::cptr() "private";

// Underlying functions for indexing.
%csmethodmodifiers pkmn::swig::pokemon_box::get_pokemon(int) "private";
%csmethodmodifiers pkmn::swig::pokemon_box::set_pokemon(int, const pkmn::swig::pokemon&) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon_box, std::string, Name, get_name, set_name);
%attribute(pkmn::swig::pokemon_box, pkmn::e_game, Game, get_game);
%attribute(pkmn::swig::pokemon_box, int, NumPokemon, get_num_pokemon);
%attribute(pkmn::swig::pokemon_box, int, Length, get_capacity);

%typemap(cscode) pkmn::swig::pokemon_box
%{
    public Pokemon this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(PokemonBox rhs)
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

        PokemonBox rhsAsPokemonBox = rhs as PokemonBox;
        if(rhsAsPokemonBox == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsPokemonBox);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_box.hpp"
