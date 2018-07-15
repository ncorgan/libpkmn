/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GCNIMPL_HPP
#define PKMN_POKEMON_PC_GCNIMPL_HPP

#include "pokemon_pc_impl.hpp"

#include "libpkmgc_includes.hpp"

#include <memory>
#include <vector>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR size_t COLOSSEUM_NUM_BOXES = 3;
    BOOST_STATIC_CONSTEXPR size_t XD_NUM_BOXES = 8;

    class pokemon_pc_gcnimpl: public pokemon_pc_impl
    {
        public:
            pokemon_pc_gcnimpl(
                int game_id,
                LibPkmGC::GC::PokemonBox* const* pp_native = nullptr
            );

            ~pokemon_pc_gcnimpl() = default;

            int get_num_boxes() final;

        private:
            std::vector<std::unique_ptr<LibPkmGC::GC::PokemonBox>> _libpkmgc_box_uptrs;

            void _from_native() final;
            void _to_native() final;

            void _update_box_names() final;
            void _update_native_box_wallpapers() final;
    };
}

#endif /* PKMN_POKEMON_PC_GCNIMPL_HPP */
