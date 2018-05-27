/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DAYCARE_H
#define PKMN_C_DAYCARE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>

#include <pkmn-c/types/pokemon_list.h>

#include <stdbool.h>

struct pkmn_daycare
{
    char* p_game;

    bool can_breed_pokemon;

    size_t levelup_pokemon_capacity;
    size_t breeding_pokemon_capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_daycare_init(
    const char* p_game,
    struct pkmn_daycare* p_daycare_out
);

PKMN_C_API enum pkmn_error pkmn_daycare_free(
    struct pkmn_daycare* p_daycare
);

PKMN_C_API const char* pkmn_daycare_strerror(
    struct pkmn_daycare* p_daycare
);

PKMN_C_API enum pkmn_error pkmn_daycare_get_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_levelup_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_daycare_set_levelup_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
);

PKMN_C_API enum pkmn_error pkmn_daycare_get_levelup_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_levelup_pokemon_list_out
);

PKMN_C_API enum pkmn_error pkmn_daycare_get_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    struct pkmn_pokemon* p_breeding_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_daycare_set_breeding_pokemon(
    const struct pkmn_daycare* p_daycare,
    size_t index,
    const struct pkmn_pokemon* p_pokemon
);

PKMN_C_API enum pkmn_error pkmn_daycare_get_breeding_pokemon_as_list(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon_list* p_breeding_pokemon_list_out
);

PKMN_C_API enum pkmn_error pkmn_daycare_get_egg(
    const struct pkmn_daycare* p_daycare,
    struct pkmn_pokemon* p_egg_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DAYCARE_H */
