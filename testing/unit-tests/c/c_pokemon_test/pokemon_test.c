/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test.h"
#include "unity.h"

#include <pkmn-c/pokemon2.h>

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/database/move_entry.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STRBUFFER_LEN 1024

#define GB_EV_MAX      65535
#define MODERN_EV_MAX  255

#define GB_IV_MAX      15
#define MODERN_IV_MAX  31

#define STAT_MAX       65536

// Empty Pokémon info structs

/*static const pkmn_pokemon_filepaths_t empty_pokemon_filepaths =
{
    .icon_filepath = NULL,
    .sprite_filepath = NULL
};
static const pkmn_pokemon_info_t empty_pokemon_info =
{
    .is_shiny = false,
    .is_egg = false,
    .pokerus_duration = 0,
    .personality = 0U,
    .gender = PKMN_GENDER_MALE,
    .ability = NULL,
    .nickname = NULL,
    .held_item = NULL,
    .experience = 0
};
static const pkmn_pokemon_origin_info_t empty_pokemon_origin_info =
{
    .original_trainer_info =
    {
        .name = NULL,
        .id = {0U},
        .gender = PKMN_GENDER_MALE
    },

    .original_game = NULL,
    .ball = NULL,
};
static const pkmn_pokemon_current_trainer_info_t empty_pokemon_current_trainer_info =
{
    .current_trainer_friendship = 0,

    .level_met = 0,

    .location_met = NULL,
    .location_met_as_egg = NULL
};
static const pkmn_pokemon_battle_info_t empty_pokemon_battle_info =
{
    .current_hp = 0,
    .condition = PKMN_CONDITION_NONE
};*/

// Subtest functions

