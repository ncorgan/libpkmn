/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_DATETIME_H
#define PKMN_C_TYPES_DATETIME_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

typedef struct {
    int year;
    int month;
    int day;

    int hour;
    int minute;
    int second;

    int frames;
} pkmn_datetime_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_current_datetime(
    pkmn_datetime_t* datetime_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_DATETIME_H */
