/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GEN4IMPL_HPP
#define PKMN_POKEMON_BOX_GEN4IMPL_HPP

#include "pokemon_box_impl.hpp"

namespace pkmn {

    class pokemon_box_gen4impl: public pokemon_box_impl {
        public:
            pokemon_box_gen4impl() {}
            pokemon_box_gen4impl(
                int game_id
            );
            pokemon_box_gen4impl(
                int game_id,
                void* native
            );

            ~pokemon_box_gen4impl();

            std::string get_name();

            void set_name(
                const std::string &name
            );

            int get_num_pokemon();

            int get_capacity();

        private:
            void _from_native();

            bool _hgss;
    };
}

#endif /* PKMN_POKEMON_BOX_GEN4IMPL_HPP */
