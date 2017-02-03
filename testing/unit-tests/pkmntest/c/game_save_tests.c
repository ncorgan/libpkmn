/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/game_save_tests.h>

#include <pkmn-c/pokemon.h>

#include <unity.h>

#include <stdbool.h>
#include <string.h>

static pkmn_error_t error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

#define LIBPKMN_OT_NAME "LibPKMN"
#define TOO_LONG_OT_NAME "LibPKMNLibPKMN"
#define LIBPKMN_OT_PID ((uint16_t)1351)
#define LIBPKMN_OT_SID ((uint16_t)32123)

#define MONEY_MAX_VALUE ((int)999999)

static const char* GB_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold",  "Silver", "Crystal"
};

static const char* RIVAL_NAME_SET_GAMES[] = {
    "Ruby", "Sapphire", "Emerald",
    "Black", "White",
    "X", "Y"
};

static const char* MALE_ONLY_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver"
};

static void test_trainer_name(
    pkmn_game_save_handle_t save
) {
    error = pkmn_game_save_set_trainer_name(
                save,
                ""
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_game_save_set_trainer_name(
                save,
                TOO_LONG_OT_NAME
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_game_save_set_trainer_name(
                save,
                LIBPKMN_OT_NAME
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_game_save_get_trainer_name(
                save,
                strbuffer,
                STRBUFFER_LEN
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, LIBPKMN_OT_NAME);
}

static void test_rival_name(
    pkmn_game_save_handle_t save,
    bool is_rival_name_set
) {
    if(is_rival_name_set) {
        error = pkmn_game_save_set_rival_name(
                    save,
                    LIBPKMN_OT_NAME
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        error = pkmn_game_save_set_rival_name(
                    save,
                    ""
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

        error = pkmn_game_save_set_rival_name(
                    save,
                    TOO_LONG_OT_NAME
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

        error = pkmn_game_save_set_rival_name(
                    save,
                    LIBPKMN_OT_NAME
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_game_save_get_rival_name(
                    save,
                    strbuffer,
                    STRBUFFER_LEN
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, LIBPKMN_OT_NAME);
    }
}

static void test_trainer_id(
    pkmn_game_save_handle_t save,
    bool is_gb_game
) {
    uint32_t trainer_id = 0;
    uint16_t trainer_id_part = 0;

    error = pkmn_game_save_get_trainer_id(
                save,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    if(is_gb_game) {
        TEST_ASSERT_EQUAL(trainer_id, LIBPKMN_OT_PID);
    } else {
        TEST_ASSERT_EQUAL(trainer_id, LIBPKMN_OT_ID);
    }

    error = pkmn_game_save_get_trainer_public_id(
                save,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, LIBPKMN_OT_PID);

    error = pkmn_game_save_get_trainer_secret_id(
                save,
                &trainer_id_part
            );
    if(is_gb_game) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(trainer_id_part, LIBPKMN_OT_SID);
    }
}

void pkmntest_game_save_test_common_fields(
    pkmn_game_save_handle_t save
) {
    TEST_ASSERT_NOT_NULL(save);

    test_trainer_name(save);

    char game[STRBUFFER_LEN] = {0};
    error = pkmn_game_save_get_game(
                save,
                game,
                STRBUFFER_LEN
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    bool is_gb_game = false;
    for(int i = 0; i < 6; ++i) {
        if(!strcmp(game, GB_GAMES[i])) {
            is_gb_game = true;
            break;
        }
    }

    error = pkmn_game_save_set_trainer_id(
                save,
                (is_gb_game ? LIBPKMN_OT_PID : LIBPKMN_OT_ID)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    test_trainer_id(save, is_gb_game);

    error = pkmn_game_save_set_trainer_public_id(
                save,
                LIBPKMN_OT_PID
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    test_trainer_id(save, is_gb_game);

    if(is_gb_game) {
        error = pkmn_game_save_set_trainer_id(
                    save,
                    LIBPKMN_OT_ID
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
        error = pkmn_game_save_set_trainer_secret_id(
                    save,
                    LIBPKMN_OT_SID
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        error = pkmn_game_save_set_trainer_secret_id(
                    save,
                    LIBPKMN_OT_SID
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        test_trainer_id(save, is_gb_game);
    }

    bool is_rival_name_set = false;
    for(int i = 0; i < 7; ++i) {
        if(!strcmp(game, RIVAL_NAME_SET_GAMES[i])) {
            is_rival_name_set = true;
            break;
        }
    }
    test_rival_name(save, is_rival_name_set);

    pkmn_gender_t gender = PKMN_MALE;
    bool is_male_only = false;
    for(int i = 0; i < 5; ++i) {
        if(!strcmp(game, MALE_ONLY_GAMES[i])) {
            is_male_only = true;
            break;
        }
    }
    if(is_male_only) {
        error = pkmn_game_save_get_trainer_gender(
                    save,
                    &gender
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(gender, PKMN_MALE);

        error = pkmn_game_save_set_trainer_gender(
                    save,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

        error = pkmn_game_save_set_trainer_gender(
                    save,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        error = pkmn_game_save_set_trainer_gender(
                    save,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        error = pkmn_game_save_get_trainer_gender(
                    save,
                    &gender
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(gender, PKMN_MALE);

        error = pkmn_game_save_set_trainer_gender(
                    save,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        error = pkmn_game_save_get_trainer_gender(
                    save,
                    &gender
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(gender, PKMN_FEMALE);

        error = pkmn_game_save_set_trainer_gender(
                    save,
                    PKMN_GENDERLESS
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    }

    error = pkmn_game_save_set_money(
                save,
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_game_save_set_money(
                save,
                (MONEY_MAX_VALUE+1)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    error = pkmn_game_save_set_money(
                save,
                123456
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int money = 0;
    error = pkmn_game_save_get_money(
                save,
                &money
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(money, 123456);
}
