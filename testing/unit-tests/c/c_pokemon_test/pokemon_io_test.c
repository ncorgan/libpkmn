/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pkmntest-c/util.h"

#include <pkmn.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024

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
static const pkmn_trainer_info2_t empty_trainer_info =
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
static const pkmn_pokemon2_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};

typedef pkmn_error_t (*pokemon_int_getter_fcn_t)(pkmn_pokemon2_t*,int*);
typedef pkmn_error_t (*pokemon_uint32_getter_fcn_t)(pkmn_pokemon2_t*,uint32_t*);
typedef pkmn_error_t (*pokemon_string_getter_fcn_t)(pkmn_pokemon2_t*,char*,size_t,size_t*);
typedef pkmn_error_t (*pokemon_bool_getter_fcn_t)(pkmn_pokemon2_t*,bool*);
typedef pkmn_error_t (*pokemon_int_buffer_getter_fcn_t)(pkmn_pokemon2_t*,int*,size_t,size_t*);
typedef pkmn_error_t (*pokemon_bool_buffer_getter_fcn_t)(pkmn_pokemon2_t*,bool*,size_t,size_t*);

static inline bool random_bool()
{
    return ((rand() % 100) > 50);
}

static void get_random_pokemon(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    int generation = game_to_generation(game);

    pkmn_string_list_t item_list = empty_string_list;
    pkmn_string_list_t move_list = empty_string_list;
    pkmn_string_list_t pokemon_list = empty_string_list;

    error = pkmn_database_item_list(game, &item_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_database_move_list(game, &move_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_database_pokemon_list(generation, true, &pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Don't deal with Deoxys issues here.
    const char* species = NULL;
    if(generation == 3)
    {
        do
        {
            species = pokemon_list.strings[rand() % pokemon_list.length];
        } while(!strcmp(species, "Deoxys"));
    }
    else
    {
        species = pokemon_list.strings[rand() % pokemon_list.length];
    }

    error = pkmn_pokemon2_init(
                species,
                game,
                "",
                ((rand() % 99) + 2),
                pokemon_ptr
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t move_index = 0; move_index < 4; ++move_index)
    {
        const char* move = NULL;
        do
        {
            move = move_list.strings[rand() % move_list.length];
        } while(strstr(move, "Shadow"));
    }

    // Get the EVs first to see which are valid. The same values
    // will be valid for IVs, so set both here.
    int EVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon2_get_EVs(
                pokemon_ptr,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    for(pkmn_stat_t stat = PKMN_STAT_HP;
        stat <= PKMN_STAT_SPDEF;
        ++stat)
    {
        if(EVs[stat] != -1)
        {
            error = pkmn_pokemon2_set_EV(
                        pokemon_ptr,
                        stat,
                        (rand() % 256)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_set_IV(
                        pokemon_ptr,
                        stat,
                        (rand() % 16)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }
    }

    if(generation >= 2)
    {
        // Keep going until one is holdable.
        do
        {
            error = pkmn_pokemon2_set_held_item(
                        pokemon_ptr,
                        item_list.strings[rand() % item_list.length]
                    );
        } while(error == PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
    if(generation >= 3)
    {
        // Just do the markings compatible with all games.
        for(pkmn_marking_t marking = PKMN_MARKING_CIRCLE;
            marking <= PKMN_MARKING_HEART;
            ++marking)
        {
            error = pkmn_pokemon2_set_has_marking(
                        pokemon_ptr,
                        marking,
                        random_bool()
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }

        // Just do the contest stats compatible with all games.
        for(pkmn_contest_stat_t contest_stat = PKMN_CONTEST_STAT_COOL;
            contest_stat <= PKMN_CONTEST_STAT_TOUGH;
            ++contest_stat)
        {
            error = pkmn_pokemon2_set_contest_stat(
                        pokemon_ptr,
                        contest_stat,
                        (rand() % 256)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }
    }
}

static void compare_pokemon_ints(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_int_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    int int1 = 0;
    int int2 = 0;

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &int1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = getter_fcn(pokemon2_ptr, &int2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(int1, int2, error_message);
}

static void compare_pokemon_uint32s(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_uint32_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    uint32_t uint1 = 0;
    uint32_t uint2 = 0;

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &uint1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = getter_fcn(pokemon2_ptr, &uint2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(uint1, uint2, error_message);
}

static void compare_pokemon_strings(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_string_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    char strbuffer1[STRBUFFER_LEN] = {0};
    char strbuffer2[STRBUFFER_LEN] = {0};

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = getter_fcn(
                pokemon1_ptr,
                strbuffer1,
                sizeof(strbuffer1),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = getter_fcn(
                pokemon2_ptr,
                strbuffer2,
                sizeof(strbuffer2),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(strbuffer1, strbuffer2, error_message);
}

static void compare_pokemon_bools(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    const char* field,
    pokemon_bool_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    bool bool1 = 0;
    bool bool2 = 0;

    pkmn_error_t error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &bool1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = getter_fcn(pokemon2_ptr, &bool2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(bool1, bool2, error_message);
}

static void compare_pokemon_int_buffers(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_int_buffer_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int* int_buffer1 = calloc(buffer_size, sizeof(int));
    int* int_buffer2 = calloc(buffer_size, sizeof(int));

    error = getter_fcn(pokemon1_ptr, int_buffer1, buffer_size, NULL);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = getter_fcn(pokemon2_ptr, int_buffer2, buffer_size, NULL);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        int_buffer1,
        int_buffer2,
        (buffer_size * sizeof(int)),
        error_message
    );

    free(int_buffer2);
    free(int_buffer1);
}

static void compare_pokemon_bool_buffers(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_bool_buffer_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    pkmn_error_t error = PKMN_ERROR_NONE;

    bool* bool_buffer1 = calloc(buffer_size, sizeof(bool));
    bool* bool_buffer2 = calloc(buffer_size, sizeof(bool));

    error = getter_fcn(pokemon1_ptr, bool_buffer1, buffer_size, NULL);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = getter_fcn(pokemon2_ptr, bool_buffer2, buffer_size, NULL);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        bool_buffer1,
        bool_buffer2,
        (buffer_size * sizeof(bool)),
        error_message
    );

    free(bool_buffer2);
    free(bool_buffer1);
}

static void compare_locations_met(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr,
    bool as_egg
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    char strbuffer1[STRBUFFER_LEN] = {0};
    char strbuffer2[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon2_get_location_met(
                pokemon1_ptr,
                as_egg,
                strbuffer1,
                sizeof(strbuffer1),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_get_location_met(
                pokemon2_ptr,
                as_egg,
                strbuffer2,
                sizeof(strbuffer2),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(strbuffer1, strbuffer2);
}

static void compare_ribbons(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_string_list_t ribbon_list = empty_string_list;
    error = pkmn_pokemon2_get_ribbon_names(
                pokemon1_ptr,
                &ribbon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    bool has_ribbon1 = false;
    bool has_ribbon2 = false;
    for(size_t ribbon_index = 0; ribbon_index < ribbon_list.length; ++ribbon_index)
    {
        error = pkmn_pokemon2_has_ribbon(
                    pokemon1_ptr,
                    ribbon_list.strings[ribbon_index],
                    &has_ribbon1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon2_has_ribbon(
                    pokemon2_ptr,
                    ribbon_list.strings[ribbon_index],
                    &has_ribbon2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_MESSAGE(
            has_ribbon1,
            has_ribbon2,
            ribbon_list.strings[ribbon_index]
        );
    }

    error = pkmn_string_list_free(&ribbon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void compare_pokemon(
    pkmn_pokemon2_t* pokemon1_ptr,
    pkmn_pokemon2_t* pokemon2_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;
    int generation = game_to_generation(pokemon1_ptr->game);

    pkmn_trainer_info2_t original_trainer_info1 = empty_trainer_info;
    pkmn_trainer_info2_t original_trainer_info2 = empty_trainer_info;

    TEST_ASSERT_EQUAL_STRING(
        pokemon1_ptr->species,
        pokemon2_ptr->species
    );

    // There is no way to determine what game an imported Generation I-II
    // Pokémon comes from, so LibPKMN defaults to a default valid game.
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL_STRING(
            pokemon1_ptr->game,
            pokemon2_ptr->game
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Icon filepath",
            pkmn_pokemon2_get_icon_filepath
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Sprite filepath",
            pkmn_pokemon2_get_sprite_filepath
        );
    }

    error = pkmn_pokemon2_get_original_trainer_info(
                pokemon1_ptr,
                &original_trainer_info1
            );
    TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_get_original_trainer_info(
                pokemon2_ptr,
                &original_trainer_info2
            );
    TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(
        original_trainer_info1.name,
        original_trainer_info2.name
    );
    TEST_ASSERT_EQUAL(
        original_trainer_info1.id.id,
        original_trainer_info2.id.id
    );
    TEST_ASSERT_EQUAL(
        original_trainer_info1.gender,
        original_trainer_info2.gender
    );

    error = pkmn_trainer_info_free(&original_trainer_info2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_trainer_info_free(&original_trainer_info1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_pokemon_strings(
        pokemon1_ptr,
        pokemon2_ptr,
        "Form",
        pkmn_pokemon2_get_form
    );
    compare_pokemon_ints(
        pokemon1_ptr,
        pokemon2_ptr,
        "Experience",
        pkmn_pokemon2_get_experience
    );
    compare_pokemon_ints(
        pokemon1_ptr,
        pokemon2_ptr,
        "Level",
        pkmn_pokemon2_get_level
    );
    compare_pokemon_strings(
        pokemon1_ptr,
        pokemon2_ptr,
        "Nickname",
        pkmn_pokemon2_get_nickname
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "EVs",
        pkmn_pokemon2_get_EVs
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "IVs",
        pkmn_pokemon2_get_IVs
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "Stats",
        pkmn_pokemon2_get_stats
    );

    if(generation >= 2)
    {
        compare_pokemon_ints(
            pokemon1_ptr,
            pokemon2_ptr,
            "Current trainer friendship",
            pkmn_pokemon2_get_current_trainer_friendship
        );
        compare_pokemon_bools(
            pokemon1_ptr,
            pokemon2_ptr,
            "Is shiny",
            pkmn_pokemon2_is_shiny
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Held item",
            pkmn_pokemon2_get_held_item
        );
        compare_pokemon_ints(
            pokemon1_ptr,
            pokemon2_ptr,
            "Level met",
            pkmn_pokemon2_get_level_met
        );
        compare_locations_met(pokemon1_ptr, pokemon2_ptr, false);
    }
    if(generation >= 3)
    {
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Ability",
            pkmn_pokemon2_get_ability
        );
        compare_pokemon_uint32s(
            pokemon1_ptr,
            pokemon2_ptr,
            "Personality",
            pkmn_pokemon2_get_personality
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Ball",
            pkmn_pokemon2_get_ball
        );
        compare_pokemon_bool_buffers(
            pokemon1_ptr,
            pokemon2_ptr,
            PKMN_NUM_MARKINGS,
            "Markings",
            pkmn_pokemon2_get_markings
        );
        compare_pokemon_int_buffers(
            pokemon1_ptr,
            pokemon2_ptr,
            PKMN_NUM_CONTEST_STATS,
            "Contest stats",
            pkmn_pokemon2_get_contest_stats
        );
        compare_ribbons(pokemon1_ptr, pokemon2_ptr);
    }
    if(generation >= 4)
    {
        compare_locations_met(pokemon1_ptr, pokemon2_ptr, true);
    }
}

static void test_pokemon_string(
    pkmn_pokemon2_t* pokemon_ptr,
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(
        expected_value,
        strbuffer,
        error_message
    );
}

static void test_pokemon_int(
    pkmn_pokemon2_t* pokemon_ptr,
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_uint32(
    pkmn_pokemon2_t* pokemon_ptr,
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_pokemon_bool(
    pkmn_pokemon2_t* pokemon_ptr,
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_value,
        value_from_pokemon,
        error_message
    );
}

static void test_location_met(
    pkmn_pokemon2_t* pokemon_ptr,
    bool as_egg,
    const char* expected_value
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(expected_value);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon2_get_location_met(
                pokemon_ptr,
                as_egg,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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

    pkmn_pokemon2_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, game);

    error = pkmn_pokemon2_export_to_file(
                &random_pokemon,
                pk1_path
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon2_t imported_pokemon = empty_pokemon;
    error = pkmn_pokemon2_init_from_file(
                pk1_path,
                &imported_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon2_free(&random_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_free(&imported_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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

    pkmn_pokemon2_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, game);

    error = pkmn_pokemon2_export_to_file(
                &random_pokemon,
                pk2_path
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon2_t imported_pokemon = empty_pokemon;
    error = pkmn_pokemon2_init_from_file(
                pk2_path,
                &imported_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon2_free(&random_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_free(&imported_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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

    pkmn_pokemon2_t random_pokemon = empty_pokemon;
    get_random_pokemon(&random_pokemon, game);

    error = pkmn_pokemon2_export_to_file(
                &random_pokemon,
                _3gpkm_path
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon2_t imported_pokemon = empty_pokemon;
    error = pkmn_pokemon2_init_from_file(
                _3gpkm_path,
                &imported_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_pokemon(&random_pokemon, &imported_pokemon);

    error = pkmn_pokemon2_free(&random_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon2_free(&imported_pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
    pkmn_pokemon2_t pokemon = empty_pokemon;
    pkmn_trainer_info2_t original_trainer_info = empty_trainer_info;

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

    error = pkmn_pokemon2_init_from_file(
                _3gpkm_path,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING("Mightyena", pokemon.species);
    TEST_ASSERT_EQUAL_STRING("Emerald", pokemon.game);

    test_pokemon_string(
        &pokemon,
        "Form",
        "Standard",
        pkmn_pokemon2_get_form
    );
    test_pokemon_string(
        &pokemon,
        "Nickname",
        "MIGHTYENA",
        pkmn_pokemon2_get_nickname
    );
    test_pokemon_bool(
        &pokemon,
        "Is shiny",
        false,
        pkmn_pokemon2_is_shiny
    );

    error = pkmn_pokemon2_get_original_trainer_info(
                &pokemon,
                &original_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("A", original_trainer_info.name);
    TEST_ASSERT_EQUAL(223997927, original_trainer_info.id.id);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, original_trainer_info.gender);

    error = pkmn_trainer_info_free(&original_trainer_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    test_pokemon_int(
        &pokemon,
        "Current trainer friendship",
        254,
        pkmn_pokemon2_get_current_trainer_friendship
    );
    test_pokemon_string(
        &pokemon,
        "Ability",
        "Intimidate",
        pkmn_pokemon2_get_ability
    );
    test_pokemon_string(
        &pokemon,
        "Ball",
        "Great Ball",
        pkmn_pokemon2_get_ball
    );
    test_pokemon_int(
        &pokemon,
        "Level met",
        25,
        pkmn_pokemon2_get_level_met
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
        pkmn_pokemon2_get_original_game
    );
    test_pokemon_uint32(
        &pokemon,
        "Personality",
        3557601241,
        pkmn_pokemon2_get_personality
    );
    test_pokemon_int(
        &pokemon,
        "Experience",
        128734,
        pkmn_pokemon2_get_experience
    );
    test_pokemon_int(
        &pokemon,
        "Level",
        50,
        pkmn_pokemon2_get_level
    );

    bool markings[PKMN_NUM_MARKINGS] = {false};
    error = pkmn_pokemon2_get_markings(
                &pokemon,
                markings,
                PKMN_NUM_MARKINGS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    for(pkmn_marking_t marking = PKMN_MARKING_CIRCLE;
        marking <= PKMN_MARKING_DIAMOND;
        ++marking)
    {
        TEST_ASSERT_FALSE(markings[marking]);
    }

    pkmn_string_list_t ribbon_names = empty_string_list;
    error = pkmn_pokemon2_get_ribbon_names(
                &pokemon,
                &ribbon_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(32, ribbon_names.length);
    for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
    {
        bool has_ribbon = false;
        error = pkmn_pokemon2_has_ribbon(
                    &pokemon,
                    ribbon_names.strings[ribbon_index],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
    error = pkmn_pokemon2_get_contest_stats(
                &pokemon,
                contest_stats,
                PKMN_NUM_CONTEST_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    for(pkmn_contest_stat_t contest_stat = PKMN_CONTEST_STAT_COOL;
        contest_stat <= PKMN_CONTEST_STAT_FEEL;
        ++contest_stat)
    {
        TEST_ASSERT_EQUAL(0, contest_stats[contest_stat]);
    }

    pkmn_move_slots_t move_slots = empty_move_slots;
    error = pkmn_pokemon2_get_moves(
                &pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(4, move_slots.length);
    TEST_ASSERT_EQUAL_STRING("Crunch", move_slots.move_slots[0].move);
    TEST_ASSERT_EQUAL_STRING("Strength", move_slots.move_slots[1].move);
    TEST_ASSERT_EQUAL_STRING("Shadow Ball", move_slots.move_slots[2].move);
    TEST_ASSERT_EQUAL_STRING("Double-Edge", move_slots.move_slots[3].move);

    error = pkmn_move_slots_free(&move_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int EVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon2_get_EVs(
                &pokemon,
                EVs,
                sizeof(EVs),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(30, EVs[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(110, EVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(32, EVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(48, EVs[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, EVs[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(17, EVs[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(83, EVs[PKMN_STAT_SPDEF]);

    int IVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon2_get_IVs(
                &pokemon,
                IVs,
                sizeof(IVs),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(26, IVs[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(28, IVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(4, IVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(13, IVs[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, IVs[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(25, IVs[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(26, IVs[PKMN_STAT_SPDEF]);

    int stats[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon2_get_stats(
                &pokemon,
                stats,
                sizeof(stats),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(146, stats[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(122, stats[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(81, stats[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(87, stats[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, stats[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(79, stats[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(88, stats[PKMN_STAT_SPDEF]);

    error = pkmn_pokemon2_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}
