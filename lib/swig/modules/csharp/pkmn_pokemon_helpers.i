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
%rename("ContainsKey") pkmn::swig::EV_map::has_key(pkmn::e_stat);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::EV_map::get_EV(pkmn::e_stat) "private";
%csmethodmodifiers pkmn::swig::EV_map::set_EV(pkmn::e_stat, int) "private";
%csmethodmodifiers pkmn::swig::EV_map::size() "private";
%csmethodmodifiers pkmn::swig::EV_map::keys() "private";
%csmethodmodifiers pkmn::swig::EV_map::cptr() "private";

%typemap(cscode) pkmn::swig::EV_map
%{
    public int this[Stat stat]
    {
        get { return GetEV(stat); }
        set { SetEV(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StatEnumList Keys
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
                                       .AddValue<StatEnumList>(this.Keys)
                                       .AddValue<string>(this.GetType().Name)
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
%rename("ContainsKey") pkmn::swig::IV_map::has_key(pkmn::e_stat);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::IV_map::get_IV(pkmn::e_stat) "private";
%csmethodmodifiers pkmn::swig::IV_map::set_IV(pkmn::e_stat, int) "private";
%csmethodmodifiers pkmn::swig::IV_map::size() "private";
%csmethodmodifiers pkmn::swig::IV_map::keys() "private";
%csmethodmodifiers pkmn::swig::IV_map::cptr() "private";

%typemap(cscode) pkmn::swig::IV_map
%{
    public int this[Stat stat]
    {
        get { return GetIV(stat); }
        set { SetIV(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public StatEnumList Keys
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
                                       .AddValue<StatEnumList>(this.Keys)
                                       .AddValue<string>(this.GetType().Name)
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
%rename("ContainsKey") pkmn::swig::marking_map::has_key(pkmn::e_marking);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::marking_map::get_marking(pkmn::e_marking) "private";
%csmethodmodifiers pkmn::swig::marking_map::set_marking(pkmn::e_marking, bool) "private";
%csmethodmodifiers pkmn::swig::marking_map::size() "private";
%csmethodmodifiers pkmn::swig::marking_map::keys() "private";
%csmethodmodifiers pkmn::swig::marking_map::cptr() "private";

%typemap(cscode) pkmn::swig::marking_map
%{
    public bool this[Marking marking]
    {
        get { return GetMarking(marking); }
        set { SetMarking(marking, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public MarkingEnumList Keys
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
                                       .AddValue<MarkingEnumList>(this.Keys)
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
%rename("ContainsKey") pkmn::swig::contest_stat_map::has_key(pkmn::e_contest_stat);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::contest_stat_map::get_contest_stat(pkmn::e_contest_stat) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::set_contest_stat(pkmn::e_contest_stat, int) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::size() "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::keys() "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::cptr() "private";

%typemap(cscode) pkmn::swig::contest_stat_map
%{
    public int this[ContestStat stat]
    {
        get { return GetContestStat(stat); }
        set { SetContestStat(stat, value); }
    }

    public int Count
    {
        get { return (int)size(); }
    }

    public ContestStatEnumList Keys
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
                                       .AddValue<ContestStatEnumList>(this.Keys)
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
    public Move Move
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
            return (this.Cptr() == rhs.Cptr()) &&
                   (this.Index() == rhs.Index());
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
                                       .AddValue<string>(this.GetType().Name)
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
        HashCodeBuilder hashCodeBuilder = HashCodeBuilder.Create().AddValue<ulong>(this.Cptr())
                                                                  .AddValue<int>(this.Count);
        for(int index = 0; index < this.Count; ++index)
        {
            hashCodeBuilder = hashCodeBuilder.AddValue<MoveSlot>(this[index]);
        }

        return hashCodeBuilder.ToHashCode();
    }
%}

%include "cpp_wrappers/pokemon_helpers.hpp"
