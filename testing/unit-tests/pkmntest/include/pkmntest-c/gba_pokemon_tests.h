/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GBA_POKEMON_TESTS_H
#define GBA_POKEMON_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon.h>

PKMNTEST_API void pkmntest_gba_invalid_pokemon_test(
    const char* game
);

PKMNTEST_API void pkmntest_gba_unown_test(
    const char* game
);

PKMNTEST_API void pkmntest_gba_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
);

#endif /* GBA_POKEMON_TESTS_H */