/*static void check_initial_pokemon_filepaths_values(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_filepaths_t pokemon_filepaths = empty_pokemon_filepaths;

    error = pkmn_pokemon2_get_filepaths(
                pokemon_ptr,
                &pokemon_filepaths
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_TRUE(file_exists(pokemon_filepaths.icon_filepath));
    TEST_ASSERT_TRUE(file_exists(pokemon_filepaths.sprite_filepath));

    error = pkmn_pokemon_filepaths_free(&pokemon_filepaths);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void check_initial_pokemon_info_values(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    pkmn_pokemon_info_t pokemon_info = empty_pokemon_info;

    error = pkmn_pokemon2_get_pokemon_info(
                pokemon_ptr,
                &pokemon_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_FALSE(pokemon_info.is_egg);
    TEST_ASSERT_EQUAL_STRING("None", pokemon_info.held_item);

    pkmn_database_pokemon_entry_t pokemon_entry;
    error = pkmn_pokemon2_get_database_entry(
                pokemon_ptr,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int level = 0;
    error = pkmn_pokemon2_get_level(
                pokemon_ptr,
                &level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int experience_at_level = 0;
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(
        experience_at_level,
        pokemon_info.experience
    );

    int level_at_experience = 0;
    error = pkmn_database_pokemon_entry_level_at_experience(
                &pokemon_entry,
                pokemon_info.experience,
                &level_at_experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(
        level_at_experience,
        level
    );

    if(generation < 2)
    {
        TEST_ASSERT_FALSE(pokemon_info.is_shiny);
        TEST_ASSERT_EQUAL(0, pokemon_info.pokerus_duration);
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, pokemon_info.gender);
    }

    if(generation < 3)
    {
        TEST_ASSERT_EQUAL(0U, pokemon_info.personality);
        TEST_ASSERT_EQUAL_STRING("None", pokemon_info.ability);
    }

    if(generation >= 5)
    {
        TEST_ASSERT_EQUAL_STRING(pokemon_ptr->species, pokemon_info.nickname);
    }
    else
    {
        char species_upper[11] = {0};
        for(size_t index = 0; index < strlen(pokemon_ptr->species); ++index)
        {
            species_upper[index] = toupper(pokemon_ptr->species[index]);
        }
        TEST_ASSERT_EQUAL_STRING(species_upper, pokemon_info.nickname);
    }

    error = pkmn_database_pokemon_entry_free(&pokemon_entry);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_info_free(&pokemon_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void check_initial_pokemon_origin_info_values(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    pkmn_pokemon_origin_info_t pokemon_origin_info = empty_pokemon_origin_info;

    pkmn_trainer_id2_t default_trainer_id = pkmn_pokemon2_default_trainer_id();

    error = pkmn_pokemon2_get_pokemon_origin_info(
                pokemon_ptr,
                &pokemon_origin_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(
        pkmn_pokemon2_default_trainer_name(),
        pokemon_origin_info.original_trainer_info.name
    );
    TEST_ASSERT_EQUAL(
        default_trainer_id.public_id,
        pokemon_origin_info.original_trainer_info.id.public_id
    );
    TEST_ASSERT_EQUAL(
        PKMN_GENDER_MALE,
        pokemon_origin_info.original_trainer_info.gender
    );

    if(generation >= 2)
    {
        int level = 0;

        error = pkmn_pokemon2_get_level(
                    pokemon_ptr,
                    &level
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(
            default_trainer_id.id,
            pokemon_origin_info.original_trainer_info.id.id
        );
        TEST_ASSERT_EQUAL(
            default_trainer_id.secret_id,
            pokemon_origin_info.original_trainer_info.id.secret_id
        );

        TEST_ASSERT_EQUAL_STRING(
            pokemon_ptr->game,
            pokemon_origin_info.original_game
        );
        TEST_ASSERT_EQUAL_STRING(
            "Premier Ball",
            pokemon_origin_info.ball
        );
    }
    else
    {
        TEST_ASSERT_EQUAL(
            ((uint32_t)default_trainer_id.public_id),
            pokemon_origin_info.original_trainer_info.id.id
        );
        TEST_ASSERT_EQUAL(
            0,
            pokemon_origin_info.original_trainer_info.id.secret_id
        );
    }

    error = pkmn_pokemon_origin_info_free(&pokemon_origin_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void check_initial_pokemon_current_trainer_info_values(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* expected_original_location
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_current_trainer_info_t current_trainer_info = empty_pokemon_current_trainer_info;

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon2_get_current_trainer_info(
                pokemon_ptr,
                &current_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 2)
    {
        int level = 0;

        error = pkmn_pokemon2_get_level(
                    pokemon_ptr,
                    &level
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(level, current_trainer_info.level_met);

        TEST_ASSERT_EQUAL_STRING(
            expected_original_location,
            current_trainer_info.location_met
        );
    }
    else
    {
        TEST_ASSERT_EQUAL(
            0,
            current_trainer_info.level_met
        );
        TEST_ASSERT_EQUAL_STRING(
            "None",
            current_trainer_info.location_met
        );
    }

    if(generation >= 4)
    {
        TEST_ASSERT_EQUAL_STRING(
            expected_original_location,
            current_trainer_info.location_met_as_egg
        );
    }
    else
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            current_trainer_info.location_met_as_egg
        );
    }

    error = pkmn_pokemon_current_trainer_info_free(&current_trainer_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void check_initial_pokemon_battle_info_values(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_battle_info_t battle_info = empty_pokemon_battle_info;

    error = pkmn_pokemon2_get_battle_info(
                pokemon_ptr,
                &battle_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int pokemon_stats[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon2_get_stats(
                pokemon_ptr,
                pokemon_stats,
                PKMN_NUM_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(
        pokemon_stats[PKMN_STAT_HP],
        battle_info.current_hp
    );

    error = pkmn_pokemon_battle_info_free(&battle_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void check_for_gb_initial_stat_values(
    int* stat_array,
    int max_value
)
{
    TEST_ASSERT_NOT_NULL(stat_array);

    for(size_t stat_index = 0; stat_index < PKMN_NUM_STATS; ++stat_index)
    {
        TEST_ASSERT_TRUE(stat_array[stat_index] >= 0);
        TEST_ASSERT_TRUE(stat_array[stat_index] <= max_value);
    }

    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_SPEED]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_EQUAL(-1, stat_array[PKMN_STAT_SPATK]);
    TEST_ASSERT_EQUAL(-1, stat_array[PKMN_STAT_SPDEF]);
}

static void check_for_modern_initial_stat_values(
    int* stat_array,
    int max_value
)
{
    TEST_ASSERT_NOT_NULL(stat_array);

    for(size_t stat_index = 0; stat_index < PKMN_NUM_STATS; ++stat_index)
    {
        TEST_ASSERT_TRUE(stat_array[stat_index] >= 0);
        TEST_ASSERT_TRUE(stat_array[stat_index] <= max_value);
    }

    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_SPEED]);
    TEST_ASSERT_EQUAL(-1, stat_array[PKMN_STAT_SPECIAL]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_SPATK]);
    TEST_ASSERT_NOT_EQUAL(-1, stat_array[PKMN_STAT_SPDEF]);
}

static void check_initial_stats_and_misc(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    int EVs[PKMN_NUM_STATS] = {0};
    int IVs[PKMN_NUM_STATS] = {0};
    int stats[PKMN_NUM_STATS] = {0};
    int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
    bool markings[PKMN_NUM_MARKINGS] = {0};

    error = pkmn_pokemon2_get_EVs(
                pokemon_ptr,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon2_get_IVs(
                pokemon_ptr,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon2_get_stats(
                pokemon_ptr,
                stats,
                PKMN_NUM_STATS,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 2)
    {
        check_for_modern_initial_stat_values(EVs, MODERN_EV_MAX);
        check_for_modern_initial_stat_values(IVs, MODERN_IV_MAX);
    }
    else
    {
        check_for_gb_initial_stat_values(EVs, GB_EV_MAX);
        check_for_gb_initial_stat_values(IVs, GB_IV_MAX);
    }

    if(generation >= 3)
    {
        check_for_modern_initial_stat_values(stats, STAT_MAX);

        // Now look at Generation III+ fields.

        error = pkmn_pokemon2_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_COOL]);
        TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_BEAUTY]);
        TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_CUTE]);
        TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_SMART]);
        TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_TOUGH]);

        if(generation >= 4)
        {
            TEST_ASSERT_EQUAL(-1, markings[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_FEEL]);
        }
        else
        {
            TEST_ASSERT_EQUAL(0, markings[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_EQUAL(-1, markings[PKMN_CONTEST_STAT_FEEL]);
        }

        error = pkmn_pokemon2_get_markings(
                    pokemon_ptr,
                    markings,
                    PKMN_NUM_MARKINGS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(size_t index = 0; index < PKMN_NUM_MARKINGS; ++index)
        {
            TEST_ASSERT_FALSE(markings[index]);
        }

        bool has_ribbon = false;

        pkmn_string_list_t ribbon_names =
        {
            .strings = NULL,
            .length = 0
        };
        error = pkmn_pokemon2_get_ribbon_names(
                    pokemon_ptr,
                    &ribbon_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(ribbon_names.length > 0);

        for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
        {
            error = pkmn_pokemon2_has_ribbon(
                        pokemon_ptr,
                        ribbon_names.strings[ribbon_index],
                        &has_ribbon
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_FALSE(has_ribbon);
        }

        error = pkmn_string_list_free(&ribbon_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
    else
    {
        check_for_gb_initial_stat_values(stats, STAT_MAX);

        // Make sure we can't query for fields that didn't exist in the
        // given generation.

        error = pkmn_pokemon2_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon2_get_markings(
                    pokemon_ptr,
                    markings,
                    PKMN_NUM_MARKINGS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void check_initial_values(
    pkmn_pokemon2_t* pokemon_ptr // TODO: params
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    check_initial_pokemon_filepaths_values(pokemon_ptr);
    check_initial_pokemon_info_values(pokemon_ptr);
    check_initial_pokemon_origin_info_values(pokemon_ptr);
    check_initial_pokemon_current_trainer_info_values(pokemon_ptr, "");
    check_initial_pokemon_battle_info_values(pokemon_ptr);
    check_initial_stats_and_misc(pokemon_ptr);
}

static void test_image_filepaths(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 2)
    {
        static const pkmn_gender_t GENDERS[] = {PKMN_GENDER_MALE, PKMN_GENDER_FEMALE};
        static const bool IS_SHINY[] = {true, false};

        for(size_t gender_index = 0; gender_index < 2; ++gender_index)
        {
            for(size_t is_shiny_index = 0; is_shiny_index < 2; ++is_shiny_index)
            {
                error = pkmn_pokemon2_set_gender(
                            pokemon_ptr,
                            GENDERS[gender_index]
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                error = pkmn_pokemon2_set_is_shiny(
                            pokemon_ptr,
                            IS_SHINY[is_shiny_index]
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                pkmn_pokemon_filepaths_t filepaths =
                {
                    .icon_filepath = NULL,
                    .sprite_filepath = NULL
                };

                error = pkmn_pokemon2_get_filepaths(
                            pokemon_ptr,
                            &filepaths
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                TEST_ASSERT_TRUE(file_exists(filepaths.icon_filepath));
                TEST_ASSERT_TRUE(file_exists(filepaths.sprite_filepath));

                error = pkmn_pokemon_filepaths_free(&filepaths);
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            }
        }
    }
}

static void test_setting_ability(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_info_t pokemon_info = empty_pokemon_info;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        pkmn_database_pokemon_entry_t pokemon_entry;

        error = pkmn_pokemon2_get_database_entry(
                    pokemon_ptr,
                    &pokemon_entry
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_TRUE(strcmp(pokemon_entry.abilities.first, "None") > 0);

        error = pkmn_pokemon2_set_ability(
                    pokemon_ptr,
                    pokemon_entry.abilities.first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_pokemon_info(
                    pokemon_ptr,
                    &pokemon_info
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            pokemon_entry.abilities.first,
            pokemon_info.ability
        );

        if(strcmp(pokemon_entry.abilities.second, "None"))
        {
            error = pkmn_pokemon_info_free(&pokemon_info);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_set_ability(
                        pokemon_ptr,
                        pokemon_entry.abilities.second
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_get_pokemon_info(
                        pokemon_ptr,
                        &pokemon_info
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(
                pokemon_entry.abilities.second,
                pokemon_info.ability
            );
        }

        error = pkmn_pokemon_info_free(&pokemon_info);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_database_pokemon_entry_free(&pokemon_entry);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
    else
    {
        error = pkmn_pokemon2_set_ability(
                    pokemon_ptr,
                    "Frenzy" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_ball(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* valid_ball_name,
    const char** invalid_ball_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_ball_name);
    TEST_ASSERT_NOT_NULL(invalid_ball_names);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_origin_info_t pokemon_origin_info = empty_pokemon_origin_info;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        error = pkmn_pokemon2_set_ball(
                    pokemon_ptr,
                    valid_ball_name
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_pokemon_origin_info(
                    pokemon_ptr,
                    &pokemon_origin_info
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            valid_ball_name,
            pokemon_origin_info.ball
        );

        error = pkmn_pokemon_origin_info_free(&pokemon_origin_info);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(size_t ball_index = 0; invalid_ball_names[ball_index]; ++ball_index)
        {
            error = pkmn_pokemon2_set_ball(
                        pokemon_ptr,
                        invalid_ball_names[ball_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon2_set_ball(
                    pokemon_ptr,
                    "Great Ball" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_condition(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_battle_info_t pokemon_battle_info = empty_pokemon_battle_info;

    int generation = game_to_generation(pokemon_ptr->game);

    pkmn_condition_t last_condition = (generation >= 3) ? PKMN_CONDITION_BAD_POISON
                                                        : PKMN_CONDITION_PARALYSIS;

    for(pkmn_condition_t condition = PKMN_CONDITION_NONE; condition < last_condition; ++condition)
    {
        error = pkmn_pokemon2_set_condition(
                    pokemon_ptr,
                    condition
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_battle_info(
                    pokemon_ptr,
                    &pokemon_battle_info
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(condition, pokemon_battle_info.condition);

        error = pkmn_pokemon_battle_info_free(&pokemon_battle_info);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
}

static void test_setting_friendship(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_current_trainer_info_t pokemon_current_trainer_info = empty_pokemon_current_trainer_info;

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon2_get_current_trainer_info(
                pokemon_ptr,
                &pokemon_current_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 2)
    {
        const int friendship = (rand() % 256);

        error = pkmn_pokemon2_set_current_trainer_friendship(
                    pokemon_ptr,
                    friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_current_trainer_info(
                    pokemon_ptr,
                    &pokemon_current_trainer_info
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(
            friendship,
            pokemon_current_trainer_info.current_trainer_friendship
        );
    }
    else
    {
        error = pkmn_pokemon2_set_current_trainer_friendship(
                    pokemon_ptr,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }

    error = pkmn_pokemon_current_trainer_info_free(&pokemon_current_trainer_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void test_setting_item(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* valid_item_name,
    const char** invalid_item_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_item_name);
    TEST_ASSERT_NOT_NULL(invalid_item_names);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_info_t pokemon_info = empty_pokemon_info;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 2)
    {
        error = pkmn_pokemon2_set_held_item(
                    pokemon_ptr,
                    valid_item_name
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_get_pokemon_info(
                    pokemon_ptr,
                    &pokemon_info
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            valid_item_name,
            pokemon_info.held_item
        );

        error = pkmn_pokemon_info_free(&pokemon_info);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(size_t item_index = 0; invalid_item_names[item_index]; ++item_index)
        {
            error = pkmn_pokemon2_set_held_item(
                        pokemon_ptr,
                        invalid_item_names[item_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon2_set_held_item(
                    pokemon_ptr,
                    "Potion" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_levels(
    pkmn_pokemon2_t* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon2_set_level(
                pokemon_ptr,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon2_set_level(
                pokemon_ptr,
                101
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    int level = 50;
    error = pkmn_pokemon2_set_level(
                pokemon_ptr,
                level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int level_from_pokemon = 0;
    error = pkmn_pokemon2_get_level(
                pokemon_ptr,
                &level_from_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(level, level_from_pokemon);

    pkmn_database_pokemon_entry_t pokemon_entry;
    error = pkmn_pokemon2_get_database_entry(
                pokemon_ptr,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_info_t pokemon_info = empty_pokemon_info;
    error = pkmn_pokemon2_get_pokemon_info(
                pokemon_ptr,
                &pokemon_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int level_at_experience = 0;
    error = pkmn_database_pokemon_entry_level_at_experience(
                &pokemon_entry,
                pokemon_info.experience,
                &level_at_experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(level, level_at_experience);

    error = pkmn_pokemon_info_free(&pokemon_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int experience = 123456;
    error = pkmn_pokemon2_set_experience(
                pokemon_ptr,
                experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon2_get_pokemon_info(
                pokemon_ptr,
                &pokemon_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(experience, pokemon_info.experience);

    error = pkmn_pokemon_info_free(&pokemon_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon2_get_level(
                pokemon_ptr,
                &level_from_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int experience_at_level = 0;
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level-1,
                &experience_at_level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(experience_at_level < experience);

    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(experience_at_level <= experience);

    int level_met = 0;
    pkmn_pokemon_current_trainer_info_t pokemon_current_trainer_info = empty_pokemon_current_trainer_info;

    switch(generation)
    {
        case 1:
            error = pkmn_pokemon2_set_level(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        10
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
            error = pkmn_pokemon2_set_level(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        level_met
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_get_current_trainer_info(
                        pokemon_ptr,
                        &pokemon_current_trainer_info
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL(
                level_met,
                pokemon_current_trainer_info.level_met
            );
            break;

        default:
            error = pkmn_pokemon2_set_level(
                        pokemon_ptr,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon2_set_level_met(
                        pokemon_ptr,
                        level_met
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_get_current_trainer_info(
                        pokemon_ptr,
                        &pokemon_current_trainer_info
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL(
                level_met,
                pokemon_current_trainer_info.level_met
            );
            break;
    }
}*/
