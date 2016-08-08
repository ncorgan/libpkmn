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
        PKMN_CONSTEXPR_OR_INLINE spinda_coords():
            x(0), y(0) {}

        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            int xcoord, int ycoord
        ): x(xcoord), y(ycoord) {}

        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            const spinda_coords &other
        ): x(other.x), y(other.y) {}

#ifndef SWIG
        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            spinda_coords&& other
        ): x(other.x), y(other.y) {}
#endif

        int x;
        int y;
    };

    struct spinda_spots {
        PKMN_CONSTEXPR_OR_INLINE spinda_spots():
            topleft(spinda_coords()),
            topright(spinda_coords()),
            bottomleft(spinda_coords()),
            bottomright(spinda_coords()) {}

        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_coords &tl,
            const spinda_coords &tr,
            const spinda_coords &bl,
            const spinda_coords &br
        ): topleft(tl),
           topright(tr),
           bottomleft(bl),
           bottomright(br) {}

#ifndef SWIG
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_coords&& tl,
            spinda_coords&& tr,
            spinda_coords&& bl,
            spinda_coords&& br
        ): topleft(tl),
           topright(tr),
           bottomleft(bl),
           bottomright(br) {}
#endif

        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            int tl_x, int tl_y,
            int tr_x, int tr_y,
            int bl_x, int bl_y,
            int br_x, int br_y
        ): topleft(spinda_coords(tl_x, tl_y)),
           topright(spinda_coords(tr_x, tr_y)),
           bottomleft(spinda_coords(bl_x, bl_y)),
           bottomright(spinda_coords(br_x, br_y)) {}

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
