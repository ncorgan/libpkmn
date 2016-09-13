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

static void item_test() {
    // Category
    error = pkmn_database_item_category(
                "Fast Ball", "SoulSilver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Apricorn balls");

    // Pocket
    error = pkmn_database_item_pocket(
                "Fast Ball", "SoulSilver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Poké Balls");

    // Description
    error = pkmn_database_item_description(
                "Fast Ball", "SoulSilver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT(actual_strlen > 0);

    // Cost
    error = pkmn_database_item_cost(
                "Fast Ball", "SoulSilver",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 300);

    // Holdable
    error = pkmn_database_item_holdable(
                "Fast Ball", "SoulSilver",
                &bool_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT(bool_result);

    // Fling Power
    error = pkmn_database_item_fling_power(
                "Fast Ball", "SoulSilver",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 0);

    // Fling Effect
    error = pkmn_database_item_fling_effect(
                "Fast Ball", "SoulSilver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");
}

static void move_test() {
    // Type
    error = pkmn_database_move_type(
                "Octazooka", "Silver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Water");

    // Description
    error = pkmn_database_move_description(
                "Octazooka", "Silver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT(actual_strlen > 0);

    // Target
    error = pkmn_database_move_target(
                "Octazooka", "Silver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Selected Pokémon");

    // Damage Class
    error = pkmn_database_move_damage_class(
                "Octazooka", "Silver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Special");

    // Base Power
    error = pkmn_database_move_base_power(
                "Octazooka", "Silver",
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 65);

    // PP
    error = pkmn_database_move_pp(
                "Octazooka", "Silver", 2,
                &int_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(int_result, 14);

    // Accuracy
    error = pkmn_database_move_accuracy(
                "Octazooka", "Silver",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.85f);

    // Effect
    error = pkmn_database_move_effect(
                "Octazooka", "Silver",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT(actual_strlen > 0);
}

static void pokemon_test() {
    pkmn_string_pair_t string_pair;

    // Species
    error = pkmn_database_pokemon_species(
                "Kricketot", "Pearl",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Cricket");

    // Pokédex Entry
    error = pkmn_database_pokemon_pokedex_entry(
                "Kricketot", "Pearl",
                strbuffer, BUFFER_SIZE,
                &actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(actual_strlen, strlen(strbuffer));
    TEST_ASSERT(actual_strlen > 0);

    // Height
    error = pkmn_database_pokemon_height(
                "Kricketot", "Pearl", "",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.3f);

    // Weight
    error = pkmn_database_pokemon_weight(
                "Kricketot", "Pearl", "",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 2.2f);

    // Chance Male
    error = pkmn_database_pokemon_chance_male(
                "Kricketot", "Pearl",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.5f);

    // Chance Female
    error = pkmn_database_pokemon_chance_female(
                "Kricketot", "Pearl",
                &float_result
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_FLOAT(float_result, 0.5f);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(item_test)
    PKMN_C_TEST(move_test)
    PKMN_C_TEST(pokemon_test)
)
