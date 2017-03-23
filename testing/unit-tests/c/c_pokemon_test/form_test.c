/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>

#include <string.h>

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

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen2_pokemon_with_xy_mega_forms[] = {
        "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar",
        NULL
    };
    for(size_t i = 0; gen2_pokemon_with_xy_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen2_pokemon_with_xy_mega_forms[i],
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

    error = pkmn_pokemon_make(
                &pokemon,
                "Steelix",
                game,
                "Mega",
                100
            );
    if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon);

    // Spiky-eared Pichu should only work in HG/SS.
    error = pkmn_pokemon_make(
                &pokemon,
                "Pichu",
                game,
                "Spiky-eared",
                100
            );
    if(!strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver")) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon);

    // Unown's "!" and "?" forms aren't in Generation II.
    pkmn_database_pokemon_entry_t unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                "Omega Ruby",
                "",
                &unown_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(28, unown_entry.forms.length);
    for(size_t i = 0; i < 26; ++i) {
        TEST_ASSERT_NOT_NULL(unown_entry.forms.strings[i]);
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Unown",
                    game,
                    unown_entry.forms.strings[i],
                    10
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_make(
                &pokemon,
                "Unown",
                game,
                "!",
                10
            );
    if(generation >= 3) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon);

    error = pkmn_pokemon_make(
                &pokemon,
                "Unown",
                game,
                "?",
                10
            );
    if(generation >= 3) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon);
}

static void test_gba_pokemon_forms(
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const char* gen3_pokemon_with_xy_mega_forms[] = {
        "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
        "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp",
        NULL
    };
    for(size_t i = 0; gen3_pokemon_with_xy_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen3_pokemon_with_xy_mega_forms[i],
                    game,
                    "Mega",
                    100
                );
        if(generation >= 6) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);
    }
    static const char* gen3_pokemon_with_oras_mega_forms[] = {
        "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
        "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza",
        NULL
    };
    for(size_t i = 0; gen3_pokemon_with_oras_mega_forms[i] != NULL; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen3_pokemon_with_oras_mega_forms[i],
                    game,
                    "Mega",
                    100
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);
    }

    // Castform should always work.
    pkmn_database_pokemon_entry_t castform_entry;
    error = pkmn_database_get_pokemon_entry(
                "Castform",
                "Omega Ruby",
                "",
                &castform_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(4, castform_entry.forms.length);
    for(size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_NOT_NULL(castform_entry.forms.strings[i]);
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Castform",
                    game,
                    castform_entry.forms.strings[i],
                    30
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_database_pokemon_entry_free(&castform_entry);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Primal Reversion forms should only work in OR/AS.
    static const char* primal_version_pokemon[] = {"Groudon", "Kyogre"};
    for(size_t i = 0; i < 2; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    primal_version_pokemon[i],
                    game,
                    "Standard",
                    70
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_make(
                    &pokemon,
                    primal_version_pokemon[i],
                    game,
                    "Primal Reversion",
                    70
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);
    }

    // In Generation III, Deoxys's form is game-specific.
    if(generation == 3) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Deoxys",
                    game,
                    "Normal",
                    70
                );
        if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") || !strcmp(game, "Colosseum") || !strcmp(game, "XD")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_make(
                    &pokemon,
                    "Deoxys",
                    game,
                    "Attack",
                    70
                );
        if(!strcmp(game, "FireRed")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_make(
                    &pokemon,
                    "Deoxys",
                    game,
                    "Defense",
                    70
                );
        if(!strcmp(game, "LeafGreen")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_make(
                    &pokemon,
                    "Deoxys",
                    game,
                    "Speed",
                    70
                );
        if(!strcmp(game, "Emerald")) {
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        } else {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon);
    } else {
        pkmn_database_pokemon_entry_t deoxys_entry;
        error = pkmn_database_get_pokemon_entry(
                    "Deoxys",
                    game,
                    "",
                    &deoxys_entry
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(4, deoxys_entry.forms.length);

        for(size_t i = 0; i < 4; ++i) {
            TEST_ASSERT_NOT_NULL(deoxys_entry.forms.strings[i]);

            error = pkmn_pokemon_make(
                        &pokemon,
                        "Deoxys",
                        game,
                        deoxys_entry.forms.strings[i],
                        70
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        }

        error = pkmn_database_pokemon_entry_free(&deoxys_entry);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
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
