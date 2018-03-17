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
typedef struct
{
    //! X coordinate.
    int x;
    //! Y coordinate.
    int y;
} pkmn_spinda_coords_t;

//! Coordinates of all four Spinda spots.
typedef struct
{
    //! Coordinates of the left ear spot.
    pkmn_spinda_coords_t left_ear;
    //! Coordinates of the right ear spot.
    pkmn_spinda_coords_t right_ear;
    //! Coordinates of the left face spot.
    pkmn_spinda_coords_t left_face;
    //! Coordinates of the right face spot.
    pkmn_spinda_coords_t right_face;
} pkmn_spinda_spots_t;

#ifdef __cplusplus
extern "C" {
#endif

//! Add the two Spinda spot coordinates.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_add_spinda_coords(
    const pkmn_spinda_coords_t* coords1_ptr,
    const pkmn_spinda_coords_t* coords2_ptr,
    pkmn_spinda_coords_t* result_out
);

//! Add all of the spots of the two Spinda spot groups.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_add_spinda_spots(
    const pkmn_spinda_spots_t* spots1_ptr,
    const pkmn_spinda_spots_t* spots2_ptr,
    pkmn_spinda_spots_t* result_out
);

//! Add the the given coordinates to all spots in the given group.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any parameter is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_add_spinda_coords_to_spots(
    const pkmn_spinda_spots_t* spots_ptr,
    const pkmn_spinda_coords_t* coords_ptr,
    pkmn_spinda_spots_t* result_out
);

//! Calculates offset of the spots on a Generation III-V Spinda.
/*!
 * \returns ::PKMN_ERROR_NONE upon successful completion
 * \returns ::PKMN_ERROR_NULL_POINTER if any spot_Offset_out is NULL
 */
PKMN_C_API pkmn_error_t pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    pkmn_spinda_spots_t* spot_offset_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SPINDA_SPOTS_H */
