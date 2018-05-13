/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DAYCARE_HPP
#define PKMN_DAYCARE_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <map>
#include <memory>
#include <string>

namespace pkmn {

    class PKMN_API daycare
    {
        public:
            typedef std::shared_ptr<daycare> sptr;

            static sptr make(const std::string& game);

            virtual std::string get_game() = 0;

            virtual const pkmn::pokemon_list_t& get_daycare_pokemon() = 0;

            virtual void add_daycare_pokemon(
                const pkmn::pokemon::sptr& pokemon
            ) = 0;

            virtual void remove_daycare_pokemon(int position) = 0;

            virtual const pkmn::pokemon_list_t& get_breeder_pokemon() = 0;

            virtual void add_breeder_pokemon(
                const pkmn::pokemon::sptr& pokemon
            ) = 0;

            virtual void remove_breeder_pokemon(int position) = 0;

            virtual pkmn::pokemon::sptr get_egg() = 0;

            #ifndef __DOXYGEN__
            daycare() {};
            virtual ~daycare() {};
            virtual void* get_native() = 0;
            #endif
    };
}

#endif /* PKMN_DAYCARE_HPP */
