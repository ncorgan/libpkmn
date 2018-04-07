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
#include <pksav/gen1/save.h>

namespace pkmn {

    class pokemon_pc_gen1impl: public pokemon_pc_impl
    {
        public:
            pokemon_pc_gen1impl() {}
            pokemon_pc_gen1impl(
                int game_id,
                struct pksav_gen1_pokemon_storage* native_ptr = nullptr
            );

            ~pokemon_pc_gen1impl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN1IMPL_HPP */
