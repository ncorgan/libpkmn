/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_TEST_H
#define POKEMON_TEST_H

#include <pkmn-c/pokemon2.h>

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
    pkmn_pokemon2_t* pokemon_ptr,
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

// Gamecube tests

void colosseum_pokemon_test();
void xd_pokemon_test();

// I/O

void test_3gpkm();

// Generation I friendship

void red_friendship_test();
void blue_friendship_test();
void yellow_friendship_test();

// Form

void red_form_test();
void blue_form_test();
void yellow_form_test();
void gold_form_test();
void silver_form_test();
void crystal_form_test();
void ruby_form_test();
void sapphire_form_test();
void emerald_form_test();
void firered_form_test();
void leafgreen_form_test();
void colosseum_form_test();
void xd_form_test();

// Gender

void gold_gender_test();
void silver_gender_test();
void crystal_gender_test();
void ruby_gender_test();
void sapphire_gender_test();
void emerald_gender_test();
void firered_gender_test();
void leafgreen_gender_test();
void colosseum_gender_test();
void xd_gender_test();

// Unown

void gold_unown_test();
void silver_unown_test();
void crystal_unown_test();
void ruby_unown_test();
void sapphire_unown_test();
void emerald_unown_test();
void firered_unown_test();
void leafgreen_unown_test();
void colosseum_unown_test();
void xd_unown_test();

#endif /* POKEMON_TEST_H */
