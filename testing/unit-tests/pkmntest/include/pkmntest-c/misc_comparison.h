/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_MISC_COMPARISON_H
#define PKMNTEST_C_MISC_COMPARISON_H

#include <pkmntest-c/config.h>

#include <pkmn-c/types/attribute_names.h>

PKMNTEST_C_API void compare_attribute_names(
    const struct pkmn_attribute_names* attribute_names1_ptr,
    const struct pkmn_attribute_names* attribute_names2_ptr
);

#endif /* PKMNTEST_C_MISC_COMPARISON_H */
