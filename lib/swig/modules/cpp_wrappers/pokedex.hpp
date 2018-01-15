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

namespace pkmn { namespace swig {

    class pokedex
    {
        public:
            pokedex():
                _pokedex(nullptr)
            {}

            pokedex(
                const pkmn::pokedex::sptr& cpp_pokedex
            ): _pokedex(cpp_pokedex)
            {}

            inline std::string get_game()
            {
                return _pokedex->get_game();
            }

            inline pokedex_has_seen_helper get_has_seen()
            {
                return pokedex_has_seen_helper(_pokedex);
            }

            // Copy, since SWIG will cast away the const in the reference.
            inline std::vector<std::string> get_all_seen()
            {
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
            std::vector<std::string> get_all_caught()
            {
                return _pokedex->get_all_caught();
            }

            inline int get_num_caught()
            {
                return _pokedex->get_num_caught();
            }

        private:
            pkmn::pokedex::sptr _pokedex;
    };

}}

#endif /* CPP_WRAPPERS_POKEDEX_HPP */
