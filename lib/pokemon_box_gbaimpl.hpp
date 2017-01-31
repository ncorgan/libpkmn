/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GBAIMPL_HPP
#define PKMN_POKEMON_BOX_GBAIMPL_HPP

#include "pokemon_box_impl.hpp"

#include <pksav/gba/pokemon.h>

namespace pkmn {

    class pokemon_box_gbaimpl: public pokemon_box_impl {
        public:
            pokemon_box_gbaimpl() {}
            pokemon_box_gbaimpl(
                int game_id
            );
            pokemon_box_gbaimpl(
                int game_id,
                pksav_gba_pokemon_box_t* native
            );
            pokemon_box_gbaimpl(
                int game_id,
                const pksav_gba_pokemon_box_t &native
            );

            ~pokemon_box_gbaimpl();

            std::string get_name();

            void set_name(
                const std::string &name
            );

            int get_num_pokemon();

            int get_capacity();

        private:
            void _from_native();
    };
}

#endif /* PKMN_POKEMON_BOX_GBAIMPL_HPP */
