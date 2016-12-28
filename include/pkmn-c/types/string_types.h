/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_STRING_TYPES_H
#define PKMN_C_TYPES_STRING_TYPES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct {
    char** strings;
    size_t length;
} pkmn_string_list_t;

typedef struct {
    char* first;
    char* second;
} pkmn_string_pair_t;

#ifdef __cplusplus
extern "C" {
#endif

static PKMN_INLINE pkmn_error_t pkmn_string_list_free(
    pkmn_string_list_t* string_list
) {
    for(size_t i = 0; i < string_list->length; ++i) {
        free(string_list->strings[i]);
    }

    free(string_list->strings);
    string_list->strings = NULL;
    string_list->length = 0;

    return PKMN_ERROR_NONE;
}

static PKMN_INLINE pkmn_error_t pkmn_string_pair_free(
    pkmn_string_pair_t* string_pair
) {
    free(string_pair->first);
    free(string_pair->second);

    string_pair->first  = NULL;
    string_pair->second = NULL;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_STRING_TYPES_H */
