/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>
%}

%include <std_string.i>

%rename(as_list) as_vector;
%include <pkmn/item_list.hpp>

%extend std::shared_ptr<pkmn::item_list> {

    const pkmn::item_slot& __getitem__(
        int index
    ) throw (std::out_of_range)
    {
        if (index == 0)
            throw std::out_of_range("Lua lists are 1-based");
        return self->get()->as_vector().at(index-1);
    }

    bool __eq__(
        const pkmn::item_list::sptr &rhs
    )
    {
        return (self->get() == rhs.get());
    }

    int __len(void*)
    {
        return int(self->get()->get_capacity());
    }

}
%template(item_list_sptr) std::shared_ptr<pkmn::item_list>;

PKMN_LUA_MAP(std::string, pkmn::item_list::sptr, item_pockets);
