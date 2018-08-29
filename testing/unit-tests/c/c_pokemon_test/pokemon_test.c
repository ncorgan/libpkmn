/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test.h"
#include "unity.h"

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/database/move_entry.h>
#include <pkmn-c/enums/enum_to_string.h>

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
    .p_name = NULL,
    .id = {0U},
    .gender = PKMN_GENDER_MALE
};

static const struct pkmn_database_move_entry empty_move_entry =
{
    .move = PKMN_MOVE_NONE,
    .p_name = NULL,
    .game = PKMN_GAME_NONE,
    .type = PKMN_TYPE_NONE,
    .p_description = NULL,
    .target = PKMN_MOVE_TARGET_NONE,
    .damage_class = PKMN_MOVE_DAMAGE_CLASS_NONE,
    .base_power = 0,
    .pp = {0},
    .accuracy = 0.0f,
    .p_effect = NULL,
    .contest_type = PKMN_CONTEST_STAT_NONE,
    .p_contest_effect = NULL,
    .p_super_contest_effect = NULL
};

static const struct pkmn_database_pokemon_entry empty_pokemon_entry =
{
    .species = PKMN_SPECIES_NONE,
    .p_species_name = NULL,
    .game = PKMN_GAME_NONE,
    .p_category = NULL,
    .p_form = NULL,
    .p_pokedex_entry = NULL,
    .height = 0.0f,
    .weight = 0.0f,
    .chance_male = 0.0f,
    .chance_female = 0.0f,
    .has_gender_differences = false,
    .base_friendship = 0,
    .types =
    {
        .first = PKMN_TYPE_NONE,
        .second = PKMN_TYPE_NONE
    },
    .abilities =
    {
        .first = PKMN_ABILITY_NONE,
        .second = PKMN_ABILITY_NONE
    },
    .hidden_ability = PKMN_ABILITY_NONE,
    .egg_groups =
    {
        .first = PKMN_EGG_GROUP_NONE,
        .second = PKMN_EGG_GROUP_NONE
    },
    .base_stats = {0},
    .EV_yields = {0},
    .experience_yield = 0,
    .levelup_moves =
    {
        .p_levelup_moves = NULL,
        .length = 0
    },
    .tm_hm_moves =
    {
        .p_enums = NULL,
        .length = 0
    },
    .egg_moves =
    {
        .p_enums = NULL,
        .length = 0
    },
    .tutor_moves =
    {
        .p_enums = NULL,
        .length = 0
    },
    .forms =
    {
        .pp_strings = NULL,
        .length = 0
    },
    .evolutions =
    {
        .p_enums = NULL,
        .length = 0
    }
};

static const struct pkmn_move_slots empty_move_slots =
{
    .p_move_slots = NULL,
    .length = 0
};

static inline bool is_game_gamecube(enum pkmn_game game)
{
    return (game == PKMN_GAME_COLOSSEUM) || (game == PKMN_GAME_XD);
}

