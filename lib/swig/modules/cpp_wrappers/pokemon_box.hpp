/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_BOX_HPP
#define CPP_WRAPPERS_POKEMON_BOX_HPP

#include "exception_internal.hpp"
#include "private_exports.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_box.hpp>

#include "swig/modules/cpp_wrappers/pokemon.hpp"

namespace pkmn { namespace swig {

    class pokemon_box
    {
        public:
            pokemon_box():
                _pokemon_box(nullptr),
                _generation(0)
            {}

            pokemon_box(
                const pkmn::pokemon_box::sptr& cpp_pokemon_box
            ): _pokemon_box(cpp_pokemon_box),
               _generation(pkmn::priv::game_name_to_generation(cpp_pokemon_box->get_game()))
            {
            }

            pokemon_box(
                const std::string& game
            ): _pokemon_box(pkmn::pokemon_box::make(game)),
               _generation(pkmn::priv::game_name_to_generation(game))
            {
            }

            inline std::string get_name()
            {
                if(_generation >= 2)
                {
                    return _pokemon_box->get_name();
                }
                else
                {
                    return "";
                }
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
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Box index",
                    index,
                    1,
                    get_capacity()
                );

                return pkmn::swig::pokemon(_pokemon_box->get_pokemon(index-1));
#else
                return pkmn::swig::pokemon(_pokemon_box->get_pokemon(index));
#endif
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon& pokemon
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Box index",
                    index,
                    1,
                    get_capacity()
                );

                _pokemon_box->set_pokemon(
                    index-1,
                    pokemon.get_internal()
                );
#else
                _pokemon_box->set_pokemon(
                    index,
                    pokemon.get_internal()
                );
#endif
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_box.get());
            }
#else
            inline bool operator==(const pokemon_box& rhs) const
            {
                return (_pokemon_box == rhs._pokemon_box);
            }

            inline bool operator!=(const pokemon_box& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokemon_box::sptr _pokemon_box;
            int _generation;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_BOX_HPP */
