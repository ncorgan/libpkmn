/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmn-c/pokemon.h>

#include <string.h>

static int game_to_generation(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    if(!strcmp(game, "Red") || !strcmp(game, "Blue") || !strcmp(game, "Yellow")) {
        return 1;
    } else if(!strcmp(game, "Gold") || !strcmp(game, "Silver") || !strcmp(game, "Crystal")) {
        return 2;
    } else if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") || !strcmp(game, "Emerald") ||
              !strcmp(game, "FireRed") || !strcmp(game, "LeafGreen") ||
              !strcmp(game, "Colosseum") || !strcmp(game, "XD")
    ) {
        return 3;
    } else if(!strcmp(game, "Diamond") || !strcmp(game, "Pearl") || !strcmp(game, "Platinum") ||
              !strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver")
    ) {
        return 4;
    } else if(!strcmp(game, "Black") || !strcmp(game, "White") ||
              !strcmp(game, "Black 2") || !strcmp(game, "White 2")
    ) {
        return 5;
    } else if(!strcmp(game, "X") || !strcmp(game, "Y") ||
              !strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")
    ) {
        return 6;
    }

    return -1;
}

static void test_gen1_pokemon_forms(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen1_pokemon_with_xy_mega_forms[] = {
        "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl",
        NULL
    };
    for(size_t i = 0; gen1_pokemon_with_xy_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_xy_mega_forms[i],
                    game,
                    "Mega",
                    100
                );
        if(generation >= 6) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon);
        }
    }
    static const char* gen1_pokemon_with_oras_mega_forms[] = {
        "Beedrill", "Pidgeot", "Slowbro",
        NULL
    };
    for(size_t i = 0; gen1_pokemon_with_oras_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_oras_mega_forms[i],
                    game,
                    "Mega",
                    100
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon);
        }
    }

    // Cosplay Pikachu should only work for OR/AS.
    pkmn_database_pokemon_entry_t oras_pikachu;
    error = pkmn_database_get_pokemon_entry(
                "Pikachu",
                "Omega Ruby",
                "",
                &oras_pikachu
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(oras_pikachu.forms.length > 0);

    for(size_t i = 1; i < oras_pikachu.forms.length; ++i) {
        TEST_ASSERT_NOT_NULL(oras_pikachu.forms.strings[i]);

        error = pkmn_pokemon_make(
                    &pokemon,
                    "Pikachu",
                    game,
                    oras_pikachu.forms.strings[i],
                    5
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon);
        }
    }

    error = pkmn_database_pokemon_entry_free(&oras_pikachu);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    static const char* gen1_pokemon_with_two_mega_forms[] = {
        "Charizard", "Mewtwo",
        NULL
    };
    for(size_t i = 0; gen1_pokemon_with_two_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_two_mega_forms[i],
                    game,
                    "Mega X",
                    50
                );
        if(generation >= 6) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon);
        }

        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_two_mega_forms[i],
                    game,
                    "Mega Y",
                    50
                );
        if(generation >= 6) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon);
        }
    }
}

static void test_gen2_pokemon_forms(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);
}

static void test_gba_pokemon_forms(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);
}

void red_form_test() {
    test_gen1_pokemon_forms("Red");
}

void blue_form_test() {
    test_gen1_pokemon_forms("Blue");
}

void yellow_form_test() {
    test_gen1_pokemon_forms("Yellow");
}

void gold_form_test() {
    test_gen1_pokemon_forms("Gold");
    test_gen2_pokemon_forms("Gold");
}

void silver_form_test() {
    test_gen1_pokemon_forms("Silver");
    test_gen2_pokemon_forms("Silver");
}

void crystal_form_test() {
    test_gen1_pokemon_forms("Crystal");
    test_gen2_pokemon_forms("Crystal");
}

void ruby_form_test() {
    test_gen1_pokemon_forms("Ruby");
    test_gen2_pokemon_forms("Ruby");
    test_gba_pokemon_forms("Ruby");
}

void sapphire_form_test() {
    test_gen1_pokemon_forms("Sapphire");
    test_gen2_pokemon_forms("Sapphire");
    test_gba_pokemon_forms("Sapphire");
}

void emerald_form_test() {
    test_gen1_pokemon_forms("Emerald");
    test_gen2_pokemon_forms("Emerald");
    test_gba_pokemon_forms("Emerald");
}

void firered_form_test() {
    test_gen1_pokemon_forms("FireRed");
    test_gen2_pokemon_forms("FireRed");
    test_gba_pokemon_forms("FireRed");
}

void leafgreen_form_test() {
    test_gen1_pokemon_forms("LeafGreen");
    test_gen2_pokemon_forms("LeafGreen");
    test_gba_pokemon_forms("LeafGreen");
}
