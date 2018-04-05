/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/misc.hpp"
#include "pokedex_gbimpl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <pksav/common/pokedex.h>

#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cassert>

namespace pkmn
{
    pokedex_gbimpl::pokedex_gbimpl(
        int game_id,
        uint8_t* native_has_seen,
        uint8_t* native_has_caught
    ): pokedex_impl(game_id)
    {
        BOOST_ASSERT(!native_has_seen == !native_has_caught);

        if(native_has_seen)
        {
            _native_has_seen = native_has_seen;
            _native_has_caught = native_has_caught;
            _our_mem = false;
        }
        else
        {
            size_t num_bytes = static_cast<size_t>(std::ceil(float(_num_pokemon) / 8.0f));
            _native_has_seen = new uint8_t[num_bytes]{0};
            _native_has_caught = new uint8_t[num_bytes]{0};

            _our_mem = true;
        }
    }

    pokedex_gbimpl::~pokedex_gbimpl()
    {
        if(_our_mem)
        {
            delete[] _native_has_seen;
            delete[] _native_has_caught;
        }
    }

    bool pokedex_gbimpl::has_seen(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool has_seen = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                _native_has_seen,
                uint16_t(species_id),
                &has_seen
            );
        )

        return has_seen;
    }

    bool pokedex_gbimpl::has_caught(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool has_caught = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                _native_has_caught,
                uint16_t(species_id),
                &has_caught
            );
        )

        return has_caught;
    }

    void pokedex_gbimpl::_set_has_seen(
        int species_id,
        bool has_seen_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                _native_has_seen,
                uint16_t(species_id),
                has_seen_value
            );
        )
    }

    void pokedex_gbimpl::_set_has_caught(
        int species_id,
        bool has_caught_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                _native_has_caught,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    void pokedex_gbimpl::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            _native_has_seen,
            _all_seen
        );
    }

    void pokedex_gbimpl::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            _native_has_caught,
            _all_caught
        );
    }
}
