/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils/misc.hpp"
#include "pokedex_gbaimpl.hpp"

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
    pokedex_gbaimpl::pokedex_gbaimpl(
        int game_id,
        struct pksav_gba_pokedex* p_native
    ): pokedex_impl(game_id),
       _rse_nat_pokedex_unlockedA(0),
       _frlg_nat_pokedex_unlockedA(0),
       _nat_pokedex_unlockedB(0),
       _nat_pokedex_unlockedC(0)
    {
        size_t num_bytes = static_cast<size_t>(std::ceil(float(_num_pokemon) / 8.0f));

        _seen_buffer.resize(num_bytes, 0);
        _owned_buffer.resize(num_bytes, 0);

        if(p_native != nullptr)
        {
            BOOST_ASSERT(p_native->p_seenA != nullptr);
            BOOST_ASSERT(p_native->p_seenB != nullptr);
            BOOST_ASSERT(p_native->p_seenC != nullptr);
            BOOST_ASSERT(p_native->p_owned != nullptr);

            std::memcpy(
                _seen_buffer.data(),
                p_native->p_seenA,
                num_bytes
            );
            std::memcpy(
                _owned_buffer.data(),
                p_native->p_owned,
                num_bytes
            );
        }

        _pksav_pokedex.p_seenA = _seen_buffer.data();
        _pksav_pokedex.p_seenB = _seen_buffer.data();
        _pksav_pokedex.p_seenC = _seen_buffer.data();
        _pksav_pokedex.p_owned = _owned_buffer.data();
        _pksav_pokedex.p_rse_nat_pokedex_unlockedA = &_rse_nat_pokedex_unlockedA;
        _pksav_pokedex.p_frlg_nat_pokedex_unlockedA = &_frlg_nat_pokedex_unlockedA;
        _pksav_pokedex.p_nat_pokedex_unlockedB = &_nat_pokedex_unlockedB;
        _pksav_pokedex.p_nat_pokedex_unlockedC = &_nat_pokedex_unlockedC;

        _p_native = &_pksav_pokedex;
    }

    bool pokedex_gbaimpl::has_seen(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbaimpl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool has_seen = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                _pksav_pokedex.p_seenA,
                uint16_t(species_id),
                &has_seen
            );
        )

        return has_seen;
    }

    bool pokedex_gbaimpl::has_caught(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbaimpl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool has_caught = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                _pksav_pokedex.p_owned,
                uint16_t(species_id),
                &has_caught
            );
        )

        return has_caught;
    }

    void pokedex_gbaimpl::_set_has_seen(
        int species_id,
        bool has_seen_value
    )
    {
        PKSAV_CALL(
            pksav_gba_pokedex_set_has_seen(
                &_pksav_pokedex,
                uint16_t(species_id),
                has_seen_value
            );
        )
    }

    void pokedex_gbaimpl::_set_has_caught(
        int species_id,
        bool has_caught_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                _pksav_pokedex.p_owned,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    void pokedex_gbaimpl::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            _pksav_pokedex.p_seenA,
            _all_seen
        );
    }

    void pokedex_gbaimpl::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            _pksav_pokedex.p_owned,
            _all_caught
        );
    }
}
