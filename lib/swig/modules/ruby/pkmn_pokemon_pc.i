/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/pokemon_pc.hpp>
%}

%include <std_string.i>

%rename("as_list") as_vector;
%rename("game") get_game;
%ignore get_num_boxes;
%rename("box_names") get_box_names;

%rename(PokemonPCInternal) pkmn::pokemon_pc;
%include <pkmn/pokemon_pc.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon_pc> {

    pkmn::shared_ptr<pkmn::pokemon_pc>(
        const std::string& game
    ) {
        return new pkmn::shared_ptr<pkmn::pokemon_pc>(pkmn::pokemon_pc::make(game));
    }

    unsigned int __len__() {
        return (unsigned int)self->get()->get_num_boxes();
    }

    pkmn::pokemon_box::sptr __getitem__(unsigned int index) {
        return self->get()->get_box((int)index);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::pokemon_pc>& rhs) {
        return ((*self) == rhs);
    }
}

%template(PokemonPC) pkmn::shared_ptr<pkmn::pokemon_pc>;
