/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_GEN6IMPL_HPP
#define PKMN_POKEMON_PARTY_GEN6IMPL_HPP

#include "pokemon_party_impl.hpp"

#include <pksav/gen6/pokemon.h>

namespace pkmn
{

    class pokemon_party_gen6impl: public pokemon_party_impl
    {
        public:
            pokemon_party_gen6impl() {}
            explicit pokemon_party_gen6impl(
                int game_id
            );
            pokemon_party_gen6impl(
                int game_id,
                pksav_gen6_pokemon_party_t* native
            );

            ~pokemon_party_gen6impl();

            int get_num_pokemon() override final;

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) override final;

        private:

            int _num_pokemon;

            void _from_native() override final;
    };
}

#endif /* PKMN_POKEMON_PARTY_GEN6IMPL_HPP */
