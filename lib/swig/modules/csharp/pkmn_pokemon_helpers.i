/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_helpers.hpp"
%}

//
// EV map
//

%typemap(csimports) pkmn::swig::EV_map "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::EV_map::EV_map();
%ignore pkmn::swig::EV_map::EV_map(const pkmn::pokemon::sptr&);
%rename("KeysFunction") pkmn::swig::EV_map::keys();
%rename("ContainsKey") pkmn::swig::EV_map::has_key(const std::string&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::EV_map::get_EV(const std::string&) "private";
%csmethodmodifiers pkmn::swig::EV_map::set_EV(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::EV_map::size() "private";
%csmethodmodifiers pkmn::swig::EV_map::keys() "private";
%csmethodmodifiers pkmn::swig::EV_map::cptr() "private";

%typemap(cscode) pkmn::swig::EV_map
%{
    public int this[string stat]
    {
        get { return GetEV(stat); }
        set { SetEV(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StringList Keys
    {
        get { return KeysFunction(); }
    }

    public bool Equals(EVMap rhs)
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

        EVMap rhsMap = rhs as EVMap;
        if(rhsMap == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMap);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Keys)
                                       .ToHashCode();
    }
%}

//
// IV map
//

%typemap(csimports) pkmn::swig::IV_map "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::IV_map::IV_map();
%ignore pkmn::swig::IV_map::IV_map(const pkmn::pokemon::sptr&);
%rename("KeysFunction") pkmn::swig::IV_map::keys();
%rename("ContainsKey") pkmn::swig::IV_map::has_key(const std::string&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::IV_map::get_IV(const std::string&) "private";
%csmethodmodifiers pkmn::swig::IV_map::set_IV(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::IV_map::size() "private";
%csmethodmodifiers pkmn::swig::IV_map::keys() "private";
%csmethodmodifiers pkmn::swig::IV_map::cptr() "private";

%typemap(cscode) pkmn::swig::IV_map
%{
    public int this[string stat]
    {
        get { return GetIV(stat); }
        set { SetIV(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StringList Keys
    {
        get { return KeysFunction(); }
    }

    public bool Equals(IVMap rhs)
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

        IVMap rhsMap = rhs as IVMap;
        if(rhsMap == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMap);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Keys)
                                       .ToHashCode();
    }
%}

//
// Marking map
//

%typemap(csimports) pkmn::swig::marking_map "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::marking_map::marking_map();
%ignore pkmn::swig::marking_map::marking_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::marking_map::get_internal;
%rename("KeysFunction") pkmn::swig::marking_map::keys();
%rename("ContainsKey") pkmn::swig::marking_map::has_key(const std::string&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::marking_map::get_marking(const std::string&) "private";
%csmethodmodifiers pkmn::swig::marking_map::set_marking(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::marking_map::size() "private";
%csmethodmodifiers pkmn::swig::marking_map::keys() "private";
%csmethodmodifiers pkmn::swig::marking_map::cptr() "private";

%typemap(cscode) pkmn::swig::marking_map
%{
    public bool this[string stat]
    {
        get { return GetMarking(stat); }
        set { SetMarking(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StringList Keys
    {
        get { return KeysFunction(); }
    }

    public bool Equals(MarkingMap rhs)
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

        MarkingMap rhsMap = rhs as MarkingMap;
        if(rhsMap == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMap);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Keys)
                                       .ToHashCode();
    }
%}

//
// Ribbon map
//

%typemap(csimports) pkmn::swig::ribbon_map "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::ribbon_map::ribbon_map();
%ignore pkmn::swig::ribbon_map::ribbon_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::ribbon_map::get_internal;
%rename("KeysFunction") pkmn::swig::ribbon_map::keys();
%rename("ContainsKey") pkmn::swig::ribbon_map::has_key(const std::string&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::ribbon_map::get_ribbon(const std::string&) "private";
%csmethodmodifiers pkmn::swig::ribbon_map::set_ribbon(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::ribbon_map::size() "private";
%csmethodmodifiers pkmn::swig::ribbon_map::keys() "private";
%csmethodmodifiers pkmn::swig::ribbon_map::cptr() "private";

%typemap(cscode) pkmn::swig::ribbon_map
%{
    public bool this[string stat]
    {
        get { return GetRibbon(stat); }
        set { SetRibbon(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StringList Keys
    {
        get { return KeysFunction(); }
    }

    public bool Equals(RibbonMap rhs)
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

        RibbonMap rhsMap = rhs as RibbonMap;
        if(rhsMap == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMap);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Keys)
                                       .ToHashCode();
    }
%}

//
// Contest stat map
//

%typemap(csimports) pkmn::swig::contest_stat_map "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::contest_stat_map::contest_stat_map();
%ignore pkmn::swig::contest_stat_map::contest_stat_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::contest_stat_map::get_internal;
%rename("KeysFunction") pkmn::swig::contest_stat_map::keys();
%rename("ContainsKey") pkmn::swig::contest_stat_map::has_key(const std::string&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::contest_stat_map::get_contest_stat(const std::string&) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::set_contest_stat(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::size() "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::keys() "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::cptr() "private";

%typemap(cscode) pkmn::swig::contest_stat_map
%{
    public int this[string stat]
    {
        get { return GetContestStat(stat); }
        set { SetContestStat(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StringList Keys
    {
        get { return KeysFunction(); }
    }

    public bool Equals(ContestStatMap rhs)
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

        ContestStatMap rhsMap = rhs as ContestStatMap;
        if(rhsMap == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMap);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<StringList>(this.Keys)
                                       .ToHashCode();
    }
%}

//
// Move slot
//

%typemap(csimports) pkmn::swig::move_slot "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::move_slot::move_slot();
%ignore pkmn::swig::move_slot::move_slot(const pkmn::pokemon::sptr&, int);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::move_slot::get_move() "private";
%csmethodmodifiers pkmn::swig::move_slot::set_move(const std::string&) "private";
%csmethodmodifiers pkmn::swig::move_slot::get_pp() "private";
%csmethodmodifiers pkmn::swig::move_slot::set_pp(int) "private";
%csmethodmodifiers pkmn::swig::move_slot::cptr() "private";
%csmethodmodifiers pkmn::swig::move_slot::index() "private";

%typemap(cscode) pkmn::swig::move_slot
%{
    public string Move
    {
        get { return GetMove(); }
        set { SetMove(value); }
    }

    public int PP
    {
        get { return GetPP(); }
        set { SetPP(value); }
    }

    public bool Equals(MoveSlot rhs)
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

        MoveSlot rhsMoveSlot = rhs as MoveSlot;
        if(rhsMoveSlot == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMoveSlot);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<int>(this.Index())
                                       .ToHashCode();
    }
%}

//
// Move slots
//

%typemap(csimports) pkmn::swig::move_slots "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::move_slots::move_slots();
%ignore pkmn::swig::move_slots::move_slots(const pkmn::pokemon::sptr&);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::move_slots::get_move_slot(int) "private";
%csmethodmodifiers pkmn::swig::move_slots::size() "private";
%csmethodmodifiers pkmn::swig::move_slots::cptr() "private";

%typemap(cscode) pkmn::swig::move_slots
%{
    public MoveSlot this[int index]
    {
        get { return GetMoveSlot(index); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public bool Equals(MoveSlots rhs)
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

        MoveSlots rhsMoveSlots = rhs as MoveSlots;
        if(rhsMoveSlots == null)
        {
            return false;
        }
        else
        {
            return this.Equals(rhsMoveSlots);
        }
    }

    public override int GetHashCode()
    {
        return HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                       .AddValue<int>(this.Count)
                                       .ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_helpers.hpp"
