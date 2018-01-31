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

static const pkmn_pokemon_filepaths_t empty_pokemon_filepaths =
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
    .level_met = 0,

    .location_met = NULL,
    .location_met_as_egg = NULL
};
static const pkmn_pokemon_current_trainer_info_t empty_pokemon_current_trainer_info =
{
    .current_trainer_friendship = 0
};
static const pkmn_pokemon_battle_info_t empty_pokemon_battle_info =
{
    .current_hp = 0,
    .condition = PKMN_CONDITION_NONE
};

// Helper functions

void check_pokemon_form(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* form
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(form);

    char form_from_pokemon[STRBUFFER_LEN] = {0};

    pkmn_error_t error = pkmn_pokemon2_get_form(
                             pokemon_ptr,
                             form_from_pokemon,
                             sizeof(form_from_pokemon),
                             NULL
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(form, form_from_pokemon);
}

void set_and_check_pokemon_form(
    pkmn_pokemon2_t* pokemon_ptr,
    const char* form
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(form);

    pkmn_error_t error = pkmn_pokemon2_set_form(
                             pokemon_ptr,
                             form
                         );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    check_pokemon_form(pokemon_ptr, form);
}

// Subtest functions

static void check_initial_pokemon_filepaths_values(
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
    pkmn_pokemon2_t* pokemon_ptr,
    const char* expected_original_location
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
        TEST_ASSERT_EQUAL(level, pokemon_origin_info.level_met);

        TEST_ASSERT_EQUAL_STRING(
            expected_original_location,
            pokemon_origin_info.location_met
        );
    }
    else
    {
        TEST_ASSERT_EQUAL(
            0,
            pokemon_origin_info.level_met
        );
        TEST_ASSERT_EQUAL_STRING(
            "None",
            pokemon_origin_info.location_met
        );
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

    if(generation >= 4)
    {
        TEST_ASSERT_EQUAL_STRING(
            expected_original_location,
            pokemon_origin_info.location_met_as_egg
        );
    }
    else
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            pokemon_origin_info.location_met_as_egg
        );
    }

    error = pkmn_pokemon_origin_info_free(&pokemon_origin_info);
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
    check_initial_pokemon_origin_info_values(pokemon_ptr, "");
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
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
}

