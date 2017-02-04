/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_TESTS_COMMON_H
#define POKEMON_TESTS_COMMON_H

#include <pkmn-c/pokemon.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_invalid_pokemon(
    const char* game
);

#ifdef __cplusplus
}
#endif

#endif /* POKEMON_TESTS_COMMON_H */
