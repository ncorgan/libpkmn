/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
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
                pksav_gba_pokemon_party_t* native
            );

            ~pokemon_party_gbaimpl();

            int get_num_pokemon();

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            );

        private:

            void _from_native();
    };
}

#endif /* PKMN_POKEMON_PARTY_GBAIMPL_HPP */
