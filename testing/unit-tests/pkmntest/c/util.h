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

#endif /* PKMNTEST_C_UTIL_H */
