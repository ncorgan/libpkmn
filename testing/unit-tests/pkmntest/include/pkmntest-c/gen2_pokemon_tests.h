/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN2_POKEMON_TESTS_H
#define GEN2_POKEMON_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon.h>

PKMNTEST_API void pkmntest_gen2_invalid_pokemon_test(
    const char* game
);

PKMNTEST_API void pkmntest_gen2_unown_test(
    const char* game
);

PKMNTEST_API void pkmntest_gen2_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
);

#endif /* GEN2_POKEMON_TESTS_H */
