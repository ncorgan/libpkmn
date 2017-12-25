/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_PARTY_HPP
#define CPP_WRAPPERS_POKEMON_PARTY_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_party.hpp>

#include "swig/modules/cpp_wrappers/pokemon.hpp"

namespace pkmn { namespace swig {

    class pokemon_party
    {
        public:
            pokemon_party():
                _pokemon_party(nullptr)
            {}

            pokemon_party(
                const pkmn::pokemon_party::sptr& cpp_pokemon_party
            ): _pokemon_party(cpp_pokemon_party)
            {
                _populate_list();
            }

            pokemon_party(
                const std::string& game
            ): _pokemon_party(pkmn::pokemon_party::make(game))
            {
                _populate_list();
            }

            pokemon_party(
                const pokemon_party& other
            ): _pokemon_party(other._pokemon_party),
               _pokemon_list(other._pokemon_list)
            {}

            bool operator==(
                const pokemon_party& rhs
            ) const
            {
                return (_pokemon_party == rhs._pokemon_party);
            }

            inline std::string get_game()
            {
                return _pokemon_party->get_game();
            }

            inline int get_num_pokemon()
            {
                return _pokemon_party->get_num_pokemon();
            }

            inline pkmn::swig::pokemon2 get_pokemon(
                int index
            )
            {
                pkmn::enforce_bounds("Party index", index, 0, 5);

                return pkmn::swig::pokemon2(_pokemon_party->get_pokemon(index));
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon2& pokemon
            )
            {
                _pokemon_party->set_pokemon(
                    index,
                    pokemon.get_internal()
                );
                _pokemon_list[index] = pokemon;
            }

            inline const std::vector<pkmn::swig::pokemon2>& as_vector()
            {
                return _pokemon_list;
            }

        private:
            pkmn::pokemon_party::sptr _pokemon_party;

            std::vector<pkmn::swig::pokemon2> _pokemon_list;

            void _populate_list()
            {
                const pkmn::pokemon_list_t& internal_vector = _pokemon_party->as_vector();
                _pokemon_list.reserve(internal_vector.size());
                for(size_t i = 0; i < internal_vector.size(); ++i)
                {
                    _pokemon_list.emplace_back(pkmn::swig::pokemon2(internal_vector[i]));
                }
            }
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PARTY_HPP */
