/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_tests_common.h"

#include <pkmn-c/pokemon.h>

#include <unity.h>

#include <stdio.h>
#include <string.h>

static int game_to_generation(
    const char* game
) {
    if(!strcmp(game, "Red") || !strcmp(game, "Blue") || !strcmp(game, "Yellow")) {
        return 1;
    } else if(!strcmp(game, "Gold") || !strcmp(game, "Silver") || !strcmp(game, "Crystal")) {
        return 2;
    } else if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") || !strcmp(game, "Emerald") ||
              !strcmp(game, "FireRed") || !strcmp(game, "LeafGreen") ||
              !strcmp(game, "Colosseum") || !strcmp(game, "XD"))
    {
        return 3;
    } else if(!strcmp(game, "Diamond") || !strcmp(game, "Pearl") || !strcmp(game, "Platinum") ||
              !strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver"))
    {
        return 4;
    } else if(!strcmp(game, "Black") || !strcmp(game, "White") ||
              !strcmp(game, "Black 2") || !strcmp(game, "White 2"))
    {
        return 5;
    } else if(!strcmp(game, "X") || !strcmp(game, "Y") ||
              !strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
    {
        return 6;
    } else {
        return 0;
    }
}

static const char* starters[] = {
    "", "Bulbasaur", "Chikorita", "Treecko", "Turtwig", "Snivy", "Chespin"
};

static const char* gen1_pokemon_with_xy_mega_forms[] = {
    "Venusaur", "Blastoise", "Alakazam", "Gengar", "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
};

static const char* gen1_pokemon_with_oras_mega_forms[] = {
    "Beedrill", "Pidgeot", "Slowbro"
};

static const char* gen1_pokemon_with_mega_xy_forms[] = {
    "Charizard", "Mewtwo"
};

static const char* gen2_pokemon_with_xy_mega_forms[] = {
    "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
};

static void test_gen1_forms(
    const char* game
) {
    int generation = game_to_generation(game);

    pkmn_pokemon_handle_t pokemon = NULL;
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Check that Mega forms only work in their given games
    for(size_t i = 0; i < 8; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_xy_mega_forms[i],
                    game,
                    "Mega",
                    100
                );

        if(generation >= 6) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);
    }
    for(size_t i = 0; i < 3; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_oras_mega_forms[i],
                    game,
                    "Mega",
                    100
                );

        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);
    }

    // Cosplay Pikachu should only work for OR/AS
    pkmn_database_pokemon_entry_t oras_pikachu;
    error = pkmn_database_get_pokemon_entry(
                "Pikachu",
                "Omega Ruby",
                "",
                &oras_pikachu
            );
    TEST_ASSERT(oras_pikachu.forms.length > 0);
    for(size_t i = 1; i < oras_pikachu.forms.length; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Pikachu",
                    game,
                    oras_pikachu.forms.strings[i],
                    5
                );
        if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_database_pokemon_entry_free(&oras_pikachu);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    for(size_t i = 0; i < 2; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_mega_xy_forms[i],
                    game,
                    "Mega X",
                    50
                );

        if(generation >= 6) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_make(
                    &pokemon,
                    gen1_pokemon_with_mega_xy_forms[i],
                    game,
                    "Mega Y",
                    50
                );

        if(generation >= 6) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);
    }
}

static void test_gen2_forms(
    const char* game
) {
    int generation = game_to_generation(game);

    pkmn_pokemon_handle_t pokemon = NULL;
    pkmn_error_t error = PKMN_ERROR_NONE;

    // Check that Mega forms only work in their given games
    for(size_t i = 0; i < 5; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    gen2_pokemon_with_xy_mega_forms[i],
                    game,
                    "Mega",
                    100
                );

        if(generation >= 6) {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_free(
                        &pokemon
                    );
        } else {
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        }

        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_pokemon_make(
                &pokemon,
                "Steelix",
                game,
                "Mega",
                100
            );
    if(!strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire")) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_free(
                    &pokemon
                );
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    }

    TEST_ASSERT_NULL(pokemon);

    // Spiky-eared Pichu should only work in HG/SS
    error = pkmn_pokemon_make(
                &pokemon,
                "Pichu",
                game,
                "Spiky-eared",
                100
            );
    if(!strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver")) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_free(
                    &pokemon
                );
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    }

    TEST_ASSERT_NULL(pokemon);

    // Unown's "!" and "?" forms aren't in Generation II
    pkmn_database_pokemon_entry_t unown_oras;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                "Omega Ruby",
                "",
                &unown_oras
            );
    TEST_ASSERT_EQUAL(unown_oras.forms.length, 28);

    for(size_t i = 0; i < 26; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    "Unown",
                    game,
                    unown_oras.forms.strings[i],
                    10
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_free(
                    &pokemon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_make(
                &pokemon,
                "Unown",
                game,
                "!",
                10
            );
    if(generation == 2) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_free(
                    &pokemon
                );
    }

    TEST_ASSERT_NULL(pokemon);

    error = pkmn_pokemon_make(
                &pokemon,
                "Unown",
                game,
                "?",
                10
            );
    if(generation == 2) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_free(
                    &pokemon
                );
    }

    TEST_ASSERT_NULL(pokemon);
}

static void test_gen3_forms(
    const char* game,
    bool gcn
) {
    (void)game;
    (void)gcn;
}

// TODO: Check Alola forms when Generation VII supported
static void test_forms(
    const char* game
) {
    int generation = game_to_generation(game);

    /*
     * Make sure forms that didn't appear for a given
     * generation's Pokémon until later don't work until
     * the correct generation.
     */
    if(generation >= 1) {
        test_gen1_forms(game);
    }
    if(generation >= 2) {
        test_gen2_forms(game);
    }
    if(generation >= 3) {
        test_gen3_forms(
            game,
            (!strcmp(game, "Colosseum") || !strcmp(game, "XD"))
        );
    }
}

static void test_invalid_starters(
    const char* game
) {
    pkmn_pokemon_handle_t pokemon = NULL;
    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(game);
    for(int i = (generation+1); i <= 6; ++i) {
        error = pkmn_pokemon_make(
                    &pokemon,
                    starters[i],
                    game,
                    "",
                    5
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_NULL(pokemon);
    }
}

void test_invalid_pokemon(
    const char* game
) {
    test_forms(game);
    test_invalid_starters(game);
}
