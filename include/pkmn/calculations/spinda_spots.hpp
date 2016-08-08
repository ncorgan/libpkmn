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

        PKMN_INLINE spinda_coords& operator=(
            const spinda_coords &rhs
        ) {
            this->x = rhs.x;
            this->y = rhs.y;
            return *this;
        }
#endif

        int x;
        int y;
    };

#ifndef SWIG
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const spinda_coords &lhs,
        const spinda_coords &rhs
    ) {
        return (lhs.x == rhs.x) and (lhs.y == rhs.y);
    }

    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const spinda_coords &lhs,
        const spinda_coords &rhs
    ) {
        return (not (lhs == rhs));
    }
#endif

    struct spinda_spots {
        PKMN_CONSTEXPR_OR_INLINE spinda_spots():
            top_left(spinda_coords()),
            top_right(spinda_coords()),
            bottom_left(spinda_coords()),
            bottom_right(spinda_coords()) {}

        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_coords &tl,
            const spinda_coords &tr,
            const spinda_coords &bl,
            const spinda_coords &br
        ): top_left(tl),
           top_right(tr),
           bottom_left(bl),
           bottom_right(br) {}

#ifndef SWIG
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_coords&& tl,
            spinda_coords&& tr,
            spinda_coords&& bl,
            spinda_coords&& br
        ): top_left(tl),
           top_right(tr),
           bottom_left(bl),
           bottom_right(br) {}
#endif

        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            int tl_x, int tl_y,
            int tr_x, int tr_y,
            int bl_x, int bl_y,
            int br_x, int br_y
        ): top_left(spinda_coords(tl_x, tl_y)),
           top_right(spinda_coords(tr_x, tr_y)),
           bottom_left(spinda_coords(bl_x, bl_y)),
           bottom_right(spinda_coords(br_x, br_y)) {}

        spinda_coords top_left;
        spinda_coords top_right;
        spinda_coords bottom_left;
        spinda_coords bottom_right;
    };

#ifndef SWIG
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const spinda_spots &lhs,
        const spinda_spots &rhs
    ) {
        return (lhs.top_left != rhs.top_left) and \
               (lhs.top_right != rhs.top_right) and \
               (lhs.bottom_left != rhs.bottom_left) and \
               (lhs.bottom_right != rhs.bottom_right);
    }

    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const spinda_spots &lhs,
        const spinda_spots &rhs
    ) {
        return (not (lhs == rhs));
    }
#endif

    PKMN_API spinda_spots gen3_spinda_spots(
        uint32_t personality
    );

    PKMN_API spinda_spots nds_spinda_spots(
        uint32_t personality
    );

}}

#endif /* PKMN_CALCULATIONS_SPINDA_SPOTS_HPP */