static void check_initial_values(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_strerror(p_pokemon)
    );

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_database_pokemon_entry pokemon_entry = empty_pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                p_pokemon,
                &pokemon_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(
        p_pokemon->species,
        pokemon_entry.species
    );
    TEST_ASSERT_EQUAL(
        p_pokemon->game,
        pokemon_entry.game
    );

    int generation = game_to_generation(p_pokemon->game);

    error = pkmn_pokemon_get_form(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING("Standard", strbuffer);

    error = pkmn_pokemon_get_nickname(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    // Pre-Generation V, Pokémon's nicknames were default to their species
    // in all-caps. Generation V and later, the default nickname was simply
    // the species.
    char pokemon_species[STRBUFFER_LEN] = {0};
    error = pkmn_species_to_string(
                p_pokemon->species,
                pokemon_species,
                sizeof(pokemon_species),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(generation >= 5)
    {
        TEST_ASSERT_EQUAL_STRING(
            pokemon_species,
            strbuffer
        );
    }
    else
    {
        char species_upper[STRBUFFER_LEN] = {0};
        for(size_t index = 0; index < strlen(pokemon_species); ++index)
        {
            species_upper[index] = toupper(pokemon_species[index]);
        }
        TEST_ASSERT_EQUAL_STRING(
            species_upper,
            strbuffer
        );
    }

    struct pkmn_trainer_info original_trainer_info = empty_trainer_info;
    error = pkmn_pokemon_get_original_trainer_info(
                p_pokemon,
                &original_trainer_info
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(
        pkmn_pokemon_default_trainer_name(),
        original_trainer_info.p_name
    );
    TEST_ASSERT_EQUAL(
        pkmn_pokemon_default_trainer_id().public_id,
        original_trainer_info.id.public_id
    );
    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, original_trainer_info.gender);

        enum pkmn_item held_item = PKMN_ITEM_POTION;
        error = pkmn_pokemon_get_held_item(
                    p_pokemon,
                    &held_item
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_ITEM_NONE, held_item);

        int current_trainer_friendship = 0;
        error = pkmn_pokemon_get_current_trainer_friendship(
                    p_pokemon,
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
                    p_pokemon,
                    &level
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(0, level);
        error = pkmn_pokemon_get_level_met(
                    p_pokemon,
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

        enum pkmn_ball ball = PKMN_BALL_NONE;
        error = pkmn_pokemon_get_ball(
                    p_pokemon,
                    &ball
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_BALL_POKE_BALL, ball);

        enum pkmn_game original_game = PKMN_GAME_NONE;
        error = pkmn_pokemon_get_original_game(
                    p_pokemon,
                    &original_game
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        // There is no distinction between Colosseum and XD in the game storage.
        if(is_game_gamecube(p_pokemon->game))
        {
            TEST_ASSERT_EQUAL(PKMN_GAME_COLOSSEUM, original_game);
        }
        else
        {
            TEST_ASSERT_EQUAL(p_pokemon->game, original_game);
        }
    }

    int experience_at_level = 0;
    int level = 0;
    int experience = 0;

    error = pkmn_pokemon_get_level(
                p_pokemon,
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
                p_pokemon,
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
        .p_move_slots = NULL,
        .length = 0
    };
    error = pkmn_pokemon_get_moves(
                p_pokemon,
                &move_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, move_slots.length);

    error = pkmn_move_slots_free(&move_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_get_icon_filepath(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    error = pkmn_pokemon_get_sprite_filepath(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    int current_hp = 0;
    error = pkmn_pokemon_get_current_hp(
                p_pokemon,
                &current_hp
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    struct pkmn_stat_enum_map stats = {NULL, 0};
    error = pkmn_pokemon_get_stats(
                p_pokemon,
                &stats
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(stats.p_values[PKMN_STAT_HP], current_hp);

    error = pkmn_stat_enum_map_free(&stats);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

static void check_initial_maps(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    int generation = game_to_generation(p_pokemon->game);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_stat_enum_map EVs = {NULL, 0};
    struct pkmn_stat_enum_map IVs = {NULL, 0};
    struct pkmn_stat_enum_map stats = {NULL, 0};

    // EVs

    error = pkmn_pokemon_get_EVs(
                p_pokemon,
                &EVs
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL(0, EVs.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_EQUAL(0, EVs.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_EQUAL(0, EVs.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_EQUAL(0, EVs.p_values[PKMN_STAT_SPEED]);
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(-1, EVs.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(0,  EVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_EQUAL(0,  EVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }
    else
    {
        TEST_ASSERT_EQUAL(0,  EVs.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, EVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_EQUAL(-1, EVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }

    error = pkmn_stat_enum_map_free(&EVs);
    PKMN_TEST_ASSERT_SUCCESS(error);

    // IVs

    error = pkmn_pokemon_get_IVs(
                p_pokemon,
                &IVs
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPEED]);
    if(generation >= 3)
    {
        TEST_ASSERT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }
    else
    {
        TEST_ASSERT_NOT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_EQUAL(-1, IVs.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }

    error = pkmn_stat_enum_map_free(&IVs);
    PKMN_TEST_ASSERT_SUCCESS(error);

    // Stats

    error = pkmn_pokemon_get_stats(
                p_pokemon,
                &stats
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_HP]);
    TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_ATTACK]);
    TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_DEFENSE]);
    TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_SPEED]);
    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }
    else
    {
        TEST_ASSERT_NOT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL]);
        TEST_ASSERT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL_ATTACK]);
        TEST_ASSERT_EQUAL(-1, stats.p_values[PKMN_STAT_SPECIAL_DEFENSE]);
    }

    error = pkmn_stat_enum_map_free(&stats);
    PKMN_TEST_ASSERT_SUCCESS(error);

    // Generation III+ maps

    struct pkmn_contest_stat_enum_map contest_stats = {NULL, 0};
    struct pkmn_marking_enum_map markings = {NULL, 0};

    bool has_ribbon = true;
    struct pkmn_string_list ribbon_names =
    {
        .pp_strings = NULL,
        .length = 0
    };

    if(generation >= 3)
    {
        error = pkmn_pokemon_get_contest_stats(
                    p_pokemon,
                    &contest_stats
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_COOL]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_BEAUTY]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_CUTE]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_SMART]);
        TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_TOUGH]);
        if(generation == 3)
        {
            TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_SHEEN]);
        }
        else
        {
            TEST_ASSERT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_FEEL]);
            TEST_ASSERT_NOT_EQUAL(-1, contest_stats.p_values[PKMN_CONTEST_STAT_SHEEN]);
        }

        error = pkmn_contest_stat_enum_map_free(&contest_stats);
        PKMN_TEST_ASSERT_SUCCESS(error);

        // Everything will be false by default, so just make sure the call
        // succeeds.
        error = pkmn_pokemon_get_markings(
                    p_pokemon,
                    &markings
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_ribbon_names(
                    p_pokemon,
                    &ribbon_names
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(ribbon_names.pp_strings);
        TEST_ASSERT_TRUE(ribbon_names.length > 0ULL);

        for(size_t ribbon_index = 0; ribbon_index < ribbon_names.length; ++ribbon_index)
        {
            error = pkmn_pokemon_has_ribbon(
                        p_pokemon,
                        ribbon_names.pp_strings[ribbon_index],
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
                    p_pokemon,
                    &contest_stats
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_get_markings(
                    p_pokemon,
                    &markings
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_get_ribbon_names(
                    p_pokemon,
                    &ribbon_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_has_ribbon(
                    p_pokemon,
                    "Cool",
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_image_filepaths(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(p_pokemon->game);

    error = pkmn_pokemon_get_icon_filepath(
                p_pokemon,
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
                            p_pokemon,
                            genders[gender_index]
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_set_is_shiny(
                            p_pokemon,
                            is_shiny_bools[is_shiny_index]
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_get_sprite_filepath(
                            p_pokemon,
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
                    p_pokemon,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));
    }
}

static void test_setting_ability(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 3)
    {
        struct pkmn_database_pokemon_entry database_entry = empty_pokemon_entry;
        error = pkmn_pokemon_get_database_entry(
                    p_pokemon,
                    &database_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_ability(
                    p_pokemon,
                    database_entry.abilities.first
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        enum pkmn_ability ability = PKMN_ABILITY_NONE;
        error = pkmn_pokemon_get_ability(
                    p_pokemon,
                    &ability
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(database_entry.abilities.first, ability);

        if(database_entry.abilities.second != PKMN_ABILITY_NONE)
        {
            error = pkmn_pokemon_set_ability(
                        p_pokemon,
                        database_entry.abilities.second
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_ability(
                        p_pokemon,
                        &ability
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(database_entry.abilities.second, ability);
        }

        if((generation >= 5) && (database_entry.hidden_ability != PKMN_ABILITY_NONE))
        {
            error = pkmn_pokemon_set_ability(
                        p_pokemon,
                        database_entry.hidden_ability
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_ability(
                        p_pokemon,
                        &ability
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(database_entry.hidden_ability, ability);
        }

        error = pkmn_database_pokemon_entry_free(&database_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_ability(
                    p_pokemon,
                    PKMN_ABILITY_NONE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_pokemon_set_ability(
                    p_pokemon,
                    PKMN_ABILITY_WONDER_GUARD
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    else
    {
        enum pkmn_ability ability = PKMN_ABILITY_NONE;
        error = pkmn_pokemon_get_ability(
                    p_pokemon,
                    &ability
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_ability(
                    p_pokemon,
                    PKMN_ABILITY_WONDER_GUARD // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_ball(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_ball valid_ball,
    const enum pkmn_ball* p_invalid_balls
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(p_invalid_balls);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 3)
    {
        error = pkmn_pokemon_set_ball(
                    p_pokemon,
                    valid_ball
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        enum pkmn_ball ball = PKMN_BALL_NONE;
        error = pkmn_pokemon_get_ball(
                    p_pokemon,
                    &ball
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(valid_ball, ball);

        for(size_t ball_index = 0;
            p_invalid_balls[ball_index] != PKMN_BALL_NONE;
            ++ball_index)
        {
            error = pkmn_pokemon_set_ball(
                        p_pokemon,
                        p_invalid_balls[ball_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        enum pkmn_ball ball = PKMN_BALL_NONE;
        error = pkmn_pokemon_get_ball(
                    p_pokemon,
                    &ball
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_ball(
                    p_pokemon,
                    PKMN_BALL_GREAT_BALL // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_condition(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    enum pkmn_condition last_condition = (generation <= 2) ? PKMN_CONDITION_PARALYSIS
                                                        : PKMN_CONDITION_BAD_POISON;
    for(enum pkmn_condition condition = PKMN_CONDITION_NONE;
        (condition <= last_condition);
        ++condition)
    {
        enum pkmn_condition condition_from_pokemon = PKMN_CONDITION_NONE;

        error = pkmn_pokemon_set_condition(
                    p_pokemon,
                    condition
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_condition(
                    p_pokemon,
                    &condition_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(condition, condition_from_pokemon);
    }
}

static void test_setting_friendship(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 2)
    {
        const int friendship = 123;
        int friendship_from_pokemon = 0;

        error = pkmn_pokemon_set_current_trainer_friendship(
                    p_pokemon,
                    friendship
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_current_trainer_friendship(
                    p_pokemon,
                    &friendship_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(friendship, friendship_from_pokemon);

        error = pkmn_pokemon_set_current_trainer_friendship(
                    p_pokemon,
                    -1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_current_trainer_friendship(
                    p_pokemon,
                    256
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    }
    else
    {
        int friendship_from_pokemon = 0;

        error = pkmn_pokemon_get_current_trainer_friendship(
                    p_pokemon,
                    &friendship_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_current_trainer_friendship(
                    p_pokemon,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_held_item(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_item valid_item,
    const enum pkmn_item* p_invalid_items
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(p_invalid_items);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_held_item(
                    p_pokemon,
                    valid_item
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        enum pkmn_item item = PKMN_ITEM_NONE;
        error = pkmn_pokemon_get_held_item(
                    p_pokemon,
                    &item
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(valid_item, item);

        for(size_t held_item_index = 0;
            p_invalid_items[held_item_index] != PKMN_ITEM_NONE;
            ++held_item_index)
        {
            error = pkmn_pokemon_set_held_item(
                        p_pokemon,
                        p_invalid_items[held_item_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        enum pkmn_item item = PKMN_ITEM_NONE;
        error = pkmn_pokemon_get_held_item(
                    p_pokemon,
                    &item
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_held_item(
                    p_pokemon,
                    PKMN_ITEM_POTION // Doesn't matter what it is
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_levels(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    error = pkmn_pokemon_set_level(
                p_pokemon,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_level(
                p_pokemon,
                101
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    struct pkmn_database_pokemon_entry database_entry = empty_pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                p_pokemon,
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
                p_pokemon,
                level
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_level(
                p_pokemon,
                &level_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(level, level_from_pokemon);

    error = pkmn_pokemon_get_experience(
                p_pokemon,
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
                p_pokemon,
                experience
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_get_experience(
                p_pokemon,
                &experience_from_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(experience, experience_from_pokemon);

    error = pkmn_pokemon_get_level(
                p_pokemon,
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
                        p_pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_get_level_met(
                        p_pokemon,
                        &level_met_from_pokemon
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        10
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
            error = pkmn_pokemon_set_level(
                        p_pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        level_met
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_level_met(
                        p_pokemon,
                        &level_met_from_pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(level_met, level_met_from_pokemon);
            break;

        default:
            error = pkmn_pokemon_set_level(
                        p_pokemon,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        -1
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        101
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

            error = pkmn_pokemon_set_level_met(
                        p_pokemon,
                        level_met
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_level_met(
                        p_pokemon,
                        &level_met_from_pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(level_met, level_met_from_pokemon);
            break;
    }
}

static void test_setting_location_met(
    const struct pkmn_pokemon* p_pokemon,
    const char* expected_original_location,
    const char** valid_locations,
    const char** invalid_locations
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(expected_original_location);
    TEST_ASSERT_NOT_NULL(valid_locations);
    TEST_ASSERT_NOT_NULL(invalid_locations);

    char strbuffer[STRBUFFER_LEN] = {0};
    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    switch(generation)
    {
        case 1:
            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        p_pokemon,
                        valid_locations[0],
                        false
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        p_pokemon,
                        valid_locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        case 2:
        case 3:
            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
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
                            p_pokemon,
                            valid_locations[location_index],
                            false
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            p_pokemon,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL_STRING(valid_locations[location_index], strbuffer);
            }

            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            error = pkmn_pokemon_set_location_met(
                        p_pokemon,
                        valid_locations[0],
                        true
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
            break;

        default:
            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
                        false,
                        strbuffer,
                        sizeof(strbuffer),
                        NULL
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);

            error = pkmn_pokemon_get_location_met(
                        p_pokemon,
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
                            p_pokemon,
                            valid_locations[location_index],
                            false
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            p_pokemon,
                            false,
                            strbuffer,
                            sizeof(strbuffer),
                            NULL
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL_STRING(valid_locations[location_index], strbuffer);

                error = pkmn_pokemon_set_location_met(
                            p_pokemon,
                            valid_locations[location_index],
                            true
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                error = pkmn_pokemon_get_location_met(
                            p_pokemon,
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
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_marking_enum_map markings = {NULL, 0};

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 3)
    {
        enum pkmn_marking last_marking = (generation == 3) ? PKMN_MARKING_HEART
                                                           : PKMN_MARKING_DIAMOND;
        for(enum pkmn_marking marking = PKMN_MARKING_CIRCLE;
            (marking <= last_marking);
            ++marking)
        {
            error = pkmn_pokemon_set_has_marking(
                        p_pokemon,
                        marking,
                        true
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_markings(
                        p_pokemon,
                        &markings
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_TRUE(markings.p_values[marking]);

            error = pkmn_marking_enum_map_free(&markings);
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_set_has_marking(
                        p_pokemon,
                        marking,
                        false
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            error = pkmn_pokemon_get_markings(
                        p_pokemon,
                        &markings
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_FALSE(markings.p_values[marking]);

            error = pkmn_marking_enum_map_free(&markings);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
    }
    else
    {
        error = pkmn_pokemon_get_markings(
                    p_pokemon,
                    &markings
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_has_marking(
                    p_pokemon,
                    PKMN_MARKING_CIRCLE,
                    true
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_moves(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_move* p_valid_moves,
    enum pkmn_move* p_invalid_moves
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(p_valid_moves);
    TEST_ASSERT_NOT_NULL(p_invalid_moves);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_set_move(
                p_pokemon,
                -1,
                p_valid_moves[0]
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_set_move(
                p_pokemon,
                4,
                p_valid_moves[0]
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    for(size_t move_index = 0;
        (move_index < 4) && (p_valid_moves[move_index] != PKMN_MOVE_NONE);
        ++move_index)
    {
        struct pkmn_database_move_entry move_entry = empty_move_entry;
        error = pkmn_database_get_move_entry(
                    p_valid_moves[move_index],
                    p_pokemon->game,
                    &move_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_move(
                    p_pokemon,
                    move_index,
                    p_valid_moves[move_index]
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        struct pkmn_move_slots move_slots = empty_move_slots;
        error = pkmn_pokemon_get_moves(
                    p_pokemon,
                    &move_slots
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            p_valid_moves[move_index],
            move_slots.p_move_slots[move_index].move
        );
        TEST_ASSERT_EQUAL(
            move_entry.pp[0],
            move_slots.p_move_slots[move_index].pp
        );

        error = pkmn_move_slots_free(&move_slots);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_move_pp(
                    p_pokemon,
                    move_index,
                    move_entry.pp[3]
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_moves(
                    p_pokemon,
                    &move_slots
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            move_entry.pp[3],
            move_slots.p_move_slots[move_index].pp
        );

        error = pkmn_move_slots_free(&move_slots);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_database_move_entry_free(&move_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    for(size_t move_index = 0;
        p_invalid_moves[move_index] != PKMN_MOVE_NONE;
        ++move_index)
    {
        error = pkmn_pokemon_set_move(
                    p_pokemon,
                    0,
                    p_invalid_moves[move_index]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
}

static void test_setting_original_game(
    const struct pkmn_pokemon* p_pokemon,
    enum pkmn_game* valid_games,
    enum pkmn_game* invalid_games
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(valid_games);
    TEST_ASSERT_NOT_NULL(invalid_games);

    enum pkmn_error error = PKMN_ERROR_NONE;
    enum pkmn_game original_game = PKMN_GAME_NONE;

    int generation = game_to_generation(p_pokemon->game);

    if(generation >= 3)
    {
        for(size_t game_index = 0;
            valid_games[game_index] != PKMN_GAME_NONE;
            ++game_index)
        {
            error = pkmn_pokemon_set_original_game(
                        p_pokemon,
                        valid_games[game_index]
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_original_game(
                        p_pokemon,
                        &original_game
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            if(is_game_gamecube(valid_games[game_index]))
            {
                TEST_ASSERT_EQUAL(PKMN_GAME_COLOSSEUM, original_game);
            }
            else
            {
                TEST_ASSERT_EQUAL(valid_games[game_index], original_game);
            }
        }

        for(size_t game_index = 0;
            invalid_games[game_index] != PKMN_GAME_NONE;
            ++game_index)
        {
            error = pkmn_pokemon_set_original_game(
                        p_pokemon,
                        invalid_games[game_index]
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }
    else
    {
        error = pkmn_pokemon_get_original_game(
                    p_pokemon,
                    &original_game
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_original_game(
                    p_pokemon,
                    valid_games[0]
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_personality(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    const uint32_t personality = 0x7F3AB3A8;
    uint32_t personality_from_pokemon = 0;

    if(generation >= 3)
    {
        error = pkmn_pokemon_set_personality(
                    p_pokemon,
                    personality
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_personality(
                    p_pokemon,
                    &personality_from_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(personality, personality_from_pokemon);
    }
    else
    {
        error = pkmn_pokemon_set_personality(
                    p_pokemon,
                    personality
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_personality(
                    p_pokemon,
                    &personality_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_pokerus(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    int pokerus_duration_from_pokemon = 0;

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_pokerus_duration(
                    p_pokemon,
                    -1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_pokerus_duration(
                    p_pokemon,
                    16
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

        const int pokerus_duration = rand() % 16;
        error = pkmn_pokemon_set_pokerus_duration(
                    p_pokemon,
                    pokerus_duration
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_pokerus_duration(
                    p_pokemon,
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
                    p_pokemon,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_get_pokerus_duration(
                    p_pokemon,
                    &pokerus_duration_from_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_stats(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_pokemon->game);

    struct pkmn_stat_enum_map EVs = {NULL, 0};
    const int max_EV_value = (generation >= 3) ? MODERN_EV_MAX : GB_EV_MAX;

    for(enum pkmn_stat stat = PKMN_STAT_HP;
        stat <= PKMN_STAT_SPECIAL_DEFENSE;
        ++stat)
    {
        error = pkmn_pokemon_get_EVs(
                    p_pokemon,
                    &EVs
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        // Only set random stats.
        if(EVs.p_values[stat] != -1)
        {
            int new_EV_value = rand() % (max_EV_value + 1);
            error = pkmn_pokemon_set_EV(
                        p_pokemon,
                        stat,
                        new_EV_value
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_stat_enum_map_free(&EVs);
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_EVs(
                        p_pokemon,
                        &EVs
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(new_EV_value, EVs.p_values[stat]);

            error = pkmn_stat_enum_map_free(&EVs);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            error = pkmn_pokemon_set_EV(
                        p_pokemon,
                        stat,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }

    struct pkmn_stat_enum_map IVs = {NULL, 0};
    const int max_IV_value = (generation >= 3) ? MODERN_IV_MAX : GB_IV_MAX;

    for(enum pkmn_stat stat = PKMN_STAT_HP;
        stat <= PKMN_STAT_SPECIAL_DEFENSE;
        ++stat)
    {
        error = pkmn_pokemon_get_IVs(
                    p_pokemon,
                    &IVs
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        if(IVs.p_values[stat] != -1)
        {
            int new_IV_value = rand() % (max_IV_value + 1);
            error = pkmn_pokemon_set_IV(
                        p_pokemon,
                        stat,
                        new_IV_value
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_stat_enum_map_free(&IVs);
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_get_IVs(
                        p_pokemon,
                        &IVs
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_EQUAL(new_IV_value, IVs.p_values[stat]);

            error = pkmn_stat_enum_map_free(&IVs);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            error = pkmn_pokemon_set_IV(
                        p_pokemon,
                        stat,
                        0
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
    }

    if(generation >= 3)
    {
        const int max_contest_stat_value = 255;

        struct pkmn_contest_stat_enum_map contest_stats = {NULL, 0};

        for(enum pkmn_contest_stat contest_stat = PKMN_CONTEST_STAT_COOL;
            contest_stat <= PKMN_CONTEST_STAT_SHEEN;
            ++contest_stat)
        {
            error = pkmn_pokemon_get_contest_stats(
                        p_pokemon,
                        &contest_stats
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            if(contest_stats.p_values[contest_stat] != -1)
            {
                int new_contest_stat_value = rand() % (max_contest_stat_value + 1);
                error = pkmn_pokemon_set_contest_stat(
                            p_pokemon,
                            contest_stat,
                            new_contest_stat_value
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_contest_stat_enum_map_free(&contest_stats);
                PKMN_TEST_ASSERT_SUCCESS(error);

                error = pkmn_pokemon_get_contest_stats(
                            p_pokemon,
                            &contest_stats
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL(
                    new_contest_stat_value,
                    contest_stats.p_values[contest_stat]
                );

                error = pkmn_contest_stat_enum_map_free(&contest_stats);
                PKMN_TEST_ASSERT_SUCCESS(error);
            }
            else
            {
                error = pkmn_pokemon_set_contest_stat(
                            p_pokemon,
                            contest_stat,
                            0
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            }
        }
    }
    else
    {
        struct pkmn_contest_stat_enum_map contest_stats = {NULL, 0};
        error = pkmn_pokemon_get_contest_stats(
                    p_pokemon,
                    &contest_stats
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_contest_stat(
                    p_pokemon,
                    PKMN_CONTEST_STAT_COOL,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
}

static void test_setting_trainer_info(
    const struct pkmn_pokemon* p_pokemon
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);

    enum pkmn_error error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    int generation = game_to_generation(p_pokemon->game);

    const char* nickname = "foobarbaz";
    const char* trainer_name = "foobar";
    union pkmn_trainer_id expected_trainer_id = {0U};

    error = pkmn_pokemon_set_nickname(
                p_pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_nickname(
                p_pokemon,
                "Too long nickname"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_nickname(
                p_pokemon,
                nickname
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_get_nickname(
                p_pokemon,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(nickname, strbuffer);

    error = pkmn_pokemon_set_original_trainer_name(
                p_pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_original_trainer_name(
                p_pokemon,
                "Too long trainer name"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_original_trainer_name(
                p_pokemon,
                trainer_name
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    if(generation >= 2)
    {
        error = pkmn_pokemon_set_original_trainer_gender(
                    p_pokemon,
                    PKMN_GENDER_MALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_gender(
                    p_pokemon,
                    PKMN_GENDER_FEMALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_set_original_trainer_gender(
                    p_pokemon,
                    PKMN_GENDER_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    else
    {
        error = pkmn_pokemon_set_original_trainer_gender(
                    p_pokemon,
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
                    p_pokemon,
                    trainer_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_public_id(
                    p_pokemon,
                    trainer_public_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_secret_id(
                    p_pokemon,
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
                    p_pokemon,
                    too_large_trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_original_trainer_secret_id(
                    p_pokemon,
                    0
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_original_trainer_id(
                    p_pokemon,
                    trainer_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_set_original_trainer_public_id(
                    p_pokemon,
                    trainer_public_id
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        expected_trainer_id.id = (uint32_t)trainer_public_id;
    }

    struct pkmn_trainer_info original_trainer_info = empty_trainer_info;
    error = pkmn_pokemon_get_original_trainer_info(
                p_pokemon,
                &original_trainer_info
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(trainer_name, original_trainer_info.p_name);

    TEST_ASSERT_EQUAL(expected_trainer_id.id, original_trainer_info.id.id);

    if(generation >= 2)
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, original_trainer_info.gender);
    }

    error = pkmn_trainer_info_free(&original_trainer_info);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void pokemon_test_common(
    const struct pkmn_pokemon* p_pokemon,
    pkmn_test_values_t* p_test_values
)
{
    TEST_ASSERT_NOT_NULL(p_pokemon);
    TEST_ASSERT_NOT_NULL(p_test_values);

    check_initial_values(p_pokemon);
    check_initial_maps(p_pokemon);
    test_setting_ability(p_pokemon);
    test_setting_ball(
        p_pokemon,
        p_test_values->valid_ball,
        (enum pkmn_ball*)(p_test_values->p_invalid_balls)
    );
    test_setting_condition(p_pokemon);
    test_image_filepaths(p_pokemon);
    test_setting_friendship(p_pokemon);
    test_setting_held_item(
        p_pokemon,
        p_test_values->valid_item,
        (enum pkmn_item*)(p_test_values->p_invalid_items)
    );
    test_setting_levels(p_pokemon);
    test_setting_location_met(
        p_pokemon,
        p_test_values->expected_original_location,
        (const char**)(p_test_values->valid_locations),
        (const char**)(p_test_values->invalid_locations)
    );
    test_setting_markings(p_pokemon);
    test_setting_moves(
        p_pokemon,
        (enum pkmn_move*)(p_test_values->p_moves),
        (enum pkmn_move*)(p_test_values->p_invalid_moves)
    );
    test_setting_original_game(
        p_pokemon,
        (enum pkmn_game*)(p_test_values->valid_original_games),
        (enum pkmn_game*)(p_test_values->invalid_original_games)
    );
    test_setting_personality(p_pokemon);
    test_setting_pokerus(p_pokemon);
    test_setting_stats(p_pokemon);
    test_setting_trainer_info(p_pokemon);
}
