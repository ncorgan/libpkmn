/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN4IMPL_HPP
#define PKMN_POKEMON_PC_GEN4IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen4/pokemon.h>

namespace pkmn {

    static PKMN_CONSTEXPR int GEN4_NUM_BOXES = 18;

    class pokemon_pc_gen4impl: public pokemon_pc_impl {
        public:
            pokemon_pc_gen4impl() {}
            pokemon_pc_gen4impl(
                int game_id
            );
            pokemon_pc_gen4impl(
                int game_id,
                pksav_gen4_pokemon_pc_t* native
            );

            ~pokemon_pc_gen4impl();

            int get_num_boxes();

        private:
            void _from_native();
            void _update_box_names();

            bool _hgss;
    };
}

#endif /* PKMN_POKEMON_PC_GEN4IMPL_HPP */
