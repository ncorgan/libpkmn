/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test.h"
#include "unity.h"

#include <pkmn-c/pokemon.h>

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

#define STAT_MAX       65535

static const struct pkmn_trainer_info empty_trainer_info =
{
    .name = NULL,
    .id = {0U},
    .gender = PKMN_GENDER_MALE
};

static const struct pkmn_database_move_entry empty_move_entry =
{
    .name = NULL,
    .game = NULL,
    .description = NULL,
    .target = NULL,
    .damage_class = NULL,
    .base_power = 0,
    .pp = {0},
    .accuracy = 0.0f,
    .effect = NULL,
    .contest_type = NULL,
    .contest_effect = NULL,
    .super_contest_effect = NULL
};

static const struct pkmn_database_pokemon_entry empty_pokemon_entry =
{
    .name = NULL,
    .game = NULL,
    .species = NULL,
    .form = NULL,
    .pokedex_entry = NULL,
    .height = 0.0f,
    .weight = 0.0f,
    .chance_male = 0.0f,
    .chance_female = 0.0f,
    .has_gender_differences = false,
    .base_friendship = 0,
    .types =
    {
        .first = NULL,
        .second = NULL
    },
    .abilities =
    {
        .first = NULL,
        .second = NULL
    },
    .hidden_ability = NULL,
    .egg_groups =
    {
        .first = NULL,
        .second = NULL
    },
    .base_stats = {0},
    .EV_yields = {0},
    .experience_yield = 0,
    .levelup_moves =
    {
        .levelup_moves = NULL,
        .length = 0
    },
    .tm_hm_moves =
    {
        .strings = NULL,
        .length = 0
    },
    .egg_moves =
    {
        .strings = NULL,
        .length = 0
    },
    .tutor_moves =
    {
        .strings = NULL,
        .length = 0
    },
    .forms =
    {
        .strings = NULL,
        .length = 0
    },
    .evolutions =
    {
        .strings = NULL,
        .length = 0
    }
};

static const struct pkmn_move_slots empty_move_slots =
{
    .move_slots = NULL,
    .length = 0
};

