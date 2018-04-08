/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/utils/paths.h>

#include <pkmn/utils/paths.hpp>

enum pkmn_error pkmn_get_appdata_dir(
    char* appdata_dir_out,
    size_t buffer_len,
    size_t* appdata_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(appdata_dir_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_appdata_dir(),
            appdata_dir_out,
            buffer_len,
            appdata_dir_length_out
        );
    )
}

enum pkmn_error pkmn_get_database_path(
    char* database_path_out,
    size_t buffer_len,
    size_t* database_path_length_out
)
{
    PKMN_CHECK_NULL_PARAM(database_path_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_database_path(),
            database_path_out,
            buffer_len,
            database_path_length_out
        );
    )
}

enum pkmn_error pkmn_get_images_dir(
    char* images_dir_out,
    size_t buffer_len,
    size_t* images_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(images_dir_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_images_dir(),
            images_dir_out,
            buffer_len,
            images_dir_length_out
        );
    )
}

enum pkmn_error pkmn_get_tmp_dir(
    char* tmp_dir_out,
    size_t buffer_len,
    size_t* tmp_dir_length_out
)
{
    PKMN_CHECK_NULL_PARAM(tmp_dir_out);

    PKMN_CPP_TO_C(
        pkmn::c::string_cpp_to_c(
            pkmn::get_tmp_dir(),
            tmp_dir_out,
            buffer_len,
            tmp_dir_length_out
        );
    )
}
