/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokedex_impl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/types/shared_ptr.hpp>

#include <pksav/common/pokedex.h>

#include <boost/thread/lock_guard.hpp>

#include <cassert>

namespace pkmn
{
    pokedex::sptr pokedex::make(
        const std::string& game
    )
    {
        int game_id = pkmn::database::game_name_to_id(game);

        return pkmn::make_shared<pokedex_impl>(game_id);
    }

    pokedex_impl::pokedex_impl(
        int game_id,
        void* native_has_seen,
        void* native_has_caught
    ): _game_id(game_id),
       _generation(pkmn::database::game_id_to_generation(game_id))
    {
        // TODO: Boost assert
        assert(!native_has_seen == !native_has_caught);

        static const std::vector<size_t> generation_pokedex_sizes =
        {
            0, 151, 251, 386, 493, 649, 721, 807
        };

        if(native_has_seen)
        {
            size_t num_bytes = std::ceil(float(generation_pokedex_sizes[_generation]) / 8.0f);
            _native_has_seen = reinterpret_cast<void*>(new uint8_t[num_bytes]);
            _native_has_caught = reinterpret_cast<void*>(new uint8_t[num_bytes]);
        }
        else
        {
            _native_has_seen = native_has_seen;
            _native_has_caught = native_has_caught;
            _our_mem = false;
        }
    }

    pokedex_impl::~pokedex_impl()
    {
        if(_our_mem)
        {
            delete[] reinterpret_cast<uint8_t*>(_native_has_seen);
            delete[] reinterpret_cast<uint8_t*>(_native_has_caught);
        }
    }

    std::string pokedex_impl::get_game()
    {
        return pkmn::database::game_id_to_name(_game_id);
    }

    bool pokedex_impl::has_seen(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool ret = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_seen),
                uint16_t(species_id),
                &ret
            );
        )

        return ret;
    }

    void pokedex_impl::set_has_seen(
        const std::string& species,
        bool has_seen_value
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);

        PKSAV_CALL(
            pksav_set_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_seen),
                uint16_t(species_id),
                has_seen_value
            );
        )
    }

    bool pokedex_impl::has_caught(
        const std::string& species
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);
        bool ret = false;

        PKSAV_CALL(
            pksav_get_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_caught),
                uint16_t(species_id),
                &ret
            );
        )

        return ret;
    }

    void pokedex_impl::set_has_caught(
        const std::string& species,
        bool has_caught_value
    )
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        int species_id = pkmn::database::species_name_to_id(species);

        PKSAV_CALL(
            pksav_set_pokedex_bit(
                reinterpret_cast<uint8_t*>(_native_has_caught),
                uint16_t(species_id),
                has_caught_value
            );
        )
    }

    void* pokedex_impl::get_native_has_seen()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        return _native_has_seen;
    }

    void* pokedex_impl::get_native_has_caught()
    {
        boost::lock_guard<pokedex_impl> lock(*this);

        return _native_has_caught;
    }
}
