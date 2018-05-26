/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_BOX_H
#define PKMN_C_POKEMON_BOX_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

struct pkmn_pokemon_box
{
    char* p_game;
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokemon_box_init(
    const char* p_game,
    struct pkmn_pokemon_box* p_pokemon_box_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_free(
    struct pkmn_pokemon_box* p_pokemon_box
);

PKMN_C_API const char* pkmn_pokemon_box_strerror(
    struct pkmn_pokemon_box* p_pokemon_box
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_name(
    struct pkmn_pokemon_box* p_pokemon_box,
    char* p_name_buffer_out,
    size_t name_buffer_length,
    size_t* p_actual_name_length_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_set_name(
    struct pkmn_pokemon_box* p_pokemon_box,
    const char* p_name
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_num_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t* p_num_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_get_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_set_pokemon(
    struct pkmn_pokemon_box* p_pokemon_box,
    size_t position,
    struct pkmn_pokemon* p_pokemon
);

PKMN_C_API enum pkmn_error pkmn_pokemon_box_as_list(
    struct pkmn_pokemon_box* p_pokemon_box,
    struct pkmn_pokemon_list* p_pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_BOX_H */
