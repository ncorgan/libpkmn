/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "util.h"

#include <pkmntest-c/pokemon_tests_common.h>

#include <pkmntest-c/gen2_pokemon_tests.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

void pkmntest_gen2_unown_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t unown = NULL;

    pkmn_database_pokemon_entry_t unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                game,
                "",
                &unown_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(unown_entry.forms.length, 26);

    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_make(
                    &unown,
                    "Unown",
                    game,
                    unown_entry.forms.strings[i],
                    5
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_form(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        // Make sure IVs are properly set
        int IV_attack = 0;
        int IV_defense = 0;
        int IV_speed = 0;
        int IV_special = 0;

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Attack",
                    &IV_attack
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Defense",
                    &IV_defense
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Speed",
                    &IV_speed
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Special",
                    &IV_special
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_calculations_gen2_unown_form(
                    IV_attack,
                    IV_defense,
                    IV_speed,
                    IV_special,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        error = pkmn_pokemon_free(&unown);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NULL(unown);
    }

    // Make sure setting IVs properly changes the form.
    error = pkmn_pokemon_make(
                &unown,
                "Unown",
                game,
                "A",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Attack",
                10
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Defense",
                9
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Speed",
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_IV(
                unown,
                "Special",
                14
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_form(
                unown,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "G");

    // Make sure setting the form properly changes the IVs.
    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_set_form(
                    unown,
                    unown_entry.forms.strings[i]
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_form(
                    unown,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        // Make sure IVs are properly set
        int IV_attack = 0;
        int IV_defense = 0;
        int IV_speed = 0;
        int IV_special = 0;

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Attack",
                    &IV_attack
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Defense",
                    &IV_defense
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Speed",
                    &IV_speed
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_IV(
                    unown,
                    "Special",
                    &IV_special
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_calculations_gen2_unown_form(
                    IV_attack,
                    IV_defense,
                    IV_speed,
                    IV_special,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);
    }

    error = pkmn_pokemon_free(&unown);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(unown);

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

typedef pkmn_error_t (*pokemon_stat_fcn_t)(pkmn_pokemon_handle_t, pkmn_string_list_t*);

static void gen2_pokemon_check_stat_map(
    pkmn_pokemon_handle_t pokemon,
    bool is_stats,
    pokemon_stat_fcn_t stat_fcn
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_string_list_t stat_names;

    error = stat_fcn(
                pokemon,
                &stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(stat_names.length, (is_stats ? 6 : 5));
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[0], "Attack");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[1], "Defense");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[2], "HP");
    if(is_stats) {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special Attack");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Special Defense");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[5], "Speed");
    } else {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Speed");
    }

    error = pkmn_string_list_free(&stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

void pkmntest_gen2_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_database_pokemon_entry_t database_entry;
    pkmn_string_list_t string_list = {
        .strings = NULL,
        .length = 0
    };
    bool bool_result = false;

    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &database_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(database_entry.name, species);
    TEST_ASSERT_EQUAL_STRING(database_entry.game, game);
    TEST_ASSERT_EQUAL_STRING(database_entry.form, "Standard");

    /*
     * Check starting values, and confirm that we can't query values
     * that didn't exist in Generation II.
     */
    error = pkmn_pokemon_get_species(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, species);

    error = pkmn_pokemon_get_form(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Standard");

    error = pkmn_pokemon_get_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    char expected_nickname[11] = {0};
    for(size_t i = 0; i < strlen(species); ++i) {
        expected_nickname[i] = toupper(species[i]);
    }
    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, expected_nickname);

    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, LIBPKMN_OT_NAME);

    uint16_t trainer_id_part = 0;
    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, (LIBPKMN_OT_ID & 0xFFFF));

    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    uint32_t trainer_id = 0;
    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, (LIBPKMN_OT_ID & 0xFFFF));

    pkmn_gender_t trainer_gender = PKMN_GENDERLESS;
    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_MALE);

    int friendship = 0;
    error = pkmn_pokemon_get_friendship(
                pokemon,
                &friendship
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(friendship, database_entry.base_friendship);

    error = pkmn_pokemon_get_ability(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_get_ball(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    int level = 0;
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level, 30);

    int level_met = 0;
    error = pkmn_pokemon_get_level_met(
                pokemon,
                &level_met
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level_met, level);

    error = pkmn_pokemon_get_location_met(
                pokemon,
                true,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_get_location_met(
                pokemon,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Special");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    uint32_t personality = 0;
    error = pkmn_pokemon_get_personality(
                pokemon,
                &personality
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    int experience_at_30 = 0;
    error = pkmn_database_pokemon_entry_experience_at_level(
                &database_entry,
                30,
                &experience_at_30
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int experience = 0;
    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(experience, experience_at_30);

    error = pkmn_pokemon_get_marking_names(
                pokemon,
                &string_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    TEST_ASSERT_NULL(string_list.strings);

    error = pkmn_pokemon_has_marking(
                pokemon,
                "Circle",
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_get_ribbon_names(
                pokemon,
                &string_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    TEST_ASSERT_NULL(string_list.strings);

    error = pkmn_pokemon_has_ribbon(
                pokemon,
                "Cool",
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_get_contest_stat_names(
                pokemon,
                &string_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    TEST_ASSERT_NULL(string_list.strings);

    int contest_stat = 0;
    error = pkmn_pokemon_get_contest_stat(
                pokemon,
                "Cool",
                &contest_stat
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    pkmn_move_slots_t move_slots = {
        .move_slots = NULL,
        .length = 0
    };
    error = pkmn_pokemon_get_moves(
                pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(move_slots.length, 4);
    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_STRING(move_slots.move_slots[i].move, "None");
        TEST_ASSERT_EQUAL(move_slots.move_slots[i].pp, 0);
    }
    error = pkmn_move_slots_free(&move_slots);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(move_slots.move_slots);
    TEST_ASSERT_EQUAL(move_slots.length, 0);

    gen2_pokemon_check_stat_map(
        pokemon,
        false,
        pkmn_pokemon_get_EV_names
    );
    gen2_pokemon_check_stat_map(
        pokemon,
        false,
        pkmn_pokemon_get_IV_names
    );
    gen2_pokemon_check_stat_map(
        pokemon,
        true,
        pkmn_pokemon_get_stat_names
    );

    error = pkmn_pokemon_get_icon_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(file_exists(strbuffer));

    error = pkmn_pokemon_get_sprite_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    /*
     * Make sure the getters and setters agree. Also make sure it fails when
     * expected.
     */
    error = pkmn_pokemon_set_nickname(
                pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_nickname(
                pokemon,
                "Too long nickname"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_pokemon_set_nickname(
                pokemon,
                "foobarbaz"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "foobarbaz");

    // Gender affects IVs, so make sure the abstraction reflects that.
    int IV_attack = 0;
    int IV_defense = 0;
    int IV_speed = 0;
    int IV_special = 0;
    pkmn_gender_t gender = PKMN_MALE;

    error = pkmn_pokemon_set_gender(
                pokemon,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV_attack
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_attack, 15);

    error = pkmn_pokemon_set_gender(
                pokemon,
                PKMN_FEMALE
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV_attack
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(IV_attack < 15);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_gender(
                pokemon,
                &gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(gender, PKMN_FEMALE);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                15
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_gender(
                pokemon,
                &gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(gender, PKMN_MALE);

    // Shininess affects IVs, so make sure the abstraction reflects that. Also check filepaths.
    error = pkmn_pokemon_set_shininess(
                pokemon,
                false
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_FALSE(bool_result);

    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV_attack
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_attack, 13);

    error = pkmn_pokemon_get_sprite_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(file_exists(strbuffer));

    // This will fail if "shiny" is anywhere in the filepath.
    TEST_ASSERT_NULL(strstr(strbuffer, "shiny"));

    error = pkmn_pokemon_set_shininess(
                pokemon,
                true
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(bool_result);

    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV_attack
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_attack, 15);

    error = pkmn_pokemon_get_IV(
                pokemon,
                "Defense",
                &IV_defense
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_defense, 10);

    error = pkmn_pokemon_get_IV(
                pokemon,
                "Speed",
                &IV_speed
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_speed, 10);

    error = pkmn_pokemon_get_IV(
                pokemon,
                "Special",
                &IV_special
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV_special, 10);

    error = pkmn_pokemon_get_sprite_filepath(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(file_exists(strbuffer));
    TEST_ASSERT_NOT_NULL(strstr(strbuffer, "shiny"));

    // Invalid item.
    error = pkmn_pokemon_set_held_item(
                pokemon,
                "Not an item"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    // Item not in this game.
    error = pkmn_pokemon_set_held_item(
                pokemon,
                "Razz Berry"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    // Valid but unholdable item.
    error = pkmn_pokemon_set_held_item(
                pokemon,
                "Bicycle"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    error = pkmn_pokemon_set_held_item(
                pokemon,
                "Berry"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Berry");

    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                "Too long trainer name"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                "foobar"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "foobar");

    error = pkmn_pokemon_set_trainer_id(
                pokemon,
                10001
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 10001);

    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, 10001);

    error = pkmn_pokemon_set_ability(
                pokemon,
                ""
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_ball(
                pokemon,
                "Great Ball"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_location_met(
                pokemon,
                "Pallet Town",
                true
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_location_met(
                pokemon,
                "Pallet Town",
                false
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_location_met(
                pokemon,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Pallet Town");

    error = pkmn_pokemon_set_trainer_gender(
                pokemon,
                PKMN_FEMALE
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_FEMALE);

    error = pkmn_pokemon_set_level_met(
                pokemon,
                3
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_level_met(
                pokemon,
                &level_met
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level_met, 3);

    error = pkmn_pokemon_set_friendship(
                pokemon,
                123
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_friendship(
                pokemon,
                &friendship
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(friendship, 123);

    error = pkmn_pokemon_set_original_game(
                pokemon,
                "Gold"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_marking(
                pokemon,
                "Circle",
                true
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_ribbon(
                pokemon,
                "Cool",
                false
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

    error = pkmn_pokemon_set_experience(
                pokemon,
                12345
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(experience, 12345);

    error = pkmn_pokemon_set_level(
                pokemon,
                45
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_level(
                pokemon,
                &level
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level, 45);

    error = pkmn_pokemon_set_move(
                pokemon,
                "Ember",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_move(
                pokemon,
                "Bounce",
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_pokemon_get_moves(
                pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(move_slots.length, 4);
    TEST_ASSERT_EQUAL_STRING(move_slots.move_slots[0].move, "None");
    error = pkmn_move_slots_free(&move_slots);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    static const char* moves[] = {
        "Ember", "Flamethrower", "Slash", "Fire Blast"
    };

    for(int i = 0; i < 4; ++i) {
        error = pkmn_pokemon_set_move(
                    pokemon,
                    moves[i],
                    i
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_get_moves(
                pokemon,
                &move_slots
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(move_slots.length, 4);

    pkmn_database_move_entry_t move_entry;
    for(size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_STRING(move_slots.move_slots[i].move, moves[i]);

        error = pkmn_database_get_move_entry(
                    move_slots.move_slots[i].move,
                    game,
                    &move_entry
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(move_slots.move_slots[i].pp, move_entry.pp[0]);

        error = pkmn_database_move_entry_free(&move_entry);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_set_EV(
                pokemon,
                "Not a stat",
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_EV(
                pokemon,
                "Attack",
                65536
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    int EV = 0;
    error = pkmn_pokemon_set_EV(
                pokemon,
                "Attack",
                12345
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_EV(
                pokemon,
                "Attack",
                &EV
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(EV, 12345);

    int IV = 0;
    error = pkmn_pokemon_set_IV(
                pokemon,
                "Not a stat",
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                16
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                12
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV, 12);
}
