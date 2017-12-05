/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_HPP
#define PKMN_POKEDEX_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/pokemon_pc.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace pkmn
{

    class PKMN_API pokedex
    {
        public:
            typedef pkmn::shared_ptr<pokedex> sptr;

            /*!
             * @brief Imports the save from the game save at the given file.
             *
             * \param filepath The filepath from which to import the save
             * \throws std::invalid_argument if the given filepath is invalid
             */
            static sptr make(
                const std::string& game
            );

            #ifndef __DOXYGEN__
            pokedex() {};
            virtual ~pokedex() {};
            #endif

            #ifndef __DOXYGEN__
            virtual void* get_native_has_seen() = 0;
            virtual void* get_native_has_caught() = 0;
            #endif
    };
}

#endif /* PKMN_POKEDEX_HPP */
