/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_TEST_H
#define POKEMON_TEST_H

#include <pkmn-c/pokemon.h>

typedef struct {
    char* valid_ball;
    char** invalid_balls;

    char* valid_item;
    char** invalid_items;

    char* expected_original_location;
    char** valid_locations;
    char** invalid_locations;

    char** moves;
    char** invalid_moves;

    char** valid_original_games;
    char** invalid_original_games;
} pkmn_test_values_t;

void pokemon_test_common(
    pkmn_pokemon_handle_t pokemon,
    pkmn_test_values_t* test_values
);

#endif /* POKEMON_TEST_H */
