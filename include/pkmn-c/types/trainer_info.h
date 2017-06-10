/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_TRAINER_INFO_H
#define PKMN_C_TYPES_TRAINER_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/gender.h>

#include <stdint.h>

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
    char nickname[PKMN_NICKNAME_LENGTH];
    char trainer_name[PKMN_TRAINER_NAME_LENGTH];
    pkmn_trainer_id_t trainer_id;
    pkmn_gender_t trainer_gender;
} pkmn_trainer_info_t;

#endif /* PKMN_C_TYPES_TRAINER_INFO_H */
