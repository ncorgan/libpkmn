/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
    struct spinda_coords
    {
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

        spinda_coords(const spinda_coords&) = default;
        spinda_coords& operator=(const spinda_coords&) = default;

#ifndef SWIG
        spinda_coords(spinda_coords&&) = default;
        spinda_coords& operator=(spinda_coords&&) = default;
#endif

        /*!
         * @brief Equality check with another Spinda spots struct.
         * \param rhs struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const spinda_coords& rhs
        ) const
        {
            return (this->x == rhs.x) and (this->y == rhs.y);
        }

        /*!
         * @brief Inequality check with another Spinda spots struct.
         * \param rhs struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const spinda_coords& rhs
        ) const
        {
            return !operator==(rhs);
        }

        /*!
         * @brief Add the given Spinda coordinates to this instance.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_coords operator+(
            const spinda_coords& rhs
        ) const
        {
            return spinda_coords(
                       this->x + rhs.x,
                       this->y + rhs.y
                   );
        }

        /*!
         * @brief Add the given Spinda coordinates to this instance in-place.
         */
        inline spinda_coords& operator+=(
            const spinda_coords& rhs
        )
        {
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
    struct spinda_spots
    {
        //! Default constructor.
        PKMN_CONSTEXPR_OR_INLINE spinda_spots():
            left_ear(spinda_coords()),
            right_ear(spinda_coords()),
            left_face(spinda_coords()),
            right_face(spinda_coords()) {}

        /*!
         * @brief Constructor taking in four coordinate sets.
         * \param l_ear Coordinates for spot on the left ear.
         * \param r_ear Coordinates for spot on the right ear.
         * \param l_face Coordinates for spot on the left of the face.
         * \param r_face Coordinates for spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            const spinda_coords& l_ear,
            const spinda_coords& r_ear,
            const spinda_coords& l_face,
            const spinda_coords& r_face
        ): left_ear(l_ear),
           right_ear(r_ear),
           left_face(l_face),
           right_face(r_face) {}

        /*!
         * @brief Constructor taking in individual parts of coordinates.
         * \param leftear_x X coordinate of spot on the left ear.
         * \param leftear_y Y coordinate of spot on the left ear.
         * \param rightear_x X coordinate of spot on the right ear.
         * \param rightear_y Y coordinate of spot on the right ear.
         * \param leftface_x X coordinate of spot on the left of the face.
         * \param leftface_y Y coordinate of spot on the left of the face.
         * \param rightface_x X coordinate of spot on the right of the face.
         * \param rightface_y Y coordinate of spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            int leftear_x, int leftear_y,
            int rightear_x, int rightear_y,
            int leftface_x, int leftface_y,
            int rightface_x, int rightface_y
        ): left_ear(spinda_coords(leftear_x, leftear_y)),
           right_ear(spinda_coords(rightear_x, rightear_y)),
           left_face(spinda_coords(leftface_x, leftface_y)),
           right_face(spinda_coords(rightface_x, rightface_y)) {}

        spinda_spots(const spinda_spots&) = default;
        spinda_spots& operator=(const spinda_spots&) = default;

#ifndef SWIG
        /*!
         * @brief Constructor taking in references to individual spots.
         * \param leftear_coords Coordinates for spot on the left ear.
         * \param rightear_coords Coordinates for spot on the right ear.
         * \param leftface_coords Coordinates for spot on the left of the face.
         * \param rightface_coords Coordinates for spot on the right of the face.
         */
        PKMN_CONSTEXPR_OR_INLINE spinda_spots(
            spinda_coords&& leftear_coords,
            spinda_coords&& rightear_coords,
            spinda_coords&& leftface_coords,
            spinda_coords&& rightface_coords
        ): left_ear(std::move(leftear_coords)),
           right_ear(std::move(rightear_coords)),
           left_face(std::move(leftface_coords)),
           right_face(std::move(rightface_coords)) {}

        spinda_spots(spinda_spots&&) = default;
        spinda_spots& operator=(spinda_spots&&) = default;
#endif

        /*!
         * @brief Equality check with another set of Spinda spot coordinates.
         * \param rhs Struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator==(
            const spinda_spots& rhs
        ) const
        {
            return (this->left_ear == rhs.left_ear) and
                   (this->right_ear == rhs.right_ear) and
                   (this->left_face == rhs.left_face) and
                   (this->right_face == rhs.right_face);
        }

        /*!
         * @brief Inequality check with another set of Spinda spot coordinates.
         * \param rhs Struct with which to compare self
         */
        PKMN_CONSTEXPR_OR_INLINE bool operator!=(
            const spinda_spots& rhs
        ) const
        {
            return !operator==(rhs);
        }

        PKMN_CONSTEXPR_OR_INLINE spinda_spots operator+(
            const spinda_spots& rhs
        ) const {
            return spinda_spots(
                       this->left_ear + rhs.left_ear,
                       this->right_ear + rhs.right_ear,
                       this->left_face + rhs.left_face,
                       this->right_face + rhs.right_face
                   );
        }

        PKMN_CONSTEXPR_OR_INLINE spinda_spots operator+(
            const spinda_coords& rhs
        ) const {
            return spinda_spots(
                       this->left_ear + rhs,
                       this->right_ear + rhs,
                       this->left_face + rhs,
                       this->right_face + rhs
                   );
        }

        inline spinda_spots& operator+=(
            const spinda_spots& rhs
        ) {
            this->left_ear += rhs.left_ear;
            this->right_ear += rhs.right_ear;
            this->left_face += rhs.left_face;
            this->right_face += rhs.right_face;
            return *this;
        }

        inline spinda_spots& operator+=(
            const spinda_coords& rhs
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
    PKMN_CONSTEXPR_OR_INLINE spinda_spots spinda_spot_offset(
        uint32_t personality
    )
    {
        return spinda_spots(
                   spinda_coords(
                       int( personality & 0x0000000F),
                       int((personality & 0x000000F0) >> 4)
                   ),
                   spinda_coords(
                       int((personality & 0x00000F00) >> 8),
                       int((personality & 0x0000F000) >> 12)
                   ),
                   spinda_coords(
                       int((personality & 0x000F0000) >> 16),
                       int((personality & 0x00F00000) >> 20)
                   ),
                   spinda_coords(
                       int((personality & 0x0F000000) >> 24),
                       int((personality & 0xF0000000) >> 28)
                   )
               );
    }

}}

#endif /* PKMN_CALCULATIONS_SPINDA_SPOTS_HPP */