static void check_initial_values(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(pokemon_ptr)
    );

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_database_pokemon_entry pokemon_entry = empty_pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                pokemon_ptr,
                &pokemon_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(
        pokemon_ptr->species,
        pokemon_entry.name
    );
    TEST_ASSERT_EQUAL_STRING(
        pokemon_ptr->game,
        pokemon_entry.game
    );

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon_get_form(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING("Standard", strbuffer);

    error = pkmn_pokemon_get_nickname(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(generation >= 5)
    {
        TEST_ASSERT_EQUAL_STRING(
            pokemon_ptr->species,
            strbuffer
        );
    }
    else
    {
        char species_upper[STRBUFFER_LEN] = {0};
        for(size_t index = 0; index < strlen(pokemon_ptr->species); ++index)
        {
            species_upper[index] = toupper(pokemon_ptr->species[index]);
        }
        TEST_ASSERT_EQUAL_STRING(
            species_upper,
            strbuffer
        );
    }

    struct pkmn_trainer_info original_trainer_info = empty_trainer_info;
    error = pkmn_pokemon_get_original_trainer_info(
                pokemon_ptr,
                &original_trainer_info
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        pkmn_pokemon_default_trainer_name(),
        original_trainer_info.name
    );
    TEST_ASSERT_EQUAL(
        pkmn_pokemon_default_trainer_id().public_id,
        original_trainer_info.id.public_id
    );
    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, original_trainer_info.gender);

        error = pkmn_pokemon_get_held_item(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);

        int current_trainer_friendship = 0;
        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon_ptr,
                    &current_trainer_friendship
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            pokemon_entry.base_friendship,
            current_trainer_friendship
        );

        int level = 0;
        int level_met = 0;

        error = pkmn_pokemon_get_level(
                    pokemon_ptr,
                    &level
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(0, level);
        error = pkmn_pokemon_get_level_met(
                    pokemon_ptr,
                    &level_met
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(level, level_met);
    }
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(
            pkmn_pokemon_default_trainer_id().id,
            original_trainer_info.id.id
        );

        error = pkmn_pokemon_get_ball(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_STRING(
            "Premier Ball",
            strbuffer
        );

        // There is no distinction between Colosseum and XD in the game storage.
        error = pkmn_pokemon_get_original_game(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        if(!strcmp(pokemon_ptr->game, "Colosseum") || !strcmp(pokemon_ptr->game, "XD"))
        {
            TEST_ASSERT_EQUAL_STRING(
                "Colosseum/XD",
                strbuffer
            );
        }
        else
        {
            TEST_ASSERT_EQUAL_STRING(
                pokemon_ptr->game,
                strbuffer
            );
        }
    }

    int experience_at_level = 0;
    int level = 0;
    int experience = 0;

    error = pkmn_pokemon_get_level(
                pokemon_ptr,
                &level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_experience(
                pokemon_ptr,
                &experience
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(experience_at_level, experience);

    error = pkmn_trainer_info_free(&original_trainer_info);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_database_pokemon_entry_free(&pokemon_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_move_slots move_slots =
    {
        .move_slots = NULL,
        .length = 0
    };
    error = pkmn_pokemon_get_moves(
                pokemon_ptr,
                &move_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, move_slots.length);

    for(size_t move_index = 0; move_index < move_slots.length; ++move_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            move_slots.move_slots[move_index].move
        );
        TEST_ASSERT_EQUAL(
            0,
            move_slots.move_slots[move_index].pp
        );
    }

    error = pkmn_move_slots_free(&move_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_get_icon_filepath(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    error = pkmn_pokemon_get_sprite_filepath(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    int current_hp = 0;
    error = pkmn_pokemon_get_current_hp(
                pokemon_ptr,
                &current_hp
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    int stats[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_stats(
                pokemon_ptr,
                stats,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(stats[PKMN_STAT_HP], current_hp);
}

static void check_initial_maps(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    int generation = game_to_generation(pokemon_ptr->game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int EVs[PKMN_NUM_STATS] = {0};
    int IVs[PKMN_NUM_STATS] = {0};
    int stats[PKMN_NUM_STATS] = {0};

    // EVs

    error = pkmn_pokemon_get_EVs(
                pokemon_ptr,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_SPEED]);
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(-1, EVs[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_SPATK]);
        TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_SPDEF]);
    }
    else
    {
        TEST_ASSERT_NOT_EQUAL(-1, EVs[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, EVs[PKMN_STAT_SPATK]);
        TEST_ASSERT_EQUAL(-1, EVs[PKMN_STAT_SPDEF]);
    }

    // IVs

    error = pkmn_pokemon_get_IVs(
                pokemon_ptr,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_SPEED]);
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(-1, IVs[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_SPATK]);
        TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_SPDEF]);
    }
    else
    {
        TEST_ASSERT_NOT_EQUAL(-1, IVs[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, IVs[PKMN_STAT_SPATK]);
        TEST_ASSERT_EQUAL(-1, IVs[PKMN_STAT_SPDEF]);
    }

    // Stats

    error = pkmn_pokemon_get_stats(
                pokemon_ptr,
                stats,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_SPEED]);
    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(-1, stats[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_SPATK]);
        TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_SPDEF]);
    }
    else
    {
        TEST_ASSERT_NOT_EQUAL(-1, stats[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, stats[PKMN_STAT_SPATK]);
        TEST_ASSERT_EQUAL(-1, stats[PKMN_STAT_SPDEF]);
    }

    // Generation III+ maps

    int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
    bool markings[PKMN_NUM_MARKINGS] = {0};

    bool has_ribbon = true;
    struct pkmn_string_list ribbon_names =
    {
        .strings = NULL,
        .length = 0
    };

    if(generation >= 3)
    {
        error = pkmn_pokemon_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_COOL]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_BEAUTY]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_CUTE]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_SMART]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_TOUGH]);
        if(generation == 3)
        {
            TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_SHEEN]);
        }
        else
        {
            TEST_ASSERT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_NOT_EQUAL(-1, contest_stats[PKMN_CONTEST_STAT_SHEEN]);
        }

        // Everything will be false by default, so just make sure the call
        // succeeds.
        error = pkmn_pokemon_get_markings(
                    pokemon_ptr,
                    markings,
                    PKMN_NUM_MARKINGS,
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_ribbon_names(
                    pokemon_ptr,
                    &ribbon_names
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(ribbon_names.strings);
        TEST_ASSERT_TRUE(ribbon_names.length > 0ULL);

        for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
        {
            error = pkmn_pokemon_has_ribbon(
                        pokemon_ptr,
                        ribbon_names.strings[ribbon_index],
                        &has_ribbon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_FALSE(has_ribbon);
        }

        error = pkmn_string_list_free(&ribbon_names);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        error = pkmn_pokemon_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_get_markings(
                    pokemon_ptr,
                    markings,
                    PKMN_NUM_MARKINGS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_get_ribbon_names(
                    pokemon_ptr,
                    &ribbon_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_has_ribbon(
                    pokemon_ptr,
                    "Cool",
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_image_filepaths(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon_get_icon_filepath(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    if(generation >= 2)
    {
        static const enum pkmn_gender genders[] = {PKMN_GENDER_MALE, PKMN_GENDER_FEMALE};
        static const bool is_shiny_bools[] = {true, false};

        for(size_t gender_index = 0; gender_index < 2; ++gender_index)
        {
            for(size_t is_shiny_index = 0; is_shiny_index < 2; ++is_shiny_index)
            {
                error = pkmn_pokemon_set_gender(
                            pokemon_ptr,
                            genders[gender_index]
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_set_is_shiny(
                            pokemon_ptr,
                            is_shiny_bools[is_shiny_index]
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_get_sprite_filepath(
                            pokemon_ptr,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_TRUE(file_exists(strbuffer));
            }
        }
    }
    else
    {
        error = pkmn_pokemon_get_sprite_filepath(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));
    }
}

static void test_setting_ability(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        struct pkmn_database_pokemon_entry database_entry = empty_pokemon_entry;
        error = pkmn_pokemon_get_database_entry(
                    pokemon_ptr,
                    &database_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_ability(
                    pokemon_ptr,
                    database_entry.abilities.first
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_ability(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(
            database_entry.abilities.first,
            strbuffer
        );

        if(strcmp(database_entry.abilities.second, "None"))
        {
            error = pkmn_pokemon_set_ability(
                        pokemon_ptr,
                        database_entry.abilities.second
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_ability(
                        pokemon_ptr,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            TEST_ASSERT_EQUAL_STRING(
                database_entry.abilities.second,
                strbuffer
            );
        }

        if((generation >= 5) && !strcmp(database_entry.hidden_ability, "None"))
        {
            error = pkmn_pokemon_set_ability(
                        pokemon_ptr,
                        database_entry.hidden_ability
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_ability(
                        pokemon_ptr,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            TEST_ASSERT_EQUAL_STRING(
                database_entry.hidden_ability,
                strbuffer
            );
        }

        error = pkmn_database_pokemon_entry_free(&database_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_ability(
                    pokemon_ptr,
                    "Not an ability"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_pokemon_set_ability(
                    pokemon_ptr,
                    "Wonder Guard"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    else
    {
        error = pkmn_pokemon_get_ability(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_ability(
                    pokemon_ptr,
                    "Wonder Guard" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_ball(
    struct pkmn_pokemon* pokemon_ptr,
    const char* valid_ball_name,
    const char** invalid_ball_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_ball_name);
    TEST_ASSERT_NOT_NULL(invalid_ball_names);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        error = pkmn_pokemon_set_ball(
                    pokemon_ptr,
                    valid_ball_name
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_ball(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(
            valid_ball_name,
            strbuffer
        );

        for(size_t ball_index = 0; invalid_ball_names[ball_index]; ++ball_index)
        {
            error = pkmn_pokemon_set_ball(
                        pokemon_ptr,
                        invalid_ball_names[ball_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon_get_ball(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_ball(
                    pokemon_ptr,
                    "Great Ball" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_condition(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    enum pkmn_condition last_condition = (generation <= 2) ? PKMN_CONDITION_PARALYSIS
                                                        : PKMN_CONDITION_BAD_POISON;
    for(enum pkmn_condition condition = PKMN_CONDITION_NONE;
        (condition <= last_condition);
        ++condition)
    {
        enum pkmn_condition condition_from_pokemon = PKMN_CONDITION_NONE;

        error = pkmn_pokemon_set_condition(
                    pokemon_ptr,
                    condition
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_condition(
                    pokemon_ptr,
                    &condition_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(condition, condition_from_pokemon);
    }
}

static void test_setting_friendship(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 2)
    {
        const int friendship = 123;
        int friendship_from_pokemon = 0;

        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon_ptr,
                    friendship
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon_ptr,
                    &friendship_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(friendship, friendship_from_pokemon);

        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon_ptr,
                    -1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon_ptr,
                    256
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    }
    else
    {
        int friendship_from_pokemon = 0;

        error = pkmn_pokemon_get_current_trainer_friendship(
                    pokemon_ptr,
                    &friendship_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_current_trainer_friendship(
                    pokemon_ptr,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_held_item(
    struct pkmn_pokemon* pokemon_ptr,
    const char* valid_item_name,
    const char** invalid_item_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_item_name);
    TEST_ASSERT_NOT_NULL(invalid_item_names);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_held_item(
                    pokemon_ptr,
                    valid_item_name
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_held_item(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        TEST_ASSERT_EQUAL_STRING(
            valid_item_name,
            strbuffer
        );

        for(size_t held_item_index = 0; invalid_item_names[held_item_index]; ++held_item_index)
        {
            error = pkmn_pokemon_set_held_item(
                        pokemon_ptr,
                        invalid_item_names[held_item_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon_get_held_item(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_held_item(
                    pokemon_ptr,
                    "Potion" // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_levels(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    error = pkmn_pokemon_set_level(
                pokemon_ptr,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_level(
                pokemon_ptr,
                101
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    struct pkmn_database_pokemon_entry database_entry = empty_pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                pokemon_ptr,
                &database_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    const int level = 50;
    int level_from_pokemon = 0;
    int level_at_experience = 0;

    const int experience = 123456;
    int experience_from_pokemon = 0;
    int experience_at_level = 0;

    // Test setting level.

    error = pkmn_pokemon_set_level(
                pokemon_ptr,
                level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_level(
                pokemon_ptr,
                &level_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(level, level_from_pokemon);

    error = pkmn_pokemon_get_experience(
                pokemon_ptr,
                &experience_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_database_pokemon_entry_level_at_experience(
                &database_entry,
                experience_from_pokemon,
                &level_at_experience
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(level, level_at_experience);

    // Test setting experience.

    error = pkmn_pokemon_set_experience(
                pokemon_ptr,
                experience
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_experience(
                pokemon_ptr,
                &experience_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(experience, experience_from_pokemon);

    error = pkmn_pokemon_get_level(
                pokemon_ptr,
                &level_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &database_entry,
                level_from_pokemon-1,
                &experience_at_level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(experience_at_level <= experience);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &database_entry,
                level_from_pokemon,
                &experience_at_level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(experience_at_level < experience);

    error = pkmn_database_pokemon_entry_free(&database_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    const int level_met = 5;
    int level_met_from_pokemon = 0;

    switch(generation)
    {
        case 1:
            error = pkmn_pokemon_set_level(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_get_level_met(
                        pokemon_ptr,
                        &level_met_from_pokemon
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        10
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
            error = pkmn_pokemon_set_level(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        level_met
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_level_met(
                        pokemon_ptr,
                        &level_met_from_pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(level_met, level_met_from_pokemon);
            break;

        default:
            error = pkmn_pokemon_set_level(
                        pokemon_ptr,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        pokemon_ptr,
                        level_met
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_level_met(
                        pokemon_ptr,
                        &level_met_from_pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(level_met, level_met_from_pokemon);
            break;
    }
}

static void test_setting_location_met(
    struct pkmn_pokemon* pokemon_ptr,
    const char* expected_original_location,
    const char** valid_locations,
    const char** invalid_locations
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(expected_original_location);
    TEST_ASSERT_NOT_NULL(valid_locations);
    TEST_ASSERT_NOT_NULL(invalid_locations);

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    switch(generation)
    {
        case 1:
            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon_ptr,
                        valid_locations[0],
                        false
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon_ptr,
                        valid_locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
        case 3:
            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            for(size_t location_index = 0; valid_locations[location_index]; ++location_index)
            {
                error = pkmn_pokemon_set_location_met(
                            pokemon_ptr,
                            valid_locations[location_index],
                            false
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            pokemon_ptr,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL_STRING(valid_locations[location_index], strbuffer);
            }

            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        pokemon_ptr,
                        valid_locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        default:
            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            error = pkmn_pokemon_get_location_met(
                        pokemon_ptr,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            for(size_t location_index = 0; valid_locations[location_index]; ++location_index)
            {
                error = pkmn_pokemon_set_location_met(
                            pokemon_ptr,
                            valid_locations[location_index],
                            false
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            pokemon_ptr,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL_STRING(valid_locations[location_index], strbuffer);

                error = pkmn_pokemon_set_location_met(
                            pokemon_ptr,
                            valid_locations[location_index],
                            true
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            pokemon_ptr,
                            true,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL_STRING(valid_locations[location_index], strbuffer);
            }

            break;
    }
}

static void test_setting_markings(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;
    bool has_markings[PKMN_NUM_MARKINGS];

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        enum pkmn_marking last_marking = (generation == 3) ? PKMN_MARKING_HEART
                                                        : PKMN_MARKING_DIAMOND;
        for(enum pkmn_marking marking = PKMN_MARKING_CIRCLE;
            (marking <= last_marking);
            ++marking)
        {
            error = pkmn_pokemon_set_has_marking(
                        pokemon_ptr,
                        marking,
                        true
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_markings(
                        pokemon_ptr,
                        has_markings,
                        PKMN_NUM_MARKINGS,
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_TRUE(has_markings[marking]);

            error = pkmn_pokemon_set_has_marking(
                        pokemon_ptr,
                        marking,
                        false
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_markings(
                        pokemon_ptr,
                        has_markings,
                        PKMN_NUM_MARKINGS,
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_FALSE(has_markings[marking]);
        }
    }
    else
    {
        error = pkmn_pokemon_get_markings(
                    pokemon_ptr,
                    has_markings,
                    PKMN_NUM_MARKINGS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_has_marking(
                    pokemon_ptr,
                    PKMN_MARKING_CIRCLE,
                    true
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_moves(
    struct pkmn_pokemon* pokemon_ptr,
    const char** valid_move_names,
    const char** invalid_move_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_move_names);
    TEST_ASSERT_NOT_NULL(invalid_move_names);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_set_move(
                pokemon_ptr,
                -1,
                valid_move_names[0]
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_move(
                pokemon_ptr,
                4,
                valid_move_names[0]
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    for(size_t move_index = 0;
        (move_index < 4) && valid_move_names[move_index];
        ++move_index)
    {
        struct pkmn_database_move_entry move_entry = empty_move_entry;
        error = pkmn_database_get_move_entry(
                    valid_move_names[move_index],
                    pokemon_ptr->game,
                    &move_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_move(
                    pokemon_ptr,
                    move_index,
                    valid_move_names[move_index]
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        struct pkmn_move_slots move_slots = empty_move_slots;
        error = pkmn_pokemon_get_moves(
                    pokemon_ptr,
                    &move_slots
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_STRING(
            valid_move_names[move_index],
            move_slots.move_slots[move_index].move
        );
        TEST_ASSERT_EQUAL(
            move_entry.pp[0],
            move_slots.move_slots[move_index].pp
        );

        error = pkmn_move_slots_free(&move_slots);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_move_pp(
                    pokemon_ptr,
                    move_index,
                    move_entry.pp[3]
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_moves(
                    pokemon_ptr,
                    &move_slots
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            move_entry.pp[3],
            move_slots.move_slots[move_index].pp
        );

        error = pkmn_move_slots_free(&move_slots);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_database_move_entry_free(&move_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    for(size_t move_index = 0; invalid_move_names[move_index]; ++move_index)
    {
        error = pkmn_pokemon_set_move(
                    pokemon_ptr,
                    0,
                    invalid_move_names[move_index]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
}

static void test_setting_original_game(
    struct pkmn_pokemon* pokemon_ptr,
    const char** valid_game_names,
    const char** invalid_game_names
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(valid_game_names);
    TEST_ASSERT_NOT_NULL(invalid_game_names);

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    if(generation >= 3)
    {
        for(size_t game_index = 0; valid_game_names[game_index]; ++game_index)
        {
            error = pkmn_pokemon_set_original_game(
                        pokemon_ptr,
                        valid_game_names[game_index]
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_original_game(
                        pokemon_ptr,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            if(!strcmp(valid_game_names[game_index], "Colosseum") ||
               !strcmp(valid_game_names[game_index], "XD"))
            {
                TEST_ASSERT_EQUAL_STRING("Colosseum/XD", strbuffer);
            }
            else
            {
                TEST_ASSERT_EQUAL_STRING(
                    valid_game_names[game_index],
                    strbuffer
                );
            }
        }

        for(size_t game_index = 0; invalid_game_names[game_index]; ++game_index)
        {
            error = pkmn_pokemon_set_original_game(
                        pokemon_ptr,
                        invalid_game_names[game_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon_get_original_game(
                    pokemon_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_original_game(
                    pokemon_ptr,
                    valid_game_names[0]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_personality(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    const uint32_t personality = 0x7F3AB3A8;
    uint32_t personality_from_pokemon = 0;

    if(generation >= 3)
    {
        error = pkmn_pokemon_set_personality(
                    pokemon_ptr,
                    personality
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_personality(
                    pokemon_ptr,
                    &personality_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(personality, personality_from_pokemon);
    }
    else
    {
        error = pkmn_pokemon_set_personality(
                    pokemon_ptr,
                    personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_personality(
                    pokemon_ptr,
                    &personality_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_pokerus(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    int pokerus_duration_from_pokemon = 0;

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_pokerus_duration(
                    pokemon_ptr,
                    -1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_pokerus_duration(
                    pokemon_ptr,
                    16
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

        const int pokerus_duration = rand() % 16;
        error = pkmn_pokemon_set_pokerus_duration(
                    pokemon_ptr,
                    pokerus_duration
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_pokerus_duration(
                    pokemon_ptr,
                    &pokerus_duration_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            pokerus_duration,
            pokerus_duration_from_pokemon
        );
    }
    else
    {
        error = pkmn_pokemon_set_pokerus_duration(
                    pokemon_ptr,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_pokerus_duration(
                    pokemon_ptr,
                    &pokerus_duration_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_stats(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(pokemon_ptr->game);

    int EVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_EVs(
                pokemon_ptr,
                EVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    const int max_EV_value = (generation >= 3) ? MODERN_EV_MAX : GB_EV_MAX;

    for(enum pkmn_stat stat = PKMN_STAT_HP; stat <= PKMN_STAT_SPDEF; ++stat)
    {
        // Only set random stats.
        if(EVs[stat] != -1)
        {
            int new_EV_value = rand() % (max_EV_value + 1);
            error = pkmn_pokemon_set_EV(
                        pokemon_ptr,
                        stat,
                        new_EV_value
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_EVs(
                        pokemon_ptr,
                        EVs,
                        PKMN_NUM_STATS,
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(new_EV_value, EVs[stat]);
        }
        else
        {
            error = pkmn_pokemon_set_EV(
                        pokemon_ptr,
                        stat,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }

    int IVs[PKMN_NUM_STATS] = {0};
    error = pkmn_pokemon_get_IVs(
                pokemon_ptr,
                IVs,
                PKMN_NUM_STATS,
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    const int max_IV_value = (generation >= 3) ? MODERN_IV_MAX : GB_IV_MAX;

    for(enum pkmn_stat stat = PKMN_STAT_HP; stat <= PKMN_STAT_SPDEF; ++stat)
    {
        if(IVs[stat] != -1)
        {
            int new_IV_value = rand() % (max_IV_value + 1);
            error = pkmn_pokemon_set_IV(
                        pokemon_ptr,
                        stat,
                        new_IV_value
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_IVs(
                        pokemon_ptr,
                        IVs,
                        PKMN_NUM_STATS,
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(new_IV_value, IVs[stat]);
        }
        else
        {
            error = pkmn_pokemon_set_IV(
                        pokemon_ptr,
                        stat,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }

    if(generation >= 3)
    {
        const int max_contest_stat_value = 255;

        int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
        error = pkmn_pokemon_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        for(enum pkmn_contest_stat contest_stat = PKMN_CONTEST_STAT_COOL;
            contest_stat <= PKMN_CONTEST_STAT_SHEEN;
            ++contest_stat)
        {
            if(contest_stats[contest_stat] != -1)
            {
                int new_contest_stat_value = rand() % (max_contest_stat_value + 1);
                error = pkmn_pokemon_set_contest_stat(
                            pokemon_ptr,
                            contest_stat,
                            new_contest_stat_value
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_get_contest_stats(
                            pokemon_ptr,
                            contest_stats,
                            PKMN_NUM_CONTEST_STATS,
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL(
                    new_contest_stat_value,
                    contest_stats[contest_stat]
                );
            }
            else
            {
                error = pkmn_pokemon_set_contest_stat(
                            pokemon_ptr,
                            contest_stat,
                            0
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            }
        }
    }
    else
    {
        int contest_stats[PKMN_NUM_CONTEST_STATS] = {0};
        error = pkmn_pokemon_get_contest_stats(
                    pokemon_ptr,
                    contest_stats,
                    PKMN_NUM_CONTEST_STATS,
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_contest_stat(
                    pokemon_ptr,
                    PKMN_CONTEST_STAT_COOL,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_trainer_info(
    struct pkmn_pokemon* pokemon_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(pokemon_ptr->game);

    const char* nickname = "foobarbaz";
    const char* trainer_name = "foobar";
    union pkmn_trainer_id expected_trainer_id = {0U};

    error = pkmn_pokemon_set_nickname(
                pokemon_ptr,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_nickname(
                pokemon_ptr,
                "Too long nickname"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_nickname(
                pokemon_ptr,
                nickname
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_get_nickname(
                pokemon_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(nickname, strbuffer);

    error = pkmn_pokemon_set_original_trainer_name(
                pokemon_ptr,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_original_trainer_name(
                pokemon_ptr,
                "Too long trainer name"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_original_trainer_name(
                pokemon_ptr,
                trainer_name
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_original_trainer_gender(
                    pokemon_ptr,
                    PKMN_GENDER_MALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_gender(
                    pokemon_ptr,
                    PKMN_GENDER_FEMALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_original_trainer_gender(
                    pokemon_ptr,
                    PKMN_GENDER_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    else
    {
        error = pkmn_pokemon_set_original_trainer_gender(
                    pokemon_ptr,
                    PKMN_GENDER_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }

    if(generation >= 3)
    {
        const uint32_t trainer_id = 0x1234ABCD;
        const uint16_t trainer_public_id = 0x1A2B;
        const uint16_t trainer_secret_id = 0x3C4D;

        error = pkmn_pokemon_set_original_trainer_id(
                    pokemon_ptr,
                    trainer_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_public_id(
                    pokemon_ptr,
                    trainer_public_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_secret_id(
                    pokemon_ptr,
                    trainer_secret_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        expected_trainer_id.public_id = trainer_public_id;
        expected_trainer_id.secret_id = trainer_secret_id;
    }
    else
    {
        const uint32_t too_large_trainer_id = 0xFFFF+1;
        const uint32_t trainer_id = 0xABCD;
        const uint16_t trainer_public_id = 0x9876;

        error = pkmn_pokemon_set_original_trainer_id(
                    pokemon_ptr,
                    too_large_trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_original_trainer_secret_id(
                    pokemon_ptr,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_original_trainer_id(
                    pokemon_ptr,
                    trainer_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_public_id(
                    pokemon_ptr,
                    trainer_public_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        expected_trainer_id.id = (uint32_t)trainer_public_id;
    }

    struct pkmn_trainer_info original_trainer_info = empty_trainer_info;
    error = pkmn_pokemon_get_original_trainer_info(
                pokemon_ptr,
                &original_trainer_info
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(trainer_name, original_trainer_info.name);

    TEST_ASSERT_EQUAL(expected_trainer_id.id, original_trainer_info.id.id);

    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, original_trainer_info.gender);
    }

    error = pkmn_trainer_info_free(&original_trainer_info);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void pokemon_test_common(
    struct pkmn_pokemon* pokemon_ptr,
    pkmn_test_values_t* test_values_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_ptr);
    TEST_ASSERT_NOT_NULL(test_values_ptr);

    check_initial_values(pokemon_ptr);
    check_initial_maps(pokemon_ptr);
    test_setting_ability(pokemon_ptr);
    test_setting_ball(
        pokemon_ptr,
        test_values_ptr->valid_ball,
        (const char**)(test_values_ptr->invalid_balls)
    );
    test_setting_condition(pokemon_ptr);
    test_image_filepaths(pokemon_ptr);
    test_setting_friendship(pokemon_ptr);
    test_setting_held_item(
        pokemon_ptr,
        test_values_ptr->valid_item,
        (const char**)(test_values_ptr->invalid_items)
    );
    test_setting_levels(pokemon_ptr);
    test_setting_location_met(
        pokemon_ptr,
        test_values_ptr->expected_original_location,
        (const char**)(test_values_ptr->valid_locations),
        (const char**)(test_values_ptr->invalid_locations)
    );
    test_setting_markings(pokemon_ptr);
    test_setting_moves(
        pokemon_ptr,
        (const char**)(test_values_ptr->moves),
        (const char**)(test_values_ptr->invalid_moves)
    );
    test_setting_original_game(
        pokemon_ptr,
        (const char**)(test_values_ptr->valid_original_games),
        (const char**)(test_values_ptr->invalid_original_games)
    );
    test_setting_personality(pokemon_ptr);
    test_setting_pokerus(pokemon_ptr);
    test_setting_stats(pokemon_ptr);
    test_setting_trainer_info(pokemon_ptr);
}
