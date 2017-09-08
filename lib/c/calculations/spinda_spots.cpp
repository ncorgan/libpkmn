/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/spinda_spots.h>

#include <pkmn/calculations/spinda_spots.hpp>

pkmn_error_t pkmn_calculations_add_spinda_coords(
    const pkmn_spinda_coords_t* coords1,
    const pkmn_spinda_coords_t* coords2,
    pkmn_spinda_coords_t* result_out
) {
    PKMN_CHECK_NULL_PARAM(coords1);
    PKMN_CHECK_NULL_PARAM(coords2);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->x = coords1->x + coords2->x;
    result_out->y = coords1->y + coords2->y;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_calculations_add_spinda_spots(
    const pkmn_spinda_spots_t* spots1,
    const pkmn_spinda_spots_t* spots2,
    pkmn_spinda_spots_t* result_out
) {
    PKMN_CHECK_NULL_PARAM(spots1);
    PKMN_CHECK_NULL_PARAM(spots2);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->left_ear.x = spots1->left_ear.x + spots2->left_ear.x;
    result_out->left_ear.y = spots1->left_ear.y + spots2->left_ear.y;
    result_out->right_ear.x = spots1->right_ear.x + spots2->right_ear.x;
    result_out->right_ear.y = spots1->right_ear.y + spots2->right_ear.y;
    result_out->left_face.x = spots1->left_face.x + spots2->left_face.x;
    result_out->left_face.y = spots1->left_face.y + spots2->left_face.y;
    result_out->right_face.x = spots1->right_face.x + spots2->right_face.x;
    result_out->right_face.y = spots1->right_face.y + spots2->right_face.y;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_calculations_add_spinda_coords_to_spots(
    const pkmn_spinda_spots_t* spots,
    const pkmn_spinda_coords_t* coords,
    pkmn_spinda_spots_t* result_out
) {
    PKMN_CHECK_NULL_PARAM(spots);
    PKMN_CHECK_NULL_PARAM(coords);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->left_ear.x = spots->left_ear.x + coords->x;
    result_out->left_ear.y = spots->left_ear.y + coords->y;
    result_out->right_ear.x = spots->right_ear.x + coords->x;
    result_out->right_ear.y = spots->right_ear.y + coords->y;
    result_out->left_face.x = spots->left_face.x + coords->x;
    result_out->left_face.y = spots->left_face.y + coords->y;
    result_out->right_face.x = spots->right_face.x + coords->x;
    result_out->right_face.y = spots->right_face.y + coords->y;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    pkmn_spinda_spots_t* spot_offset_out
) {
    PKMN_CHECK_NULL_PARAM(spot_offset_out);

    PKMN_CPP_TO_C(
        pkmn::calculations::spinda_spots cpp = pkmn::calculations::spinda_spot_offset(
                                                   personality
                                               );
        spot_offset_out->left_ear.x = cpp.left_ear.x;
        spot_offset_out->left_ear.y = cpp.left_ear.y;
        spot_offset_out->right_ear.x = cpp.right_ear.x;
        spot_offset_out->right_ear.y = cpp.right_ear.y;
        spot_offset_out->left_face.x = cpp.left_face.x;
        spot_offset_out->left_face.y = cpp.left_face.y;
        spot_offset_out->right_face.x = cpp.right_face.x;
        spot_offset_out->right_face.y = cpp.right_face.y;
    )
}
