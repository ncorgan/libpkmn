/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

// TODO: equality, hash code

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
%ignore pkmn::swig::EV_map::get_internal;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::EV_map::get_EV(const std::string&) "private";
%csmethodmodifiers pkmn::swig::EV_map::set_EV(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::EV_map::swig_equals(const EV_map&) const "private";

%typemap(cscode) pkmn::swig::EV_map
%{
    public int this[string stat]
    {
        get { return GetEV(stat); }
        set { SetEV(stat, value); }
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
%ignore pkmn::swig::IV_map::get_internal;

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::IV_map::get_IV(const std::string&) "private";
%csmethodmodifiers pkmn::swig::IV_map::set_IV(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::IV_map::swig_equals(const IV_map&) const "private";

%typemap(cscode) pkmn::swig::IV_map
%{
    public int this[string stat]
    {
        get { return GetIV(stat); }
        set { SetIV(stat, value); }
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

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::marking_map::get_marking(const std::string&) "private";
%csmethodmodifiers pkmn::swig::marking_map::set_marking(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::marking_map::swig_equals(const marking_map&) const "private";

%typemap(cscode) pkmn::swig::marking_map
%{
    public bool this[string stat]
    {
        get { return GetMarking(stat); }
        set { SetMarking(stat, value); }
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

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::ribbon_map::get_ribbon(const std::string&) "private";
%csmethodmodifiers pkmn::swig::ribbon_map::set_ribbon(const std::string&, bool) "private";
%csmethodmodifiers pkmn::swig::ribbon_map::swig_equals(const ribbon_map&) const "private";

%typemap(cscode) pkmn::swig::ribbon_map
%{
    public bool this[string stat]
    {
        get { return GetRibbon(stat); }
        set { SetRibbon(stat, value); }
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

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::contest_stat_map::get_contest_stat(const std::string&) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::set_contest_stat(const std::string&, int) "private";
%csmethodmodifiers pkmn::swig::contest_stat_map::swig_equals(const contest_map&) const "private";

%typemap(cscode) pkmn::swig::contest_stat_map
%{
    public int this[string stat]
    {
        get { return GetContestStat(stat); }
        set { SetContestStat(stat, value); }
    }
%}

//
// Move slot
//

%typemap(csimports) pkmn::swig::move_slot2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::move_slot2::move_slot2();
%ignore pkmn::swig::move_slot2::move_slot2(const pkmn::pokemon::sptr&, int);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::move_slot2::get_move() "private";
%csmethodmodifiers pkmn::swig::move_slot2::set_move(const std::string&) "private";
%csmethodmodifiers pkmn::swig::move_slot2::get_pp() "private";
%csmethodmodifiers pkmn::swig::move_slot2::set_pp(int) "private";
%csmethodmodifiers pkmn::swig::move_slot2::swig_equals(const move_slot2&) const "private";

%typemap(cscode) pkmn::swig::move_slot2
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
%}

//
// Move slots
//

%typemap(csimports) pkmn::swig::move_slots2 "
using System;
using System.Runtime.InteropServices;"

%ignore pkmn::swig::move_slots2::move_slots2();
%ignore pkmn::swig::move_slots2::move_slots2(const pkmn::pokemon::sptr&, int);

// Make C++ methods private, replace with properties for more idiomatic C#.

%csmethodmodifiers pkmn::swig::move_slots2::get_move_slot(int) "private";
%csmethodmodifiers pkmn::swig::move_slots2::swig_equals(const move_slots2&) const "private";

%typemap(cscode) pkmn::swig::move_slots2
%{
    public MoveSlot2 this[int index]
    {
        get { return GetMoveSlot(index); }
    }
%}

%include "cpp_wrappers/pokemon_helpers.hpp"
