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

    BOOST_STATIC_CONSTEXPR int GBA_NUM_BOXES = 14;

    class pokemon_pc_gbaimpl: public pokemon_pc_impl {
        public:
            pokemon_pc_gbaimpl() {}
            explicit pokemon_pc_gbaimpl(
                int game_id
            );
            pokemon_pc_gbaimpl(
                int game_id,
                struct pksav_gba_pokemon_pc* native
            );
            pokemon_pc_gbaimpl(
                int game_id,
                const struct pksav_gba_pokemon_pc& native
            );

            ~pokemon_pc_gbaimpl();

            int get_num_boxes() override final;

        private:
            void _from_native() override final;
            void _update_box_names() override final;
            void _update_native_box_wallpapers() override final;
    };
}

#endif /* PKMN_POKEMON_PC_GBAIMPL_HPP */
