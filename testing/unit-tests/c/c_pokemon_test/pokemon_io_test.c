/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/pokemon.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN];

#if defined(PKMN_PLATFORM_MINGW) || defined(PKMN_PLATFORM_WIN32)
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

struct expected_stat {
    char* name;
    int value;
};

void test_3gpkm() {
    pkmn_error_t error = PKMN_ERROR_NONE;

    const char* LIBPKMN_TEST_FILES = getenv("LIBPKMN_TEST_FILES");
    TEST_ASSERT_NOT_NULL(LIBPKMN_TEST_FILES);

    pkmn_trainer_info_t trainer_info =
    {
        .trainer_name = {0},
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_GENDERLESS
    };

    snprintf(
        strbuffer,
        sizeof(strbuffer),
        "%s%s3gpkm%sMIGHTYENA.3gpkm",
        LIBPKMN_TEST_FILES, FS_SEPARATOR, FS_SEPARATOR
    );

    pkmn_pokemon_handle_t mightyena = NULL;
    error = pkmn_pokemon_from_file(
                &mightyena,
                strbuffer
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(mightyena);

    error = pkmn_pokemon_get_species(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Mightyena", strbuffer);

    error = pkmn_pokemon_get_game(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Emerald", strbuffer);

    error = pkmn_pokemon_get_form(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Standard", strbuffer);

    error = pkmn_pokemon_get_nickname(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("MIGHTYENA", strbuffer);

    error = pkmn_pokemon_get_original_trainer_info(
                mightyena,
                &trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("A", trainer_info.trainer_name);
    TEST_ASSERT_EQUAL(61415, trainer_info.trainer_id.public_id);
    TEST_ASSERT_EQUAL(3417, trainer_info.trainer_id.secret_id);
    TEST_ASSERT_EQUAL(223997927, trainer_info.trainer_id.id);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, trainer_info.trainer_gender);

    bool is_shiny = false;
    error = pkmn_pokemon_is_shiny(
                mightyena,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_FALSE(is_shiny);

    error = pkmn_pokemon_get_held_item(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Heart Scale", strbuffer);

    int friendship = 0;
    error = pkmn_pokemon_get_current_trainer_friendship(
                mightyena,
                &friendship
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(254, friendship);

    error = pkmn_pokemon_get_ability(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Intimidate", strbuffer);

    error = pkmn_pokemon_get_ball(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Great Ball", strbuffer);

    int level_met = 0;
    error = pkmn_pokemon_get_level_met(
                mightyena,
                &level_met
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(25, level_met);

    error = pkmn_pokemon_get_location_met(
                mightyena,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Route 120", strbuffer);

    error = pkmn_pokemon_get_original_game(
                mightyena,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Emerald", strbuffer);

    uint32_t personality = 0;
    error = pkmn_pokemon_get_personality(
                mightyena,
                &personality
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3557601241, personality);

    int experience = 0;
    error = pkmn_pokemon_get_experience(
                mightyena,
                &experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(128734, experience);

    int level = 0;
    error = pkmn_pokemon_get_level(
                mightyena,
                &level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(50, level);

    pkmn_string_list_t marking_names;
    error = pkmn_pokemon_get_marking_names(
                mightyena,
                &marking_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(4, marking_names.length);

    bool has_marking = false;
    for(size_t i = 0; i < marking_names.length; ++i) {
        error = pkmn_pokemon_has_marking(
                    mightyena,
                    marking_names.strings[i],
                    &has_marking
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_FALSE(has_marking);
    }

    error = pkmn_string_list_free(&marking_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t ribbon_names;
    error = pkmn_pokemon_get_ribbon_names(
                mightyena,
                &ribbon_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(32, ribbon_names.length);

    bool has_ribbon = false;
    for(size_t i = 0; i < ribbon_names.length; ++i) {
        error = pkmn_pokemon_has_ribbon(
                    mightyena,
                    ribbon_names.strings[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        if(strcmp(ribbon_names.strings[i], "Champion") == 0) {
            TEST_ASSERT_TRUE(has_ribbon);
        } else {
            TEST_ASSERT_FALSE(has_ribbon);
        }
    }

    error = pkmn_string_list_free(&ribbon_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t contest_stat_names;
    error = pkmn_pokemon_get_contest_stat_names(
                mightyena,
                &contest_stat_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(6, contest_stat_names.length);

    int contest_stat = 0;
    for(size_t i = 0; i < ribbon_names.length; ++i) {
        error = pkmn_pokemon_get_contest_stat(
                    mightyena,
                    contest_stat_names.strings[i],
                    &contest_stat
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0, contest_stat);
    }

    error = pkmn_string_list_free(&contest_stat_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    static const char* expected_mightyena_moves[] = {
        "Crunch", "Strength", "Shadow Ball", "Double-Edge"
    };

    pkmn_move_slots_t mightyena_moves;
    error = pkmn_pokemon_get_moves(
                mightyena,
                &mightyena_moves
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(4, mightyena_moves.length);
    for(size_t i = 0; i < mightyena_moves.length; ++i) {
        TEST_ASSERT_EQUAL_STRING(expected_mightyena_moves[i], mightyena_moves.move_slots[i].move);
    }

    error = pkmn_move_slots_free(&mightyena_moves);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(expected_EVs[i].value, EV);
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
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(expected_IVs[i].value, IV);
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
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(expected_stats[i].value, stat);
    }

    error = pkmn_pokemon_free(&mightyena);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(mightyena);
}
