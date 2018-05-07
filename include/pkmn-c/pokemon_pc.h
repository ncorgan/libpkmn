/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PC_H
#define PKMN_C_POKEMON_PC_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon_box.h>

#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/string_types.h>

struct pkmn_pokemon_pc
{
    char* p_game;
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokemon_pc_init(
    const char* p_game,
    struct pkmn_pokemon_pc* p_pokemon_pc_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_pc_free(
    struct pkmn_pokemon_pc* p_pokemon_pc
);

PKMN_C_API const char* pkmn_pokemon_pc_strerror(
    struct pkmn_pokemon_pc* p_pokemon_pc
);

PKMN_C_API enum pkmn_error pkmn_pokemon_pc_get_box(
    struct pkmn_pokemon_pc* p_pokemon_pc,
    size_t index,
    struct pkmn_pokemon_box* p_pokemon_box_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_pc_get_box_names(
    struct pkmn_pokemon_pc* p_pokemon_pc,
    struct pkmn_string_list* p_box_names_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_pc_as_list(
    struct pkmn_pokemon_pc* pokemon_box_ptr,
    struct pkmn_pokemon_box_list* p_pokemon_box_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PC_H */
