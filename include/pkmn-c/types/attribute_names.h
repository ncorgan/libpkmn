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
struct pkmn_attribute_names
{
    //! Numeric attribute names.
    struct pkmn_string_list numeric_attribute_names;
    //! String attribute names.
    struct pkmn_string_list string_attribute_names;
    //! Boolean attribute names.
    struct pkmn_string_list boolean_attribute_names;
};

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
PKMN_C_API enum pkmn_error pkmn_attribute_names_free(
    struct pkmn_attribute_names* attribute_names_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ATTRIBUTE_NAMES_H */
