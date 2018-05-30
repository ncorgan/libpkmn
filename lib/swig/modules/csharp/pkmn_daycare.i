/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/daycare.hpp"
%}

%import <csharp/pkmn_daycare_helpers.i>

%include <attribute.i>

%typemap(csimports) pkmn::swig::daycare "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::daycare::daycare();
%ignore pkmn::swig::daycare::daycare(const pkmn::daycare::sptr&);
%ignore pkmn::swig::daycare::daycare(const pkmn::swig::daycare&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::daycare::cptr() "private";

// Convert getter/setter functions into attributes for more idiomatic C#.

%attributestring(pkmn::swig::daycare, std::string, Game, get_game);
%attribute(pkmn::swig::daycare, bool, CanBreedPokemon, can_breed_pokemon);
%attributeval(pkmn::swig::daycare, pkmn::swig::daycare_levelup_pokemon, LevelupPokemon, get_levelup_pokemon);
%attributeval(pkmn::swig::daycare, pkmn::swig::daycare_breeding_pokemon, BreedingPokemon, get_breeding_pokemon);

%typemap(cscode) pkmn::swig::daycare
%{
    public bool Equals(Daycare other)
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

        Daycare otherAsDaycare = other as Daycare;
        if(otherAsDaycare == null)
        {
            return false;
        }
        else
        {
            return this.Equals(otherAsDaycare);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/daycare.hpp"
