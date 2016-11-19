/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/spinda_spots.h>

#include <pkmn/calculations/spinda_spots.hpp>

pkmn_error_t pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    pkmn_spinda_spots_t* spot_offset_out
) {
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
