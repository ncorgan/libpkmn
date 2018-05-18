/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_GBAIMPL_HPP
#define PKMN_POKEMON_PARTY_GBAIMPL_HPP

#include "pokemon_party_impl.hpp"

#include <pksav/gba/pokemon.h>

namespace pkmn {

    class pokemon_party_gbaimpl: public pokemon_party_impl {
        public:
            pokemon_party_gbaimpl() {}
            explicit pokemon_party_gbaimpl(
                int game_id
            );
            pokemon_party_gbaimpl(
                int game_id,
                struct pksav_gba_pokemon_party* native
            );

            ~pokemon_party_gbaimpl();

            int get_num_pokemon() override final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) override final;

        private:

            void _from_native() override final;
    };
}

#endif /* PKMN_POKEMON_PARTY_GBAIMPL_HPP */
