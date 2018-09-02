/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_GCNIMPL_HPP
#define PKMN_POKEMON_BOX_GCNIMPL_HPP

#include "pokemon_box_impl.hpp"

#include "pkmgc/includes.hpp"

#include <memory>

namespace pkmn {

    class pokemon_box_gcnimpl: public pokemon_box_impl
    {
        public:
            pokemon_box_gcnimpl(
                int game_id,
                LibPkmGC::GC::PokemonBox* p_libpkmgc_native = nullptr
            );

            ~pokemon_box_gcnimpl() = default;

            std::string get_name() final;

            void set_name(
                const std::string& name
            ) final;

            int get_num_pokemon() final;

            int get_capacity() final;

            std::string get_wallpaper() final;

            void set_wallpaper(
                const std::string& wallpaper
            ) final;

        private:

            std::unique_ptr<LibPkmGC::GC::PokemonBox> _libpkmgc_box_uptr;

            void _set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            void _from_native() final;
            void _to_native() final;
    };
}

#endif /* PKMN_POKEMON_BOX_GCNIMPL_HPP */
