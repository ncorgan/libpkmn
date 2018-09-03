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
    .species = PKMN_SPECIES_NONE,
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

static void pokemon_io_test(
    enum pkmn_game game,
    const char* extension
)
{
    TEST_ASSERT_NOT_NULL(extension);

    enum pkmn_error error = PKMN_ERROR_NONE;

    char tmp_dir[STRBUFFER_LEN] = {0};
    error = pkmn_get_tmp_dir(
                tmp_dir,
                sizeof(tmp_dir),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    char tmp_path[STRBUFFER_LEN] = {0};
    snprintf(
        tmp_path,
        sizeof(tmp_path),
        "%s%s%d_%d.%s",
        tmp_dir,
        FS_SEPARATOR,
        (int)game,
        rand(),
        extension
    );

    struct pkmn_pokemon random_pokemon = empty_pokemon;
    get_random_pokemon(
        &random_pokemon,
        NULL,
        PKMN_SPECIES_NONE, // Can be any
        game
    );

    error = pkmn_pokemon_export_to_file(
                &random_pokemon,
                tmp_path
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_pokemon imported_pokemon = empty_pokemon;
    error = pkmn_pokemon_init_from_file(
                tmp_path,
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
    TEST_ASSERT_NOT_EQUAL(DeleteFile(tmp_path), 0);
#else
    TEST_ASSERT_EQUAL(remove(tmp_path), 0);
#endif
}

// Generation I

void red_pk1_test()
{
    pokemon_io_test(PKMN_GAME_RED, "pk1");
}

void blue_pk1_test()
{
    pokemon_io_test(PKMN_GAME_BLUE, "pk1");
}

void yellow_pk1_test()
{
    pokemon_io_test(PKMN_GAME_YELLOW, "pk1");
}

// Generation II

void gold_pk2_test()
{
    pokemon_io_test(PKMN_GAME_GOLD, "pk2");
}

void silver_pk2_test()
{
    pokemon_io_test(PKMN_GAME_SILVER, "pk2");
}

void crystal_pk2_test()
{
    pokemon_io_test(PKMN_GAME_CRYSTAL, "pk2");
}

// Game Boy Advance

void ruby_3gpkm_test()
{
    pokemon_io_test(PKMN_GAME_RUBY, "3gpkm");
}

void ruby_pk3_test()
{
    pokemon_io_test(PKMN_GAME_RUBY, "pk3");
}

void sapphire_3gpkm_test()
{
    pokemon_io_test(PKMN_GAME_SAPPHIRE, "3gpkm");
}

void sapphire_pk3_test()
{
    pokemon_io_test(PKMN_GAME_SAPPHIRE, "pk3");
}

void emerald_3gpkm_test()
{
    pokemon_io_test(PKMN_GAME_EMERALD, "3gpkm");
}

void emerald_pk3_test()
{
    pokemon_io_test(PKMN_GAME_EMERALD, "pk3");
}

void firered_3gpkm_test()
{
    pokemon_io_test(PKMN_GAME_FIRERED, "3gpkm");
}

void firered_pk3_test()
{
    pokemon_io_test(PKMN_GAME_FIRERED, "pk3");
}

void leafgreen_3gpkm_test()
{
    pokemon_io_test(PKMN_GAME_LEAFGREEN, "3gpkm");
}

void leafgreen_pk3_test()
{
    pokemon_io_test(PKMN_GAME_LEAFGREEN, "pk3");
}

// Gamecube

void colosseum_ck3_test()
{
    pokemon_io_test(PKMN_GAME_COLOSSEUM, "ck3");
}

void xd_xk3_test()
{
    pokemon_io_test(PKMN_GAME_XD, "xk3");
}

// Outside files

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

    TEST_ASSERT_EQUAL(PKMN_SPECIES_MIGHTYENA, pokemon.species);
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

    enum pkmn_ability ability = PKMN_ABILITY_NONE;
    error = pkmn_pokemon_get_ability(
                &pokemon,
                &ability
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_ABILITY_INTIMIDATE, ability);

    enum pkmn_ball ball = PKMN_BALL_NONE;
    error = pkmn_pokemon_get_ball(
                &pokemon,
                &ball
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_BALL_GREAT_BALL, ball);

    enum pkmn_game original_game = PKMN_GAME_NONE;
    error = pkmn_pokemon_get_original_game(
                &pokemon,
                &original_game
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GAME_EMERALD, original_game);

    struct pkmn_marking_enum_map markings = {NULL, 0};
    error = pkmn_pokemon_get_markings(
                &pokemon,
                &markings
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(enum pkmn_marking marking = PKMN_MARKING_CIRCLE;
        marking <= PKMN_MARKING_DIAMOND;
        ++marking)
    {
        TEST_ASSERT_FALSE(markings.p_values[marking]);
    }
    error = pkmn_marking_enum_map_free(&markings);
    PKMN_TEST_ASSERT_SUCCESS(error);

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

    struct pkmn_contest_stat_enum_map contest_stats = {NULL, 0};
    error = pkmn_pokemon_get_contest_stats(
                &pokemon,
                &contest_stats
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(enum pkmn_contest_stat contest_stat = PKMN_CONTEST_STAT_COOL;
        contest_stat <= PKMN_CONTEST_STAT_FEEL;
        ++contest_stat)
    {
        TEST_ASSERT_EQUAL(0, contest_stats.p_values[contest_stat]);
    }
    error = pkmn_contest_stat_enum_map_free(&contest_stats);
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_move_slots move_slots = empty_move_slots;
    error = pkmn_pokemon_get_moves(
                &pokemon,
                &move_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, move_slots.length);
    TEST_ASSERT_EQUAL(PKMN_MOVE_CRUNCH, move_slots.p_move_slots[0].move);
    TEST_ASSERT_EQUAL(PKMN_MOVE_STRENGTH, move_slots.p_move_slots[1].move);
    TEST_ASSERT_EQUAL(PKMN_MOVE_SHADOW_BALL, move_slots.p_move_slots[2].move);
    TEST_ASSERT_EQUAL(PKMN_MOVE_DOUBLE_EDGE, move_slots.p_move_slots[3].move);

    error = pkmn_move_slots_free(&move_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_stat_enum_map EVs = {NULL, 0};
    error = pkmn_pokemon_get_EVs(
                &pokemon,
                &EVs
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(30, EVs.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(110, EVs.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(32, EVs.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(48, EVs.p_values[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, EVs.p_values[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(17, EVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
    TEST_ASSERT_EQUAL(83, EVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);

    error = pkmn_stat_enum_map_free(&EVs);
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_stat_enum_map IVs = {NULL, 0};
    error = pkmn_pokemon_get_IVs(
                &pokemon,
                &IVs
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(26, IVs.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(28, IVs.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(4, IVs.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(13, IVs.p_values[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(25, IVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
    TEST_ASSERT_EQUAL(26, IVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);

    error = pkmn_stat_enum_map_free(&IVs);
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_stat_enum_map stats = {NULL, 0};
    error = pkmn_pokemon_get_stats(
                &pokemon,
                &stats
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(146, stats.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(122, stats.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(81, stats.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(87, stats.p_values[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(79, stats.p_values[PKMN_STAT_SPECIAL_ATTACK]);
    TEST_ASSERT_EQUAL(88, stats.p_values[PKMN_STAT_SPECIAL_DEFENSE]);

    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}
