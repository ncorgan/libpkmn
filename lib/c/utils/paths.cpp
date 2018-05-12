/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/utils/paths.h>

#include <pkmn/utils/paths.hpp>

enum pkmn_error pkmn_get_appdata_dir(
    char* p_appdata_dir_out,
    size_t buffer_len,
    size_t* p_appdata_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_appdata_dir_out);
    // p_appdata_dir_length_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_appdata_dir(),
            p_appdata_dir_out,
            buffer_len,
            p_appdata_dir_length_out
        );
    )
}

enum pkmn_error pkmn_get_database_path(
    char* p_database_path_out,
    size_t buffer_len,
    size_t* p_database_path_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_database_path_out);
    // p_database_path_length_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_database_path(),
            p_database_path_out,
            buffer_len,
            p_database_path_length_out
        );
    )
}

enum pkmn_error pkmn_get_images_dir(
    char* p_images_dir_out,
    size_t buffer_len,
    size_t* p_images_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_images_dir_out);
    // p_images_dir_length_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_images_dir(),
            p_images_dir_out,
            buffer_len,
            p_images_dir_length_out
        );
    )
}

enum pkmn_error pkmn_get_tmp_dir(
    char* p_tmp_dir_out,
    size_t buffer_len,
    size_t* p_tmp_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(p_tmp_dir_out);
    // p_tmp_dir_length_out can be NULL

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_tmp_dir(),
            p_tmp_dir_out,
            buffer_len,
            p_tmp_dir_length_out
        );
    )
}
