/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ATTRIBUTE_NAMES_H
#define PKMN_C_TYPES_ATTRIBUTE_NAMES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/string_types.h>

//! Lists of attributes for each type from the given class.
typedef struct
{
    //! Numeric attribute names.
    pkmn_string_list_t numeric_attribute_names;
    //! String attribute names.
    pkmn_string_list_t string_attribute_names;
    //! Boolean attribute names.
    pkmn_string_list_t boolean_attribute_names;
} pkmn_attribute_names_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Free all dynamic memory in a pkmn_attribute_names_t.
 *
 * \param attribute_names_ptr Pointer to the attribute names struct to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if attribute_names_ptr is NULL
 */
PKMN_C_API pkmn_error_t pkmn_attribute_names_free(
    pkmn_attribute_names_t* attribute_names_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ATTRIBUTE_NAMES_H */
