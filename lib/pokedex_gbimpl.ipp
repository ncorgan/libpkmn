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
        pksav_type* native_ptr
    ): pokedex_impl(game_id)
    {
        if(native_ptr)
        {
            _native_ptr = native_ptr;
            _our_mem = false;
        }
        else
        {
            size_t num_bytes = static_cast<size_t>(std::ceil(float(_num_pokemon) / 8.0f));
            _native_ptr = new pksav_type;

            GBIMPL_RCAST(_native_ptr)->seen_ptr = new uint8_t[num_bytes]{0};
            GBIMPL_RCAST(_native_ptr)->owned_ptr = new uint8_t[num_bytes]{0};

            _our_mem = true;
        }
    }

    template <typename pksav_type>
    pokedex_gbimpl<pksav_type>::~pokedex_gbimpl()
    {
        if(_our_mem)
        {
            delete[] GBIMPL_RCAST(_native_ptr)->seen_ptr;
            delete[] GBIMPL_RCAST(_native_ptr)->owned_ptr;
            delete GBIMPL_RCAST(_native_ptr);
        }
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
                GBIMPL_RCAST(_native_ptr)->seen_ptr,
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
                GBIMPL_RCAST(_native_ptr)->owned_ptr,
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
                GBIMPL_RCAST(_native_ptr)->seen_ptr,
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
                GBIMPL_RCAST(_native_ptr)->owned_ptr,
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_seen()
    {
        _update_member_vector_with_pksav(
            GBIMPL_RCAST(_native_ptr)->seen_ptr,
            _all_seen
        );
    }

    template <typename pksav_type>
    void pokedex_gbimpl<pksav_type>::_update_all_caught()
    {
        _update_member_vector_with_pksav(
            GBIMPL_RCAST(_native_ptr)->owned_ptr,
            _all_caught
        );
    }
}

#endif /* PKMN_POKEDEX_GBIMPL_IPP */
