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

    class pokemon_party2
    {
        public:
            pokemon_party2():
                _pokemon_party(nullptr)
            {}

            pokemon_party2(
                const pkmn::pokemon_party::sptr& cpp_pokemon_party
            ): _pokemon_party(cpp_pokemon_party)
            {
            }

            pokemon_party2(
                const std::string& game
            ): _pokemon_party(pkmn::pokemon_party::make(game))
            {
            }

            bool operator==(
                const pokemon_party2& rhs
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
            }

            uintmax_t cptr()
            {
                return uintmax_t(_pokemon_party.get());
            }

        private:
            pkmn::pokemon_party::sptr _pokemon_party;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PARTY_HPP */
