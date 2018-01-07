/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_BOX_HPP
#define CPP_WRAPPERS_POKEMON_BOX_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_box.hpp>

#include "swig/modules/cpp_wrappers/pokemon.hpp"

namespace pkmn { namespace swig {

    class pokemon_box
    {
        public:
            pokemon_box():
                _pokemon_box(nullptr)
            {}

            pokemon_box(
                const pkmn::pokemon_box::sptr& cpp_pokemon_box
            ): _pokemon_box(cpp_pokemon_box)
            {
            }

            pokemon_box(
                const std::string& game
            ): _pokemon_box(pkmn::pokemon_box::make(game))
            {
            }

            bool operator==(
                const pokemon_box& rhs
            ) const
            {
                return (_pokemon_box == rhs._pokemon_box);
            }

            inline std::string get_name()
            {
                return _pokemon_box->get_name();
            }

            inline void set_name(
                const std::string& name
            )
            {
                _pokemon_box->set_name(name);
            }

            inline std::string get_game()
            {
                return _pokemon_box->get_game();
            }

            inline int get_num_pokemon()
            {
                return _pokemon_box->get_num_pokemon();
            }

            inline int get_capacity()
            {
                return _pokemon_box->get_capacity();
            }

            inline pkmn::swig::pokemon get_pokemon(
                int index
            )
            {
                int capacity = _pokemon_box->get_capacity();
                pkmn::enforce_bounds("Box index", index, 0, (capacity-1));

                return pkmn::swig::pokemon(_pokemon_box->get_pokemon(index));
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon& pokemon
            )
            {
                _pokemon_box->set_pokemon(
                    index,
                    pokemon.get_internal()
                );
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_box.get());
            }

        private:
            pkmn::pokemon_box::sptr _pokemon_box;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_BOX_HPP */
