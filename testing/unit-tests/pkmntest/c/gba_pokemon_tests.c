/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_tests_common.h>

#include <pkmntest-c/gba_pokemon_tests.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static const char* markings[] = {
    "Circle", "Heart", "Square", "Triangle"
};

static const char* contest_types[] = {
    "Beauty", "Cool", "Cute", "Feel", "Smart", "Tough"
};

static const char* contest_levels[] = {
    "", " Super", " Hyper", " Master"
};

static const char* ribbons[] = {
    "Champion", "Winning", "Victory", "Artist",
    "Effort", "Marine", "Land", "Sky",
    "Country", "National", "Earth", "World",
};

void pkmntest_gba_unown_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t unown = NULL;
    uint32_t personality = 0;

    pkmn_database_pokemon_entry_t unown_entry;
    error = pkmn_database_get_pokemon_entry(
                "Unown",
                game,
                "",
                &unown_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(unown_entry.forms.length, 28);

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

        // Make sure personality is properly set.
        error = pkmn_pokemon_get_personality(
                    unown,
                    &personality
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_calculations_gen3_unown_form(
                    personality,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, unown_entry.forms.strings[i]);

        error = pkmn_pokemon_free(&unown);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NULL(unown);
    }

    // Make sure setting the personality properly sets the form.
    error = pkmn_pokemon_make(
                &unown,
                "Unown",
                game,
                "A",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_set_personality(
                unown,
                0x4C07DE71
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    error = pkmn_pokemon_get_form(
                unown,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "B");

    for(size_t i = 0; i < unown_entry.forms.length; ++i) {
        error = pkmn_pokemon_set_form(
                    unown,
                    unown_entry.forms.strings[i]
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_personality(
                    unown,
                    &personality
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_calculations_gen3_unown_form(
                    personality,
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

static void check_stat_names(
    pkmn_pokemon_handle_t pokemon,
    pokemon_stat_fcn_t stat_fcn
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_string_list_t stat_names;

    error = stat_fcn(
                pokemon,
                &stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(stat_names.length, 6);
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[0], "Attack");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[1], "Defense");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[2], "HP");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special Attack");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Special Defense");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[5], "Speed");

    error = pkmn_string_list_free(&stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

/*
 * Due to the more restrictive API, doing what the other tests do and
 * checking to make sure other values aren't affected would be time-consuming,
 * so just check each value.
 */

static void test_markings(
    pkmn_pokemon_handle_t pokemon
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_string_list_t marking_names;
    error = pkmn_pokemon_get_marking_names(
                pokemon,
                &marking_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(marking_names.length, 4);

    error = pkmn_string_list_free(&marking_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    bool has_marking = false;
    for(int i = 0; i < 4; ++i) {
        error = pkmn_pokemon_has_marking(
                    pokemon,
                    markings[i],
                    &has_marking
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_FALSE(has_marking);

        error = pkmn_pokemon_set_marking(
                    pokemon,
                    markings[i],
                    true
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_has_marking(
                    pokemon,
                    markings[i],
                    &has_marking
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_TRUE(has_marking);
    }
}

static void test_ribbons(
    pkmn_pokemon_handle_t pokemon
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    bool has_ribbon = false;
    for(int i = 0; i < 6; ++i) {
        // Skip Feel, no ribbons for that
        if(strcmp(contest_types[i], "Feel")) {
            for(int j = 0; j < 4; ++j) {
                snprintf(strbuffer, sizeof(strbuffer), "%s%s", contest_types[i], contest_levels[j]);

                error = pkmn_pokemon_has_ribbon(
                            pokemon,
                            strbuffer,
                            &has_ribbon
                        );
                TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
                TEST_ASSERT_FALSE(has_ribbon);

                error = pkmn_pokemon_set_ribbon(
                            pokemon,
                            strbuffer,
                            true
                        );
                TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

                error = pkmn_pokemon_has_ribbon(
                            pokemon,
                            strbuffer,
                            &has_ribbon
                        );
                TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
                TEST_ASSERT_TRUE(has_ribbon);
            }
        }
    }

    for(int i = 0; i < 12; ++i) {
        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    ribbons[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_FALSE(has_ribbon);

        error = pkmn_pokemon_set_ribbon(
            pokemon,
            ribbons[i],
            true
        );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_has_ribbon(
                    pokemon,
                    ribbons[i],
                    &has_ribbon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_TRUE(has_ribbon);
    }
}

static void test_contest_stats(
    pkmn_pokemon_handle_t pokemon
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_string_list_t contest_stat_names;
    error = pkmn_pokemon_get_contest_stat_names(
                pokemon,
                &contest_stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(contest_stat_names.length, 6);

    error = pkmn_string_list_free(&contest_stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int value = 0;
    for(int i = 0; i < 6; ++i) {
        error = pkmn_pokemon_set_contest_stat(
                    pokemon,
                    contest_types[i],
                    100
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_get_contest_stat(
                    pokemon,
                    contest_types[i],
                    &value
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(value, 100);
    }

    // Confirm range errors.
    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Cute",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Cute",
                256
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    // Make sure Sheen doesn't work in Generation III.
    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Sheen",
                100
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_get_contest_stat(
                pokemon,
                "Sheen",
                &value
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
}

void pkmntest_gba_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_database_pokemon_entry_t pokemon_entry;

    /*
     * Check known starting values, and confirm that we can't query values
     * that didn't exist in Generation III.
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

    uint32_t trainer_id = 0;
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, ((LIBPKMN_OT_ID & 0xFFFF0000) >> 16));

    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, LIBPKMN_OT_ID);

    pkmn_gender_t trainer_gender = PKMN_GENDERLESS;
    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_MALE);

    error = pkmn_pokemon_get_database_entry(
                pokemon,
                &pokemon_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int friendship = 0;
    error = pkmn_pokemon_get_friendship(
                pokemon,
                &friendship
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(friendship, pokemon_entry.base_friendship);

    error = pkmn_pokemon_get_ability(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Blaze");

    error = pkmn_pokemon_get_ball(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Premier Ball");

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
    TEST_ASSERT_EQUAL(level, level_met);

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
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Fateful encounter");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    int experience_from_pokemon = 0;
    int experience_at_30 = 0;

    error = pkmn_pokemon_get_experience(
                pokemon,
                &experience_from_pokemon
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_database_pokemon_entry_experience_at_level(
                &pokemon_entry,
                level,
                &experience_at_30
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(experience_from_pokemon, experience_at_30);

    pkmn_move_slots_t moves;
    error = pkmn_pokemon_get_moves(
                pokemon,
                &moves
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(moves.length, 4);

    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_STRING(moves.move_slots[i].move, "None");
        TEST_ASSERT_EQUAL(moves.move_slots[i].pp, 0);
    }

    error = pkmn_move_slots_free(&moves);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    check_stat_names(
        pokemon,
        pkmn_pokemon_get_EV_names
    );
    check_stat_names(
        pokemon,
        pkmn_pokemon_get_IV_names
    );
    check_stat_names(
        pokemon,
        pkmn_pokemon_get_stat_names
    );

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

    bool is_shiny = false;

    error = pkmn_pokemon_set_shininess(
                pokemon,
                false
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_FALSE(is_shiny);

    error = pkmn_pokemon_set_shininess(
                pokemon,
                true
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_is_shiny(
                pokemon,
                &is_shiny
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_TRUE(is_shiny);

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
                "Berry"
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
                "Mach Bike"
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
                "Razz Berry"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Razz Berry");

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
                0x1234ABCD
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, 0x1234ABCD);
    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0xABCD);
    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0x1234);

    error = pkmn_pokemon_set_trainer_public_id(
                pokemon,
                0x1A2B
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, 0x12341A2B);
    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0x1A2B);
    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0x1234);

    error = pkmn_pokemon_set_trainer_secret_id(
                pokemon,
                0x3C4D
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id, 0x3C4D1A2B);
    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0x1A2B);
    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_id_part, 0x3C4D);

    error = pkmn_pokemon_set_trainer_gender(
                pokemon,
                PKMN_MALE
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                &trainer_gender
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(trainer_gender, PKMN_MALE);

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

    error = pkmn_pokemon_set_trainer_gender(
                pokemon,
                PKMN_GENDERLESS
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

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

    error = pkmn_pokemon_set_friendship(
                pokemon,
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_friendship(
                pokemon,
                256
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    error = pkmn_pokemon_set_ability(
                pokemon,
                "Blaze"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_ability(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Blaze");

    error = pkmn_pokemon_set_ability(
                pokemon,
                "None"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_ability(
                pokemon,
                "Torrent" // Invalid
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_ability(
                pokemon,
                "Speed Boost" // Hidden ability
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_pokemon_set_ball(
                pokemon,
                "Great Ball"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_ball(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Great Ball");

    error = pkmn_pokemon_set_ball(
                pokemon,
                "Friend Ball" // Not in Generation III
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    error = pkmn_pokemon_set_level_met(
                pokemon,
                67
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_level_met(
                pokemon,
                &level_met
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(level_met, 67);

    error = pkmn_pokemon_set_level_met(
                pokemon,
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_level_met(
                pokemon,
                101
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    const char* location;
    if(!strcmp(game, "FireRed") || !strcmp(game, "LeafGreen")) {
        location = "Viridian Forest";
    } else {
        location = "Petalburg Woods";
    }
    error = pkmn_pokemon_set_location_met(
                pokemon,
                location,
                true
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    error = pkmn_pokemon_set_location_met(
                pokemon,
                location,
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
    TEST_ASSERT_EQUAL_STRING(strbuffer, location);

    error = pkmn_pokemon_set_original_game(
                pokemon,
                "Ruby"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Ruby");

    error = pkmn_pokemon_set_original_game(
        pokemon,
        "Not a game"
    );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_original_game(
        pokemon,
        "Red" // Impossible
    );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_original_game(
        pokemon,
        "HeartGold" // From a later game
    );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Ruby");

    uint32_t personality = 0;
    error = pkmn_pokemon_set_personality(
                pokemon,
                0x7F3AB3A8
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_personality(
                pokemon,
                &personality
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(personality, 0x7F3AB3A8);

    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Cool",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Cool",
                256
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                "Not a stat",
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    test_markings(pokemon);
    test_ribbons(pokemon);
    test_contest_stats(pokemon);

    error = pkmn_pokemon_set_move(
                pokemon,
                "Pound",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_move(
                pokemon,
                "Pound",
                4
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_move(
                pokemon,
                "Shadow Sky",
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    static const char* move_names[] = {
        "Swallow", "Flamethrower", "Return", "Fire Blast"
    };
    for(int i = 0; i < 4; ++i) {
        error = pkmn_pokemon_set_move(
                    pokemon,
                    move_names[i],
                    i
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_get_moves(
                pokemon,
                &moves
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_database_move_entry_t move_entry;
    for(int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_STRING(moves.move_slots[i].move, move_names[i]);

        error = pkmn_database_get_move_entry(
                    moves.move_slots[i].move,
                    game,
                    &move_entry
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        TEST_ASSERT_EQUAL(moves.move_slots[i].pp, move_entry.pp[0]);

        error = pkmn_database_move_entry_free(&move_entry);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_set_EV(
                pokemon,
                "Attack",
                100
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int EV = 0;
    error = pkmn_pokemon_get_EV(
                pokemon,
                "Attack",
                &EV
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(EV, 100);
    
    error = pkmn_pokemon_set_EV(
                pokemon,
                "Not a stat",
                100
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_EV(
                pokemon,
                "Attack",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_EV(
                pokemon,
                "Attack",
                256
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                10
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int IV = 0;
    error = pkmn_pokemon_get_IV(
                pokemon,
                "Attack",
                &IV
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(IV, 10);

    error = pkmn_pokemon_set_IV(
                pokemon,
                "Not a stat",
                10
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                -1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_set_IV(
                pokemon,
                "Attack",
                32
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
}
