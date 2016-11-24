/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/utils/paths.h>

#include <pkmn/utils/paths.hpp>

pkmn_error_t pkmn_get_appdata_dir(
    char* appdata_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::get_appdata_dir(),
            appdata_dir_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_get_database_path(
    char* database_path_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::get_database_path(),
            database_path_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_get_images_dir(
    char* images_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::get_images_dir(),
            images_dir_out,
            buffer_len,
            actual_strlen_out
        );
    )
}

pkmn_error_t pkmn_get_tmp_dir(
    char* tmp_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C(
        pkmn::std_string_to_c_str(
            pkmn::get_tmp_dir(),
            tmp_dir_out,
            buffer_len,
            actual_strlen_out
        );
    )
}
