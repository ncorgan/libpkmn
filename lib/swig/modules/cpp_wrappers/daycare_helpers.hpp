/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_DAYCARE_HELPERS_HPP
#define CPP_WRAPPERS_DAYCARE_HELPERS_HPP

#include "pokemon.hpp"

#include "exception_internal.hpp"
#include "private_exports.hpp"
#include "utils/misc.hpp"

#include <pkmn/config.hpp>
#include <pkmn/daycare.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    class daycare_levelup_pokemon
    {
        public:
            explicit daycare_levelup_pokemon(
                const pkmn::daycare::sptr& daycare
            ): _daycare(daycare)
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
            }

            inline pkmn::swig::pokemon get_pokemon(int index)
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    1,
                    _daycare->get_levelup_pokemon_capacity()
                );
#else
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    0,
                    _daycare->get_levelup_pokemon_capacity()-1
                );
#endif

                return pkmn::swig::pokemon(
                           _daycare->get_levelup_pokemon().at(index)
                       );
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon& new_pokemon
            )
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    1,
                    _daycare->get_levelup_pokemon_capacity()
                );
#endif

                _daycare->set_levelup_pokemon(
                    index,
                    new_pokemon.get_internal()
                );
            }

            inline int get_capacity()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return _daycare->get_levelup_pokemon_capacity();
            }

        private:
            pkmn::daycare::sptr _daycare;
    };

    class daycare_breeding_pokemon
    {
        public:
            explicit daycare_breeding_pokemon(
                const pkmn::daycare::sptr& daycare
            ): _daycare(daycare)
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
            }

            inline pkmn::swig::pokemon get_pokemon(int index)
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    1,
                    _daycare->get_breeding_pokemon_capacity()
                );
#else
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    0,
                    _daycare->get_breeding_pokemon_capacity()-1
                );
#endif

                return pkmn::swig::pokemon(
                           _daycare->get_breeding_pokemon().at(index)
                       );
            }

            inline void set_pokemon(
                int index,
                const pkmn::swig::pokemon& new_pokemon
            )
            {
                BOOST_ASSERT(_daycare.get() != nullptr);
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Index",
                    index,
                    1,
                    _daycare->get_breeding_pokemon_capacity()
                );
#endif

                _daycare->set_breeding_pokemon(
                    index,
                    new_pokemon.get_internal()
                );
            }

            inline int get_capacity()
            {
                BOOST_ASSERT(_daycare.get() != nullptr);

                return _daycare->get_breeding_pokemon_capacity();
            }

        private:
            pkmn::daycare::sptr _daycare;
    };

}}

#endif /* CPP_WRAPPERS_DAYCARE_HELPERS_HPP */
