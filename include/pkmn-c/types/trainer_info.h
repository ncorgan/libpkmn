/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_TRAINER_INFO_H
#define PKMN_C_TYPES_TRAINER_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/trainer_id.h>

typedef struct
{
    char* name;
    pkmn_trainer_id_t id;
    pkmn_gender_t gender;
} pkmn_trainer_info_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_trainer_info_free(
    pkmn_trainer_info_t* trainer_info_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_TRAINER_INFO_H */
