/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN2_POKEMON_TESTS_H
#define GEN2_POKEMON_TESTS_H

#include <pkmn-c/config.h>
#include <pkmn-c/pokemon.h>

PKMN_API void pkmntest_gen2_invalid_pokemon_test(
    const char* game
);

PKMN_API void pkmntest_gen2_unown_test(
    const char* game
);

#endif /* GEN2_POKEMON_TESTS_H */
