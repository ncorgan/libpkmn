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

// Generation I tests

void red_pokemon_test();
void blue_pokemon_test();
void yellow_pokemon_test();

// Generation II tests

void gold_pokemon_test();
void silver_pokemon_test();
void crystal_pokemon_test();

// Game Boy Advance tests

void ruby_pokemon_test();
void sapphire_pokemon_test();
void emerald_pokemon_test();
void firered_pokemon_test();
void leafgreen_pokemon_test();

// Generation I friendship

void red_friendship_test();
void blue_friendship_test();
void yellow_friendship_test();

// I/O

void test_3gpkm();

#endif /* POKEMON_TEST_H */
