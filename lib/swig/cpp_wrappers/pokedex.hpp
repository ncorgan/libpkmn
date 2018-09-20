/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEDEX_HPP
#define CPP_WRAPPERS_POKEDEX_HPP

#include "pokedex_helpers.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokedex.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    class pokedex
    {
        public:
            pokedex():
                _pokedex(nullptr),
                _game(pkmn::e_game::NONE)
            {}

            pokedex(pkmn::e_game game):
                _game(game)
            {
                _pokedex = pkmn::pokedex::make(game);
                BOOST_ASSERT(_pokedex.get() != nullptr);
            }

            pokedex(
                const pkmn::pokedex::sptr& cpp_pokedex
            ): _pokedex(cpp_pokedex),
               _game(cpp_pokedex->get_game())
            {
                BOOST_ASSERT(_pokedex.get() != nullptr);
            }

            inline pkmn::e_game get_game()
            {
                return _game;
            }

            inline pokedex_has_seen_helper get_has_seen()
            {
                return pokedex_has_seen_helper(_pokedex);
            }

            // Copy, since SWIG will cast away the const in the reference.
            inline std::vector<pkmn::e_species> get_all_seen()
            {
                BOOST_ASSERT(_pokedex.get() != nullptr);
                return _pokedex->get_all_seen();
            }

            inline int get_num_seen()
            {
                return _pokedex->get_num_seen();
            }

            inline pokedex_has_caught_helper get_has_caught()
            {
                return pokedex_has_caught_helper(_pokedex);
            }

            // Copy, since SWIG will cast away the const in the reference.
            std::vector<pkmn::e_species> get_all_caught()
            {
                BOOST_ASSERT(_pokedex.get() != nullptr);
                return _pokedex->get_all_caught();
            }

            inline int get_num_caught()
            {
                return _pokedex->get_num_caught();
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_pokedex.get());
            }
#else
            inline bool operator==(const pokedex& rhs) const
            {
                BOOST_ASSERT(_pokedex.get() != nullptr);
                BOOST_ASSERT(rhs._pokedex.get() != nullptr);

                return (_pokedex == rhs._pokedex);
            }

            inline bool operator!=(const pokedex& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokedex::sptr _pokedex;

            pkmn::e_game _game;
    };

}}

#endif /* CPP_WRAPPERS_POKEDEX_HPP */
