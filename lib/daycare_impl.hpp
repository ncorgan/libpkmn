/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DAYCARE_IMPL_HPP
#define PKMN_DAYCARE_IMPL_HPP

#include "pokemon_impl.hpp"
#include "utils/misc.hpp"

#include <pkmn/daycare.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn {

    class daycare_impl: public daycare,
                        private boost::noncopyable,
                        public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            explicit daycare_impl(int game_id);
            ~daycare_impl();

            std::string get_game() override final;

            const pkmn::pokemon::sptr& get_levelup_pokemon(
                int index
            ) override final;

            const pkmn::pokemon_list_t& get_levelup_pokemon_as_vector() override final;

            const pkmn::pokemon::sptr& get_breeding_pokemon(
                int index
            ) override final;

            const pkmn::pokemon_list_t& get_breeding_pokemon_as_vector() override final;

            const pkmn::pokemon::sptr& get_egg() override final;

            void* get_native() override final;

        protected:

            int _game_id;
            int _generation;

            pkmn::pokemon_list_t _levelup_pokemon;
            pkmn::pokemon_list_t _breeding_pokemon;

            pkmn::pokemon::sptr _egg;

            void* _p_native;
            bool _is_our_mem;

            pkmn::pokemon_list_t& _get_levelup_pokemon_ref();

            virtual void _from_native_levelup() = 0;
            virtual void _to_native_levelup() = 0;

            virtual void _from_native_breeding() = 0;
            virtual void _to_native_breeding() = 0;

            template
            <typename native_pc_type, typename native_party_data_type>
            void copy_box_pokemon(
                pkmn::pokemon_list_t& r_pokemon_list,
                size_t index
            )
            {
                pokemon_impl* old_box_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(r_pokemon_list[index].get());

                // Create a copy of the Pokémon. Set the old sptr to use this so it owns the memory.
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

                // The old Pokémon's party data may have been allocated.
                if(old_box_pokemon_impl_ptr->_our_party_mem)
                {
                    delete reinterpret_cast<native_party_data_type*>(old_box_pokemon_impl_ptr->_native_party);
                }

                old_box_pokemon_impl_ptr->_native_pc = reinterpret_cast<void*>(box_pokemon_pc_copy_ptr);
                old_box_pokemon_impl_ptr->_native_party = reinterpret_cast<void*>(box_pokemon_party_data_copy_ptr);
                old_box_pokemon_impl_ptr->_our_pc_mem = true;
                old_box_pokemon_impl_ptr->_our_party_mem = true;
            }
    };
}

#endif /* PKMN_DAYCARE_IMPL_HPP */
