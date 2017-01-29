/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_io_tests.h>

#include <pkmn-c/pokemon.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN];

static const char* LIBPKMN_TEST_FILES = "@LIBPKMN_TEST_FILES@";

struct expected_stat {
    char* name;
    int value;
};

void pkmntest_test_3gpkm() {
    pkmn_error_t error = PKMN_ERROR_NONE;

    /*
     * Test files in repo and compare to known values.
     */

    char mightyena_filepath[STRBUFFER_LEN];
#if defined(PKMN_PLATFORM_MINGW) || defined(PKMN_PLATFORM_WIN32)
    snprintf(mightyena_filepath, sizeof(mightyena_filepath), "%s\\3gpkm\\MIGHTYENA.3gpkm", LIBPKMN_TEST_FILES);
#else
    snprintf(mightyena_filepath, sizeof(mightyena_filepath), "%s/3gpkm/MIGHTYENA.3gpkm", LIBPKMN_TEST_FILES);
#endif

    pkmn_pokemon_handle_t mightyena = NULL;
    error = pkmn_pokemon_from_file(
                &mightyena,
                mightyena_filepath
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(mightyena);

    error = pkmn_pokemon_get_species(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Mightyena");

    error = pkmn_pokemon_get_game(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Emerald");

    error = pkmn_pokemon_get_form(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Standard");

    error = pkmn_pokemon_get_nickname(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "MIGHTYENA");

    bool is_shiny = false;
    error = pkmn_pokemon_is_shiny(
                mightyena,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_FALSE(is_shiny);

    error = pkmn_pokemon_get_held_item(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Heart Scale");

    error = pkmn_pokemon_get_trainer_name(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "A");

    uint16_t trainer_public_id = 0;
    error = pkmn_pokemon_get_trainer_public_id(
                mightyena,
                &trainer_public_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_public_id, 61415);

    uint16_t trainer_secret_id = 0;
    error = pkmn_pokemon_get_trainer_secret_id(
                mightyena,
                &trainer_secret_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_secret_id, 3417);

    uint32_t trainer_id = 0;
    error = pkmn_pokemon_get_trainer_id(
                mightyena,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, 223997927);

    pkmn_gender_t trainer_gender = PKMN_MALE;
    error = pkmn_pokemon_get_trainer_gender(
                mightyena,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_FEMALE);

    int friendship = 0;
    error = pkmn_pokemon_get_friendship(
                mightyena,
                &friendship
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(friendship, 254);

    error = pkmn_pokemon_get_ability(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Intimidate");

    error = pkmn_pokemon_get_ball(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Great Ball");

    int level_met = 0;
    error = pkmn_pokemon_get_level_met(
                mightyena,
                &level_met
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level_met, 25);

    error = pkmn_pokemon_get_location_met(
                mightyena,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Route 120");

    error = pkmn_pokemon_get_original_game(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Emerald");

    uint32_t personality = 0;
    error = pkmn_pokemon_get_personality(
                mightyena,
                &personality
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(personality, 3557601241);

    int experience = 0;
    error = pkmn_pokemon_get_experience(
                mightyena,
                &experience
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(experience, 128734);

    int level = 0;
    error = pkmn_pokemon_get_level(
                mightyena,
                &level
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level, 50);

    pkmn_string_list_t marking_names;
    error = pkmn_pokemon_get_marking_names(
                mightyena,
                &marking_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(marking_names.length, 4);

    bool has_marking = false;
    for(size_t i = 0; i < marking_names.length; ++i) {
        error = pkmn_pokemon_has_marking(
                    mightyena,
                    marking_names.strings[i],
                    &has_marking
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_FALSE(has_marking);
    }

    error = pkmn_string_list_free(&marking_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_string_list_t ribbon_names;
    error = pkmn_pokemon_get_ribbon_names(
                mightyena,
                &ribbon_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(ribbon_names.length, 32);

    bool has_ribbon = false;
    for(size_t i = 0; i < ribbon_names.length; ++i) {
        error = pkmn_pokemon_has_ribbon(
                    mightyena,
                    ribbon_names.strings[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        if(strcmp(ribbon_names.strings[i], "Champion") == 0) {
            TEST_ASSERT_TRUE(has_ribbon);
        } else {
            TEST_ASSERT_FALSE(has_ribbon);
        }
    }

    error = pkmn_string_list_free(&ribbon_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_string_list_t contest_stat_names;
    error = pkmn_pokemon_get_contest_stat_names(
                mightyena,
                &contest_stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(contest_stat_names.length, 6);

    int contest_stat = 0;
    for(size_t i = 0; i < ribbon_names.length; ++i) {
        error = pkmn_pokemon_get_contest_stat(
                    mightyena,
                    contest_stat_names.strings[i],
                    &contest_stat
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(contest_stat, 0);
    }

    error = pkmn_string_list_free(&contest_stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    static const char* expected_mightyena_moves[] = {
        "Crunch", "Strength", "Shadow Ball", "Double-Edge"
    };

    pkmn_move_slots_t mightyena_moves;
    error = pkmn_pokemon_get_moves(
                mightyena,
                &mightyena_moves
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(mightyena_moves.length, 4);
    for(size_t i = 0; i < mightyena_moves.length; ++i) {
        TEST_ASSERT_EQUAL_STRING(mightyena_moves.move_slots[i].move, expected_mightyena_moves[i]);
    }

    error = pkmn_move_slots_free(&mightyena_moves);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    static const struct expected_stat expected_EVs[] = {
        {"HP", 30},
        {"Attack", 110},
        {"Defense", 32},
        {"Speed", 48},
        {"Special Attack", 17},
        {"Special Defense", 83}
    };

    int EV = 0;
    for(int i = 0; i < 6; ++i) {
        error = pkmn_pokemon_get_EV(
                    mightyena,
                    expected_EVs[i].name,
                    &EV
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(EV, expected_EVs[i].value);
    }

    static const struct expected_stat expected_IVs[] = {
        {"HP", 26},
        {"Attack", 28},
        {"Defense", 4},
        {"Speed", 13},
        {"Special Attack", 25},
        {"Special Defense", 26}
    };

    int IV = 0;
    for(int i = 0; i < 6; ++i) {
        error = pkmn_pokemon_get_IV(
                    mightyena,
                    expected_IVs[i].name,
                    &IV
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(IV, expected_IVs[i].value);
    }

    static const struct expected_stat expected_stats[] = {
        {"HP", 146},
        {"Attack", 122},
        {"Defense", 81},
        {"Speed", 87},
        {"Special Attack", 79},
        {"Special Defense", 88}
    };

    int stat = 0;
    for(int i = 0; i < 6; ++i) {
        error = pkmn_pokemon_get_stat(
                    mightyena,
                    expected_stats[i].name,
                    &stat
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(stat, expected_stats[i].value);
    }

    error = pkmn_pokemon_free(&mightyena);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(mightyena);
}
