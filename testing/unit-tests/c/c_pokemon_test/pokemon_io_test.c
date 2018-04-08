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

static const pkmn_string_list_t empty_string_list =
{
    .strings = NULL,
    .length = 0
};
static const pkmn_trainer_info_t empty_trainer_info =
{
    .name = NULL,
    .id = {0},
    .gender = PKMN_GENDER_GENDERLESS
};
static const pkmn_move_slots_t empty_move_slots =
{
    .move_slots = NULL,
    .length = 0
};
static const pkmn_pokemon_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};

static void test_pokemon_string(
    pkmn_pokemon_t* pokemon_ptr,
    const char* field,
    const char* expected_value,
    pokemon_string_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(expected_value);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(
                pokemon_ptr,
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
    pkmn_pokemon_t* pokemon_ptr,
    const char* field,
    int expected_value,
    pokemon_int_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;
    int value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(pokemon_ptr, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_uint32(
    pkmn_pokemon_t* pokemon_ptr,
    const char* field,
    uint32_t expected_value,
    pokemon_uint32_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;
    uint32_t value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(pokemon_ptr, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_bool(
    pkmn_pokemon_t* pokemon_ptr,
    const char* field,
    bool expected_value,
    pokemon_bool_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;
    bool value_from_pokemon = 0;

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(
        error_message,
        sizeof(error_message),
        "Mismatched %s",
        field
    );

    error = getter_fcn(pokemon_ptr, &value_from_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_location_met(
    pkmn_pokemon_t* pokemon_ptr,
    bool as_egg,
    const char* expected_value
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(expected_value);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon_get_location_met(
                pokemon_ptr,
                as_egg,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(expected_value, strbuffer);
}

// Generation I

static void pk1_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

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
        "%s%s%s_%d.pk1",
        tmp_dir,
        FS_SEPARATOR,
        game,
        rand()
    );

    pkmn_pokemon_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                pk1_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    pkmn_pokemon_t imported_pokemon = empty_pokemon;
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
    pk1_test("Red");
}

void blue_pk1_test()
{
    pk1_test("Blue");
}

void yellow_pk1_test()
{
    pk1_test("Yellow");
}

// Generation II

static void pk2_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

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
        "%s%s%s_%d.pk2",
        tmp_dir,
        FS_SEPARATOR,
        game,
        rand()
    );

    pkmn_pokemon_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                pk2_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    pkmn_pokemon_t imported_pokemon = empty_pokemon;
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
    pk2_test("Gold");
}

void silver_pk2_test()
{
    pk2_test("Silver");
}

void crystal_pk2_test()
{
    pk2_test("Crystal");
}

// Generation III

static void _3gpkm_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;

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
        "%s%s%s_%d.3gpkm",
        tmp_dir,
        FS_SEPARATOR,
        game,
        rand()
    );

    pkmn_pokemon_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, NULL, NULL, game);

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                _3gpkm_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    pkmn_pokemon_t imported_pokemon = empty_pokemon;
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
    _3gpkm_test("Ruby");
}

void sapphire_3gpkm_test()
{
    _3gpkm_test("Sapphire");
}

void emerald_3gpkm_test()
{
    _3gpkm_test("Emerald");
}

void firered_3gpkm_test()
{
    _3gpkm_test("FireRed");
}

void leafgreen_3gpkm_test()
{
    _3gpkm_test("LeafGreen");
}

void test_outside_3gpkm()
{
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_t pokemon = empty_pokemon;
    pkmn_trainer_info_t original_trainer_info = empty_trainer_info;

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

    TEST_ASSERT_EQUAL_STRING("Mightyena", pokemon.species);
    TEST_ASSERT_EQUAL_STRING("Emerald", pokemon.game);

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
    TEST_ASSERT_EQUAL_STRING("A", original_trainer_info.name);
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
    test_pokemon_string(
        &pokemon,
        "Original game",
        "Emerald",
        pkmn_pokemon_get_original_game
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

    bool markings[PKMN_NUM_MARKINGS] = {false};
    error = pkmn_pokemon_get_markings(
                &pokemon,
                markings,
                PKMN_NUM_MARKINGS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(pkmn_marking_t marking = PKMN_MARKING_CIRCLE;
        marking <= PKMN_MARKING_DIAMOND;
        ++marking)
    {
        TEST_ASSERT_FALSE(markings[marking]);
    }

    pkmn_string_list_t ribbon_names = empty_string_list;
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
                    ribbon_names.strings[ribbon_index],
                    &has_ribbon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        if(!strcmp(ribbon_names.strings[ribbon_index], "Champion"))
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
    for(pkmn_contest_stat_t contest_stat = PKMN_CONTEST_STAT_COOL;
        contest_stat <= PKMN_CONTEST_STAT_FEEL;
        ++contest_stat)
    {
        TEST_ASSERT_EQUAL(0, contest_stats[contest_stat]);
    }

    pkmn_move_slots_t move_slots = empty_move_slots;
    error = pkmn_pokemon_get_moves(
                &pokemon,
                &move_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, move_slots.length);
    TEST_ASSERT_EQUAL_STRING("Crunch", move_slots.move_slots[0].move);
    TEST_ASSERT_EQUAL_STRING("Strength", move_slots.move_slots[1].move);
    TEST_ASSERT_EQUAL_STRING("Shadow Ball", move_slots.move_slots[2].move);
    TEST_ASSERT_EQUAL_STRING("Double-Edge", move_slots.move_slots[3].move);

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
