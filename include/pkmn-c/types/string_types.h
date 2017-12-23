/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_STRING_TYPES_H
#define PKMN_C_TYPES_STRING_TYPES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

typedef struct
{
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

static inline pkmn_error_t pkmn_string_list_free(
    pkmn_string_list_t* string_list_ptr
)
{
    if(!string_list_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    for(size_t i = 0; i < string_list_ptr->length; ++i)
    {
        free(string_list_ptr->strings[i]);
    }

    free(string_list_ptr->strings);
    string_list_ptr->strings = NULL;
    string_list_ptr->length = 0;

    return PKMN_ERROR_NONE;
}

static inline pkmn_error_t pkmn_string_pair_free(
    pkmn_string_pair_t* string_pair_ptr
)
{
    if(!string_pair_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(string_pair_ptr->first);
    free(string_pair_ptr->second);

    string_pair_ptr->first  = NULL;
    string_pair_ptr->second = NULL;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_STRING_TYPES_H */
