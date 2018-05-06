/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include "pokemon_test.h"

#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN];
static enum pkmn_error error = PKMN_ERROR_NONE;

static const char* contest_types[] =
{
    "Cool", "Beauty", "Cute", "Smart", "Tough"
};

static const char* contest_levels[] =
{
    "", " Super", " Hyper", " Master"
};

static const char* ribbons[] =
{
    "Champion", "Winning", "Victory", "Artist",
    "Effort", "Marine", "Land", "Sky",
    "Country", "National", "Earth", "World"
};

static void check_initial_ribbons(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    struct pkmn_string_list ribbon_names;
    error = pkmn_pokemon_get_ribbon_names(
                pokemon_ptr,
                &ribbon_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(32, ribbon_names.length);

    for(int type_index = 0; type_index < 5; ++type_index)
    {
        for(int level_index = 0; level_index < 4; ++level_index)
        {
            snprintf(
                strbuffer,
                sizeof(strbuffer),
                "%s%s",
                contest_types[type_index],
                contest_levels[level_index]
            );

            TEST_ASSERT_TRUE(
                string_list_contains(&ribbon_names, strbuffer)
            );
        }
    }

    for(int i = 0; i < 12; ++i)
    {
        TEST_ASSERT_TRUE(
            string_list_contains(&ribbon_names, ribbons[i])
        );
    }

    error = pkmn_string_list_free(&ribbon_names);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void test_contest_ribbons(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    char super_ribbon_name[STRBUFFER_LEN] = {0};
    char hyper_ribbon_name[STRBUFFER_LEN] = {0};
    char master_ribbon_name[STRBUFFER_LEN] = {0};
    bool has_ribbon = false;

    for(int type_index = 0; type_index < 5; ++type_index)
    {
        snprintf(
            super_ribbon_name,
            sizeof(super_ribbon_name),
            "%s Super",
            contest_types[type_index]
        );
        snprintf(
            hyper_ribbon_name,
            sizeof(hyper_ribbon_name),
            "%s Hyper",
            contest_types[type_index]
        );
        snprintf(
            master_ribbon_name,
            sizeof(master_ribbon_name),
            "%s Master",
            contest_types[type_index]
        );

        error = pkmn_pokemon_set_has_ribbon(
                    pokemon_ptr,
                    hyper_ribbon_name,
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    contest_types[type_index],
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    super_ribbon_name,
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    hyper_ribbon_name,
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_ribbon);
        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    master_ribbon_name,
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_ribbon);
    }
}

static void test_ribbons(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    bool has_ribbon = false;
    for(int ribbon_index = 0; ribbon_index < 12; ++ribbon_index)
    {
        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    ribbons[ribbon_index],
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_ribbon);

        error = pkmn_pokemon_set_has_ribbon(
                    pokemon_ptr,
                    ribbons[ribbon_index],
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    ribbons[ribbon_index],
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_ribbon);
    }
}

static void gba_pokemon_test(
    const char* species,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(species);
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = NULL,
        .game = NULL,
        .p_internal = NULL
    };

    error = pkmn_pokemon_init(
                species,
                game,
                "",
                30,
                &pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon.p_internal);

    pkmn_test_values_t test_values =
    {
        .valid_ball = "Great Ball",
        .invalid_balls = (char*[]){"Friend Ball", "Heal Ball", NULL},

        .valid_item = "Razz Berry",
        .invalid_items = (char*[]){"Berry", "Mach Bike", NULL},

        .expected_original_location = "Fateful encounter",
        .valid_locations = (char*[]){"Petalburg Woods", "Viridian Forest", NULL},
        .invalid_locations = (char*[]){"New Bark Town", "Twinleaf Town", NULL},

        .moves = (char*[]){"Swallow", "Flamethrower", "Return", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Shadow Sky", "Roost", NULL},

        .valid_original_games = (char*[]){
            "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen",
            "Colosseum", "XD", "Colosseum/XD",
            NULL
        },
        .invalid_original_games = (char*[]){"Gold", "HeartGold", NULL}
    };

    pokemon_test_common(
        &pokemon,
        &test_values
    );

    // Test ribbons.
    check_initial_ribbons(&pokemon);
    test_contest_ribbons(&pokemon);
    test_ribbons(&pokemon);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void gcn_pokemon_test(
    const char* species,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(species);
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = NULL,
        .game = NULL,
        .p_internal = NULL
    };

    error = pkmn_pokemon_init(
                species,
                game,
                "",
                30,
                &pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon.p_internal);

    pkmn_test_values_t test_values =
    {
        .valid_ball = "Great Ball",
        .invalid_balls = (char*[]){"Friend Ball", "Heal Ball", NULL},

        .valid_item = "Razz Berry",
        .invalid_items = (char*[]){"Berry", "Mach Bike", NULL},

        .expected_original_location = "Distant land",
        .valid_locations = (char*[]){"Phenac City", "Orre Colosseum", NULL},
        .invalid_locations = (char*[]){"New Bark Town", "Twinleaf Town", NULL},

        .moves = (char*[]){"Swallow", "Flamethrower", "Return", "Fire Blast", NULL},
        .invalid_moves = (char*[]){"Roost", "Flame Burst", NULL},

        .valid_original_games = (char*[]){
            "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen",
            "Colosseum", "XD", "Colosseum/XD",
            NULL
        },
        .invalid_original_games = (char*[]){"Gold", "HeartGold", NULL}
    };

    pokemon_test_common(
        &pokemon,
        &test_values
    );

    // Test ribbons.
    check_initial_ribbons(&pokemon);
    test_contest_ribbons(&pokemon);
    test_ribbons(&pokemon);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void ruby_pokemon_test()
{
    gba_pokemon_test("Torchic", "Ruby");
}

void sapphire_pokemon_test()
{
    gba_pokemon_test("Mudkip", "Sapphire");
}

void emerald_pokemon_test()
{
    gba_pokemon_test("Treecko", "Emerald");
}

void firered_pokemon_test()
{
    gba_pokemon_test("Charmander", "FireRed");
}

void leafgreen_pokemon_test()
{
    gba_pokemon_test("Bulbasaur", "LeafGreen");
}

void colosseum_pokemon_test()
{
    gcn_pokemon_test("Espeon", "Colosseum");
}

void xd_pokemon_test()
{
    gcn_pokemon_test("Umbreon", "XD");
}
