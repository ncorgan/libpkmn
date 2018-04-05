/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_GBIMPL_HPP
#define PKMN_POKEDEX_GBIMPL_HPP

#include "pokedex_impl.hpp"

namespace pkmn
{
    class pokedex_gbimpl: public pokedex_impl
    {
        public:
            pokedex_gbimpl() {}
            pokedex_gbimpl(
                int game_id,
                uint8_t* native_has_seen = nullptr,
                uint8_t* native_has_caught = nullptr
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

        private:
            uint8_t* _native_has_seen;
            uint8_t* _native_has_caught;
    };

}

#endif /* PKMN_POKEDEX_GBIMPL_HPP */
