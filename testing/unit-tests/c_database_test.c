/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#include <string.h>

#define BUFFER_SIZE 512
static char strbuffer[BUFFER_SIZE];
static pkmn_error_t error;
static size_t actual_strlen = 0;
static int int_result = 0;
static float float_result = 0.0f;
static bool bool_result = false;

static void pokemon_test() {
    pkmn_string_pair_t string_pair_result;
    pkmn_string_list_t string_list_result = NULL;
    pkmn_levelup_moves_t levelup_moves_result = NULL;
    size_t list_length = 0;

    // Make sure trying to query an invalid Pokémon results in an error
    error = pkmn_database_pokemon_types(
                "Not a Pokémon", "Black 2", "Sunny",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_database_pokemon_types(
                "Castform", "Not a game", "Sunny",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);
    error = pkmn_database_pokemon_types(
                "Castform", "Black 2", "Not a form",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

    // Species
    error = pkmn_database_pokemon_species(
                "Stunfisk", "Black 2",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer)+1);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Trap");

    // Pokédex Entry
    error = pkmn_database_pokemon_pokedex_entry(
                "Stunfisk", "Black 2",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer)+1);
    TEST_ASSERT(actual_strlen > 0);

    // Height
    error = pkmn_database_pokemon_height(
                "Stunfisk", "Black 2", "",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.7f);

    // Weight
    error = pkmn_database_pokemon_weight(
                "Stunfisk", "Black 2", "",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 11.0f);

    // Chance Male
    error = pkmn_database_pokemon_chance_male(
                "Stunfisk", "Black 2",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.5f);

    // Chance Female
    error = pkmn_database_pokemon_chance_female(
                "Stunfisk", "Black 2",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.5f);

    // Has Gender Differences
    error = pkmn_database_pokemon_has_gender_differences(
                "Stunfisk", "Black 2",
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(!bool_result);

    // Base Happiness
    error = pkmn_database_pokemon_base_happiness(
                "Stunfisk", "Black 2",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 70);

    // Types
    error = pkmn_database_pokemon_types(
                "Stunfisk", "Black 2", "",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(string_pair_result.first, "Ground");
    TEST_ASSERT_EQUAL_STRING(string_pair_result.second, "Electric");
    pkmn_string_pair_free(&string_pair_result);
    TEST_ASSERT_NULL(string_pair_result.first);
    TEST_ASSERT_NULL(string_pair_result.second);

    // Abilities
    error = pkmn_database_pokemon_abilities(
                "Stunfisk", "Black 2", "",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(string_pair_result.first, "Static");
    TEST_ASSERT_EQUAL_STRING(string_pair_result.second, "Limber");
    pkmn_string_pair_free(&string_pair_result);
    TEST_ASSERT_NULL(string_pair_result.first);
    TEST_ASSERT_NULL(string_pair_result.second);

    // Hidden Ability
    error = pkmn_database_pokemon_hidden_ability(
                "Stunfisk", "Black 2",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer)+1);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Sand Veil");

    // Types
    error = pkmn_database_pokemon_egg_groups(
                "Stunfisk", "Black 2", "",
                &string_pair_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(string_pair_result.first, "Water 1");
    TEST_ASSERT_EQUAL_STRING(string_pair_result.second, "Amorphous");
    pkmn_string_pair_free(&string_pair_result);
    TEST_ASSERT_NULL(string_pair_result.first);
    TEST_ASSERT_NULL(string_pair_result.second);

    // Base Stat
    error = pkmn_database_pokemon_base_stat(
                "Stunfisk", "Black 2", "", "Defense",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 84);

    // EV Yield
    error = pkmn_database_pokemon_EV_yield(
                "Stunfisk", "Black 2", "Defense",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 0);

    // Experience Yield
    error = pkmn_database_pokemon_experience_yield(
                "Stunfisk", "Black 2",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 165);

    // Experience At Level
    error = pkmn_database_pokemon_experience_at_level(
                "Stunfisk", "Black 2", 50,
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 125000);

    // Level At Experience
    error = pkmn_database_pokemon_level_at_experience(
                "Stunfisk", "Black 2", 200000,
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 58);

    // Levelup Moves
    error = pkmn_database_pokemon_levelup_moves(
                "Stunfisk", "Black 2", "",
                &levelup_moves_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(levelup_moves_result);
    TEST_ASSERT(list_length > 0);
    pkmn_levelup_moves_free(&levelup_moves_result, list_length);
    TEST_ASSERT_NULL(levelup_moves_result);

    // TM/HM Moves
    error = pkmn_database_pokemon_tm_hm_moves(
                "Stunfisk", "Black 2", "",
                &string_list_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(string_list_result);
    TEST_ASSERT(list_length > 0);
    pkmn_string_list_free(&string_list_result, list_length);
    TEST_ASSERT_NULL(string_list_result);

    // Egg Moves
    error = pkmn_database_pokemon_egg_moves(
                "Stunfisk", "Black 2", "",
                &string_list_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(string_list_result);
    TEST_ASSERT(list_length > 0);
    pkmn_string_list_free(&string_list_result, list_length);
    TEST_ASSERT_NULL(string_list_result);

    // Tutor Moves
    error = pkmn_database_pokemon_tutor_moves(
                "Stunfisk", "Black 2", "",
                &string_list_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(string_list_result);
    TEST_ASSERT(list_length > 0);
    pkmn_string_list_free(&string_list_result, list_length);
    TEST_ASSERT_NULL(string_list_result);

    // Forms
    error = pkmn_database_pokemon_forms(
                "Stunfisk", "Black 2",
                &string_list_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(string_list_result);
    TEST_ASSERT_EQUAL(list_length, 1);
    pkmn_string_list_free(&string_list_result, list_length);
    TEST_ASSERT_NULL(string_list_result);

    // Evolutions
    error = pkmn_database_pokemon_evolutions(
                "Stunfisk", "Black 2",
                &string_list_result, &list_length
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(string_list_result);
    TEST_ASSERT_EQUAL(list_length, 0);
    pkmn_string_list_free(&string_list_result, list_length);
    TEST_ASSERT_NULL(string_list_result);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(pokemon_test)
)
