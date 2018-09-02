/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DAYCARE_GCNIMPL_HPP
#define PKMN_DAYCARE_GCNIMPL_HPP

#include "daycare_impl.hpp"

#include <boost/config.hpp>

namespace pkmn {

    class daycare_gcnimpl: public daycare_impl
    {
        public:
            daycare_gcnimpl(int game_id, void* p_native = nullptr);
            ~daycare_gcnimpl();

            int get_levelup_pokemon_capacity() final;

            bool can_breed_pokemon() final;

            int get_breeding_pokemon_capacity() final;

            void _from_native_levelup() final;
            void _to_native_levelup() final;

            void _from_native_breeding() final;
            void _to_native_breeding() final;

            BOOST_STATIC_CONSTEXPR int  LEVELUP_CAPACITY = 1;
            BOOST_STATIC_CONSTEXPR int  BREEDING_CAPACITY = 0;
            BOOST_STATIC_CONSTEXPR bool CAN_BREED_POKEMON = false;

        private:
            void _set_levelup_pokemon(
                int position,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;

            void _set_breeding_pokemon(
                int position,
                const pkmn::pokemon::sptr& new_pokemon
            ) final;
    };
}

#endif /* PKMN_DAYCARE_GCNIMPL_HPP */
