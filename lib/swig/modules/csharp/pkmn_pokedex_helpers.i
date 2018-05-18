/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokedex_helpers.hpp"
%}

//
// Has Seen helper
//

%typemap(csimports) pkmn::swig::pokedex_has_seen_helper "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper();
%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex_has_seen_helper::pokedex_has_seen_helper(const pkmn::pokedex::sptr&, bool);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokedex_has_seen_helper::cptr() "private";

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::pokedex_has_seen_helper::get_has_seen(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokedex_has_seen_helper::set_has_seen(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::pokedex_has_seen_helper_map::cptr() "private";

%typemap(cscode) pkmn::swig::pokedex_has_seen_helper
%{
    public bool this[string species]
    {
        get { return GetHasSeen(species); }
        set { SetHasSeen(species, value); }
    }

    public bool Equals(PokedexHasSeenHelper rhs)
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

        PokedexHasSeenHelper rhsHelper = rhs as PokedexHasSeenHelper;
        if(rhsHelper == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsHelper);
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
// Has Caught helper
//

%typemap(csimports) pkmn::swig::pokedex_has_caught_helper "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::pokedex_has_caught_helper::pokedex_has_caught_helper();
%ignore pkmn::swig::pokedex_has_caught_helper::pokedex_has_caught_helper(const pkmn::pokedex::sptr&);
%ignore pkmn::swig::pokedex_has_caught_helper::pokedex_has_caught_helper(const pkmn::pokedex::sptr&, bool);

// Needed for equality check.
%csmethodmodifiers pkmn::swig::pokedex_has_caught_helper::cptr() "private";

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::pokedex_has_caught_helper::get_has_caught(const std::string&) "private";
%csmethodmodifiers pkmn::swig::pokedex_has_caught_helper::set_has_caught(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::pokedex_has_caught_helper_map::cptr() "private";

%typemap(cscode) pkmn::swig::pokedex_has_caught_helper
%{
    public bool this[string species]
    {
        get { return GetHasCaught(species); }
        set { SetHasCaught(species, value); }
    }

    public bool Equals(PokedexHasCaughtHelper rhs)
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

        PokedexHasCaughtHelper rhsHelper = rhs as PokedexHasCaughtHelper;
        if(rhsHelper == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsHelper);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<string>(this.GetType().Name)
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/pokedex_helpers.hpp"
