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

#include <pkmn/enums/game.hpp>

#include <map>
#include <memory>
#include <string>

namespace pkmn {

    class PKMN_API daycare: public std::enable_shared_from_this<daycare>
    {
        public:
            typedef std::shared_ptr<daycare> sptr;

            static sptr make(pkmn::e_game game);

            virtual pkmn::e_game get_game() = 0;

            virtual const pkmn::pokemon::sptr& get_levelup_pokemon(
                int index
            ) = 0;

            virtual void set_levelup_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            virtual const pkmn::pokemon_list_t& get_levelup_pokemon_as_vector() = 0;

            virtual int get_levelup_pokemon_capacity() = 0;

            virtual bool can_breed_pokemon() = 0;

            virtual const pkmn::pokemon::sptr& get_breeding_pokemon(
                int index
            ) = 0;

            virtual void set_breeding_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            virtual const pkmn::pokemon_list_t& get_breeding_pokemon_as_vector() = 0;

            virtual int get_breeding_pokemon_capacity() = 0;

            virtual const pkmn::pokemon::sptr& get_egg() = 0;

            #ifndef __DOXYGEN__
            daycare() {};
            virtual ~daycare() {};
            virtual void* get_native() = 0;
            #endif
    };
}

#endif /* PKMN_DAYCARE_HPP */
