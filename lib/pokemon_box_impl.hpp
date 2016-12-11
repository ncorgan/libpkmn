/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_BOX_IMPL_HPP
#define PKMN_POKEMON_BOX_IMPL_HPP

#include "pokemon_impl.hpp"

#include <pkmn/pokemon_box.hpp>

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

#include <string>

#define POKEMON_BOX_LOCK_OWN_AND_POKEMON_MEMORY_MUTEXES(index) \
    boost::mutex::scoped_lock native_box_mutex(_native_mutex); \
    boost::mutex::scoped_lock pokemon_pc_mutex(reinterpret_cast<pokemon_impl*>(_pokemon_list.at(index).get())->_native_pc_mutex); \
    boost::mutex::scoped_lock pokemon_party_mutex(reinterpret_cast<pokemon_impl*>(_pokemon_list.at(index).get())->_native_party_mutex);

#define POKEMON_BOX_LOCK_ALL_MEMORY_MUTEXES(index) \
    boost::mutex::scoped_lock native_box_mutex(_native_mutex); \
    std::vector<boost::mutex::scoped_lock> pokemon_mutexes; \
    for(auto iter = _pokemon_list.begin(); iter != _pokemon_list.end(); ++iter) { \
        pokemon_mutexes.emplace_back(boost::mutex::scoped_lock(reinterpret_cast<pokemon_impl*>(iter->get())->_native_pc_mutex)); \
        pokemon_mutexes.emplace_back(boost::mutex::scoped_lock(reinterpret_cast<pokemon_impl*>(iter->get())->_native_party_mutex)); \
    }

namespace pkmn {

    class pokemon_box_impl: public pokemon_box, public boost::noncopyable {
        public:
            pokemon_box_impl() {}
            pokemon_box_impl(
                int game_id
            );

            virtual ~pokemon_box_impl() {}

            std::string get_game();

            pkmn::pokemon::sptr get_pokemon(
                int index
            );

            const pkmn::pokemon_list_t& as_vector();
            
            void* get_native();

            boost::mutex _native_mutex;

        protected:
            pkmn::pokemon_list_t _pokemon_list;
            std::string _box_name;

            void* _native;
            bool _our_mem;

            int _game_id;

            virtual void _from_native() = 0;
    };

}

#endif /* PKMN_POKEMON_BOX_IMPL_HPP */
