/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_SPINDA_SPOTS_H
#define PKMN_C_CALCULATIONS_SPINDA_SPOTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

//! X/Y coordinates of an individual Spinda spot.
struct pkmn_spinda_coords
{
    int x;
    int y;
};

//! Coordinates of all four Spinda spots.
struct pkmn_spinda_spots
{
    //! Coordinates of the left ear spot.
    struct pkmn_spinda_coords left_ear;
    //! Coordinates of the right ear spot.
    struct pkmn_spinda_coords right_ear;
    //! Coordinates of the left face spot.
    struct pkmn_spinda_coords left_face;
    //! Coordinates of the right face spot.
    struct pkmn_spinda_coords right_face;
};

#ifdef __cplusplus
extern "C" {
#endif

//! Add the two Spinda spot coordinates.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_add_spinda_coords(
    const struct pkmn_spinda_coords* p_coords1,
    const struct pkmn_spinda_coords* p_coords2,
    struct pkmn_spinda_coords* p_result_out
);

//! Add all of the spots of the two Spinda spot groups.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_add_spinda_spots(
    const struct pkmn_spinda_spots* p_spots1,
    const struct pkmn_spinda_spots* p_spots2,
    struct pkmn_spinda_spots* p_result_out
);

//! Add the the given coordinates to all spots in the given group.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_add_spinda_coords_to_spots(
    const struct pkmn_spinda_spots* p_spots,
    const struct pkmn_spinda_coords* p_coords,
    struct pkmn_spinda_spots* p_result_out
);

//! Calculates offset of the spots on a Generation III-V Spinda.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any spot_Offset_out is NULL
 */
PKMN_C_API enum pkmn_error pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    struct pkmn_spinda_spots* p_spot_offset_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SPINDA_SPOTS_H */
