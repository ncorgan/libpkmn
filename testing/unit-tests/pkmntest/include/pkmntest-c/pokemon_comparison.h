/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_C_POKEMON_COMPARISON_H
#define PKMNTEST_C_POKEMON_COMPARISON_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon2.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PKMNTEST_C_API void get_random_pokemon(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game
);

PKMNTEST_C_API void compare_pokemon(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
);

#endif /* PKMNTEST_C_POKEMON_COMPARISON_H */
