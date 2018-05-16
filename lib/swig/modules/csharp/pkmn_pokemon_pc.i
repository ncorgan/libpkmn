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

%typemap(csimports) pkmn::swig::pokemon_pc "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokemon_pc::pokemon_pc();
%ignore pkmn::swig::pokemon_pc::pokemon_pc(const pkmn::pokemon_pc::sptr&);
%ignore pkmn::swig::pokemon_pc::as_vector;

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokemon_pc::cptr() "private";

// Underlying functions for indexing.
%csmethodmodifiers pkmn::swig::pokemon_pc::get_box(int) "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokemon_pc, std::string, Game, get_game);
%attribute(pkmn::swig::pokemon_pc, int, Length, get_num_boxes);
%attributeval(pkmn::swig::pokemon_pc, %arg(std::vector<std::string>), BoxNames, get_box_names);

%typemap(cscode) pkmn::swig::pokemon_pc
%{
    public PokemonBox this[int index]
    {
        get { return GetBox(index); }
    }

    public bool Equals(PokemonPC rhs)
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

        PokemonPC rhsAsPokemonPC = rhs as PokemonPC;
        if(rhsAsPokemonPC == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsAsPokemonPC);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(Cptr())
                              .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_pc.hpp"
