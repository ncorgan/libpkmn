/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_HPP
#define PKMN_POKEMON_PARTY_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API pokemon_party
    {
        public:
            typedef std::shared_ptr<pokemon_party> sptr;

            static sptr make(
                const std::string& game
            );

            virtual std::string get_game() = 0;

            virtual int get_num_pokemon() = 0;

            virtual pkmn::pokemon::sptr get_pokemon(
                int index
            ) = 0;

            virtual void set_pokemon(
                int index,
                pkmn::pokemon::sptr new_pokemon
            ) = 0;

            virtual const pkmn::pokemon_list_t& as_vector() = 0;

            #ifndef __DOXYGEN__
            pokemon_party() {}
            virtual ~pokemon_party() {}
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_POKEMON_PARTY_HPP */
