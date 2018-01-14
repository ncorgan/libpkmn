/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_IMPL_HPP
#define PKMN_POKEMON_BOX_IMPL_HPP

#include "misc_common.hpp"

#include "pokemon_impl.hpp"

#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon_box.hpp>

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

#include <string>

namespace pkmn {

    class pokemon_box_impl: public pokemon_box, public boost::noncopyable {
        public:
            pokemon_box_impl() {}
            explicit pokemon_box_impl(
                int game_id
            );

            virtual ~pokemon_box_impl() {}

            std::string get_game() override final;

            pkmn::pokemon::sptr get_pokemon(
                int index
            ) override final;

            const pkmn::pokemon_list_t& as_vector() override final;

            void* get_native() override final;

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

            void* _native;
            bool _our_mem;

            boost::mutex _mem_mutex;

            int _game_id, _generation;

            virtual void _from_native() = 0;

            template
            <typename native_pc_type, typename native_party_data_type>
            void copy_box_pokemon(
                size_t index
            )
            {
                pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

                // Create a copy of the party Pokémon. Set the old sptr to use this so it owns the memory.
                native_pc_type* box_pokemon_pc_copy_ptr = new native_pc_type;
                native_party_data_type* box_pokemon_party_data_copy_ptr = new native_party_data_type;

                rcast_equal<native_pc_type>(
                    old_box_pokemon_impl_ptr->_native_pc,
                    box_pokemon_pc_copy_ptr
                );
                rcast_equal<native_party_data_type>(
                    old_box_pokemon_impl_ptr->_native_party,
                    box_pokemon_party_data_copy_ptr
                );

                old_box_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(box_pokemon_pc_copy_ptr);
                old_box_pokemon_impl_ptr->_native_party = reinterpret_cast<void*>(box_pokemon_party_data_copy_ptr);
                old_box_pokemon_impl_ptr->_our_pc_mem = true;
                old_box_pokemon_impl_ptr->_our_party_mem = true;
            }
    };

}

#endif /* PKMN_POKEMON_BOX_IMPL_HPP */
