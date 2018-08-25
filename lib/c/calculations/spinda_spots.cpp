/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "error_internal.hpp"

#include <pkmn-c/calculations/spinda_spots.h>

#include <pkmn/calculations/spinda_spots.hpp>

enum pkmn_error pkmn_calculations_add_spinda_coords(
    const struct pkmn_spinda_coords* p_coords1,
    const struct pkmn_spinda_coords* p_coords2,
    struct pkmn_spinda_coords* p_result_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_coords1,
                "p_coords1"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_coords2,
                    "p_coords2"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_result_out,
                    "p_result_out"
                );
    }
    if(!error)
    {
        p_result_out->x = p_coords1->x + p_coords2->x;
        p_result_out->y = p_coords1->y + p_coords2->y;
    }

    return error;
}

enum pkmn_error pkmn_calculations_add_spinda_spots(
    const struct pkmn_spinda_spots* p_spots1,
    const struct pkmn_spinda_spots* p_spots2,
    struct pkmn_spinda_spots* p_result_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_spots1,
                "p_spots1"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_spots2,
                    "p_spots2"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_result_out,
                    "p_result_out"
                );
    }
    if(!error)
    {
        p_result_out->left_ear.x   = p_spots1->left_ear.x + p_spots2->left_ear.x;
        p_result_out->left_ear.y   = p_spots1->left_ear.y + p_spots2->left_ear.y;
        p_result_out->right_ear.x  = p_spots1->right_ear.x + p_spots2->right_ear.x;
        p_result_out->right_ear.y  = p_spots1->right_ear.y + p_spots2->right_ear.y;
        p_result_out->left_face.x  = p_spots1->left_face.x + p_spots2->left_face.x;
        p_result_out->left_face.y  = p_spots1->left_face.y + p_spots2->left_face.y;
        p_result_out->right_face.x = p_spots1->right_face.x + p_spots2->right_face.x;
        p_result_out->right_face.y = p_spots1->right_face.y + p_spots2->right_face.y;
    }

    return error;
}

enum pkmn_error pkmn_calculations_add_spinda_coords_to_spots(
    const struct pkmn_spinda_spots* p_spots,
    const struct pkmn_spinda_coords* p_coords,
    struct pkmn_spinda_spots* p_result_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_spots,
                "p_spots"
            );
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_coords,
                    "p_coords"
                );
    }
    if(!error)
    {
        error = pkmn::c::check_for_null_param(
                    p_result_out,
                    "p_result_out"
                );
    }
    if(!error)
    {
        p_result_out->left_ear.x   = p_spots->left_ear.x + p_coords->x;
        p_result_out->left_ear.y   = p_spots->left_ear.y + p_coords->y;
        p_result_out->right_ear.x  = p_spots->right_ear.x + p_coords->x;
        p_result_out->right_ear.y  = p_spots->right_ear.y + p_coords->y;
        p_result_out->left_face.x  = p_spots->left_face.x + p_coords->x;
        p_result_out->left_face.y  = p_spots->left_face.y + p_coords->y;
        p_result_out->right_face.x = p_spots->right_face.x + p_coords->x;
        p_result_out->right_face.y = p_spots->right_face.y + p_coords->y;
    }

    return error;
}

enum pkmn_error pkmn_calculations_spinda_spot_offset(
    uint32_t personality,
    struct pkmn_spinda_spots* p_spot_offset_out
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn::c::check_for_null_param(
                p_spot_offset_out,
                "p_spot_offset_out"
            );
    if(!error)
    {
        auto impl = [&]()
        {
            pkmn::calculations::spinda_spots cpp = pkmn::calculations::spinda_spot_offset(
                                                       personality
                                                   );
            p_spot_offset_out->left_ear.x   = cpp.left_ear.x;
            p_spot_offset_out->left_ear.y   = cpp.left_ear.y;
            p_spot_offset_out->right_ear.x  = cpp.right_ear.x;
            p_spot_offset_out->right_ear.y  = cpp.right_ear.y;
            p_spot_offset_out->left_face.x  = cpp.left_face.x;
            p_spot_offset_out->left_face.y  = cpp.left_face.y;
            p_spot_offset_out->right_face.x = cpp.right_face.x;
            p_spot_offset_out->right_face.y = cpp.right_face.y;
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
