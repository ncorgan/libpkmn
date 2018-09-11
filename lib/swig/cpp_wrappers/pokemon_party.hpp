/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_PARTY_HPP
#define CPP_WRAPPERS_POKEMON_PARTY_HPP

#include "exception_internal.hpp"

#include "swig/cpp_wrappers/pokemon.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_party.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    class pokemon_party
    {
        public:
            explicit pokemon_party(
                const pkmn::pokemon_party::sptr& cpp_pokemon_party
            ): _pokemon_party(cpp_pokemon_party)
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);
            }

            explicit pokemon_party(
                pkmn::e_game game
            ): _pokemon_party(pkmn::pokemon_party::make(game))
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);
            }

            inline pkmn::e_game get_game()
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                return _pokemon_party->get_game();
            }

            inline int get_num_pokemon()
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                return _pokemon_party->get_num_pokemon();
            }

            inline pkmn::swig::pokemon get_pokemon(
                int index
            )
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

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
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                // Lua arrays start at 1.
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Party index",
                    index,
                    1,
                    6
                );
                --index;
#endif
                const pkmn::pokemon::sptr& internal_pokemon = pokemon.get_internal();
                if(internal_pokemon.get() != nullptr)
                {
                    // This Pokémon instance was created in LibPKMN, so just
                    // pass in the underlying sptr.
                    _pokemon_party->set_pokemon(
                        index,
                        pokemon.get_internal()
                    );
                }
                else
                {
                    // This is a subclass of the SWIG class, so pass it
                    // straight through.
                    _pokemon_party->set_pokemon(
                        index,
                        pokemon
                    );
                }
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                return uintmax_t(_pokemon_party.get());
            }
#else
            inline bool operator==(const pokemon_party& rhs) const
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                return (_pokemon_party == rhs._pokemon_party);
            }

            inline bool operator!=(const pokemon_party& rhs) const
            {
                BOOST_ASSERT(_pokemon_party.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokemon_party::sptr _pokemon_party;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PARTY_HPP */
