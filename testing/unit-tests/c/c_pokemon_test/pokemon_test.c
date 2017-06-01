/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_test.h"
#include "unity.h"

#include <pkmntest-c/util.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/database/move_entry.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

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

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_get_form(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Standard", strbuffer);

    static char nickname[STRBUFFER_LEN] = {0};
    error = pkmn_pokemon_get_nickname(
                pokemon,
                nickname,
                sizeof(nickname)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_species(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation < 5) {
        int i = 0;
        while(strbuffer[i]) {
            strbuffer[i] = toupper(strbuffer[i]);
            ++i;
        }
    }
    TEST_ASSERT_EQUAL_STRING(strbuffer, nickname);

    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(PKMN_DEFAULT_TRAINER_NAME, strbuffer);

    if(generation >= 2) {
        error = pkmn_pokemon_get_held_item(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);
    }

    pkmn_gender_t trainer_gender = PKMN_GENDERLESS;
    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, trainer_gender);

    uint16_t trainer_public_id = 0;
    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_public_id
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL((PKMN_DEFAULT_TRAINER_ID & 0xFFFF), trainer_public_id);

    uint32_t trainer_id = 0;
    uint32_t expected_trainer_id = 0;
    if(generation >= 3) {
        uint16_t trainer_secret_id = 0;
        error = pkmn_pokemon_get_trainer_secret_id(
                    pokemon,
                    &trainer_secret_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(((PKMN_DEFAULT_TRAINER_ID & 0xFFFF0000) >> 16), trainer_secret_id);

        expected_trainer_id = PKMN_DEFAULT_TRAINER_ID;
    } else {
        expected_trainer_id = (PKMN_DEFAULT_TRAINER_ID & 0xFFFF);
    }

    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(expected_trainer_id, trainer_id);

    pkmn_database_pokemon_entry_t pokemon_entry;
    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(generation >= 2) {
        int friendship = 0;
        error = pkmn_pokemon_get_friendship(
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
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Premier Ball", strbuffer);

        error = pkmn_pokemon_get_original_game(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
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
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(file_exists(strbuffer));

        error = pkmn_pokemon_get_sprite_filepath(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
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
                sizeof(game)
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
                sizeof(game)
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
                    sizeof(strbuffer)
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
                        sizeof(strbuffer)
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
                        sizeof(strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(pokemon_entry.hidden_ability, strbuffer);
        }
    } else {
        error = pkmn_pokemon_get_ability(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
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
                sizeof(game)
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
                    sizeof(strbuffer)
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
                    sizeof(strbuffer)
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
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_get_icon_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(file_exists(strbuffer));

    if(generation >= 2) {
        pkmn_gender_t genders[] = {PKMN_MALE, PKMN_FEMALE};
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
                            sizeof(strbuffer)
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                TEST_ASSERT_TRUE(file_exists(strbuffer));
            }
        }
    } else {
        error = pkmn_pokemon_get_sprite_filepath(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
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
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    int friendship = 0;
    if(generation >= 2) {
        error = pkmn_pokemon_set_friendship(
                    pokemon,
                    123
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(123, friendship);
    } else {
        error = pkmn_pokemon_get_friendship(
                    pokemon,
                    &friendship
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
        error = pkmn_pokemon_set_friendship(
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
                sizeof(game)
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
                    sizeof(strbuffer)
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
                    sizeof(strbuffer)
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
                sizeof(game)
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
                sizeof(game)
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
                        sizeof(strbuffer)
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
                        sizeof(strbuffer)
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
                        sizeof(strbuffer)
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
                            sizeof(strbuffer)
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
                        sizeof(strbuffer)
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
                        sizeof(strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_EQUAL_STRING(expected_original_location, strbuffer);
            error = pkmn_pokemon_get_location_met(
                        pokemon,
                        true,
                        strbuffer,
                        sizeof(strbuffer)
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
                            sizeof(strbuffer)
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
                            sizeof(strbuffer)
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
                sizeof(game)
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
                sizeof(game)
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
                sizeof(game)
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
                        sizeof(strbuffer)
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
                    sizeof(strbuffer)
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
                sizeof(game)
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
                sizeof(game)
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

    error = pkmn_pokemon_get_game(
                pokemon,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);
    TEST_ASSERT_NOT_EQUAL(-1, generation);

    error = pkmn_pokemon_set_nickname(
                pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
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
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("foobarbaz", strbuffer);

    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                "Too long trainer name"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                "foobar"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("foobar", strbuffer);

    if(generation >= 2) {
        pkmn_gender_t trainer_gender = PKMN_GENDERLESS;

        error = pkmn_pokemon_set_trainer_gender(
                    pokemon,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_gender(
                    pokemon,
                    &trainer_gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_MALE, trainer_gender);

        error = pkmn_pokemon_set_trainer_gender(
                    pokemon,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_gender(
                    pokemon,
                    &trainer_gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_FEMALE, trainer_gender);

        error = pkmn_pokemon_set_trainer_gender(
                    pokemon,
                    PKMN_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    } else {
        error = pkmn_pokemon_set_trainer_gender(
                    pokemon,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }

    uint32_t trainer_id = 0;
    uint16_t trainer_public_id = 0;
    if(generation >= 3) {
        uint16_t trainer_secret_id = 0;

        error = pkmn_pokemon_set_trainer_id(
                    pokemon,
                    0x1234ABCD
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_id(
                    pokemon,
                    &trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x1234ABCD, trainer_id);
        error = pkmn_pokemon_get_trainer_public_id(
                    pokemon,
                    &trainer_public_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0xABCD, trainer_public_id);
        error = pkmn_pokemon_get_trainer_secret_id(
                    pokemon,
                    &trainer_secret_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x1234, trainer_secret_id);

        error = pkmn_pokemon_set_trainer_public_id(
                    pokemon,
                    0x1A2B
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_id(
                    pokemon,
                    &trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x12341A2B, trainer_id);
        error = pkmn_pokemon_get_trainer_public_id(
                    pokemon,
                    &trainer_public_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x1A2B, trainer_public_id);
        error = pkmn_pokemon_get_trainer_secret_id(
                    pokemon,
                    &trainer_secret_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x1234, trainer_secret_id);

        error = pkmn_pokemon_set_trainer_secret_id(
                    pokemon,
                    0x3C4D
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_id(
                    pokemon,
                    &trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x3C4D1A2B, trainer_id);
        error = pkmn_pokemon_get_trainer_public_id(
                    pokemon,
                    &trainer_public_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x1A2B, trainer_public_id);
        error = pkmn_pokemon_get_trainer_secret_id(
                    pokemon,
                    &trainer_secret_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x3C4D, trainer_secret_id);
    } else {
        error = pkmn_pokemon_set_trainer_id(
                    pokemon,
                    0xFFFF+1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
        error = pkmn_pokemon_set_trainer_secret_id(
                    pokemon,
                    0xFFFF
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_set_trainer_id(
                    pokemon,
                    0xABCD
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_id(
                    pokemon,
                    &trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0xABCD, trainer_id);
        error = pkmn_pokemon_get_trainer_public_id(
                    pokemon,
                    &trainer_public_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0xABCD, trainer_public_id);

        error = pkmn_pokemon_set_trainer_public_id(
                    pokemon,
                    0x9876
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_get_trainer_id(
                    pokemon,
                    &trainer_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x9876, trainer_id);
        error = pkmn_pokemon_get_trainer_public_id(
                    pokemon,
                    &trainer_public_id
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(0x9876, trainer_public_id);
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
                sizeof(game)
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
    if(strcmp(game, "Colosseum") && strcmp(game, "XD")) {
        test_image_filepaths(pokemon);
    }
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
}
