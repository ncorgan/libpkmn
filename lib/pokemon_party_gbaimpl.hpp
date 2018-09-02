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

    class pokemon_party_gbaimpl: public pokemon_party_impl
    {
        public:
            pokemon_party_gbaimpl(
                int game_id,
                struct pksav_gba_pokemon_party* p_native = nullptr
            );
            ~pokemon_party_gbaimpl() = default;

            int get_num_pokemon() final;

        private:

            struct pksav_gba_pokemon_party _pksav_party;

            void _set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            void _from_native() final;
            void _to_native() final;
    };
}

#endif /* PKMN_POKEMON_PARTY_GBAIMPL_HPP */
