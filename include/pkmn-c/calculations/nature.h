/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_CALCULATIONS_NATURE_H
#define PKMN_C_CALCULATIONS_NATURE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_calculations_nature(
    uint32_t personality,
    char* nature_out,
    size_t buffer_len
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_CALCULATIONS_NATURE_H */
