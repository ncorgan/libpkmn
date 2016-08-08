/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SPINDA_SPOTS_HPP
#define PKMN_CALCULATIONS_SPINDA_SPOTS_HPP

#include <pkmn/config.hpp>

#include <cstdint>

namespace pkmn { namespace calculations {

    struct spinda_coords {
        int x;
        int y;
    };

    struct spinda_spots {
        spinda_coords topleft;
        spinda_coords topright;
        spinda_coords bottomleft;
        spinda_coords bottomright;
    };

    PKMN_API spinda_spots get_gen3_spinda_spots(
        uint32_t personality
    );

    PKMN_API spinda_spots get_nds_spinda_spots(
        uint32_t personality
    );

}}

#endif /* PKMN_CALCULATIONS_SPINDA_SPOTS_HPP */
