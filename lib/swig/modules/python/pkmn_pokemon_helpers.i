/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

%ignore pkmn::swig::EV_map::EV_map();
%ignore pkmn::swig::EV_map::EV_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::EV_map::cptr();

// Replace methods with more idiomatic Python.

%attributeval(pkmn::swig::EV_map, %arg(std::vector<pkmn::e_stat>), keys, keys);

%extend pkmn::swig::EV_map
{
    int __getitem__(pkmn::e_stat stat)
    {
        return self->get_EV(stat);
    }

    void __setitem__(
        pkmn::e_stat stat,
        int value
    )
    {
        self->set_EV(stat, value);
    }

    int __len__()
    {
        return int(self->keys().size());
    }
}

//
// IV map
//

%ignore pkmn::swig::IV_map::IV_map();
%ignore pkmn::swig::IV_map::IV_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::IV_map::cptr();

// Replace methods with more idiomatic Python.

%attributeval(pkmn::swig::IV_map, %arg(std::vector<pkmn::e_stat>), keys, keys);

%extend pkmn::swig::IV_map
{
    int __getitem__(pkmn::e_stat stat)
    {
        return self->get_IV(stat);
    }

    void __setitem__(
        pkmn::e_stat stat,
        int value
    )
    {
        self->set_IV(stat, value);
    }

    int __len__()
    {
        return int(self->keys().size());
    }
}

//
// Marking map
//

%ignore pkmn::swig::marking_map::marking_map();
%ignore pkmn::swig::marking_map::marking_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::marking_map::get_internal;

// Replace methods with more idiomatic Python.

%attributeval(pkmn::swig::marking_map, %arg(std::vector<pkmn::e_marking>), keys, keys);

%extend pkmn::swig::marking_map
{
    bool __getitem__(pkmn::e_marking marking)
    {
        return self->get_marking(marking);
    }

    void __setitem__(
        pkmn::e_marking marking,
        bool value
    )
    {
        self->set_marking(marking, value);
    }

    int __len__()
    {
        return int(self->keys().size());
    }
}

//
// Ribbon map
//

%ignore pkmn::swig::ribbon_map::ribbon_map();
%ignore pkmn::swig::ribbon_map::ribbon_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::ribbon_map::get_internal;

// Replace methods with more idiomatic Python.

%attributeval(pkmn::swig::ribbon_map, %arg(std::vector<std::string>), keys, keys);

%extend pkmn::swig::ribbon_map
{
    bool __getitem__(const std::string& ribbon)
    {
        return self->get_ribbon(ribbon);
    }

    void __setitem__(
        const std::string& ribbon,
        bool value
    )
    {
        self->set_ribbon(ribbon, value);
    }

    int __len__()
    {
        return int(self->keys().size());
    }
}

//
// Contest Stat map
//

%ignore pkmn::swig::contest_stat_map::contest_stat_map();
%ignore pkmn::swig::contest_stat_map::contest_stat_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::contest_stat_map::cptr();

// Replace methods with more idiomatic Python.

%attributeval(pkmn::swig::contest_stat_map, %arg(std::vector<pkmn::e_contest_stat>), keys, keys);

%extend pkmn::swig::contest_stat_map
{
    int __getitem__(pkmn::e_contest_stat stat)
    {
        return self->get_contest_stat(stat);
    }

    void __setitem__(
        pkmn::e_contest_stat stat,
        int value
    )
    {
        self->set_contest_stat(stat, value);
    }

    int __len__()
    {
        return int(self->keys().size());
    }
}

//
// Move slot
//

%ignore pkmn::swig::move_slot::move_slot();
%ignore pkmn::swig::move_slot::move_slot(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::move_slot::index();
%ignore pkmn::swig::move_slot::cptr();

// Replace methods with more idiomatic Python.

%attribute(pkmn::swig::move_slot, pkmn::e_move, move, get_move, set_move);
%attribute(pkmn::swig::move_slot, int, pp, get_pp, set_pp);

//
// Move slots
//

%ignore pkmn::swig::move_slots::move_slots();
%ignore pkmn::swig::move_slots::move_slots(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::move_slots::size();
%ignore pkmn::swig::move_slots::cptr();

// Replace methods with more idiomatic Python.

%extend pkmn::swig::move_slots
{
    pkmn::swig::move_slot __getitem__(int index)
    {
        return self->get_move_slot(index);
    }

    int __len__()
    {
        return int(self->size());
    }
}

%include "cpp_wrappers/pokemon_helpers.hpp"
