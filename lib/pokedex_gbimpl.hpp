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

#include <vector>

namespace pkmn
{
    template <typename pksav_type>
    class pokedex_gbimpl: public pokedex_impl
    {
        public:
            pokedex_gbimpl(
                int game_id,
                pksav_type* p_native = nullptr
            );
            ~pokedex_gbimpl() = default;

            bool has_seen(
                pkmn::e_species species
            ) final;

            bool has_caught(
                pkmn::e_species species
            ) final;

        private:
            pksav_type _pksav_pokedex;

            // Local storage of buffer so we don't depend on someone else's
            // pointer.
            std::vector<uint8_t> _seen_buffer;
            std::vector<uint8_t> _owned_buffer;

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

    typedef pokedex_gbimpl<struct pksav_gen1_pokedex_lists> pokedex_gen1impl;
    typedef pokedex_gbimpl<struct pksav_gen2_pokedex_lists> pokedex_gen2impl;
}

#include "pokedex_gbimpl.ipp"

#endif /* PKMN_POKEDEX_GBIMPL_HPP */
