/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEDEX_H
#define PKMN_C_POKEDEX_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/types/string_types.h>

#include <stdbool.h>

struct pkmn_pokedex
{
    char* p_game;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokedex_init(
    const char* game,
    struct pkmn_pokedex* pokedex_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_free(
    struct pkmn_pokedex* pokedex_ptr
);

PKMN_C_API const char* pkmn_pokedex_strerror(
    struct pkmn_pokedex* pokedex_ptr
);

PKMN_C_API enum pkmn_error pkmn_pokedex_has_seen(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool* has_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_set_has_seen(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool has_seen
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_all_seen(
    struct pkmn_pokedex* pokedex_ptr,
    struct pkmn_string_list* all_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_num_seen(
    struct pkmn_pokedex* pokedex_ptr,
    size_t* num_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_has_caught(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool* has_caught_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_set_has_caught(
    struct pkmn_pokedex* pokedex_ptr,
    const char* species,
    bool has_caught
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_all_caught(
    struct pkmn_pokedex* pokedex_ptr,
    struct pkmn_string_list* all_caught_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_num_caught(
    struct pkmn_pokedex* pokedex_ptr,
    size_t* num_caught_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEDEX_H */
