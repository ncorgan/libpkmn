/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_GBIMPL_HPP
#define PKMN_POKEDEX_GBIMPL_HPP

#include "pokedex_impl.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>

namespace pkmn
{
    template <typename pksav_type>
    class pokedex_gbimpl: public pokedex_impl
    {
        public:
            pokedex_gbimpl() {}
            pokedex_gbimpl(
                int game_id,
                pksav_type* native_ptr = nullptr
            );

            ~pokedex_gbimpl();

            bool has_seen(
                const std::string& species
            ) override final;

            bool has_caught(
                const std::string& species
            ) override final;

            void _set_has_seen(
                int species_id,
                bool has_seen_value
            ) override final;

            void _set_has_caught(
                int species_id,
                bool has_caught_value
            ) override final;

            void _update_all_seen() override final;
            void _update_all_caught() override final;
    };

    typedef pokedex_gbimpl<struct pksav_gen1_pokedex_lists> pokedex_gen1impl;
    typedef pokedex_gbimpl<struct pksav_gen2_pokedex_lists> pokedex_gen2impl;
}

#include "pokedex_gbimpl.ipp"

#endif /* PKMN_POKEDEX_GBIMPL_HPP */
