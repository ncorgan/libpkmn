/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_IMPL_HPP
#define PKMN_POKEMON_PARTY_IMPL_HPP

#include "misc_common.hpp"

#include "pokemon_impl.hpp"

#include <pkmn/pokemon_party.hpp>

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <string>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int PARTY_SIZE = 6;

    class pokemon_party_impl: public pokemon_party, public boost::noncopyable {
        public:
            pokemon_party_impl() {}
            explicit pokemon_party_impl(
                int game_id
            );

            virtual ~pokemon_party_impl() {}

            std::string get_game() override final;

            pkmn::pokemon::sptr get_pokemon(
                int index
            ) override final;

            const pkmn::pokemon_list_t& as_vector() override final;

            void* get_native() override final;

        protected:
            pkmn::pokemon_list_t _pokemon_list;

            void* _native;
            bool _our_mem;

            boost::recursive_mutex _mem_mutex;

            int _game_id, _generation;

            virtual void _from_native() = 0;

            template
            <typename native_pc_type, typename native_party_data_type>
            void copy_party_pokemon(
                size_t index
            )
            {
                pokemon_impl* old_party_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

                // Create a copy of the party Pokémon. Set the old sptr to use this so it owns the memory.
                native_pc_type* party_pokemon_pc_copy_ptr = new native_pc_type;
                native_party_data_type* party_pokemon_party_data_copy_ptr = new native_party_data_type;

                rcast_equal<native_pc_type>(
                    old_party_pokemon_impl_ptr->_native_pc,
                    party_pokemon_pc_copy_ptr
                );
                rcast_equal<native_party_data_type>(
                    old_party_pokemon_impl_ptr->_native_party,
                    party_pokemon_party_data_copy_ptr
                );

                old_party_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(party_pokemon_pc_copy_ptr);
                old_party_pokemon_impl_ptr->_native_party = reinterpret_cast<void*>(party_pokemon_party_data_copy_ptr);
                old_party_pokemon_impl_ptr->_our_pc_mem = true;
                old_party_pokemon_impl_ptr->_our_party_mem = true;
            }
    };

}

#endif /* PKMN_POKEMON_PARTY_IMPL_HPP */
