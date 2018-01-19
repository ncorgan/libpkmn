/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
            }

            pokemon_party(
                const std::string& game
            ): _pokemon_party(pkmn::pokemon_party::make(game))
            {
            }

            inline std::string get_game()
            {
                return _pokemon_party->get_game();
            }

            inline int get_num_pokemon()
            {
                return _pokemon_party->get_num_pokemon();
            }

            inline pkmn::swig::pokemon get_pokemon(
                int index
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Party index",
                    index,
                    1,
                    6
                );

                return pkmn::swig::pokemon(_pokemon_party->get_pokemon(index-1));
#else
                return pkmn::swig::pokemon(_pokemon_party->get_pokemon(index));
#endif
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon& pokemon
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Party index",
                    index,
                    1,
                    6
                );

                _pokemon_party->set_pokemon(
                    index-1,
                    pokemon.get_internal()
                );
#else
                _pokemon_party->set_pokemon(
                    index,
                    pokemon.get_internal()
                );
#endif
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_party.get());
            }
#else
            inline bool operator==(const pokemon_party& rhs) const
            {
                return (_pokemon_party == rhs._pokemon_party);
            }

            inline bool operator!=(const pokemon_party& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokemon_party::sptr _pokemon_party;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PARTY_HPP */
