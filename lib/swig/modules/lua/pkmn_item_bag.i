/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_bag.hpp>
%}

%include <std_string.i>

%include <pkmn/item_bag.hpp>

%extend pkmn::shared_ptr<pkmn::item_bag> {

    pkmn::item_list::sptr __getitem__(
        const std::string &key
    ) {
        return self->get()->get_pocket(key);
    }

    bool __eq__(
        const pkmn::item_bag::sptr &rhs
    ) {
        return (self->get() == rhs.get());
    }

    int __len(void*) {
        return int(self->get()->get_pockets().size());
    }

}
%template(item_bag_sptr) pkmn::shared_ptr<pkmn::item_bag>;
