/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/item_bag.hpp>
%}

%include <std_string.i>

%rename("game") get_game;
%rename("pockets") get_pockets;
%rename("pocket_names") get_pocket_names;

%rename(ItemBagInternal) pkmn::item_bag;
%include <pkmn/item_bag.hpp>

%extend pkmn::shared_ptr<pkmn::item_bag> {

    pkmn::shared_ptr<pkmn::item_bag>(
        const std::string& game
    ) {
        return new pkmn::shared_ptr<pkmn::item_bag>(pkmn::item_bag::make(game));
    }

    int __len__() {
        return self->get()->get_pockets().size();
    }

    pkmn::shared_ptr<pkmn::item_list> __getitem__(const std::string& index) {
        return self->get()->get_pocket(index);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::item_bag>& rhs) {
        return ((*self) == rhs);
    }

    pkmn::item_bag::sptr* each()
    {
        if ( !rb_block_given_p() )
            rb_raise( rb_eArgError, "no block given");

        const pkmn::item_pockets_t& item_pockets = self->get()->get_pockets();

        VALUE r;
        for(auto iter = item_pockets.begin(); iter != item_pockets.end(); ++iter)
        {
            r = swig::from<pkmn::item_list::sptr>(iter->second);
            rb_yield(r);
        }

        return self;
    }
}

%template(ItemBag) pkmn::shared_ptr<pkmn::item_bag>;
