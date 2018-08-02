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

// Replace methods with more idiomatic Ruby.

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

    size_t __len__()
    {
        return self->keys().size();
    }

    pkmn::swig::EV_map* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE k, v;
        std::vector<pkmn::e_stat> keys = self->keys();
        for(pkmn::e_stat key: keys)
        {
            k = SWIG_From_int(int(key));
            v = SWIG_From_int(self->get_EV(key));
            rb_yield_values(2, k, v);
        }

        return self;
    }
}

//
// IV map
//

%ignore pkmn::swig::IV_map::IV_map();
%ignore pkmn::swig::IV_map::IV_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::IV_map::cptr();

// Replace methods with more idiomatic Ruby.

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

    size_t __len__()
    {
        return int(self->keys().size());
    }

    pkmn::swig::IV_map* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE k, v;
        std::vector<pkmn::e_stat> keys = self->keys();
        for(pkmn::e_stat key: keys)
        {
            k = SWIG_From_int(int(key));
            v = SWIG_From_int(self->get_IV(key));
            rb_yield_values(2, k, v);
        }

        return self;
    }
}

//
// Marking map
//

%ignore pkmn::swig::marking_map::marking_map();
%ignore pkmn::swig::marking_map::marking_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::marking_map::get_internal;

// Replace methods with more idiomatic Ruby.

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

    size_t __len__()
    {
        return int(self->keys().size());
    }

    pkmn::swig::marking_map* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE k, v;
        std::vector<pkmn::e_marking> keys = self->keys();
        for(pkmn::e_marking key: keys)
        {
            k = SWIG_From_int(int(key));
            v = self->get_marking(key) ? Qtrue : Qfalse;
            rb_yield_values(2, k, v);
        }

        return self;
    }
}

//
// Ribbon map
//

%ignore pkmn::swig::ribbon_map::ribbon_map();
%ignore pkmn::swig::ribbon_map::ribbon_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::ribbon_map::get_internal;

// Replace methods with more idiomatic Ruby.

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

    size_t __len__()
    {
        return int(self->keys().size());
    }

    pkmn::swig::ribbon_map* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE k, v;
        std::vector<std::string> keys = self->keys();
        for(const std::string& key: keys)
        {
            k = SWIG_From_std_string(key);
            v = self->get_ribbon(key) ? Qtrue : Qfalse;
            rb_yield_values(2, k, v);
        }

        return self;
    }
}

//
// Contest Stat map
//

%ignore pkmn::swig::contest_stat_map::contest_stat_map();
%ignore pkmn::swig::contest_stat_map::contest_stat_map(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::contest_stat_map::cptr();

// Replace methods with more idiomatic Ruby.

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

    size_t __len__()
    {
        return int(self->keys().size());
    }

    pkmn::swig::contest_stat_map* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE k, v;
        std::vector<pkmn::e_contest_stat> keys = self->keys();
        for(pkmn::e_contest_stat key: keys)
        {
            k = SWIG_From_int(int(key));
            v = SWIG_From_int(self->get_contest_stat(key));
            rb_yield_values(2, k, v);
        }

        return self;
    }
}

//
// Move slot
//

%ignore pkmn::swig::move_slot::move_slot();
%ignore pkmn::swig::move_slot::move_slot(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::move_slot::index();
%ignore pkmn::swig::move_slot::cptr();

// Replace methods with more idiomatic Ruby.

%attribute(pkmn::swig::move_slot, pkmn::e_move, move, get_move, set_move);
%attribute(pkmn::swig::move_slot, int, pp, get_pp, set_pp);

//
// Move slots
//

// Needed to avoid compile error
%{
    namespace swig
    {
        template <> struct traits<pkmn::swig::move_slot>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "pkmn::swig::move_slot";
            }
        };
    }
%}

%ignore pkmn::swig::move_slots::move_slots();
%ignore pkmn::swig::move_slots::move_slots(const pkmn::pokemon::sptr&);
%ignore pkmn::swig::move_slots::size();
%ignore pkmn::swig::move_slots::cptr();

// Replace methods with more idiomatic Ruby.

%extend pkmn::swig::move_slots
{
    pkmn::swig::move_slot __getitem__(int index)
    {
        return self->get_move_slot(index);
    }

    size_t __len__()
    {
        return int(self->size());
    }

    pkmn::swig::move_slots* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE r;
        for(size_t move_index = 0; move_index < self->size(); ++move_index)
        {
            r = swig::from<pkmn::swig::move_slot>(self->get_move_slot(move_index));
            rb_yield(r);
        }

        return self;
    }
}

%include "cpp_wrappers/pokemon_helpers.hpp"
