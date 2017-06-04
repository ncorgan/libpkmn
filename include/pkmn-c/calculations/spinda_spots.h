/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_SPINDA_SPOTS_H
#define PKMN_C_CALCULATIONS_SPINDA_SPOTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

typedef struct {
    int x;
    int y;
} pkmn_spinda_coords_t;

typedef struct {
    pkmn_spinda_coords_t left_ear;
    pkmn_spinda_coords_t right_ear;
    pkmn_spinda_coords_t left_face;
    pkmn_spinda_coords_t right_face;
} pkmn_spinda_spots_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_add_spinda_coords(
    const pkmn_spinda_coords_t* coords1,
    const pkmn_spinda_coords_t* coords2,
    pkmn_spinda_coords_t* result_out
);

PKMN_API pkmn_error_t pkmn_calculations_add_spinda_spots(
    const pkmn_spinda_spots_t* spots1,
    const pkmn_spinda_spots_t* spots2,
    pkmn_spinda_spots_t* result_out
);

PKMN_API pkmn_error_t pkmn_calculations_add_spinda_coords_to_spots(
    const pkmn_spinda_spots_t* spots,
    const pkmn_spinda_coords_t* coords,
    pkmn_spinda_spots_t* result_out
);

PKMN_API pkmn_error_t pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    pkmn_spinda_spots_t* spot_offset_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_SPINDA_SPOTS_H */
