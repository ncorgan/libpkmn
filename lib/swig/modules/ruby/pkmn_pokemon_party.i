/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <ruby/stl_macros.i>

%{
    #include <pkmn/pokemon_party.hpp>
%}

%include <std_string.i>

%rename("as_list") as_vector;
%rename("game") get_game;
%rename("num_pokemon") get_num_pokemon;

%rename(PokemonPartyInternal) pkmn::pokemon_party;
%include <pkmn/pokemon_party.hpp>

%extend pkmn::shared_ptr<pkmn::pokemon_party> {

    pkmn::shared_ptr<pkmn::pokemon_party>(
        const std::string& game
    ) {
        return new pkmn::shared_ptr<pkmn::pokemon_party>(pkmn::pokemon_party::make(game));
    }

    unsigned int __len__() {
        (void)self;
        return 6;
    }

    pkmn::pokemon::sptr __getitem__(unsigned int index) {
        return self->get()->get_pokemon(index);
    }

    void __setitem__(unsigned int index, pkmn::pokemon::sptr pokemon) {
        self->get()->set_pokemon(index, pokemon);
    }

    bool __eq__(const pkmn::shared_ptr<pkmn::pokemon_party>& rhs) {
        return ((*self) == rhs);
    }
}

%template(PokemonParty) pkmn::shared_ptr<pkmn::pokemon_party>;
