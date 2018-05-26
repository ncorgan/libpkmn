/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_POKEMON_PARTY_H
#define PKMN_C_POKEMON_PARTY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/types/pokemon_list.h>

struct pkmn_pokemon_party
{
    char* p_game;
    size_t capacity;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_pokemon_party_init(
    const char* p_game,
    struct pkmn_pokemon_party* p_pokemon_party_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_party_free(
    struct pkmn_pokemon_party* p_pokemon_party
);

PKMN_C_API const char* pkmn_pokemon_party_strerror(
    struct pkmn_pokemon_party* p_pokemon_party
);

PKMN_C_API enum pkmn_error pkmn_pokemon_party_get_num_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t* p_num_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_party_get_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon_out
);

PKMN_C_API enum pkmn_error pkmn_pokemon_party_set_pokemon(
    struct pkmn_pokemon_party* p_pokemon_party,
    size_t position,
    struct pkmn_pokemon* p_pokemon
);

PKMN_C_API enum pkmn_error pkmn_pokemon_party_as_list(
    struct pkmn_pokemon_party* p_pokemon_party,
    struct pkmn_pokemon_list* p_pokemon_list_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_POKEMON_PARTY_H */
