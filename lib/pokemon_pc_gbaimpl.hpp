/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GBAIMPL_HPP
#define PKMN_POKEMON_PC_GBAIMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gba/pokemon.h>

namespace pkmn {

    static PKMN_CONSTEXPR int GBA_NUM_BOXES = 14;

    class pokemon_pc_gbaimpl: public pokemon_pc_impl {
        public:
            pokemon_pc_gbaimpl() {}
            pokemon_pc_gbaimpl(
                int game_id
            );
            pokemon_pc_gbaimpl(
                int game_id,
                pksav_gba_pokemon_pc_t* native
            );
            pokemon_pc_gbaimpl(
                int game_id,
                const pksav_gba_pokemon_pc_t &native
            );

            ~pokemon_pc_gbaimpl();

            int get_num_boxes();

        private:
            void _from_native();
            void _update_box_names();
    };
}

#endif /* PKMN_POKEMON_PC_GBAIMPL_HPP */
