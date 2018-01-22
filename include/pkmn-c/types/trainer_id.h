/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_TRAINER_ID_H
#define PKMN_C_TYPES_TRAINER_ID_H

#include <stdint.h>

#pragma pack(push, 1)
typedef union
{
    uint32_t id;
    struct
    {
        uint16_t public_id;
        uint16_t secret_id;
    };
} pkmn_trainer_id2_t;
#pragma pack(pop)

#endif /* PKMN_C_TYPES_TRAINER_ID_H */
