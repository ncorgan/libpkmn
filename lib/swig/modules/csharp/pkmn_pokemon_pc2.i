/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_pc.hpp"
%}

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokemon_pc2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_pc2::pokemon_pc2();
%ignore pkmn::swig::pokemon_pc2::pokemon_pc2(const pkmn::pokemon_pc::sptr&);
%ignore pkmn::swig::pokemon_pc2::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon_pc2::cptr() "private";

// Underlying functions for indexing.
%csmethodmodifiers pkmn::swig::pokemon_pc2::get_box(int) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon_pc2, std::string, Game, get_game);
%attribute(pkmn::swig::pokemon_pc2, int, Length, get_num_boxes);
%attributeval(pkmn::swig::pokemon_pc2, %arg(std::vector<std::string>), BoxNames, get_box_names);

%typemap(cscode) pkmn::swig::pokemon_pc2
%{
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
