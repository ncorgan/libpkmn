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
            daycare_impl(int game_id, void* p_native = nullptr);

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

            pkmn::pokemon::sptr get_egg() override final;

        private:

            pkmn::pokemon_list_t _pokemon;
            pkmn::pokemon_list_t _daycare_pokemon;

            void* _p_native;
    };
}

#endif /* PKMN_DAYCARE_IMPL_HPP */
