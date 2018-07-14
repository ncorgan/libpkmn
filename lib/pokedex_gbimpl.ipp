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

namespace pkmn
{
    template <typename pksav_type>
    pokedex_gbimpl<pksav_type>::pokedex_gbimpl(
        int game_id,
        pksav_type* p_native
    ): pokedex_impl(game_id)
    {
        const size_t num_bytes = (_generation == 1) ? PKSAV_GEN1_POKEDEX_BUFFER_SIZE_BYTES
                                                    : PKSAV_GEN2_POKEDEX_BUFFER_SIZE_BYTES;
        _seen_buffer.resize(num_bytes, 0);
        _owned_buffer.resize(num_bytes, 0);

        if(p_native != nullptr)
        {
            BOOST_ASSERT(p_native->p_seen != nullptr);
            BOOST_ASSERT(p_native->p_owned != nullptr);

            std::memcpy(
                _seen_buffer.data(),
                p_native->p_seen,
                num_bytes
            );
            std::memcpy(
                _owned_buffer.data(),
                p_native->p_owned,
                num_bytes
            );
        }

        _pksav_pokedex.p_seen  = _seen_buffer.data();
        _pksav_pokedex.p_owned = _owned_buffer.data();

        _p_native = &_pksav_pokedex;
    }

    template <typename pksav_type>
    bool pokedex_gbimpl<pksav_type>::has_seen(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool has_seen = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                _pksav_pokedex.p_seen,
                uint16_t(species_id),
                &has_seen
            );
        )

        return has_seen;
    }

    template <typename pksav_type>
    bool pokedex_gbimpl<pksav_type>::has_caught(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_gbimpl> lock(*this);

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

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_set_has_seen(
        int species_id,
        bool has_seen_value
    )
    {
        PKSAV_CALL(
            pksav_set_pokedex_bit(
                _pksav_pokedex.p_seen,
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
                _pksav_pokedex.p_owned,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            _pksav_pokedex.p_seen,
            _all_seen
        );
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            _pksav_pokedex.p_owned,
            _all_caught
        );
    }
}

#endif /* PKMN_POKEDEX_GBIMPL_IPP */
