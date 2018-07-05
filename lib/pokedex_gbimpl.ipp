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

#include <boost/thread/lock_guard.hpp>

#ifndef PKMN_POKEDEX_GBIMPL_IPP
#define PKMN_POKEDEX_GBIMPL_IPP

#define GBIMPL_RCAST(ptr) (reinterpret_cast<pksav_type*>(ptr))

namespace pkmn
{
    template <typename pksav_type>
    pokedex_gbimpl<pksav_type>::pokedex_gbimpl(
        int game_id,
        pksav_type* p_native
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
            _p_native = new pksav_type;

            GBIMPL_RCAST(_p_native)->p_seen = new uint8_t[num_bytes];
            GBIMPL_RCAST(_p_native)->p_owned = new uint8_t[num_bytes];

            std::memset(GBIMPL_RCAST(_p_native)->p_seen,  0, num_bytes);
            std::memset(GBIMPL_RCAST(_p_native)->p_owned, 0, num_bytes);

            _our_mem = true;
        }
    }

    template <typename pksav_type>
    pokedex_gbimpl<pksav_type>::~pokedex_gbimpl()
    {
        if(_our_mem)
        {
            delete[] GBIMPL_RCAST(_p_native)->p_seen;
            delete[] GBIMPL_RCAST(_p_native)->p_owned;
            delete GBIMPL_RCAST(_p_native);
        }
    }

    template <typename pksav_type>
    bool pokedex_gbimpl<pksav_type>::has_seen(
        pkmn::e_species species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

        bool has_seen = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                GBIMPL_RCAST(_p_native)->p_seen,
                uint16_t(species),
                &has_seen
            );
        )

        return has_seen;
    }

    template <typename pksav_type>
    bool pokedex_gbimpl<pksav_type>::has_caught(
        pkmn::e_species species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

        bool has_caught = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                GBIMPL_RCAST(_p_native)->p_owned,
                uint16_t(species),
                &has_caught
            );
        )

        return has_caught;
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_set_has_seen(
        int species_id,
        bool has_seen_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                GBIMPL_RCAST(_p_native)->p_seen,
                uint16_t(species_id),
                has_seen_value
            );
        )
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_set_has_caught(
        int species_id,
        bool has_caught_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                GBIMPL_RCAST(_p_native)->p_owned,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            GBIMPL_RCAST(_p_native)->p_seen,
            _all_seen
        );
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            GBIMPL_RCAST(_p_native)->p_owned,
            _all_caught
        );
    }
}

#endif /* PKMN_POKEDEX_GBIMPL_IPP */
