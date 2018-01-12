/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_bag.hpp"
%}

%include <attribute.i>

%ignore pkmn::swig::item_bag::item_bag();
%ignore pkmn::swig::item_bag::item_bag(const pkmn::item_bag::sptr&);
%ignore pkmn::swig::item_bag::cptr();
%ignore pkmn::swig::item_bag::at(int);

// Convert getter/setter functions into attributes for more idiomatic Lua.

%attributestring(pkmn::swig::item_bag, std::string, game, get_game);
%attributeval(pkmn::swig::item_bag, %arg(std::vector<std::string>), pocket_names, get_pocket_names);

%extend pkmn::swig::item_bag
{
    pkmn::swig::item_list __getitem__(
        const std::string& pocket_name
    )
    {
        return self->get_pocket(pocket_name);
    }

    int __len(void*)
    {
        return int(self->get_pocket_names().size());
    }
}

%include "cpp_wrappers/item_bag.hpp"
