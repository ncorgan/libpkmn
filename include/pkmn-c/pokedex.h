/*
 * p_Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEDEX_H
#define PKMN_C_POKEDEX_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/game.h>

#include <pkmn-c/types/string_types.h>

#include <stdbool.h>

struct pkmn_pokedex
{
    enum pkmn_game game;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokedex_init(
    enum pkmn_game game,
    struct pkmn_pokedex* p_pokedex_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_free(
    struct pkmn_pokedex* p_pokedex
);

PKMN_C_API const char* pkmn_pokedex_strerror(
    const struct pkmn_pokedex* p_pokedex
);

PKMN_C_API enum pkmn_error pkmn_pokedex_has_seen(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool* p_has_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_set_has_seen_species(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool has_seen
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_all_seen(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_string_list* p_all_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_num_seen(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_seen_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_has_caught(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool* p_has_caught_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_set_has_caught_species(
    const struct pkmn_pokedex* p_pokedex,
    const char* p_species,
    bool has_caught
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_all_caught(
    const struct pkmn_pokedex* p_pokedex,
    struct pkmn_string_list* p_all_caught_out
);

PKMN_C_API enum pkmn_error pkmn_pokedex_get_num_caught(
    const struct pkmn_pokedex* p_pokedex,
    size_t* p_num_caught_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEDEX_H */
