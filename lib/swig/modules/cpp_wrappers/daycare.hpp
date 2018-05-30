/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_DAYCARE_HPP
#define CPP_WRAPPERS_DAYCARE_HPP

#include "daycare_helpers.hpp"
#include "pokemon.hpp"

#include "exception_internal.hpp"
#include "private_exports.hpp"
#include "utils/misc.hpp"

#include <pkmn/config.hpp>
#include <pkmn/daycare.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    class daycare
    {
        public:
            explicit daycare(
                const pkmn::daycare::sptr& cpp_daycare
            ): _daycare(cpp_daycare),
               _generation(pkmn::priv::game_name_to_generation(cpp_daycare->get_game()))
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
            }

            daycare(
                const std::string& game
            ): _daycare(pkmn::daycare::make(game)),
               _generation(pkmn::priv::game_name_to_generation(game))
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
            }

            inline std::string get_game()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return _daycare->get_game();
            }

            inline bool can_breed_pokemon()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return _daycare->can_breed_pokemon();
            }

            inline daycare_levelup_pokemon get_levelup_pokemon()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return daycare_levelup_pokemon(_daycare);
            }

            inline daycare_breeding_pokemon get_breeding_pokemon()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return daycare_breeding_pokemon(_daycare);
            }

#ifdef SWIGCSHARP
            // For equality and hash codes
            uintmax_t cptr()
            {
                return uintmax_t(_daycare.get());
            }
#else
            bool operator==(const daycare& rhs) const
            {
                return (_daycare == rhs._daycare);
            }

            bool operator!=(const daycare& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::daycare::sptr _daycare;
            int _generation;
    };
}}

#endif /* CPP_WRAPPERS_DAYCARE_HPP */
