/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_EXCEPTION_INTERNAL_HPP
#define PKMN_EXCEPTION_INTERNAL_HPP

#include <pkmn/exception.hpp>

#include <limits>
#include <string>

namespace pkmn
{
    PKMN_INLINE void enforce_EV_bounds(
        const std::string& stat,
        int value,
        bool is_game_modern
    )
    {
        std::string field = stat + " EV";
        int max_value = is_game_modern ? 255 : 65535;

        pkmn::enforce_bounds(
            field,
            value,
            0,
            max_value
        );
    }

    PKMN_INLINE void enforce_IV_bounds(
        const std::string& stat,
        int value,
        bool is_game_modern
    )
    {
        std::string field = stat + " IV";
        int max_value = is_game_modern ? 31 : 15;

        pkmn::enforce_bounds(
            field,
            value,
            0,
            max_value
        );
    }

    PKMN_INLINE void enforce_gb_trainer_id_bounds(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_bounds(
            "Trainer ID",
            trainer_id,
            0U,
            uint32_t(std::numeric_limits<uint16_t>::max())
        );
    }
}

#endif /* PKMN_EXCEPTION_INTERNAL_HPP */
