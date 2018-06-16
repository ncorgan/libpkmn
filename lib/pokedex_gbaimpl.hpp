/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_GBAIMPL_HPP
#define PKMN_POKEDEX_GBAIMPL_HPP

#include "pokedex_impl.hpp"

#include <pksav/gba/pokedex.h>

namespace pkmn
{
    class pokedex_gbaimpl: public pokedex_impl
    {
        public:
            pokedex_gbaimpl() {}
            pokedex_gbaimpl(
                int game_id,
                struct pksav_gba_pokedex* p_native = nullptr
            );

            ~pokedex_gbaimpl();

            bool has_seen(
                const std::string& species
            ) final;

            bool has_caught(
                const std::string& species
            ) final;

            void _set_has_seen(
                int species_id,
                bool has_seen_value
            ) final;

            void _set_has_caught(
                int species_id,
                bool has_caught_value
            ) final;

            void _update_all_seen() final;
            void _update_all_caught() final;
    };

}

#endif /* PKMN_POKEDEX_GBAIMPL_HPP */
