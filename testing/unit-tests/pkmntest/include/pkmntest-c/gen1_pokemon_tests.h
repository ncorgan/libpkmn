/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_POKEMON_TESTS_H
#define GEN1_POKEMON_TESTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/pokemon.h>

PKMN_API void pkmntest_gen1_invalid_pokemon_test(
    const char* game
);

PKMN_API void pkmntest_gen1_friendship_test(
    const char* game
);

PKMN_API void pkmntest_gen1_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
);

#endif /* GEN1_POKEMON_TESTS_H */
