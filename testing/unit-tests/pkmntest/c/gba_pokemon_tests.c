/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_tests_common.h"

#include <pkmntest-c/gba_pokemon_tests.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>
#include <time.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

/*static const char* contest_types[] = {
    "Cool", "Beauty", "Cute", "Smart", "Tough"
};

static const char* contest_levels[] = {
    "", " Super", " Hyper", " Master"
};

static const char* ribbons[] = {
    "Champion", "Winning", "Victory", "Artist",
    "Effort", "Marine", "Land", "Sky",
    "Country", "National", "Earth", "World",
};*/

static bool seeded = false;

void pkmntest_gba_invalid_pokemon_test(
    const char* game
) {
    test_invalid_pokemon(game);
}

/*typedef pkmn_error_t (*pokemon_stat_fcn_t)(pkmn_pokemon_handle_t, pkmn_string_list_t*);

static void gba_pokemon_check_stat_map(
    pkmn_pokemon_handle_t pokemon,
    bool is_stats,
    pokemon_stat_fcn_t stat_fcn
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_string_list_t stat_names;

    error = stat_fcn(
                pokemon,
                &stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(stat_names.length, (is_stats ? 6 : 5));
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[0], "Attack");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[1], "Defense");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[2], "HP");
    if(is_stats) {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special Attack");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Special Defense");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[5], "Speed");
    } else {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Speed");
    }

    error = pkmn_string_list_free(&stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}*/

void pkmntest_gba_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
) {
    if(seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_database_pokemon_entry_t pokemon_entry;

    /*
     * Check known starting values, and confirm that we can't query values
     * that didn't exist in Generation III.
     */

    error = pkmn_pokemon_get_species(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, species);

    error = pkmn_pokemon_get_form(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Standard");

    error = pkmn_pokemon_get_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    char expected_nickname[11] = {0};
    for(size_t i = 0; i < strlen(species); ++i) {
        expected_nickname[i] = toupper(species[i]);
    }
    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_nickname);

    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, LIBPKMN_OT_NAME);

    uint32_t trainer_id = 0;
    uint16_t trainer_id_part = 0;

    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, (LIBPKMN_OT_ID & 0xFFFF));

    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, ((LIBPKMN_OT_ID & 0xFFFF0000) >> 16));

    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, LIBPKMN_OT_ID);

    pkmn_gender_t trainer_gender = PKMN_GENDERLESS;
    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_MALE);

    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int friendship = 0;
    error = pkmn_pokemon_get_friendship(
                pokemon,
                &friendship
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(friendship, pokemon_entry.base_friendship);

    error = pkmn_pokemon_get_ability(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Blaze");

    error = pkmn_pokemon_get_ball(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Premier Ball");

    int level = 0;
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level, 30);

    int level_met = 0;
    error = pkmn_pokemon_get_level_met(
                pokemon,
                &level_met
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level, level_met);

    error = pkmn_pokemon_get_location_met(
                pokemon,
                true,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    error = pkmn_pokemon_get_location_met(
                pokemon,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Fateful encounter");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    int experience_from_pokemon = 0;
    int experience_at_30 = 0;

    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience_from_pokemon
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_30
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(experience_from_pokemon, experience_at_30);
}
