/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/item_list.hpp"
%}

%include <std_string.i>

%include <attribute.i>

%ignore pkmn::swig::item_list::item_list();
%ignore pkmn::swig::item_list::item_list(const pkmn::item_list::sptr&);
%ignore pkmn::swig::item_list::cptr();
%ignore pkmn::swig::item_list::at(int);
%ignore pkmn::swig::item_list::get_capacity();

// Convert getter/setter functions into attributes for more idiomatic Ruby.

%attributestring(pkmn::swig::item_list, std::string, name, get_name);
%attributestring(pkmn::swig::item_list, std::string, game, get_game);
%attribute(pkmn::swig::item_list, int, num_items, get_num_items);
%attributeval(pkmn::swig::item_list, %arg(std::vector<std::string>), valid_items, get_valid_items);

%include "cpp_wrappers/item_list.hpp"

%extend pkmn::swig::item_list
{
    size_t __len__()
    {
        return size_t(self->get_capacity());
    }

    pkmn::swig::item_slot __getitem__(size_t index)
    {
        return self->at(int(index));
    }

    pkmn::swig::item_list* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        VALUE r;
        for(int item_index = 0; item_index < self->get_capacity(); ++item_index)
        {
            r = swig::from<pkmn::swig::item_slot>(self->at(item_index));
            rb_yield(r);
        }

        return self;
    }
}
