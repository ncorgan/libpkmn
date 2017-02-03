/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_PARTY_TESTS_H
#define POKEMON_PARTY_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon_party.h>

PKMNTEST_API void pkmntest_test_empty_pokemon_party(
    pkmn_pokemon_party_handle_t party,
    const char* game
);

PKMNTEST_API void pkmntest_test_setting_pokemon_in_party(
    pkmn_pokemon_party_handle_t party
);

#endif /* POKEMON_PARTY_TESTS_H */
