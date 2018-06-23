/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_comparison.h>
#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

#ifdef PKMN_C_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

#if defined(PKMN_C_PLATFORM_MINGW) || defined(PKMN_C_PLATFORM_WIN32)
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

static const struct pkmn_string_list empty_string_list =
{
    .pp_strings = NULL,
    .length = 0
};
static const struct pkmn_trainer_info empty_trainer_info =
{
    .p_name = NULL,
    .id = {0},
    .gender = PKMN_GENDER_GENDERLESS
};
static const struct pkmn_move_slots empty_move_slots =
{
    .p_move_slots = NULL,
    .length = 0
};
static const struct pkmn_pokemon empty_pokemon =
{
    .p_species = NULL,
    .game = PKMN_GAME_NONE,
    .p_internal = NULL
};

static void test_pokemon_string(
    struct pkmn_pokemon* p_pokemon,
    const char* field,
    const char* expected_value,
    pokemon_string_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(expected_value);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(
        expected_value,
        strbuffer,
        error_message
    );
}

static void test_pokemon_int(
    struct pkmn_pokemon* p_pokemon,
    const char* field,
    int expected_value,
    pokemon_int_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;
    int value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(p_pokemon, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_uint32(
    struct pkmn_pokemon* p_pokemon,
    const char* field,
    uint32_t expected_value,
    pokemon_uint32_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;
    uint32_t value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(p_pokemon, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_bool(
    struct pkmn_pokemon* p_pokemon,
    const char* field,
    bool expected_value,
    pokemon_bool_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;
    bool value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(p_pokemon, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_location_met(
    struct pkmn_pokemon* p_pokemon,
    bool as_egg,
    const char* expected_value
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(expected_value);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon_get_location_met(
                p_pokemon,
                as_egg,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(expected_value, strbuffer);
}

// Generation I

static void pk1_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    char tmp_dir[STRBUFFER_LEN] = {0};
    error = pkmn_get_tmp_dir(
                tmp_dir,
                sizeof(tmp_dir),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    char pk1_path[STRBUFFER_LEN] = {0};
    snprintf(
        pk1_path,
        sizeof(pk1_path),
        "%s%s%d_%d.pk1",
        tmp_dir,
        FS_SEPARATOR,
        (int)game,
        rand()
    );

    struct pkmn_pokemon random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                pk1_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_pokemon imported_pokemon = empty_pokemon;
    error = pkmn_pokemon_init_from_file(
                pk1_path,
                &imported_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(&imported_pokemon)
    );

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon_free(&random_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_free(&imported_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);

#ifdef PKMN_C_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(DeleteFile(pk1_path), 0);
#else
    TEST_ASSERT_EQUAL(remove(pk1_path), 0);
#endif
}

void red_pk1_test()
{
    pk1_test(PKMN_GAME_RED);
}

void blue_pk1_test()
{
    pk1_test(PKMN_GAME_BLUE);
}

void yellow_pk1_test()
{
    pk1_test(PKMN_GAME_YELLOW);
}

// Generation II

static void pk2_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    char tmp_dir[STRBUFFER_LEN] = {0};
    error = pkmn_get_tmp_dir(
                tmp_dir,
                sizeof(tmp_dir),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    char pk2_path[STRBUFFER_LEN] = {0};
    snprintf(
        pk2_path,
        sizeof(pk2_path),
        "%s%s%d_%d.pk2",
        tmp_dir,
        FS_SEPARATOR,
        (int)game,
        rand()
    );

    struct pkmn_pokemon random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                pk2_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_pokemon imported_pokemon = empty_pokemon;
    error = pkmn_pokemon_init_from_file(
                pk2_path,
                &imported_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(&imported_pokemon)
    );

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon_free(&random_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_free(&imported_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);

#ifdef PKMN_C_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(DeleteFile(pk2_path), 0);
#else
    TEST_ASSERT_EQUAL(remove(pk2_path), 0);
#endif
}

void gold_pk2_test()
{
    pk2_test(PKMN_GAME_GOLD);
}

void silver_pk2_test()
{
    pk2_test(PKMN_GAME_SILVER);
}

void crystal_pk2_test()
{
    pk2_test(PKMN_GAME_CRYSTAL);
}

// Generation III

static void _3gpkm_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    char tmp_dir[STRBUFFER_LEN] = {0};
    error = pkmn_get_tmp_dir(
                tmp_dir,
                sizeof(tmp_dir),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    char _3gpkm_path[STRBUFFER_LEN] = {0};
    snprintf(
        _3gpkm_path,
        sizeof(_3gpkm_path),
        "%s%s%d_%d.3gpkm",
        tmp_dir,
        FS_SEPARATOR,
        (int)game,
        rand()
    );

    struct pkmn_pokemon random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                _3gpkm_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_pokemon imported_pokemon = empty_pokemon;
    error = pkmn_pokemon_init_from_file(
                _3gpkm_path,
                &imported_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(&imported_pokemon)
    );

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon_free(&random_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_free(&imported_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);

#ifdef PKMN_C_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(DeleteFile(_3gpkm_path), 0);
#else
    TEST_ASSERT_EQUAL(remove(_3gpkm_path), 0);
#endif
}

void ruby_3gpkm_test()
{
    _3gpkm_test(PKMN_GAME_RUBY);
}

void sapphire_3gpkm_test()
{
    _3gpkm_test(PKMN_GAME_SAPPHIRE);
}

void emerald_3gpkm_test()
{
    _3gpkm_test(PKMN_GAME_EMERALD);
}

void firered_3gpkm_test()
{
    _3gpkm_test(PKMN_GAME_FIRERED);
}

void leafgreen_3gpkm_test()
{
    _3gpkm_test(PKMN_GAME_LEAFGREEN);
}

void test_outside_3gpkm()
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon = empty_pokemon;
    struct pkmn_trainer_info original_trainer_info = empty_trainer_info;

    const char* LIBPKMN_TEST_FILES = getenv("LIBPKMN_TEST_FILES");
    TEST_ASSERT_NOT_NULL(LIBPKMN_TEST_FILES);

    char _3gpkm_path[STRBUFFER_LEN] = {0};
    snprintf(
        _3gpkm_path,
        sizeof(_3gpkm_path),
        "%s%s3gpkm%sMIGHTYENA.3gpkm",
        LIBPKMN_TEST_FILES,
        FS_SEPARATOR,
        FS_SEPARATOR
    );

    error = pkmn_pokemon_init_from_file(
                _3gpkm_path,
                &pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING("Mightyena", pokemon.p_species);
    TEST_ASSERT_EQUAL(PKMN_GAME_EMERALD, pokemon.game);

    test_pokemon_string(
        &pokemon,
        "Form",
        "Standard",
        pkmn_pokemon_get_form
    );
    test_pokemon_string(
        &pokemon,
        "Nickname",
        "MIGHTYENA",
        pkmn_pokemon_get_nickname
    );
    test_pokemon_bool(
        &pokemon,
        "Is shiny",
        false,
        pkmn_pokemon_is_shiny
    );

    error = pkmn_pokemon_get_original_trainer_info(
                &pokemon,
                &original_trainer_info
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING("A", original_trainer_info.p_name);
    TEST_ASSERT_EQUAL(223997927, original_trainer_info.id.id);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, original_trainer_info.gender);

    error = pkmn_trainer_info_free(&original_trainer_info);
    PKMN_TEST_ASSERT_SUCCESS(error);

    test_pokemon_int(
        &pokemon,
        "Current trainer friendship",
        254,
        pkmn_pokemon_get_current_trainer_friendship
    );
    test_pokemon_string(
        &pokemon,
        "Ability",
        "Intimidate",
        pkmn_pokemon_get_ability
    );
    test_pokemon_string(
        &pokemon,
        "Ball",
        "Great Ball",
        pkmn_pokemon_get_ball
    );
    test_pokemon_int(
        &pokemon,
        "Level met",
        25,
        pkmn_pokemon_get_level_met
    );
    test_location_met(
        &pokemon,
        false,
        "Route 120"
    );
    test_pokemon_uint32(
        &pokemon,
        "Personality",
        3557601241,
        pkmn_pokemon_get_personality
    );
    test_pokemon_int(
        &pokemon,
        "Experience",
        128734,
        pkmn_pokemon_get_experience
    );
    test_pokemon_int(
        &pokemon,
        "Level",
        50,
        pkmn_pokemon_get_level
    );

    enum pkmn_game original_game = PKMN_GAME_NONE;
    error = pkmn_pokemon_get_original_game(
        &pokemon,
        &original_game
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GAME_EMERALD, original_game);

    bool markings[PKMN_NUM_MARKINGS] = {false};
    error = pkmn_pokemon_get_markings(
                &pokemon,
                markings,
                PKMN_NUM_MARKINGS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(enum pkmn_marking marking = PKMN_MARKING_CIRCLE;
        marking <= PKMN_MARKING_DIAMOND;
        ++marking)
    {
        TEST_ASSERT_FALSE(markings[marking]);
    }

    struct pkmn_string_list ribbon_names = empty_string_list;
    error = pkmn_pokemon_get_ribbon_names(
                &pokemon,
                &ribbon_names
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(32, ribbon_names.length);
    for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
    {
        bool has_ribbon = false;
        error = pkmn_pokemon_has_ribbon(
                    &pokemon,
                    ribbon_names.pp_strings[ribbon_index],
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        if(!strcmp(ribbon_names.pp_strings[ribbon_index], "Champion"))
        {
            TEST_ASSERT_TRUE(has_ribbon);
        }
        else
        {
            TEST_ASSERT_FALSE(has_ribbon);
        }
    }

    error = pkmn_string_list_free(&ribbon_names);
    PKMN_TEST_ASSERT_SUCCESS(error);

    int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
    error = pkmn_pokemon_get_contest_stats(
                &pokemon,
                contest_stats,
                PKMN_NUM_CONTEST_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(enum pkmn_contest_stat contest_stat = PKMN_CONTEST_STAT_COOL;
        contest_stat <= PKMN_CONTEST_STAT_FEEL;
        ++contest_stat)
    {
        TEST_ASSERT_EQUAL(0, contest_stats[contest_stat]);
    }

    struct pkmn_move_slots move_slots = empty_move_slots;
    error = pkmn_pokemon_get_moves(
                &pokemon,
                &move_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, move_slots.length);
    TEST_ASSERT_EQUAL_STRING("Crunch", move_slots.p_move_slots[0].p_move);
    TEST_ASSERT_EQUAL_STRING("Strength", move_slots.p_move_slots[1].p_move);
    TEST_ASSERT_EQUAL_STRING("Shadow Ball", move_slots.p_move_slots[2].p_move);
    TEST_ASSERT_EQUAL_STRING("Double-Edge", move_slots.p_move_slots[3].p_move);

    error = pkmn_move_slots_free(&move_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);

    int EVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_EVs(
                &pokemon,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(30, EVs[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(110, EVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(32, EVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(48, EVs[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, EVs[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(17, EVs[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(83, EVs[PKMN_STAT_SPDEF]);

    int IVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_IVs(
                &pokemon,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(26, IVs[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(28, IVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(4, IVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(13, IVs[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, IVs[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(25, IVs[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(26, IVs[PKMN_STAT_SPDEF]);

    int stats[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_stats(
                &pokemon,
                stats,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(146, stats[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(122, stats[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(81, stats[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(87, stats[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, stats[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(79, stats[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(88, stats[PKMN_STAT_SPDEF]);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}
