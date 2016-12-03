/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_HPP
#define PKMN_POKEMON_BOX_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API pokemon_box {
        public:
            typedef pkmn::shared_ptr<pokemon_box> sptr;

            #ifndef __DOXYGEN__
            pokemon_box() {}
            virtual ~pokemon_box() {}
            #endif

            static sptr make(
                const std::string &game
            );

            virtual std::string get_name() = 0;

            virtual void set_name(
                const std::string &name
            ) = 0;

            virtual std::string get_game()  = 0;

            virtual int get_capacity() = 0;

            virtual pkmn::pokemon::sptr get_pokemon(
                int index
            ) = 0;

            virtual const pkmn::pokemon_list_t& as_vector() = 0;

            // TODO: set_pokemon, requires memory logic

            #ifndef __DOXYGEN__
            virtual void* get_native() = 0;
            #endif
    };

    typedef std::vector<pkmn::pokemon_box::sptr> pokemon_box_list_t;

}

#endif /* PKMN_POKEMON_BOX_HPP */
