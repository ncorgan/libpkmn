/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_IMPL_HPP
#define PKMN_POKEMON_PC_IMPL_HPP

#include "pokemon_box_impl.hpp"

#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon_pc.hpp>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <string>

namespace pkmn {

    class pokemon_pc_impl: public pokemon_pc,
                           private boost::noncopyable,
                           public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokemon_pc_impl() {}
            explicit pokemon_pc_impl(
                int game_id
            );

            virtual ~pokemon_pc_impl() {}

            std::string get_game() override final;

            pkmn::pokemon_box::sptr get_box(
                int index
            ) override final;

            const pkmn::pokemon_box_list_t& as_vector() override final;

            const std::vector<std::string>& get_box_names() override final;

            void* get_native() override final;

            // For internal use

            inline void set_pokedex(
                pkmn::pokedex::sptr pokedex
            )
            {
                for(const auto& box: _box_list)
                {
                    BOOST_ASSERT(dynamic_cast<pokemon_box_impl*>(box.get()));
                    dynamic_cast<pokemon_box_impl*>(box.get())->set_pokedex(pokedex);
                }
            }

        protected:
            pkmn::pokemon_box_list_t _box_list;
            std::vector<std::string> _box_names;

            void* _native;
            bool _our_mem;

            int _game_id, _generation;

            virtual void _from_native() = 0;
            virtual void _update_box_names() = 0;
    };

}

#endif /* PKMN_POKEMON_PC_IMPL_HPP */
