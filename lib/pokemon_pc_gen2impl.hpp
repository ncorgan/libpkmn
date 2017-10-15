/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN2IMPL_HPP
#define PKMN_POKEMON_PC_GEN2IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen2/pokemon.h>

namespace pkmn {

    static PKMN_CONSTEXPR int GEN2_NUM_BOXES = 14;

    typedef struct {
        pksav_gen2_pokemon_box_t* boxes[GEN2_NUM_BOXES];
        pksav_gen2_pokemon_box_names_t* box_names;
    } gen2_pokemon_full_pc_t;

    class pokemon_pc_gen2impl: public pokemon_pc_impl {
        public:
            pokemon_pc_gen2impl() {}
            explicit pokemon_pc_gen2impl(
                int game_id
            );
            pokemon_pc_gen2impl(
                int game_id,
                pksav_gen2_pokemon_box_t** native,
                pksav_gen2_pokemon_box_names_t* native_box_names,
                bool copy
            );

            ~pokemon_pc_gen2impl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN2IMPL_HPP */
