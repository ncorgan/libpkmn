/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_IMPL_HPP
#define PKMN_POKEMON_PARTY_IMPL_HPP

#include "mem/scoped_lock.hpp"

#include <pkmn/pokemon_party.hpp>

#include <boost/noncopyable.hpp>

#include <string>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int PARTY_SIZE = 6;

    // Forward declarations
    class pokemon_impl;
    class pokemon_party_impl;
    namespace mem {
        void set_pokemon_in_party(
                 pokemon_impl* new_pokemon,
                 pokemon_party_impl* party,
                 int index
             );
    }

    class pokemon_party_impl: public pokemon_party, public boost::noncopyable {
        public:
            pokemon_party_impl() {}
            explicit pokemon_party_impl(
                int game_id
            );

            virtual ~pokemon_party_impl() {}

            std::string get_game();

            pkmn::pokemon::sptr get_pokemon(
                int index
            );

            const pkmn::pokemon_list_t& as_vector();

            void* get_native();

            typedef pkmn::mem::scoped_lock<pokemon_party_impl> pokemon_party_scoped_lock;
            friend pokemon_party_scoped_lock;
            friend void pkmn::mem::set_pokemon_in_party(
                            pokemon_impl* new_pokemon,
                            pokemon_party_impl* party,
                            int index
                        );

        protected:
            pkmn::pokemon_list_t _pokemon_list;

            void* _native;
            bool _our_mem;

            boost::mutex _mem_mutex;

            int _game_id, _generation;

            virtual void _from_native() = 0;
    };

}

#endif /* PKMN_POKEMON_PARTY_IMPL_HPP */
