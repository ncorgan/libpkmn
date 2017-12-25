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

    class pokemon_box2
    {
        public:
            pokemon_box2():
                _pokemon_box(nullptr)
            {}

            pokemon_box2(
                const pkmn::pokemon_box::sptr& cpp_pokemon_box
            ): _pokemon_box(cpp_pokemon_box)
            {
                _populate_list();
            }

            pokemon_box2(
                const std::string& game
            ): _pokemon_box(pkmn::pokemon_box::make(game))
            {
                _populate_list();
            }

            bool operator==(
                const pokemon_box2& rhs
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

            inline pkmn::swig::pokemon2 get_pokemon(
                int index
            )
            {
                int capacity = _pokemon_box->get_capacity();
                pkmn::enforce_bounds("Box index", index, 0, (capacity-1));

                return pkmn::swig::pokemon2(_pokemon_box->get_pokemon(index));
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon2& pokemon
            )
            {
                _pokemon_box->set_pokemon(
                    index,
                    pokemon.get_internal()
                );
                _pokemon_list[index] = pokemon;
            }

            inline const std::vector<pkmn::swig::pokemon2>& as_vector()
            {
                return _pokemon_list;
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_box.get());
            }

        private:
            pkmn::pokemon_box::sptr _pokemon_box;

            std::vector<pkmn::swig::pokemon2> _pokemon_list;

            void _populate_list()
            {
                const pkmn::pokemon_list_t& internal_vector = _pokemon_box->as_vector();
                _pokemon_list.reserve(internal_vector.size());
                for(size_t i = 0; i < internal_vector.size(); ++i)
                {
                    _pokemon_list.emplace_back(pkmn::swig::pokemon2(internal_vector[i]));
                }
            }
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_BOX_HPP */
