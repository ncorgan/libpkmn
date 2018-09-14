/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_IMPL_HPP
#define PKMN_POKEMON_BOX_IMPL_HPP

#include "types/mutex_helpers.hpp"

#include "utils/misc.hpp"

#include "pokemon_impl.hpp"

#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon_box.hpp>

#include <boost/thread/recursive_mutex.hpp>

#include <string>

namespace pkmn {

    class pokemon_box_impl: public pokemon_box,
                            public pkmn::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokemon_box_impl() {}
            explicit pokemon_box_impl(
                int game_id
            );

            virtual ~pokemon_box_impl() {}

            pkmn::e_game get_game() final;

            const pkmn::pokemon::sptr& get_pokemon(
                int index
            ) final;

            void set_pokemon(
                int index,
                const pkmn::pokemon& new_pokemon
            ) final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            const pkmn::pokemon_list_t& as_vector() final;

            void* get_native() final;

            // For internal use

            inline void set_pokedex(
                pkmn::pokedex::sptr pokedex
            )
            {
                _pokedex = pokedex;
            }

        protected:
            pkmn::pokemon_list_t _pokemon_list;
            std::string _box_name;

            pkmn::pokedex::sptr _pokedex;

            void* _p_native;

            int _game_id, _generation;

            virtual void _set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            virtual void _from_native() = 0;
            virtual void _to_native() = 0;
    };

}

#endif /* PKMN_POKEMON_BOX_IMPL_HPP */
