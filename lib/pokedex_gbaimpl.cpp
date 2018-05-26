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

#define GBAIMPL_RCAST(ptr) (reinterpret_cast<struct pksav_gba_pokedex*>(ptr))

namespace pkmn
{
    pokedex_gbaimpl::pokedex_gbaimpl(
        int game_id,
        struct pksav_gba_pokedex* p_native
    ): pokedex_impl(game_id)
    {
        if(p_native)
        {
            _p_native = p_native;
            _our_mem = false;
        }
        else
        {
            size_t num_bytes = static_cast<size_t>(std::ceil(float(_num_pokemon) / 8.0f));

            _p_native = new struct pksav_gba_pokedex;

            GBAIMPL_RCAST(_p_native)->p_seenA = new uint8_t[num_bytes];
            GBAIMPL_RCAST(_p_native)->p_seenB = new uint8_t[num_bytes];
            GBAIMPL_RCAST(_p_native)->p_seenC = new uint8_t[num_bytes];
            GBAIMPL_RCAST(_p_native)->p_owned = new uint8_t[num_bytes];

            std::memset(GBAIMPL_RCAST(_p_native)->p_seenA, 0, num_bytes);
            std::memset(GBAIMPL_RCAST(_p_native)->p_seenB, 0, num_bytes);
            std::memset(GBAIMPL_RCAST(_p_native)->p_seenC, 0, num_bytes);
            std::memset(GBAIMPL_RCAST(_p_native)->p_owned, 0, num_bytes);

            GBAIMPL_RCAST(_p_native)->p_rse_nat_pokedex_unlockedA  = new uint16_t(0);
            GBAIMPL_RCAST(_p_native)->p_frlg_nat_pokedex_unlockedA = new uint8_t(0);
            GBAIMPL_RCAST(_p_native)->p_nat_pokedex_unlockedB      = new uint8_t(0);
            GBAIMPL_RCAST(_p_native)->p_nat_pokedex_unlockedC      = new uint16_t(0);

            _our_mem = true;
        }
    }

    pokedex_gbaimpl::~pokedex_gbaimpl()
    {
        boost::lock_guard<pokedex_gbaimpl> lock(*this);

        if(_our_mem)
        {
            delete[] GBAIMPL_RCAST(_p_native)->p_seenA;
            delete[] GBAIMPL_RCAST(_p_native)->p_seenB;
            delete[] GBAIMPL_RCAST(_p_native)->p_seenC;
            delete[] GBAIMPL_RCAST(_p_native)->p_owned;

            delete GBAIMPL_RCAST(_p_native)->p_rse_nat_pokedex_unlockedA;
            delete GBAIMPL_RCAST(_p_native)->p_frlg_nat_pokedex_unlockedA;
            delete GBAIMPL_RCAST(_p_native)->p_nat_pokedex_unlockedB;
            delete GBAIMPL_RCAST(_p_native)->p_nat_pokedex_unlockedC;
        }
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
                GBAIMPL_RCAST(_p_native)->p_seenA, // The others should match
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
                GBAIMPL_RCAST(_p_native)->p_owned,
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
                GBAIMPL_RCAST(_p_native),
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
                GBAIMPL_RCAST(_p_native)->p_owned,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    void pokedex_gbaimpl::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            GBAIMPL_RCAST(_p_native)->p_seenA,
            _all_seen
        );
    }

    void pokedex_gbaimpl::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            GBAIMPL_RCAST(_p_native)->p_owned,
            _all_caught
        );
    }
}
