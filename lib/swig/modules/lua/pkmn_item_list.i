/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_list.hpp"

    #include <pkmn/exception.hpp>
%}

%include <attribute.i>

%ignore pkmn::swig::item_list::item_list();
%ignore pkmn::swig::item_list::item_list(const pkmn::item_list::sptr&);
%ignore pkmn::swig::item_list::cptr();
%ignore pkmn::swig::item_list::at(int);
%ignore pkmn::swig::item_list::get_capacity();

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::swig::item_list, std::string, name, get_name);
%attribute(pkmn::swig::item_list, pkmn::e_game, game, get_game);
%attribute(pkmn::swig::item_list, int, num_items, get_num_items);
%attributeval(pkmn::swig::item_list, %arg(std::vector<std::string>), valid_items, get_valid_items);

%extend pkmn::swig::item_list
{
    pkmn::swig::item_slot __getitem__(
        size_t position
    )
    {
        return self->at(int(position));
    }

    size_t __len(void*)
    {
        return self->get_capacity();
    }
}

%include "cpp_wrappers/item_list.hpp"
