/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN1IMPL_HPP
#define PKMN_POKEMON_PC_GEN1IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen1/pokemon.h>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int GEN1_NUM_BOXES = 12;

    // TODO: replace with pksav_gen1_pokemon_storage pointer
    typedef struct
    {
        struct pksav_gen1_pokemon_box* boxes[GEN1_NUM_BOXES];
    } gen1_pokemon_boxes_t;

    class pokemon_pc_gen1impl: public pokemon_pc_impl {
        public:
            pokemon_pc_gen1impl() {}
            explicit pokemon_pc_gen1impl(
                int game_id
            );
            pokemon_pc_gen1impl(
                int game_id,
                struct pksav_gen1_pokemon_box* native,
                bool copy
            );
            pokemon_pc_gen1impl(
                int game_id,
                struct pksav_gen1_pokemon_box** native,
                bool copy
            );

            ~pokemon_pc_gen1impl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN1IMPL_HPP */
