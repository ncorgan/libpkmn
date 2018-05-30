/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/daycare_helpers.hpp"
%}

%include <attribute.i>

//
// Levelup Pokémon
//

%typemap(csimports) pkmn::swig::daycare_levelup_pokemon "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::daycare_levelup_pokemon::daycare_levelup_pokemon(const pkmn::daycare::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::daycare_levelup_pokemon::cptr() "private";

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::daycare_levelup_pokemon::get_pokemon(int) "private";
%csmethodmodifiers pkmn::swig::daycare_levelup_pokemon::set_pokemon(int, const pkmn::swig::pokemon&) "private";
%csmethodmodifiers pkmn::swig::daycare_levelup_pokemon_map::cptr() "private";

%attribute(pkmn::swig::daycare_levelup_pokemon, int, Length, get_capacity)

%typemap(cscode) pkmn::swig::daycare_levelup_pokemon
%{
    public Pokemon this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(DaycareLevelupPokemon rhs)
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

        DaycareLevelupPokemon rhsPokemon = rhs as DaycareLevelupPokemon;
        if(rhsPokemon == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsPokemon);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<string>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

//
// Breeding Pokémon
//

%typemap(csimports) pkmn::swig::daycare_breeding_pokemon "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::daycare_breeding_pokemon::daycare_breeding_pokemon(const pkmn::daycare::sptr&);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::daycare_breeding_pokemon::cptr() "private";

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::daycare_breeding_pokemon::get_pokemon(int) "private";
%csmethodmodifiers pkmn::swig::daycare_breeding_pokemon::set_pokemon(int, const pkmn::swig::pokemon&) "private";
%csmethodmodifiers pkmn::swig::daycare_breeding_pokemon_map::cptr() "private";

%attribute(pkmn::swig::daycare_breeding_pokemon, int, Length, get_capacity)

%typemap(cscode) pkmn::swig::daycare_breeding_pokemon
%{
    public Pokemon this[int index]
    {
        get { return GetPokemon(index); }
        set { SetPokemon(index, value); }
    }

    public bool Equals(DaycareBreedingPokemon rhs)
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

        DaycareBreedingPokemon rhsPokemon = rhs as DaycareBreedingPokemon;
        if(rhsPokemon == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsPokemon);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<string>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/daycare_helpers.hpp"
