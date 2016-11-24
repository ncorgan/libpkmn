/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_UTILS_PATHS_H
#define PKMN_C_UTILS_PATHS_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_get_appdata_dir(
    char* appdata_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_database_path(
    char* database_path_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_images_dir(
    char* images_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

PKMN_API pkmn_error_t pkmn_get_tmp_dir(
    char* tmp_dir_out,
    size_t buffer_len,
    size_t* actual_strlen_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_UTILS_PATHS_H */
