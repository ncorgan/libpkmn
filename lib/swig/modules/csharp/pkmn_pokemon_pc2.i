/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_pc.hpp"
%}

%typemap(csimports) pkmn::swig::pokemon_pc2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_pc2::pokemon_pc2();
%ignore pkmn::swig::pokemon_pc2::pokemon_pc2(const pkmn::pokemon_pc::sptr&);
%ignore pkmn::swig::pokemon_pc2::as_vector;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::pokemon_pc2::get_game() "private";
%csmethodmodifiers pkmn::swig::pokemon_pc2::get_num_boxes() "private";
%csmethodmodifiers pkmn::swig::pokemon_pc2::get_box(int) "private";
%csmethodmodifiers pkmn::swig::pokemon_pc2::get_box_names() "private";
%csmethodmodifiers pkmn::swig::pokemon_pc2::cptr() "private";

%typemap(cscode) pkmn::swig::pokemon_pc2
%{
    public string Game
    {
        get { return GetGame(); }
    }

    public int Length
    {
        get { return GetNumBoxes(); }
    }

    public PokemonBox2 this[int index]
    {
        get { return GetBox(index); }
    }

    public bool Equals(PokemonPC2 rhs)
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

        PokemonPC2 rhsAsPokemonPC2 = rhs as PokemonPC2;
        if(rhsAsPokemonPC2 == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsPokemonPC2);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_pc.hpp"
