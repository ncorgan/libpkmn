/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GCNIMPL_HPP
#define PKMN_POKEMON_BOX_GCNIMPL_HPP

#include "pokemon_box_impl.hpp"

#include <LibPkmGC/GC/Common/PokemonBox.h>
#include <LibPkmGC/Colosseum/Common/PokemonBox.h>
#include <LibPkmGC/XD/Common/PokemonBox.h>

namespace pkmn {

    class pokemon_box_gcnimpl: public pokemon_box_impl {
        public:
            pokemon_box_gcnimpl() {}
            pokemon_box_gcnimpl(
                int game_id
            );
            pokemon_box_gcnimpl(
                int game_id,
                LibPkmGC::GC::PokemonBox* native
            );

            ~pokemon_box_gcnimpl();

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

#endif /* PKMN_POKEMON_BOX_GCNIMPL_HPP */
