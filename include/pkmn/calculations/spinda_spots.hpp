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

    //! X/Y coordinates of an indiviudal Spinda spot.
    struct spinda_coords {
        //! Default constructor.
        PKMN_CONSTEXPR_OR_INLINE spinda_coords():
            x(0), y(0) {}

        /*!
         * @brief Constructor taking X and Y coordinates.
         * \param xcoord X coordinate
         * \param ycoord Y coordinate
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            int xcoord, int ycoord
        ): x(xcoord), y(ycoord) {}

        /*!
         * @brief Copy constructor.
         * \param other Coordinates to copy
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            const spinda_coords &other
        ): x(other.x), y(other.y) {}

#ifndef SWIG
        /*!
         * @brief Move constructor.
         * \param other Reference to coordinates whose members to move
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_coords(
            spinda_coords&& other
        ): x(other.x), y(other.y) {}

        /*!
         * @brief Assignment operator.
         * \param rhs Coordinates whose members to assign.
         */
        PKMN_INLINE spinda_coords& operator=(
            const spinda_coords &rhs
        ) {
            this->x = rhs.x;
            this->y = rhs.y;
            return *this;
        }
#endif

        //! X coordinate.
        int x;
        //! Y coordinate.
        int y;
    };

#ifndef SWIG
    /*!
     * @brief Equality check between two Spinda spot coordinates.
     * \param lhs Coordinates to left of == operator
     * \param rhs Coordinates to right of == operator
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const spinda_coords &lhs,
        const spinda_coords &rhs
    ) {
        return (lhs.x == rhs.x) and (lhs.y == rhs.y);
    }

    /*!
     * @brief Inequality check between two Spinda spot coordinates.
     * \param lhs Coordinates to left of != operator
     * \param rhs Coordinates to right of != operator
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const spinda_coords &lhs,
        const spinda_coords &rhs
    ) {
        return (not (lhs == rhs));
    }
#endif

    //! Coordinates of all four Spinda spots.
    struct spinda_spots {
        //! Default constructor.
        PKMN_CONSTEXPR_OR_INLINE spinda_spots():
            top_left(spinda_coords()),
            top_right(spinda_coords()),
            bottom_left(spinda_coords()),
            bottom_right(spinda_coords()) {}

        /*!
         * @brief Constructor taking in four coordinate sets.
         * \param tl Coordinates for top-left spot.
         * \param tr Coordinates for top-right spot.
         * \param bl Coordinates for bottom-left spot.
         * \param br Coordinates for bottom-right spot.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_coords &tl,
            const spinda_coords &tr,
            const spinda_coords &bl,
            const spinda_coords &br
        ): top_left(tl),
           top_right(tr),
           bottom_left(bl),
           bottom_right(br) {}

        /*!
         * @brief Copy constructor.
         * \param spots Coordinate set whose coordinates to copy.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_spots &spots
        ): top_left(spots.top_left),
           top_right(spots.top_right),
           bottom_left(spots.bottom_left),
           bottom_right(spots.bottom_right) {}

        /*!
         * @brief Constructor taking in individual parts of coordinates.
         * \param tl_x X coordinate of top-left spot.
         * \param tl_y Y coordinate of top-left spot.
         * \param tr_x X coordinate of top-right spot.
         * \param tr_y Y coordinate of top-right spot.
         * \param bl_x X coordinate of bottom-left spot.
         * \param bl_y Y coordinate of bottom-left spot.
         * \param br_x X coordinate of bottom-right spot.
         * \param br_y Y coordinate of bottom-right spot.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            int tl_x, int tl_y,
            int tr_x, int tr_y,
            int bl_x, int bl_y,
            int br_x, int br_y
        ): top_left(spinda_coords(tl_x, tl_y)),
           top_right(spinda_coords(tr_x, tr_y)),
           bottom_left(spinda_coords(bl_x, bl_y)),
           bottom_right(spinda_coords(br_x, br_y)) {}

#ifndef SWIG
        /*!
         * @brief Constructor taking in references to individual spots.
         * \param tl Coordinates for top-left spot.
         * \param tr Coordinates for top-right spot.
         * \param bl Coordinates for bottom-left spot.
         * \param br Coordinates for bottom-right spot.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_coords&& tl,
            spinda_coords&& tr,
            spinda_coords&& bl,
            spinda_coords&& br
        ): top_left(tl),
           top_right(tr),
           bottom_left(bl),
           bottom_right(br) {}

        /*!
         * @brief Move constructor.
         * \param spots Coordinate set whose coordinates to move.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_spots&& spots
        ): top_left(spots.top_left),
           top_right(spots.top_right),
           bottom_left(spots.bottom_left),
           bottom_right(spots.bottom_right) {}

        /*!
         * @brief Assignment operator.
         * \param rhs Spots whose member to assign.
         */
        PKMN_INLINE spinda_spots& operator=(
            const spinda_spots &rhs
        ) {
            this->top_left = rhs.top_left;
            this->top_right = rhs.top_right;
            this->bottom_left = rhs.bottom_left;
            this->bottom_right = rhs.bottom_right;
            return *this;
        }
#endif

        //! Coordinates of top-left spot.
        spinda_coords top_left;

        //! Coordinates of top-right spot.
        spinda_coords top_right;

        //! Coordinates of bottom-left spot.
        spinda_coords bottom_left;

        //! Coordinates of top-right spot.
        spinda_coords bottom_right;
    };

#ifndef SWIG
    /*!
     * @brief Equality check between two sets of Spinda spot coordinates.
     * \param lhs Coordinates to left of == operator
     * \param rhs Coordinates to right of == operator
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const spinda_spots &lhs,
        const spinda_spots &rhs
    ) {
        return (lhs.top_left != rhs.top_left) and \
               (lhs.top_right != rhs.top_right) and \
               (lhs.bottom_left != rhs.bottom_left) and \
               (lhs.bottom_right != rhs.bottom_right);
    }

    /*!
     * @brief Inequality check between two sets of Spinda spot coordinates.
     * \param lhs Coordinates to left of != operator
     * \param rhs Coordinates to right of != operator
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const spinda_spots &lhs,
        const spinda_spots &rhs
    ) {
        return (not (lhs == rhs));
    }
#endif

    /*!
     * @brief Calculates positions of Spinda's spots in a Generation III sprite.
     *
     * The locations of Spinda's four spots are calculated based on the Spinda's
     * personality. As such, there are 2^32 combinations.
     *
     * \param personality Spinda's personality
     * \returns Locations of Spinda's spots
     */
    PKMN_API spinda_spots gen3_spinda_spots(
        uint32_t personality
    );

    /*!
     * @brief Calculates positions of Spinda's spots in a Generation IV-V sprite.
     *
     * The locations of Spinda's four spots are calculated based on the Spinda's
     * personality. As such, there are 2^32 combinations.
     *
     * \param personality Spinda's personality
     * \returns Locations of Spinda's spots
     */
    PKMN_API spinda_spots nds_spinda_spots(
        uint32_t personality
    );

}}

#endif /* PKMN_CALCULATIONS_SPINDA_SPOTS_HPP */
