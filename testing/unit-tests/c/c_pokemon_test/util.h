/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMNTEST_C_UTIL_H
#define PKMNTEST_C_UTIL_H

#include <pkmntest-c/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static PKMNTEST_INLINE bool file_exists(
    const char* filepath
) {
    FILE* file = fopen(filepath, "rb");
    if(file) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

static PKMNTEST_INLINE  bool string_list_contains(
    pkmn_string_list_t* string_list,
    const char* string
) {
    for(size_t i = 0; i < string_list->length; ++i) {
        if(!strcmp(string_list->strings[i], string)) {
            return true;
        }
    }

    return false;
}

#endif /* PKMNTEST_C_UTIL_H */
