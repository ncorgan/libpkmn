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

struct pkmn_trainer_info
{
    char* name;
    union pkmn_trainer_id id;
    enum pkmn_gender gender;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_trainer_info_free(
    struct pkmn_trainer_info* trainer_info_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_TRAINER_INFO_H */
