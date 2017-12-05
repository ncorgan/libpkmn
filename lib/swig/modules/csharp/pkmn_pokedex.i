/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokedex.hpp>
%}

%rename(pokedex_base) pkmn::pokedex;

%csmethodmodifiers pkmn::pokedex::get_game "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokedex>::__cptr "private";
%csmethodmodifiers pkmn::shared_ptr<pkmn::pokedex>::__sptr_eq "private";

%typemap(cscode) pkmn::shared_ptr<pkmn::pokedex> %{
    public string Game
    {
        get
        {
            return GetGame();
        }
    }

    /// <summary>Compares two Pokedex instances to determine value equality.</summary>
    /// <remarks>
    /// Returns true if the internal shared_ptrs' pointers are equal.
    /// </remarks>
    /// <param name="rhs">Pokedex with which to compare self</param>
    /// <returns>Whether or not Pokedex instances are equal</returns>
    /// </remarks>
    public bool Equals(Pokedex rhs)
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
            return this.__sptr_eq(rhs);
        }
    }

    /// <summary>Compares an instance of Pokedex to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not Pokedex and Object are equal</returns>
    public override bool Equals(System.Object rhs)
    {
        if(rhs == null)
        {
            return false;
        }

        Pokedex rhsSptr = rhs as Pokedex;
        if(rhsSptr == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsSptr);
        }
    }

    /// <summary>Generates a unique hash code for the given Pokedex.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(__cptr())
                              .ToHashCode();
    }
%}

%include <pkmn/pokedex.hpp>
%template(Pokedex) pkmn::shared_ptr<pkmn::pokedex>;
