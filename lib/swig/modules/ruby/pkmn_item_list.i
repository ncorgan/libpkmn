/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/item_list.hpp>
%}

%include <std_string.i>

%rename("as_list") as_vector;
%rename("name") get_name;
%rename("game") get_game;
%rename("capacity") get_capacity;
%rename("num_items") get_num_items;
%rename("valid_items") get_valid_items;

%rename(ItemListInternal) pkmn::item_list;
%include <pkmn/item_list.hpp>

%extend pkmn::shared_ptr<pkmn::item_list> {

    pkmn::shared_ptr<pkmn::item_list>(
        const std::string& name,
        const std::string& game
    ) {
        return new pkmn::shared_ptr<pkmn::item_list>(pkmn::item_list::make(name, game));
    }

    int __len__() {
        return self->get()->get_capacity();
    }

    pkmn::item_slot __getitem__(int index) {
        return self->get()->at(index);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::item_list>& rhs) {
        return ((*self) == rhs);
    }
}

%template(ItemList) pkmn::shared_ptr<pkmn::item_list>;
PKMN_RUBY_MAP(std::string, pkmn::item_list::sptr, StringItemPocketHash);
