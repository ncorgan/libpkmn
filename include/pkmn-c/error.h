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

    PKMN_ERROR_PKSAV_ERROR = 1,

    PKMN_ERROR_RANGE_ERROR = 10,
    PKMN_ERROR_UNIMPLEMENTED_ERROR = 11,
    PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR = 12,

    PKMN_ERROR_NULL_POINTER = 20,

    PKMN_ERROR_INVALID_ARGUMENT = 30,
    PKMN_ERROR_DOMAIN_ERROR = 31,
    PKMN_ERROR_LENGTH_ERROR = 32,
    PKMN_ERROR_OUT_OF_RANGE = 33,
    PKMN_ERROR_LOGIC_ERROR = 39,

    PKMN_ERROR_STD_RANGE_ERROR = 40,
    PKMN_ERROR_OVERFLOW_ERROR = 41,
    PKMN_ERROR_UNDERFLOW_ERROR = 42,
    PKMN_ERROR_RUNTIME_ERROR = 49,

    PKMN_ERROR_STD_EXCEPTION = 50,

    PKMN_ERROR_UNKNOWN_ERROR = 60
} pkmn_error_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API const char* pkmn_strerror();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ERROR_H */
