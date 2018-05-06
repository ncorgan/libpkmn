/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/misc_comparison.h>
#include <pkmntest-c/pokemon_comparison.h>
#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <unity.h>

#include <string.h>

#define STRBUFFER_LEN 1024
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
static const struct pkmn_attribute_names empty_attribute_names =
{
    .numeric_attribute_names =
    {
        .pp_strings = NULL,
        .length = 0
    },
    .string_attribute_names =
    {
        .pp_strings = NULL,
        .length = 0
    },
    .boolean_attribute_names =
    {
        .pp_strings = NULL,
        .length = 0
    }
};

static inline bool random_bool()
{
    return (bool)(rand() % 2);
}

void get_random_pokemon(
    struct pkmn_pokemon* pokemon_ptr,
    struct pkmn_string_list* item_list_ptr,
    const char* species,
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(game);

    enum pkmn_error error = PKMN_ERROR_NONE;
    int generation = game_to_generation(game);

    struct pkmn_string_list internal_item_list = empty_string_list;
    struct pkmn_string_list* internal_item_list_ptr = NULL;

    if(item_list_ptr)
    {
        internal_item_list_ptr = item_list_ptr;
    }
    else
    {
        error = pkmn_database_item_list(game, &internal_item_list);
        PKMN_TEST_ASSERT_SUCCESS(error);
        internal_item_list_ptr = &internal_item_list;
    }

    struct pkmn_string_list move_list = empty_string_list;
    struct pkmn_string_list pokemon_list = empty_string_list;

    error = pkmn_database_move_list(game, &move_list);
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(!species)
    {
        error = pkmn_database_pokemon_list(generation, true, &pokemon_list);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    // Don't deal with Deoxys issues here.
    const char* actual_species = NULL;
    if(species)
    {
        actual_species = species;
    }
    else
    {
        if(generation == 3)
        {
            do
            {
                actual_species = pokemon_list.pp_strings[rand() % pokemon_list.length];
            } while(!strcmp(actual_species, "Deoxys"));
        }
        else
        {
            actual_species = pokemon_list.pp_strings[rand() % pokemon_list.length];
        }
    }
    TEST_ASSERT_NOT_NULL(actual_species);

    error = pkmn_pokemon_init(
                actual_species,
                game,
                "",
                ((rand() % 62) + 2), // To not run into Generation II level met limit
                pokemon_ptr
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(pokemon_ptr)
    );

    for(size_t move_index = 0; move_index < 4; ++move_index)
    {
        const char* move = NULL;
        do
        {
            move = move_list.pp_strings[rand() % move_list.length];
        } while(strstr(move, "Shadow"));
    }

    // Get the EVs first to see which are valid. The same values
    // will be valid for IVs, so set both here.
    int EVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_EVs(
                pokemon_ptr,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    for(enum pkmn_stat stat = PKMN_STAT_HP;
        stat <= PKMN_STAT_SPDEF;
        ++stat)
    {
        if(EVs[stat] != -1)
        {
            error = pkmn_pokemon_set_EV(
                        pokemon_ptr,
                        stat,
                        (rand() % 256)
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_set_IV(
                        pokemon_ptr,
                        stat,
                        (rand() % 16)
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
    }

    if(generation >= 2)
    {
        // Keep going until one is holdable.
        do
        {
            error = pkmn_pokemon_set_held_item(
                        pokemon_ptr,
                        internal_item_list_ptr->pp_strings[rand() % internal_item_list_ptr->length]
                    );
        } while(error == PKMN_ERROR_INVALID_ARGUMENT);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_is_shiny(
                    pokemon_ptr,
                    random_bool()
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    if(generation >= 3)
    {
        // Just do the markings compatible with all games.
        for(enum pkmn_marking marking = PKMN_MARKING_CIRCLE;
            marking <= PKMN_MARKING_HEART;
            ++marking)
        {
            error = pkmn_pokemon_set_has_marking(
                        pokemon_ptr,
                        marking,
                        random_bool()
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }

        // Just do the contest stats compatible with all games.
        for(enum pkmn_contest_stat contest_stat = PKMN_CONTEST_STAT_COOL;
            contest_stat <= PKMN_CONTEST_STAT_TOUGH;
            ++contest_stat)
        {
            error = pkmn_pokemon_set_contest_stat(
                        pokemon_ptr,
                        contest_stat,
                        (rand() % 256)
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }

        struct pkmn_string_list ribbon_names = empty_string_list;
        for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
        {
            error = pkmn_pokemon_set_has_ribbon(
                        pokemon_ptr,
                        ribbon_names.pp_strings[ribbon_index],
                        random_bool()
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
        }

        error = pkmn_string_list_free(&ribbon_names);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    if(!species)
    {
        error = pkmn_string_list_free(&pokemon_list);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    error = pkmn_string_list_free(&move_list);
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(!item_list_ptr)
    {
        error = pkmn_string_list_free(&internal_item_list);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
}

void compare_pokemon_ints(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    const char* field,
    pokemon_int_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    int int1 = 0;
    int int2 = 0;

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &int1);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = getter_fcn(pokemon_ptr, &int2);
    PKMN_TEST_ASSERT_SUCCESS(error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(int1, int2, error_message);
}

void compare_pokemon_uint32s(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    const char* field,
    pokemon_uint32_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    uint32_t uint1 = 0;
    uint32_t uint2 = 0;

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &uint1);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = getter_fcn(pokemon_ptr, &uint2);
    PKMN_TEST_ASSERT_SUCCESS(error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(uint1, uint2, error_message);
}

void compare_pokemon_strings(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon2_ptr,
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

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = getter_fcn(
                pokemon1_ptr,
                strbuffer1,
                sizeof(strbuffer1),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = getter_fcn(
                pokemon2_ptr,
                strbuffer2,
                sizeof(strbuffer2),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(strbuffer1, strbuffer2, error_message);
}

void compare_pokemon_bools(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    const char* field,
    pokemon_bool_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    bool bool1 = 0;
    bool bool2 = 0;

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = getter_fcn(pokemon1_ptr, &bool1);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = getter_fcn(pokemon_ptr, &bool2);
    PKMN_TEST_ASSERT_SUCCESS(error);

    char error_message[STRBUFFER_LEN] = {0};
    snprintf(error_message, sizeof(error_message), "Mismatched %s", field);

    TEST_ASSERT_EQUAL_MESSAGE(bool1, bool2, error_message);
}

void compare_pokemon_int_buffers(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_int_buffer_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int* int_buffer1 = calloc(buffer_size, sizeof(int));
    int* int_buffer2 = calloc(buffer_size, sizeof(int));

    error = getter_fcn(pokemon1_ptr, int_buffer1, buffer_size, NULL);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = getter_fcn(pokemon_ptr, int_buffer2, buffer_size, NULL);
    PKMN_TEST_ASSERT_SUCCESS(error);

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

void compare_pokemon_bool_buffers(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    size_t buffer_size,
    const char* field,
    pokemon_bool_buffer_getter_fcn_t getter_fcn
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(field);
    TEST_ASSERT_NOT_NULL(getter_fcn);

    enum pkmn_error error = PKMN_ERROR_NONE;

    bool* bool_buffer1 = calloc(buffer_size, sizeof(bool));
    bool* bool_buffer2 = calloc(buffer_size, sizeof(bool));

    error = getter_fcn(pokemon1_ptr, bool_buffer1, buffer_size, NULL);
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = getter_fcn(pokemon_ptr, bool_buffer2, buffer_size, NULL);
    PKMN_TEST_ASSERT_SUCCESS(error);

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

void compare_pokemon_original_trainer_info(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_trainer_info original_trainer_info1 = empty_trainer_info;
    struct pkmn_trainer_info original_trainer_info2 = empty_trainer_info;

    error = pkmn_pokemon_get_original_trainer_info(
                pokemon1_ptr,
                &original_trainer_info1
            );
    TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_original_trainer_info(
                pokemon_ptr,
                &original_trainer_info2
            );
    TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(
        original_trainer_info1.p_name,
        original_trainer_info2.p_name
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
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_trainer_info_free(&original_trainer_info1);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void compare_pokemon_moves(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_move_slots move_slots1 = empty_move_slots;
    struct pkmn_move_slots move_slots2 = empty_move_slots;

    error = pkmn_pokemon_get_moves(
                pokemon1_ptr,
                &move_slots1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_moves(
                pokemon_ptr,
                &move_slots2
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL(4, move_slots1.length);
    TEST_ASSERT_EQUAL(4, move_slots2.length);

    for(size_t move_index = 0; move_index < 4; ++move_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            move_slots1.p_move_slots[move_index].p_move,
            move_slots2.p_move_slots[move_index].p_move
        );
        TEST_ASSERT_EQUAL(
            move_slots1.p_move_slots[move_index].pp,
            move_slots2.p_move_slots[move_index].pp
        );
    }

    error = pkmn_move_slots_free(&move_slots2);
    error = pkmn_move_slots_free(&move_slots1);
}

void compare_pokemon_locations_met(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr,
    bool as_egg
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    char strbuffer1[STRBUFFER_LEN] = {0};
    char strbuffer2[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon_get_location_met(
                pokemon1_ptr,
                as_egg,
                strbuffer1,
                sizeof(strbuffer1),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_location_met(
                pokemon_ptr,
                as_egg,
                strbuffer2,
                sizeof(strbuffer2),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(strbuffer1, strbuffer2);
}

void compare_pokemon_ribbons(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_string_list ribbon_list = empty_string_list;
    error = pkmn_pokemon_get_ribbon_names(
                pokemon1_ptr,
                &ribbon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    bool has_ribbon1 = false;
    bool has_ribbon2 = false;
    for(size_t ribbon_index = 0; ribbon_index < ribbon_list.length; ++ribbon_index)
    {
        error = pkmn_pokemon_has_ribbon(
                    pokemon1_ptr,
                    ribbon_list.pp_strings[ribbon_index],
                    &has_ribbon1
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    ribbon_list.pp_strings[ribbon_index],
                    &has_ribbon2
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_MESSAGE(
            has_ribbon1,
            has_ribbon2,
            ribbon_list.pp_strings[ribbon_index]
        );
    }

    error = pkmn_string_list_free(&ribbon_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void compare_pokemon(
    struct pkmn_pokemon* pokemon1_ptr,
    struct pkmn_pokemon* pokemon2_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon2_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon1_ptr->game);

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
            pkmn_pokemon_get_icon_filepath
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Sprite filepath",
            pkmn_pokemon_get_sprite_filepath
        );
    }

    compare_pokemon_strings(
        pokemon1_ptr,
        pokemon2_ptr,
        "Form",
        pkmn_pokemon_get_form
    );
    compare_pokemon_ints(
        pokemon1_ptr,
        pokemon2_ptr,
        "Experience",
        pkmn_pokemon_get_experience
    );
    compare_pokemon_ints(
        pokemon1_ptr,
        pokemon2_ptr,
        "Level",
        pkmn_pokemon_get_level
    );
    compare_pokemon_strings(
        pokemon1_ptr,
        pokemon2_ptr,
        "Nickname",
        pkmn_pokemon_get_nickname
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "EVs",
        pkmn_pokemon_get_EVs
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "IVs",
        pkmn_pokemon_get_IVs
    );
    compare_pokemon_int_buffers(
        pokemon1_ptr,
        pokemon2_ptr,
        PKMN_NUM_STATS,
        "Stats",
        pkmn_pokemon_get_stats
    );

    compare_pokemon_original_trainer_info(
        pokemon1_ptr,
        pokemon2_ptr
    );
    compare_pokemon_moves(
        pokemon1_ptr,
        pokemon2_ptr
    );

    if(generation >= 2)
    {
        compare_pokemon_ints(
            pokemon1_ptr,
            pokemon2_ptr,
            "Current trainer friendship",
            pkmn_pokemon_get_current_trainer_friendship
        );
        compare_pokemon_bools(
            pokemon1_ptr,
            pokemon2_ptr,
            "Is shiny",
            pkmn_pokemon_is_shiny
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Held item",
            pkmn_pokemon_get_held_item
        );
        compare_pokemon_ints(
            pokemon1_ptr,
            pokemon2_ptr,
            "Level met",
            pkmn_pokemon_get_level_met
        );
        compare_pokemon_locations_met(pokemon1_ptr, pokemon2_ptr, false);
    }
    if(generation >= 3)
    {
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Ability",
            pkmn_pokemon_get_ability
        );
        compare_pokemon_uint32s(
            pokemon1_ptr,
            pokemon2_ptr,
            "Personality",
            pkmn_pokemon_get_personality
        );
        compare_pokemon_strings(
            pokemon1_ptr,
            pokemon2_ptr,
            "Ball",
            pkmn_pokemon_get_ball
        );
        compare_pokemon_bool_buffers(
            pokemon1_ptr,
            pokemon2_ptr,
            PKMN_NUM_MARKINGS,
            "Markings",
            pkmn_pokemon_get_markings
        );
        compare_pokemon_int_buffers(
            pokemon1_ptr,
            pokemon2_ptr,
            PKMN_NUM_CONTEST_STATS,
            "Contest stats",
            pkmn_pokemon_get_contest_stats
        );
        compare_pokemon_ribbons(pokemon1_ptr, pokemon2_ptr);
    }
    if(generation >= 4)
    {
        compare_pokemon_locations_met(pokemon1_ptr, pokemon2_ptr, true);
    }

    // Compare attributes.

    struct pkmn_attribute_names attribute_names1 = empty_attribute_names;
    error = pkmn_pokemon_get_attribute_names(
                pokemon1_ptr,
                &attribute_names1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    struct pkmn_attribute_names attribute_names2 = empty_attribute_names;
    error = pkmn_pokemon_get_attribute_names(
                pokemon2_ptr,
                &attribute_names2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_attribute_names(
        &attribute_names1,
        &attribute_names2
    );

    for(size_t attribute_index = 0;
        attribute_index < attribute_names1.numeric_attribute_names.length;
        ++attribute_index)
    {
        int attribute_value1 = 0;
        int attribute_value2 = 0;

        error = pkmn_pokemon_get_numeric_attribute(
                    pokemon1_ptr,
                    attribute_names1.numeric_attribute_names.pp_strings[attribute_index],
                    &attribute_value1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_numeric_attribute(
                    pokemon2_ptr,
                    attribute_names2.numeric_attribute_names.pp_strings[attribute_index],
                    &attribute_value2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL(attribute_value1, attribute_value2);
    }

    for(size_t attribute_index = 0;
        attribute_index < attribute_names1.string_attribute_names.length;
        ++attribute_index)
    {
        char attribute_value1[STRBUFFER_LEN] = {0};
        char attribute_value2[STRBUFFER_LEN] = {0};

        error = pkmn_pokemon_get_string_attribute(
                    pokemon1_ptr,
                    attribute_names1.string_attribute_names.pp_strings[attribute_index],
                    attribute_value1,
                    sizeof(attribute_value1),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_string_attribute(
                    pokemon2_ptr,
                    attribute_names2.string_attribute_names.pp_strings[attribute_index],
                    attribute_value2,
                    sizeof(attribute_value2),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(attribute_value1, attribute_value2);
    }

    for(size_t attribute_index = 0;
        attribute_index < attribute_names1.boolean_attribute_names.length;
        ++attribute_index)
    {
        bool attribute_value1 = 0;
        bool attribute_value2 = 0;

        error = pkmn_pokemon_get_boolean_attribute(
                    pokemon1_ptr,
                    attribute_names1.boolean_attribute_names.pp_strings[attribute_index],
                    &attribute_value1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_boolean_attribute(
                    pokemon2_ptr,
                    attribute_names2.boolean_attribute_names.pp_strings[attribute_index],
                    &attribute_value2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL(attribute_value1, attribute_value2);
    }

    error = pkmn_attribute_names_free(&attribute_names1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_attribute_names_free(&attribute_names2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}
