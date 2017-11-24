/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN6IMPL_HPP
#define PKMN_POKEMON_PC_GEN6IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen6/pokemon.h>

namespace pkmn {

    typedef struct {
        pksav_gen6_pokemon_pc_t* pc_ptr;
        pksav_gen6_pokemon_box_info_t* box_info_ptr;
    } gen6_pokemon_full_pc_t;

    class pokemon_pc_gen6impl: public pokemon_pc_impl {
        public:
            pokemon_pc_gen6impl() {}
            explicit pokemon_pc_gen6impl(
                int game_id
            );
            pokemon_pc_gen6impl(
                int game_id,
                pksav_gen6_pokemon_pc_t* pc_ptr,
                pksav_gen6_pokemon_box_info_t* box_info_ptr,
                bool copy
            );

            ~pokemon_pc_gen6impl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN6IMPL_HPP */
