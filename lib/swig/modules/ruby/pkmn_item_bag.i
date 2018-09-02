/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include "cpp_wrappers/item_bag.hpp"
%}

%include <std_string.i>

%include <attribute.i>

%ignore pkmn::swig::item_bag::item_bag();
%ignore pkmn::swig::item_bag::item_bag(const pkmn::item_bag::sptr&);
%ignore pkmn::swig::item_bag::cptr();
%ignore pkmn::swig::item_bag::at(int);
%ignore pkmn::swig::item_bag::get_pocket(const std::string&);

// Convert getter/setter functions into attributes for more idiomatic Ruby.

%attribute(pkmn::swig::item_bag, pkmn::e_game, game, get_game);
%attributeval(pkmn::swig::item_bag, %arg(std::vector<std::string>), pocket_names, get_pocket_names);

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
    size_t __len__()
    {
        return self->get_pocket_names().size();
    }

    pkmn::swig::item_list __getitem__(const std::string& name)
    {
        return self->get_pocket(name);
    }

    pkmn::swig::item_bag* each()
    {
        if(!rb_block_given_p())
        {
            rb_raise(rb_eArgError, "no block given");
        }

        const std::vector<std::string>& pocket_names = self->get_pocket_names();

        VALUE r;
        for(const std::string& pocket_name: pocket_names)
        {
            r = swig::from<pkmn::swig::item_list>(self->get_pocket(pocket_name));
            rb_yield(r);
        }

        return self;
    }
}
