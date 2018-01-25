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

#include <stdint.h>
#include <stdlib.h>

// These lengths include the null terminator.
#define PKMN_NICKNAME_LENGTH 13
#define PKMN_TRAINER_NAME_LENGTH 8

#pragma pack(push, 1)
typedef union
{
    struct
    {
        uint16_t public_id;
        uint16_t secret_id;
    };
    uint32_t id;
} pkmn_trainer_id_t;
#pragma pack(pop)

typedef struct
{
    char trainer_name[PKMN_TRAINER_NAME_LENGTH];
    pkmn_trainer_id_t trainer_id;
    pkmn_gender_t trainer_gender;
} pkmn_trainer_info_t;

typedef struct
{
    char* name;
    pkmn_trainer_id2_t id;
    pkmn_gender_t gender;
} pkmn_trainer_info2_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_trainer_info_free(
    pkmn_trainer_info2_t* trainer_info_ptr
)
{
    if(!trainer_info_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    free(trainer_info_ptr->name);
    trainer_info_ptr->name = NULL;

    trainer_info_ptr->id.id = 0U;
    trainer_info_ptr->gender = PKMN_GENDER_MALE;

    return PKMN_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_TRAINER_INFO_H */
