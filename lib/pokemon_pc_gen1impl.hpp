/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN1IMPL_HPP
#define PKMN_POKEMON_PC_GEN1IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen1/pokemon.h>

namespace pkmn {

    static PKMN_CONSTEXPR int GEN1_NUM_BOXES = 12;

    typedef struct {
        pksav_gen1_pokemon_box_t* boxes[GEN1_NUM_BOXES];
    } gen1_pokemon_boxes_t;

    class pokemon_pc_gen1impl: public pokemon_pc_impl {
        public:
            pokemon_pc_gen1impl() {}
            pokemon_pc_gen1impl(
                int game_id
            );
            pokemon_pc_gen1impl(
                int game_id,
                pksav_gen1_pokemon_box_t* native,
                bool copy
            );
            pokemon_pc_gen1impl(
                int game_id,
                pksav_gen1_pokemon_box_t** native,
                bool copy
            );

            ~pokemon_pc_gen1impl();

            int get_num_boxes();

        private:
            void _from_native();
            void _update_box_names();
    };
}

#endif /* PKMN_POKEMON_PC_GEN1IMPL_HPP */
