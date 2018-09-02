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

            pkmn::e_game get_game() final;

            const pkmn::pokemon::sptr& get_levelup_pokemon(
                int index
            ) final;

            void set_levelup_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            const pkmn::pokemon_list_t& get_levelup_pokemon_as_vector() final;

            const pkmn::pokemon::sptr& get_breeding_pokemon(
                int index
            ) final;

            void set_breeding_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            const pkmn::pokemon_list_t& get_breeding_pokemon_as_vector() final;

            const pkmn::pokemon::sptr& get_egg() final;

            void* get_native() final;

        protected:

            int _game_id;
            int _generation;

            pkmn::pokemon_list_t _levelup_pokemon;
            pkmn::pokemon_list_t _breeding_pokemon;

            pkmn::pokemon::sptr _egg;

            void* _p_native;
            bool _is_our_mem;

            virtual void _set_levelup_pokemon(
                int position,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            virtual void _set_breeding_pokemon(
                int position,
                const pkmn::pokemon::sptr& new_pokemon
            ) = 0;

            pkmn::pokemon_list_t& _get_levelup_pokemon_ref();

            virtual void _from_native_levelup() = 0;
            virtual void _to_native_levelup() = 0;

            virtual void _from_native_breeding() = 0;
            virtual void _to_native_breeding() = 0;
    };
}

#endif /* PKMN_DAYCARE_IMPL_HPP */
