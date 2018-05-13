/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DAYCARE_IMPL_HPP
#define PKMN_DAYCARE_IMPL_HPP

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

            const pkmn::pokemon_list_t& get_levelup_pokemon() override final;

            void add_levelup_pokemon(
                const pkmn::pokemon::sptr& pokemon
            ) override final;

            void remove_levelup_pokemon(int position) override final;

            const pkmn::pokemon_list_t& get_breeding_pokemon() override final;

            void add_breeding_pokemon(
                const pkmn::pokemon::sptr& pokemon
            ) override final;

            void remove_breeding_pokemon(int position) override final;

            const pkmn::pokemon::sptr& get_egg() override final;

        protected:

            int _game_id;
            int _generation;

            bool _can_breed;

            pkmn::pokemon_list_t _levelup_pokemon;
            pkmn::pokemon_list_t _breeding_pokemon;

            pkmn::pokemon::sptr _egg;

            void* _p_native;

            pkmn::pokemon_list_t& _get_levelup_pokemon_ref();

            virtual void _from_native_levelup() = 0;
            virtual void _to_native_levelup() = 0;

            virtual void _from_native_breeding() = 0;
            virtual void _to_native_breeding() = 0;
    };
}

#endif /* PKMN_DAYCARE_IMPL_HPP */
