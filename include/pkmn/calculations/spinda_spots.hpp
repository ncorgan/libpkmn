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

    //! X/Y coordinates of an individual Spinda spot.
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
            left_ear(spinda_coords()),
            right_ear(spinda_coords()),
            left_face(spinda_coords()),
            right_face(spinda_coords()) {}

        /*!
         * @brief Constructor taking in four coordinate sets.
         * \param le Coordinates for spot on the left ear.
         * \param re Coordinates for spot on the right ear.
         * \param lf Coordinates for spot on the left of the face.
         * \param rf Coordinates for spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_coords &le,
            const spinda_coords &re,
            const spinda_coords &lf,
            const spinda_coords &rf
        ): left_ear(le),
           right_ear(re),
           left_face(lf),
           right_face(rf) {}

        /*!
         * @brief Copy constructor.
         * \param spots Coordinate set whose coordinates to copy.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_spots &spots
        ): left_ear(spots.left_ear),
           right_ear(spots.right_ear),
           left_face(spots.left_face),
           right_face(spots.right_face) {}

        /*!
         * @brief Constructor taking in individual parts of coordinates.
         * \param le_x X coordinate of spot on the left ear.
         * \param le_y Y coordinate of spot on the left ear.
         * \param re_x X coordinate of spot on the right ear.
         * \param re_y Y coordinate of spot on the right ear.
         * \param lf_x X coordinate of spot on the left of the face.
         * \param lf_y Y coordinate of spot on the left of the face.
         * \param rf_x X coordinate of spot on the right of the face.
         * \param rf_y Y coordinate of spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            int le_x, int le_y,
            int re_x, int re_y,
            int lf_x, int lf_y,
            int rf_x, int rf_y
        ): left_ear(spinda_coords(le_x, le_y)),
           right_ear(spinda_coords(re_x, re_y)),
           left_face(spinda_coords(lf_x, lf_y)),
           right_face(spinda_coords(rf_x, rf_y)) {}

#ifndef SWIG
        /*!
         * @brief Constructor taking in references to individual spots.
         * \param le Coordinates for spot on the left ear.
         * \param re Coordinates for spot on the right ear.
         * \param lf Coordinates for spot on the left of the face.
         * \param rf Coordinates for spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_coords&& le,
            spinda_coords&& re,
            spinda_coords&& lf,
            spinda_coords&& rf
        ): left_ear(le),
           right_ear(re),
           left_face(lf),
           right_face(rf) {}

        /*!
         * @brief Move constructor.
         * \param spots Coordinate set whose coordinates to move.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_spots&& spots
        ): left_ear(spots.left_ear),
           right_ear(spots.right_ear),
           left_face(spots.left_face),
           right_face(spots.right_face) {}

        /*!
         * @brief Assignment operator.
         * \param rhs Spots whose member to assign.
         */
        PKMN_INLINE spinda_spots& operator=(
            const spinda_spots &rhs
        ) {
            this->left_ear = rhs.left_ear;
            this->right_ear = rhs.right_ear;
            this->left_face = rhs.left_face;
            this->right_face = rhs.right_face;
            return *this;
        }
#endif

        //! Coordinates of spot on the left ear.
        spinda_coords left_ear;

        //! Coordinates of spot on the right ear.
        spinda_coords right_ear;

        //! Coordinates of spot on the left of the face.
        spinda_coords left_face;

        //! Coordinates of spot on the right of the face.
        spinda_coords right_face;
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
        return (lhs.left_ear != rhs.left_ear) and \
               (lhs.right_ear != rhs.right_ear) and \
               (lhs.left_face != rhs.left_face) and \
               (lhs.right_face != rhs.right_face);
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
