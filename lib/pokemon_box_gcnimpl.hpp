/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

            std::string get_name() final;

            void set_name(
                const std::string& name
            ) final;

            int get_num_pokemon() final;

            int get_capacity() final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            std::string get_wallpaper() final;

            void set_wallpaper(
                const std::string& wallpaper
            ) final;

        private:
            void _from_native() final;
    };
}

#endif /* PKMN_POKEMON_BOX_GCNIMPL_HPP */
