/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/item_bag.hpp"
%}

%include <std_string.i>

%rename("ItemBag") item_bag;

%rename("game") get_game;
%rename("pocket_names") get_pocket_names;

%include "cpp_wrappers/item_bag.hpp"

// Needed to avoid compile error
%{
    namespace swig
    {
        template <> struct traits<pkmn::swig::item_list>
        {
            typedef pointer_category category;
            static const char* type_name()
            {
                return "pkmn::swig::item_list";
            }
        };
    }
%}

%extend pkmn::swig::item_bag
{
    int __len__() {
        return self->get_pocket_names().size();
    }

    pkmn::swig::item_list __getitem__(const std::string& index) {
        return self->get_pocket(index);
    }

    bool __eq__(const std::shared_ptr<pkmn::item_bag>& rhs) {
        return ((*self) == rhs);
    }

    pkmn::swig::item_bag* each()
    {
        if ( !rb_block_given_p() )
            rb_raise( rb_eArgError, "no block given");

        const std::vector<std::string>& pocket_names = self->get_pocket_names();

        VALUE r;
        for(auto iter = pocket_names.begin(); iter != pocket_names.end(); ++iter)
        {
            r = swig::from<pkmn::swig::item_list>(self->get_pocket(*iter));
            rb_yield(r);
        }

        return self;
    }
}
