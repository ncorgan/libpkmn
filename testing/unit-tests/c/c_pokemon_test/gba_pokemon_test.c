/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmntest-c/util.h>

#include <pkmn.h>

#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN];
static pkmn_error_t error = PKMN_ERROR_NONE;

static const char* contest_types[] = {
    "Cool", "Beauty", "Cute", "Smart", "Tough"
};

static const char* contest_levels[] = {
    "", " Super", " Hyper", " Master"
};

static const char* ribbons[] = {
    "Champion", "Winning", "Victory", "Artist",
    "Effort", "Marine", "Land", "Sky",
    "Country", "National", "Earth", "World"
};

static void check_initial_ribbons(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    pkmn_string_list_t ribbon_names;
    error = pkmn_pokemon_get_ribbon_names(
                pokemon,
                &ribbon_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(32, ribbon_names.length);

    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < 4; ++j) {
            snprintf(
                strbuffer,
                sizeof(strbuffer),
                "%s%s",
                contest_types[i],
                contest_levels[j]
            );

            TEST_ASSERT_TRUE(
                string_list_contains(&ribbon_names, strbuffer)
            );
        }
    }

    for(int i = 0; i < 12; ++i) {
        TEST_ASSERT_TRUE(
            string_list_contains(&ribbon_names, ribbons[i])
        );
    }

    error = pkmn_string_list_free(&ribbon_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void test_contest_ribbons(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    
    static char super_ribbon_name[STRBUFFER_LEN] = {0};
    static char hyper_ribbon_name[STRBUFFER_LEN] = {0};
    static char master_ribbon_name[STRBUFFER_LEN] = {0};
    bool has_ribbon = false;

    for(int i = 0; i < 5; ++i) {
        snprintf(
            super_ribbon_name,
            sizeof(super_ribbon_name),
            "%s Super",
            contest_types[i]
        );
        snprintf(
            hyper_ribbon_name,
            sizeof(hyper_ribbon_name),
            "%s Hyper",
            contest_types[i]
        );
        snprintf(
            master_ribbon_name,
            sizeof(master_ribbon_name),
            "%s Master",
            contest_types[i]
        );

        error = pkmn_pokemon_set_ribbon(
                    pokemon,
                    hyper_ribbon_name,
                    true
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    contest_types[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    super_ribbon_name,
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    hyper_ribbon_name,
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    master_ribbon_name,
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_FALSE(has_ribbon);
    }
}

static void test_ribbons(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    bool has_ribbon = false;
    for(int i = 0; i < 12; ++i) {
        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    ribbons[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_FALSE(has_ribbon);

        error = pkmn_pokemon_set_ribbon(
                    pokemon,
                    ribbons[i],
                    true
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    ribbons[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(has_ribbon);
    }
}

static void gba_pokemon_test(
    const char* species,
    const char* game
) {
    TEST_ASSERT_NOT_NULL(species);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    error = pkmn_pokemon_make(
                &pokemon,
                species,
                game,
                "",
                30
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon);

    pkmn_test_values_t test_values = {
        .valid_ball = "Great Ball",
        .invalid_balls = (char*[]){"Friend Ball", "Heal Ball", NULL},

        .valid_item = "Razz Berry",
        .invalid_items = (char*[]){"Berry", "Mach Bike", NULL},

        .expected_original_location = "Fateful encounter",
        .valid_locations = (char*[]){"Petalburg Woods", "Viridian Forest", NULL},
        .invalid_locations = (char*[]){"New Bark Town", "Twinleaf Town", NULL},

        .moves = (char*[]){"Swallow", "Flamethrower", "Return", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Shadow Sky", "Roost", NULL},

        // TODO: test Colosseum/XD
        .valid_original_games = (char*[]){"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen",  NULL},
        .invalid_original_games = (char*[]){"Gold", "HeartGold", NULL}
    };

    pokemon_test_common(
        pokemon,
        &test_values
    );

    // Test ribbons.
    check_initial_ribbons(pokemon);
    test_contest_ribbons(pokemon);
    test_ribbons(pokemon);

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

void ruby_pokemon_test() {
    gba_pokemon_test("Torchic", "Ruby");
}

void sapphire_pokemon_test() {
    gba_pokemon_test("Mudkip", "Sapphire");
}

void emerald_pokemon_test() {
    gba_pokemon_test("Treecko", "Emerald");
}

void firered_pokemon_test() {
    gba_pokemon_test("Charmander", "FireRed");
}

void leafgreen_pokemon_test() {
    gba_pokemon_test("Bulbasaur", "LeafGreen");
}
