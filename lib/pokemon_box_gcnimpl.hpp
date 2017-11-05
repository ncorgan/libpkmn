/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GCNIMPL_HPP
#define PKMN_POKEMON_BOX_GCNIMPL_HPP

#include "pokemon_box_impl.hpp"

#include "libpkmgc_includes.hpp"

namespace pkmn {

    class pokemon_box_gcnimpl: public pokemon_box_impl {
        public:
            pokemon_box_gcnimpl() {}
            explicit pokemon_box_gcnimpl(
                int game_id
            );
            pokemon_box_gcnimpl(
                int game_id,
                LibPkmGC::GC::PokemonBox* native
            );

            ~pokemon_box_gcnimpl();

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

#endif /* PKMN_POKEMON_BOX_GCNIMPL_HPP */
