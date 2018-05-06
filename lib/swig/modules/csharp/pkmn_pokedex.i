/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokedex.hpp"
%}

%import <csharp/pkmn_pokedex_helpers.i>

%include <attribute.i>

%typemap(csimports) pkmn::swig::pokedex "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokedex::pokedex();
%ignore pkmn::swig::pokedex::pokedex(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex::pokedex(const pkmn::swig::pokedex&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokedex::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::pokedex, std::string, Game, get_game);
%attribute(pkmn::swig::pokedex, int, NumSeen, get_num_seen);
%attributeval(pkmn::swig::pokedex, pkmn::swig::pokedex_has_seen_helper, SeenPokemonMap, get_has_seen);
%attributeval(pkmn::swig::pokedex, %arg(std::vector<std::string>), AllSeen, get_all_seen);
%attribute(pkmn::swig::pokedex, int, NumCaught, get_num_caught);
%attributeval(pkmn::swig::pokedex, pkmn::swig::pokedex_has_caught_helper, CaughtPokemonMap, get_has_caught);
%attributeval(pkmn::swig::pokedex, %arg(std::vector<std::string>), AllCaught, get_all_caught);

%typemap(cscode) pkmn::swig::pokedex
%{
    public bool Equals(Pokedex other)
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

        Pokedex otherAsPokedex = other as Pokedex;
        if(otherAsPokedex == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsPokedex);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/pokedex.hpp"
