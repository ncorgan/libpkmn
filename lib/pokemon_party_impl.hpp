/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_IMPL_HPP
#define PKMN_POKEMON_PARTY_IMPL_HPP

#include "utils/misc.hpp"

#include "pokemon_impl.hpp"

#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon_party.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <string>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int PARTY_SIZE = 6;

    class pokemon_party_impl: public pokemon_party,
                              public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokemon_party_impl() {}
            explicit pokemon_party_impl(
                int game_id
            );

            virtual ~pokemon_party_impl() {}

            pkmn::e_game get_game() final;

            const pkmn::pokemon::sptr& get_pokemon(
                int index
            ) final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            const pkmn::pokemon_list_t& as_vector() final;

            void* get_native() final;

            // For internal use

            inline void set_pokedex(
                const pkmn::pokedex::sptr& pokedex
            )
            {
                _pokedex = pokedex;
            }

        protected:
            pkmn::pokemon_list_t _pokemon_list;

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

#endif /* PKMN_POKEMON_PARTY_IMPL_HPP */
