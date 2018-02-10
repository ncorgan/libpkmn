/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_SPINDA_SPOTS_HPP
#define PKMN_CALCULATIONS_SPINDA_SPOTS_HPP

#include <pkmn/config.hpp>

#include <algorithm>
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
        ): x(std::move(other.x)), y(std::move(other.y)) {}

        /*!
         * @brief Assignment operator.
         * \param rhs Coordinates whose members to assign.
         */
        inline spinda_coords& operator=(
            const spinda_coords &rhs
        ) {
            this->x = rhs.x;
            this->y = rhs.y;
            return *this;
        }
#endif

        /*!
         * @brief Equality check with another Spinda spots struct.
         * \param rhs struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const spinda_coords &rhs
        ) const {
            return (this->x == rhs.x) and (this->y == rhs.y);
        }

        /*!
         * @brief Inequality check with another Spinda spots struct.
         * \param rhs struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const spinda_coords &rhs
        ) const {
            return (this->x != rhs.x) or (this->y != rhs.y);
        }

        PKMN_CONSTEXPR_OR_INLINE spinda_coords operator+(
            const spinda_coords &rhs
        ) const {
            return spinda_coords(
                       this->x + rhs.x,
                       this->y + rhs.y
                   );
        }

        inline spinda_coords& operator+=(
            const spinda_coords &rhs
        ) {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        //! X coordinate.
        int x;
        //! Y coordinate.
        int y;
    };

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
        ): left_ear(std::move(le)),
           right_ear(std::move(re)),
           left_face(std::move(lf)),
           right_face(std::move(rf)) {}

        /*!
         * @brief Move constructor.
         * \param spots Coordinate set whose coordinates to move.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_spots&& spots
        ): left_ear(std::move(spots.left_ear)),
           right_ear(std::move(spots.right_ear)),
           left_face(std::move(spots.left_face)),
           right_face(std::move(spots.right_face)) {}

        /*!
         * @brief Assignment operator.
         * \param rhs Spots whose member to assign.
         */
        inline spinda_spots& operator=(
            const spinda_spots &rhs
        ) {
            this->left_ear = rhs.left_ear;
            this->right_ear = rhs.right_ear;
            this->left_face = rhs.left_face;
            this->right_face = rhs.right_face;
            return *this;
        }
#endif

        /*!
         * @brief Equality check with another set of Spinda spot coordinates.
         * \param rhs Struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const spinda_spots &rhs
        ) const {
            return (this->left_ear == rhs.left_ear) and \
                   (this->right_ear == rhs.right_ear) and \
                   (this->left_face == rhs.left_face) and \
                   (this->right_face == rhs.right_face);
        }

        /*!
         * @brief Inequality check with another set of Spinda spot coordinates.
         * \param rhs Struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const spinda_spots &rhs
        ) const {
            return (this->left_ear != rhs.left_ear) or \
                   (this->right_ear != rhs.right_ear) or \
                   (this->left_face != rhs.left_face) or \
                   (this->right_face != rhs.right_face);
        }

        PKMN_CONSTEXPR_OR_INLINE spinda_spots operator+(
            const spinda_spots &rhs
        ) const {
            return spinda_spots(
                       this->left_ear + rhs.left_ear,
                       this->right_ear + rhs.right_ear,
                       this->left_face + rhs.left_face,
                       this->right_face + rhs.right_face
                   );
        }

        PKMN_CONSTEXPR_OR_INLINE spinda_spots operator+(
            const spinda_coords &rhs
        ) const {
            return spinda_spots(
                       this->left_ear + rhs,
                       this->right_ear + rhs,
                       this->left_face + rhs,
                       this->right_face + rhs
                   );
        }

        inline spinda_spots& operator+=(
            const spinda_spots &rhs
        ) {
            this->left_ear += rhs.left_ear;
            this->right_ear += rhs.right_ear;
            this->left_face += rhs.left_face;
            this->right_face += rhs.right_face;
            return *this;
        }

        inline spinda_spots& operator+=(
            const spinda_coords &rhs
        ) {
            this->left_ear += rhs;
            this->right_ear += rhs;
            this->left_face += rhs;
            this->right_face += rhs;
            return *this;
        }

        //! Coordinates of spot on the left ear.
        spinda_coords left_ear;

        //! Coordinates of spot on the right ear.
        spinda_coords right_ear;

        //! Coordinates of spot on the left of the face.
        spinda_coords left_face;

        //! Coordinates of spot on the right of the face.
        spinda_coords right_face;
    };

    /*!
     * @brief Calculates offsets of the spots on a Generation III-V Spinda.
     *
     * The locations of Spinda's four spots are calculated based on the Spinda's
     * personality. As such, there are 2^32 combinations.
     *
     * \param personality Spinda's personality
     * \returns Locations of Spinda's spots
     */
    PKMN_API spinda_spots spinda_spot_offset(
        uint32_t personality
    );

}}

#endif /* PKMN_CALCULATIONS_SPINDA_SPOTS_HPP */
