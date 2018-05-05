/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN2IMPL_HPP
#define PKMN_POKEMON_PC_GEN2IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/save.h>

namespace pkmn {

    class pokemon_pc_gen2impl: public pokemon_pc_impl
    {
        public:
            pokemon_pc_gen2impl() {}
            pokemon_pc_gen2impl(
                int game_id,
                struct pksav_gen2_pokemon_storage* p_native = nullptr
            );

            ~pokemon_pc_gen2impl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
            void _update_native_box_wallpapers() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN2IMPL_HPP */
