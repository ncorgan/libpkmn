/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_BREEDING_COMPATIBILITY_H
#define PKMN_C_BREEDING_COMPATIBILITY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_breeding_are_pokemon_species_compatible(
    const char* p_species1,
    const char* p_species2,
    bool* p_are_pokemon_species_compatible_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BREEDING_COMPATIBILITY_H */
