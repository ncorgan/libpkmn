/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/pokemon_box.hpp>
%}

%include <std_string.i>

%rename("as_list") as_vector;
%rename("name") get_name;
%rename("name=") set_name;
%rename("game") get_game;
%rename("num_pokemon") get_num_pokemon;

%rename(PokemonBoxInternal) pkmn::pokemon_box;
%include <pkmn/pokemon_box.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon_box> {

    pkmn::shared_ptr<pkmn::pokemon_box>(
        const std::string& game
    ) {
        return new pkmn::shared_ptr<pkmn::pokemon_box>(pkmn::pokemon_box::make(game));
    }

    unsigned int __len__() {
        return (unsigned int)self->get()->get_capacity();
    }

    pkmn::pokemon::sptr __getitem__(unsigned int index) {
        return self->get()->get_pokemon((int)index);
    }

    void __setitem__(unsigned int index, pkmn::pokemon::sptr pokemon) {
        self->get()->set_pokemon((int)index, pokemon);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::pokemon_box>& rhs) {
        return ((*self) == rhs);
    }
}

%template(PokemonBox) pkmn::shared_ptr<pkmn::pokemon_box>;
PKMN_RUBY_VECTOR(pkmn::shared_ptr<pkmn::pokemon_box>, PokemonBoxList);
