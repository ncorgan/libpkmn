/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ERROR_H
#define PKMN_C_ERROR_H

#include <pkmn-c/config.h>

typedef enum {
    PKMN_ERROR_NONE = 0,

    PKMN_INVALID_ARGUMENT = 10,
    PKMN_DOMAIN_ERROR = 11,
    PKMN_LENGTH_ERROR = 12,
    PKMN_OUT_OF_RANGE = 13,
    PKMN_LOGIC_ERROR = 19,

    PKMN_RANGE_ERROR = 20,
    PKMN_OVERFLOW_ERROR = 21,
    PKMN_UNDERFLOW_ERROR = 22,
    PKMN_RUNTIME_ERROR = 29,

    PKMN_STD_EXCEPTION = 50,

    PKMN_UNKNOWN_ERROR = 60
} pkmn_error_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API const char* pkmn_strerror();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ERROR_H */
