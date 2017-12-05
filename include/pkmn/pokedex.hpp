/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_HPP
#define PKMN_POKEDEX_HPP

#include <pkmn/config.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <string>

namespace pkmn
{

    class PKMN_API pokedex
    {
        public:
            typedef pkmn::shared_ptr<pokedex> sptr;

            static sptr make(
                const std::string& game
            );

            #ifndef __DOXYGEN__
            pokedex() {};
            virtual ~pokedex() {};
            #endif

            virtual std::string get_game() = 0;

            virtual bool has_seen(
                const std::string& species
            ) = 0;

            virtual void set_has_seen(
                const std::string& species,
                bool has_seen_value
            ) = 0;

            virtual bool has_caught(
                const std::string& species
            ) = 0;

            virtual void set_has_caught(
                const std::string& species,
                bool has_caught_value
            ) = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native_has_seen() = 0;
            virtual void* get_native_has_caught() = 0;
            #endif
    };
}

#endif /* PKMN_POKEDEX_HPP */
