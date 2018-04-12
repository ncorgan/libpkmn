/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_HPP
#define PKMN_POKEDEX_HPP

#include <pkmn/config.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn
{
    class PKMN_API pokedex
    {
        public:
            typedef std::shared_ptr<pokedex> sptr;

            static sptr make(
                const std::string& game
            );

            virtual std::string get_game() = 0;

            virtual bool has_seen(
                const std::string& species
            ) = 0;

            virtual void set_has_seen(
                const std::string& species,
                bool has_seen_value
            ) = 0;

            virtual const std::vector<std::string>& get_all_seen() = 0;

            virtual int get_num_seen() = 0;

            virtual bool has_caught(
                const std::string& species
            ) = 0;

            virtual void set_has_caught(
                const std::string& species,
                bool has_caught_value
            ) = 0;

            virtual const std::vector<std::string>& get_all_caught() = 0;

            virtual int get_num_caught() = 0;

            #ifndef __DOXYGEN__
            pokedex() {};
            virtual ~pokedex() {};
            virtual void* get_native() = 0;
            #endif
    };
}

#endif /* PKMN_POKEDEX_HPP */
