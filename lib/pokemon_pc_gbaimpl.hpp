/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GBAIMPL_HPP
#define PKMN_POKEMON_PC_GBAIMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gba/pokemon.h>

namespace pkmn {

    class pokemon_pc_gbaimpl: public pokemon_pc_impl
    {
        public:
            pokemon_pc_gbaimpl(
                int game_id,
                const struct pksav_gba_pokemon_pc* p_native = nullptr
            );

            ~pokemon_pc_gbaimpl() = default;

            int get_num_boxes() final;

        private:
            struct pksav_gba_pokemon_pc _pksav_pc;

            void _from_native() final;
            void _to_native() final;

            void _update_box_names() final;
            void _update_native_box_wallpapers() final;
    };
}

#endif /* PKMN_POKEMON_PC_GBAIMPL_HPP */
