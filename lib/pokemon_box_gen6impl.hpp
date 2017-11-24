/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GEN6IMPL_HPP
#define PKMN_POKEMON_BOX_GEN6IMPL_HPP

#include "pokemon_box_impl.hpp"

#include <pksav/gen6/pokemon.h>

namespace pkmn {

    class pokemon_box_gen6impl: public pokemon_box_impl {
        public:
            pokemon_box_gen6impl() {}
            explicit pokemon_box_gen6impl(
                int game_id
            );
            pokemon_box_gen6impl(
                int game_id,
                pksav_gen6_pokemon_box_t* native
            );
            pokemon_box_gen6impl(
                int game_id,
                const pksav_gen6_pokemon_box_t &native
            );

            ~pokemon_box_gen6impl();

            std::string get_name() override final;

            void set_name(
                const std::string &name
            ) override final;

            int get_num_pokemon() override final;

            int get_capacity() override final;

            void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) override final;

        private:
            void _from_native() override final;
    };
}

#endif /* PKMN_POKEMON_BOX_GEN6IMPL_HPP */
