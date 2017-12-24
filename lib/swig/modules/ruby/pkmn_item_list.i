/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpps/item_list.hpp"
%}

%include <std_string.i>

%rename("ItemSlot") item_slot;
%rename("item") get_item;
%rename("item=") set_item;
%rename("amount") get_amount;
%rename("amount=") set_amount;

%rename("ItemList") item_list;
%rename("name") get_name;
%rename("game") get_game;
%rename("length") get_capacity;
%rename("num_items") get_num_items;
%rename("valid_items") get_valid_items;

%include "cpps/item_list.hpp"

// Needed to avoid compile error
%{
    namespace swig
    {
        template <> struct traits<pkmn::swig::item_slot>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "pkmn::swig::item_slot";
            }
        };
    }
%}

%extend pkmn::swig::item_list
{
    int __len__() {
        return self->get_capacity();
    }

    pkmn::swig::item_slot __getitem__(int index) {
        return self->at(index);
    }

    bool __eq__(const pkmn::swig::item_list& rhs) {
        return ((*self) == rhs);
    }

    pkmn::swig::item_list* each()
    {
        if ( !rb_block_given_p() )
            rb_raise( rb_eArgError, "no block given");

        VALUE r;
        for(int i = 0; i < self->get_capacity(); ++i)
        {
            r = swig::from<pkmn::swig::item_slot>(self->at(i));
            rb_yield(r);
        }

        return self;
    }
}