/*

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static char game[STRBUFFER_LEN] = {0};
static pkmn_error_t error = PKMN_ERROR_NONE;

// These are actually one more than the max, but we need this for the modulo.
#define GB_EV_MAX      65536
#define MODERN_EV_MAX  256

#define GB_IV_MAX      16
#define MODERN_IV_MAX  32

#define STAT_MAX       65536

static void check_initial_values(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    pkmn_trainer_info_t trainer_info =
    {
        .trainer_name = {0},
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_GENDER_GENDERLESS
    };

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_get_form(
                pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Standard", strbuffer);

    char expected_nickname[STRBUFFER_LEN] = {0};
    error = pkmn_pokemon_get_species(
                pokemon,
                expected_nickname,
                sizeof(expected_nickname),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    if(generation < 5)
    {
        int i = 0;
        while(expected_nickname[i])
        {
            expected_nickname[i] = toupper(expected_nickname[i]);
            ++i;
        }
    }

    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(
        expected_nickname,
        strbuffer
    );

    error = pkmn_pokemon_get_original_trainer_info(
                pokemon,
                &trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(
        PKMN_DEFAULT_TRAINER_NAME,
        trainer_info.trainer_name
    );
    TEST_ASSERT_EQUAL(
        (PKMN_DEFAULT_TRAINER_ID & 0xFFFF),
        trainer_info.trainer_id.public_id
    );
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(
            (PKMN_DEFAULT_TRAINER_ID >> 16),
            trainer_info.trainer_id.secret_id
        );
        TEST_ASSERT_EQUAL(
            PKMN_DEFAULT_TRAINER_ID,
            trainer_info.trainer_id.id
        );
    }
    else
    {
        TEST_ASSERT_EQUAL(0, trainer_info.trainer_id.secret_id);
        TEST_ASSERT_EQUAL(
            (PKMN_DEFAULT_TRAINER_ID & 0xFFFF),
            trainer_info.trainer_id.id
        );
    }
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, trainer_info.trainer_gender);

    if(generation >= 2) {
        error = pkmn_pokemon_get_held_item(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);
    }

    pkmn_database_pokemon_entry_t pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 2) {
        int friendship = 0;
        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(pokemon_entry.base_friendship, friendship);
    }

    if(generation >= 3) {
        error = pkmn_pokemon_get_ball(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Premier Ball", strbuffer);

        error = pkmn_pokemon_get_original_game(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        if(!strcmp(game, "Colosseum") || !strcmp(game, "XD")) {
            TEST_ASSERT_EQUAL_STRING("Colosseum/XD", strbuffer);
        } else {
            TEST_ASSERT_EQUAL_STRING(game, strbuffer);
        }
    }

    int expected_experience = 0;
    int experience = 0;

    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                30,
                &expected_experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_experience, experience);

    error = pkmn_database_pokemon_entry_free(&pokemon_entry);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int level = 0;
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(30, level);

    if(generation >= 2) {
        int level_met = 0;
        error = pkmn_pokemon_get_level_met(
                    pokemon,
                    &level_met
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(level, level_met);
    }

    pkmn_move_slots_t move_slots;
    error = pkmn_pokemon_get_moves(
                pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(4, move_slots.length);
    for(size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_STRING("None", move_slots.move_slots[i].move);
        TEST_ASSERT_EQUAL(0, move_slots.move_slots[i].pp);
    }
    error = pkmn_move_slots_free(&move_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(strcmp(game, "Colosseum") && strcmp(game, "XD")) {
        error = pkmn_pokemon_get_icon_filepath(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));

        error = pkmn_pokemon_get_sprite_filepath(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));
    }
}

static void check_initial_maps(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    pkmn_string_list_t EV_names;
    error = pkmn_pokemon_get_EV_names(
                pokemon,
                &EV_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(string_list_contains(&EV_names, "HP"));
    TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Attack"));
    TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Defense"));
    TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Speed"));
    if(generation >= 3) {
        TEST_ASSERT_FALSE(string_list_contains(&EV_names, "Special"));
        TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Special Attack"));
        TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Special Defense"));
    } else {
        TEST_ASSERT_TRUE(string_list_contains(&EV_names, "Special"));
        TEST_ASSERT_FALSE(string_list_contains(&EV_names, "Special Attack"));
        TEST_ASSERT_FALSE(string_list_contains(&EV_names, "Special Defense"));
    }
    for(size_t i = 0; i < EV_names.length; ++i) {
        int EV = 0;
        error = pkmn_pokemon_get_EV(
                    pokemon,
                    EV_names.strings[i],
                    &EV
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(EV >= 0);
        TEST_ASSERT_TRUE(EV <= ((generation >= 3) ? MODERN_EV_MAX : GB_EV_MAX));
    }
    error = pkmn_string_list_free(&EV_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t IV_names;
    error = pkmn_pokemon_get_IV_names(
                pokemon,
                &IV_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(string_list_contains(&IV_names, "HP"));
    TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Attack"));
    TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Defense"));
    TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Speed"));
    if(generation >= 3) {
        TEST_ASSERT_FALSE(string_list_contains(&IV_names, "Special"));
        TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Special Attack"));
        TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Special Defense"));
    } else {
        TEST_ASSERT_TRUE(string_list_contains(&IV_names, "Special"));
        TEST_ASSERT_FALSE(string_list_contains(&IV_names, "Special Attack"));
        TEST_ASSERT_FALSE(string_list_contains(&IV_names, "Special Defense"));
    }
    for(size_t i = 0; i < IV_names.length; ++i) {
        int IV = 0;
        error = pkmn_pokemon_get_IV(
                    pokemon,
                    IV_names.strings[i],
                    &IV
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(IV >= 0);
        TEST_ASSERT_TRUE(IV <= ((generation >= 3) ? MODERN_IV_MAX : GB_IV_MAX));
    }
    error = pkmn_string_list_free(&IV_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t stat_names;
    error = pkmn_pokemon_get_stat_names(
                pokemon,
                &stat_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(string_list_contains(&stat_names, "HP"));
    TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Attack"));
    TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Defense"));
    TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Speed"));
    if(generation >= 2) {
        TEST_ASSERT_FALSE(string_list_contains(&stat_names, "Special"));
        TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Special Attack"));
        TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Special Defense"));
    } else {
        TEST_ASSERT_TRUE(string_list_contains(&stat_names, "Special"));
        TEST_ASSERT_FALSE(string_list_contains(&stat_names, "Special Attack"));
        TEST_ASSERT_FALSE(string_list_contains(&stat_names, "Special Defense"));
    }
    for(size_t i = 0; i < stat_names.length; ++i) {
        int stat = 0;
        error = pkmn_pokemon_get_stat(
                    pokemon,
                    stat_names.strings[i],
                    &stat
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(stat >= 0);
        TEST_ASSERT_TRUE(stat <= STAT_MAX);
    }
    error = pkmn_string_list_free(&stat_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 3) {
        pkmn_string_list_t contest_stat_names;
        error = pkmn_pokemon_get_contest_stat_names(
                    pokemon,
                    &contest_stat_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Cool"));
        TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Beauty"));
        TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Cute"));
        TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Smart"));
        TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Tough"));
        if(generation == 3) {
            TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Feel"));
            TEST_ASSERT_FALSE(string_list_contains(&contest_stat_names, "Sheen"));
        } else {
            TEST_ASSERT_FALSE(string_list_contains(&contest_stat_names, "Feel"));
            TEST_ASSERT_TRUE(string_list_contains(&contest_stat_names, "Sheen"));
        }
        for(size_t i = 0; i < contest_stat_names.length; ++i) {
            int contest_stat = 0;
            error = pkmn_pokemon_get_contest_stat(
                        pokemon,
                        contest_stat_names.strings[i],
                        &contest_stat
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL(0, contest_stat);
        }
        error = pkmn_string_list_free(&contest_stat_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        pkmn_string_list_t marking_names;
        error = pkmn_pokemon_get_marking_names(
                    pokemon,
                    &marking_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Circle"));
        TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Triangle"));
        TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Square"));
        TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Heart"));
        if(generation >= 4) {
            TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Star"));
            TEST_ASSERT_TRUE(string_list_contains(&marking_names, "Diamond"));
        } else {
            TEST_ASSERT_FALSE(string_list_contains(&marking_names, "Star"));
            TEST_ASSERT_FALSE(string_list_contains(&marking_names, "Diamond"));
        }
        for(size_t i = 0; i < marking_names.length; ++i) {
            bool has_marking = 0;
            error = pkmn_pokemon_has_marking(
                        pokemon,
                        marking_names.strings[i],
                        &has_marking
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_FALSE(has_marking);
        }
        error = pkmn_string_list_free(&marking_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
}

static void test_setting_ability(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    if(generation >= 3) {
        pkmn_database_pokemon_entry_t pokemon_entry;
        error = pkmn_pokemon_get_database_entry(
                    pokemon,
                    &pokemon_entry
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT(strcmp("None", pokemon_entry.abilities.first));

        error = pkmn_pokemon_set_ability(
                    pokemon,
                    pokemon_entry.abilities.first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_ability(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(pokemon_entry.abilities.first, strbuffer);

        if(strcmp(pokemon_entry.abilities.second, "None")) {
            error = pkmn_pokemon_set_ability(
                        pokemon,
                        pokemon_entry.abilities.second
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_get_ability(
                        pokemon,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(pokemon_entry.abilities.second, strbuffer);
        }

        if(generation >= 5) {
            TEST_ASSERT(strcmp(pokemon_entry.hidden_ability, "None"));

            error = pkmn_pokemon_set_ability(
                        pokemon,
                        pokemon_entry.hidden_ability
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_get_ability(
                        pokemon,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(pokemon_entry.hidden_ability, strbuffer);
        }
    } else {
        error = pkmn_pokemon_get_ability(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_ability(
                    pokemon,
                    "Wonder Guard"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_ball(
    pkmn_pokemon_handle_t pokemon,
    char* ball_name,
    char** invalid_ball_names
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_NOT_NULL(ball_name);
    TEST_ASSERT_NOT_NULL(invalid_ball_names);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    if(generation >= 3) {
        error = pkmn_pokemon_set_ball(
                    pokemon,
                    ball_name
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_ball(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(ball_name, strbuffer);

        size_t i = 0;
        while(invalid_ball_names[i]) {
            error = pkmn_pokemon_set_ball(
                        pokemon,
                        invalid_ball_names[i]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            ++i;
        }
    } else {
        error = pkmn_pokemon_get_ball(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_ball(
                    pokemon,
                    "Great Ball"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_image_filepaths(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_get_icon_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    if(generation >= 2) {
        pkmn_gender_t genders[] = {PKMN_GENDER_MALE, PKMN_GENDER_FEMALE};
        bool shininess[] = {false, true};

        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) {
                error = pkmn_pokemon_set_gender(
                            pokemon,
                            genders[i]
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                error = pkmn_pokemon_set_shininess(
                            pokemon,
                            shininess[j]
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                error = pkmn_pokemon_get_sprite_filepath(
                            pokemon,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                TEST_ASSERT_TRUE(file_exists(strbuffer));
            }
        }
    } else {
        error = pkmn_pokemon_get_sprite_filepath(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));
    }
}

static void test_setting_friendship(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    int friendship = 0;
    if(generation >= 2) {
        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon,
                    123
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(123, friendship);
    } else {
        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon,
                    123
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_item(
    pkmn_pokemon_handle_t pokemon,
    char* item_name,
    char** invalid_item_names
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_NOT_NULL(item_name);
    TEST_ASSERT_NOT_NULL(invalid_item_names);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    if(generation >= 2) {
        error = pkmn_pokemon_set_held_item(
                    pokemon,
                    item_name
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_held_item(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(item_name, strbuffer);

        size_t i = 0;
        while(invalid_item_names[i]) {
            error = pkmn_pokemon_set_held_item(
                        pokemon,
                        invalid_item_names[i]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            ++i;
        }
    } else {
        error = pkmn_pokemon_get_held_item(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_held_item(
                    pokemon,
                    "Great item"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_levels(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    pkmn_database_pokemon_entry_t pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_set_level(
                pokemon,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_level(
                pokemon,
                101
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    int level = 0;
    int level_at_experience = 0;
    int experience = 0;
    int experience_at_level = 0;

    error = pkmn_pokemon_set_level(
                pokemon,
                50
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(50, level);
    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_database_pokemon_entry_level_at_experience(
                &pokemon_entry,
                experience,
                &level_at_experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(level, level_at_experience);

    error = pkmn_pokemon_set_experience(
                pokemon,
                123456
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(123456, experience);
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level-1,
                &experience_at_level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(experience_at_level < experience);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_level
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(experience_at_level <= experience);

    int level_met = 0;
    switch(generation) {
        case 1:
            error = pkmn_pokemon_set_level(
                        pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_get_level_met(
                        pokemon,
                        &level_met
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        10
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
            error = pkmn_pokemon_set_level(
                        pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            break;

        default:
            error = pkmn_pokemon_set_level(
                        pokemon,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        pokemon,
                        5
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_get_level_met(
                        pokemon,
                        &level_met
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL(5, level_met);
            break;
    }
}

static void test_setting_location_met(
    pkmn_pokemon_handle_t pokemon,
    char* expected_original_location,
    char** locations,
    char** invalid_locations
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_NOT_NULL(expected_original_location);
    TEST_ASSERT_NOT_NULL(locations);
    TEST_ASSERT_NOT_NULL(invalid_locations);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    size_t i = 0;
    switch(generation) {
        case 1:
            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon,
                        locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon,
                        locations[0],
                        false
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
        case 3:
            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            i = 0;
            while(locations[i]) {
                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            locations[i],
                            false
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                error = pkmn_pokemon_get_location_met(
                            pokemon,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                TEST_ASSERT_EQUAL_STRING(locations[i], strbuffer);

                ++i;
            }

            i = 0;
            while(locations[i]) {
                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            invalid_locations[i],
                            false
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

                ++i;
            }

            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon,
                        locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        default:
            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);
            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            i = 0;
            while(locations[i]) {
                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            locations[i],
                            false
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                error = pkmn_pokemon_get_location_met(
                            pokemon,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                TEST_ASSERT_EQUAL_STRING(locations[i], strbuffer);

                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            locations[i],
                            true
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                error = pkmn_pokemon_get_location_met(
                            pokemon,
                            true,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                TEST_ASSERT_EQUAL_STRING(locations[i], strbuffer);

                ++i;
            }

            i = 0;
            while(locations[i]) {
                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            invalid_locations[i],
                            false
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
                error = pkmn_pokemon_set_location_met(
                            pokemon,
                            invalid_locations[i],
                            true
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

                ++i;
            }
            break;
    }
}

static void test_setting_markings(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    pkmn_string_list_t marking_names;
    bool has_marking = false;

    if(generation >= 3) {
        error = pkmn_pokemon_get_marking_names(
                    pokemon,
                    &marking_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(size_t i = 0; i < marking_names.length; ++i) {
            error = pkmn_pokemon_has_marking(
                        pokemon,
                        marking_names.strings[i],
                        &has_marking
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_FALSE(has_marking);
            error = pkmn_pokemon_set_marking(
                        pokemon,
                        marking_names.strings[i],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_has_marking(
                        pokemon,
                        marking_names.strings[i],
                        &has_marking
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_TRUE(has_marking);
        }

        error = pkmn_string_list_free(&marking_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        error = pkmn_pokemon_get_marking_names(
                    pokemon,
                    &marking_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_has_marking(
                    pokemon,
                    "Circle",
                    &has_marking
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_marking(
                    pokemon,
                    "Circle",
                    true
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_moves(
    pkmn_pokemon_handle_t pokemon,
    char** move_names,
    char** invalid_move_names
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    for(size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_NOT_NULL(move_names[i]);
    }
    TEST_ASSERT_NULL(move_names[4]);

    TEST_ASSERT_NOT_NULL(invalid_move_names);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(int i = 0; i < 4; ++i) {
        error = pkmn_pokemon_set_move(
                    pokemon,
                    move_names[i],
                    i
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_set_move(
                pokemon,
                move_names[0],
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_move(
                pokemon,
                move_names[0],
                4
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    pkmn_move_slots_t move_slots;
    error = pkmn_pokemon_get_moves(
                pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(4, move_slots.length);

    pkmn_database_move_entry_t move_entry;
    for(size_t i = 0; i < 4; ++i) {
        error = pkmn_database_get_move_entry(
                    move_slots.move_slots[i].move,
                    game,
                    &move_entry
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL_STRING(move_names[i], move_slots.move_slots[i].move);
        TEST_ASSERT_EQUAL(move_entry.pp[0], move_slots.move_slots[i].pp);

        error = pkmn_database_move_entry_free(&move_entry);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_move_slots_free(&move_slots);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    size_t i = 0;
    while(invalid_move_names[i]) {
        error = pkmn_pokemon_set_move(
                    pokemon,
                    invalid_move_names[i],
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        ++i;
    }
}

static void test_setting_original_game(
    pkmn_pokemon_handle_t pokemon,
    char** games,
    char** invalid_games
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_NOT_NULL(games);
    TEST_ASSERT_NOT_NULL(invalid_games);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    if(generation >= 3) {
        size_t i = 0;
        while(games[i]) {
            error = pkmn_pokemon_set_original_game(
                        pokemon,
                        games[i]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_get_original_game(
                        pokemon,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            if(!strcmp(games[i], "Colosseum") || !strcmp(games[i], "XD")) {
                TEST_ASSERT_EQUAL_STRING("Colosseum/XD", strbuffer);
            } else {
                TEST_ASSERT_EQUAL_STRING(games[i], strbuffer);
            }

            ++i;
        }

        i = 0;
        while(invalid_games[i]) {
            error = pkmn_pokemon_set_original_game(
                        pokemon,
                        invalid_games[i]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

            ++i;
        }
    } else {
        error = pkmn_pokemon_get_original_game(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_original_game(
                    pokemon,
                    games[0]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_personality(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    uint32_t personality = 0;
    if(generation >= 3) {
        error = pkmn_pokemon_set_personality(
                    pokemon,
                    0x7F3AB3A8
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_personality(
                    pokemon,
                    &personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x7F3AB3A8, personality);
    } else {
        error = pkmn_pokemon_get_personality(
                    pokemon,
                    &personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_personality(
                    pokemon,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_stats(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    pkmn_string_list_t EV_names;
    int EV = 0;
    int new_value = 0;

    error = pkmn_pokemon_get_EV_names(
                pokemon,
                &EV_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t i = 0; i < EV_names.length; ++i) {
        new_value = rand() % ((generation >= 3) ? MODERN_EV_MAX : GB_EV_MAX);
        error = pkmn_pokemon_set_EV(
                    pokemon,
                    EV_names.strings[i],
                    new_value
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_EV(
                    pokemon,
                    EV_names.strings[i],
                    &EV
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(new_value, EV);
    }

    error = pkmn_string_list_free(&EV_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t IV_names;
    int IV = 0;
    new_value = 0;

    error = pkmn_pokemon_get_IV_names(
                pokemon,
                &IV_names
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t i = 0; i < IV_names.length; ++i) {
        new_value = rand() % ((generation >= 3) ? MODERN_IV_MAX : GB_IV_MAX);
        error = pkmn_pokemon_set_IV(
                    pokemon,
                    IV_names.strings[i],
                    new_value
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_IV(
                    pokemon,
                    IV_names.strings[i],
                    &IV
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(new_value, IV);
    }

    error = pkmn_string_list_free(&IV_names);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_string_list_t contest_stat_names;
    int contest_stat = 0;
    new_value = 0;

    if(generation >= 3) {
        error = pkmn_pokemon_get_contest_stat_names(
                    pokemon,
                    &contest_stat_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(size_t i = 0; i < contest_stat_names.length; ++i) {
            new_value = rand() % 256;
            error = pkmn_pokemon_set_contest_stat(
                        pokemon,
                        contest_stat_names.strings[i],
                        new_value
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_get_contest_stat(
                        pokemon,
                        contest_stat_names.strings[i],
                        &contest_stat
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL(new_value, contest_stat);
        }

        error = pkmn_string_list_free(&contest_stat_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    } else {
        error = pkmn_pokemon_get_contest_stat_names(
                    pokemon,
                    &contest_stat_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_contest_stat(
                    pokemon,
                    "Cool",
                    &contest_stat
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_contest_stat(
                    pokemon,
                    "Cool",
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_trainer_info(
    pkmn_pokemon_handle_t pokemon
) {
    TEST_ASSERT_NOT_NULL(pokemon);

    pkmn_trainer_info_t getting_trainer_info =
    {
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_GENDER_GENDERLESS
    };
    pkmn_trainer_info_t setting_trainer_info =
    {
        .trainer_name = {0},
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_GENDER_FEMALE
    };

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_set_nickname(
                pokemon,
                "Too long nickname"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_nickname(
                pokemon,
                "foobarbaz"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL_STRING("foobarbaz", strbuffer);

    strncpy(
        setting_trainer_info.trainer_name,
        "Too long trainer name",
        sizeof(setting_trainer_info.trainer_name)
    );
    error = pkmn_pokemon_set_original_trainer_info(
                pokemon,
                &setting_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    strncpy(
        setting_trainer_info.trainer_name,
        "foobar",
        sizeof(setting_trainer_info.trainer_name)
    );
    error = pkmn_pokemon_set_original_trainer_info(
                pokemon,
                &setting_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_get_original_trainer_info(
                pokemon,
                &getting_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(
        setting_trainer_info.trainer_name,
        getting_trainer_info.trainer_name
    );

    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(
            setting_trainer_info.trainer_gender,
            getting_trainer_info.trainer_gender
        );
    }
}

void pokemon_test_common(
    pkmn_pokemon_handle_t pokemon,
    pkmn_test_values_t* test_values
) {
    TEST_ASSERT_NOT_NULL(pokemon);
    TEST_ASSERT_NOT_NULL(test_values);

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    check_initial_values(pokemon);
    check_initial_maps(pokemon);
    test_setting_ability(pokemon);
    test_setting_ball(
        pokemon,
        test_values->valid_ball,
        test_values->invalid_balls
    );
    test_image_filepaths(pokemon);
    test_setting_friendship(pokemon);
    test_setting_item(
        pokemon,
        test_values->valid_item,
        test_values->invalid_items
    );
    test_setting_levels(pokemon);
    test_setting_location_met(
        pokemon,
        test_values->expected_original_location,
        test_values->valid_locations,
        test_values->invalid_locations
    );
    test_setting_markings(pokemon);
    test_setting_moves(
        pokemon,
        test_values->moves,
        test_values->invalid_moves
    );
    test_setting_original_game(
        pokemon,
        test_values->valid_original_games,
        test_values->invalid_original_games
    );
    test_setting_personality(pokemon);
    test_setting_stats(pokemon);
    test_setting_trainer_info(pokemon);
}*/
