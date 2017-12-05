/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_IMPL_HPP
#define PKMN_POKEDEX_IMPL_HPP

#include <pkmn/pokedex.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn
{
    class pokedex_impl: public pokedex,
                        public boost::noncopyable,
                        public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokedex_impl() {}
            pokedex_impl(
                int game_id,
                void* native_has_seen = nullptr,
                void* native_has_caught = nullptr
            );

            ~pokedex_impl();

            pokedex_impl(pokedex_impl&&) = default;
            pokedex_impl& operator=(pokedex_impl&&) = default;

            void* get_native_has_seen() override final;

            void* get_native_has_caught() override final;

        protected:
            int _game_id;
            int _generation;

            bool _our_mem;

            void* _native_has_seen;
            void* _native_has_caught;
    };

}

#endif /* PKMN_POKEDEX_IMPL_HPP */
