/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ATTRIBUTE_NAMES_H
#define PKMN_C_TYPES_ATTRIBUTE_NAMES_H

#include <pkmn-c/error.h>

#include <pkmn-c/types/string_types.h>

typedef struct
{
    pkmn_string_list_t numeric_attribute_names;
    pkmn_string_list_t string_attribute_names;
} pkmn_attribute_names_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_attribute_names_free(
    pkmn_attribute_names_t* attribute_names_ptr
)
{
    if(!attribute_names_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_string_list_free(&attribute_names_ptr->numeric_attribute_names);
    pkmn_string_list_free(&attribute_names_ptr->string_attribute_names);
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ATTRIBUTE_NAMES_H */
