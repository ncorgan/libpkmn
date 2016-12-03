/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GEN2IMPL_HPP
#define PKMN_POKEMON_BOX_GEN2IMPL_HPP

#include "pokemon_box_impl.hpp"

#include <pksav/gen2/pokemon.h>

namespace pkmn {

    class pokemon_box_gen2impl: public pokemon_box_impl {
        public:
            pokemon_box_gen2impl() {}
            pokemon_box_gen2impl(
                int game_id
            );
            pokemon_box_gen2impl(
                int game_id,
                pksav_gen2_pokemon_box_t* native
            );
            pokemon_box_gen2impl(
                int game_id,
                const pksav_gen2_pokemon_box_t &native
            );

            ~pokemon_box_gen2impl();

            std::string get_name();

            void set_name(
                const std::string &name
            );

            int get_capacity();

        private:
            void _from_native();
    };
}

#endif /* PKMN_POKEMON_BOX_GEN2IMPL_HPP */
