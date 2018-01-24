/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_POKEMON_INFO_H
#define PKMN_C_TYPES_POKEMON_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn-c/types/gender.h>

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    char* form;
    char* nickname;
    pkmn_gender_t gender;
    bool is_egg;
    char* held_item;
    char* ability;
    uint32_t personality;

    pkmn_database_pokemon_entry_t database_entry;
} pkmn_pokemon_info_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline pkmn_error_t pkmn_pokemon_info_free(
    pkmn_pokemon_info_t* pokemon_info_ptr
)
{
    if(!pokemon_info_ptr)
    {
        return PKMN_ERROR_NULL_POINTER;
    }

    pkmn_error_t error = PKMN_ERROR_NONE;

    free(pokemon_info_ptr->form);
    pokemon_info_ptr->form = NULL;

    free(pokemon_info_ptr->nickname);
    pokemon_info_ptr->nickname = NULL;

    pokemon_info_ptr->gender = PKMN_GENDER_MALE;
    pokemon_info_ptr->is_egg = false;

    free(pokemon_info_ptr->held_item);
    pokemon_info_ptr->held_item = NULL;

    free(pokemon_info_ptr->ability);
    pokemon_info_ptr->ability = NULL;

    pokemon_info_ptr->personality = 0U;

    error = pkmn_database_pokemon_entry_free(&pokemon_info_ptr->database_entry);

    return error;
}

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_POKEMON_INFO_H */
