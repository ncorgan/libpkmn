/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/spinda_spots.h>

#include <pkmn/calculations/spinda_spots.hpp>

pkmn_error_t pkmn_calculations_add_spinda_coords(
    const pkmn_spinda_coords_t* coords1_ptr,
    const pkmn_spinda_coords_t* coords2_ptr,
    pkmn_spinda_coords_t* result_out
)
{
    PKMN_CHECK_NULL_PARAM(coords1_ptr);
    PKMN_CHECK_NULL_PARAM(coords2_ptr);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->x = coords1_ptr->x + coords2_ptr->x;
    result_out->y = coords1_ptr->y + coords2_ptr->y;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_calculations_add_spinda_spots(
    const pkmn_spinda_spots_t* spots1_ptr,
    const pkmn_spinda_spots_t* spots2_ptr,
    pkmn_spinda_spots_t* result_out
)
{
    PKMN_CHECK_NULL_PARAM(spots1_ptr);
    PKMN_CHECK_NULL_PARAM(spots2_ptr);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->left_ear.x = spots1_ptr->left_ear.x + spots2_ptr->left_ear.x;
    result_out->left_ear.y = spots1_ptr->left_ear.y + spots2_ptr->left_ear.y;
    result_out->right_ear.x = spots1_ptr->right_ear.x + spots2_ptr->right_ear.x;
    result_out->right_ear.y = spots1_ptr->right_ear.y + spots2_ptr->right_ear.y;
    result_out->left_face.x = spots1_ptr->left_face.x + spots2_ptr->left_face.x;
    result_out->left_face.y = spots1_ptr->left_face.y + spots2_ptr->left_face.y;
    result_out->right_face.x = spots1_ptr->right_face.x + spots2_ptr->right_face.x;
    result_out->right_face.y = spots1_ptr->right_face.y + spots2_ptr->right_face.y;

    return PKMN_ERROR_NONE;
}

pkmn_error_t pkmn_calculations_add_spinda_coords_to_spots(
    const pkmn_spinda_spots_t* spots_ptr,
    const pkmn_spinda_coords_t* coords_ptr,
    pkmn_spinda_spots_t* result_out
) {
    PKMN_CHECK_NULL_PARAM(spots_ptr);
    PKMN_CHECK_NULL_PARAM(coords_ptr);
    PKMN_CHECK_NULL_PARAM(result_out);

    result_out->left_ear.x = spots_ptr->left_ear.x + coords_ptr->x;
    result_out->left_ear.y = spots_ptr->left_ear.y + coords_ptr->y;
    result_out->right_ear.x = spots_ptr->right_ear.x + coords_ptr->x;
    result_out->right_ear.y = spots_ptr->right_ear.y + coords_ptr->y;
    result_out->left_face.x = spots_ptr->left_face.x + coords_ptr->x;
    result_out->left_face.y = spots_ptr->left_face.y + coords_ptr->y;
    result_out->right_face.x = spots_ptr->right_face.x + coords_ptr->x;
    result_out->right_face.y = spots_ptr->right_face.y + coords_ptr->y;

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
