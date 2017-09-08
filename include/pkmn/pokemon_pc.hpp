/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_HPP
#define PKMN_POKEMON_PC_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon_box.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API pokemon_pc {
        public:
            typedef pkmn::shared_ptr<pokemon_pc> sptr;

            #ifndef __DOXYGEN__
            pokemon_pc() {}
            virtual ~pokemon_pc() {}
            #endif

            static sptr make(
                const std::string &game
            );

            virtual std::string get_game()  = 0;

            virtual int get_num_boxes() = 0;

            virtual pkmn::pokemon_box::sptr get_box(
                int index
            ) = 0;

            virtual const pkmn::pokemon_box_list_t& as_vector() = 0;

            virtual const std::vector<std::string>& get_box_names() = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_POKEMON_PC_HPP */
