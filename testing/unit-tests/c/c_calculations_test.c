/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#include <math.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 512
static char strbuffer[BUFFER_SIZE];
static pkmn_error_t error;
static int int_result = 0;
static bool bool_result = false;
static pkmn_gender_t pkmn_gender_t_result = PKMN_MALE;
static pkmn_hidden_power_t pkmn_hidden_power_t_result = {
                               .type = NULL,
                               .base_power = 0
                           };
static pkmn_spinda_spots_t pkmn_spinda_spots_t_result;

static void gen2_unown_form_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_unown_form(
                -1, 0, 0, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_unown_form(
                16, 0, 0, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_unown_form(
                0, -1, 0, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_unown_form(
                0, 16, 0, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_unown_form(
                0, 0, -1, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_unown_form(
                -1, 0, 16, 0,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_unown_form(
                0, 0, 0, -1,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_unown_form(
                0, 0, 0, 16,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
     */
    error = pkmn_calculations_gen2_unown_form(
                10, 9, 1, 14,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "G");

    error = pkmn_calculations_gen2_unown_form(
                5, 15, 10, 5,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "S");
}

static void gen3_unown_form_test() {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
     */
    error = pkmn_calculations_gen3_unown_form(
                0x4C07DE71,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "B");
}

static void wurmple_becomes_silcoon_test() {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
     */
    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x5CF4091C, true,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x091C5CF4, true,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(!bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x091C5CF4, false,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x5CF4091C, false,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(!bool_result);
}

static void gen2_gender_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                "Not a species", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_calculations_gen2_pokemon_gender(
                "Bulbasaur", -1,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_pokemon_gender(
                "Bulbasaur", 16,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                "Nidorino", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                "Nidorino", 15,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);

    /*
     * 25% male, 75% female
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                "Vulpix", 11,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                "Vulpix", 12,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);

    /*
     * All female
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                "Nidorina", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                "Nidorina", 15,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);

    /*
     * Genderless
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                "Magnemite", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDERLESS, pkmn_gender_t_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                "Magnemite", 15,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDERLESS, pkmn_gender_t_result);
}

static void modern_gender_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_modern_pokemon_gender(
                "Not a species", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    error = pkmn_calculations_modern_pokemon_gender(
                "Nidorino", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);
    error = pkmn_calculations_modern_pokemon_gender(
                "Nidorino", 0xFFFFFFFF,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);

    /*
     * 25% male, 75% female
     */
    error = pkmn_calculations_modern_pokemon_gender(
                "Vulpix", 190,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);
    error = pkmn_calculations_modern_pokemon_gender(
                "Vulpix", 191,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_MALE, pkmn_gender_t_result);

    /*
     * All female
     */
    error = pkmn_calculations_modern_pokemon_gender(
                "Nidorina", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);
    error = pkmn_calculations_modern_pokemon_gender(
                "Nidorina", 0xFFFFFFFF,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_FEMALE, pkmn_gender_t_result);

    /*
     * Genderless
     */
    error = pkmn_calculations_modern_pokemon_gender(
                "Magnemite", 0,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDERLESS, pkmn_gender_t_result);
    error = pkmn_calculations_modern_pokemon_gender(
                "Magnemite", 0xFFFFFFFF,
                &pkmn_gender_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_GENDERLESS, pkmn_gender_t_result);
}

static void gen2_hidden_power_test() {
    /*
     * Make sure expected errors are returned and no new memory is allocated.
     */
    error = pkmn_calculations_gen2_hidden_power(
                -1, 0, 0, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_gen2_hidden_power(
                16, 0, 0, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_gen2_hidden_power(
                0, -1, 0, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 16, 0, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_gen2_hidden_power(
                0, 0, -1, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 16, 0,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 0, -1,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 0, 16,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
     */
    error = pkmn_calculations_gen2_hidden_power(
                15, 15, 15, 14,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_NOT_NULL(pkmn_hidden_power_t_result.type);
    TEST_ASSERT_EQUAL_STRING(pkmn_hidden_power_t_result.type, "Dark");
    TEST_ASSERT_EQUAL(69, pkmn_hidden_power_t_result.base_power);
    pkmn_hidden_power_free(&pkmn_hidden_power_t_result);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
}

static void modern_hidden_power_test() {
    /*
     * Make sure expected exceptions are thrown and no new memory is allocated.
     */
    error = pkmn_calculations_modern_hidden_power(
        -1, 0, 0, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        32, 0, 0, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_modern_hidden_power(
        0, -1, 0, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        0, 32, 0, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, -1, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 32, 0, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, -1, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 32, 0, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, -1, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, 32, 0,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, 0, -1,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);
    error = pkmn_calculations_modern_hidden_power(
        0, -1, 0, 0, 0, 32,
        &pkmn_hidden_power_t_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    TEST_ASSERT_NULL(pkmn_hidden_power_t_result.type);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
     */
    error = pkmn_calculations_modern_hidden_power(
                30, 31, 31, 31, 30, 31,
                &pkmn_hidden_power_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pkmn_hidden_power_t_result.type);
    TEST_ASSERT_EQUAL_STRING(pkmn_hidden_power_t_result.type, "Grass");
    TEST_ASSERT_EQUAL(70, pkmn_hidden_power_t_result.base_power);
}

static void _nature_test() {
    static const char* natures[] = {
        "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
        "Bold", "Docile", "Relaxed", "Impish", "Lax",
        "Timid", "Hasty", "Serious", "Jolly", "Naive",
        "Modest", "Mild", "Quiet", "Bashful", "Rash",
        "Calm", "Gentle", "Sassy", "Careful", "Quirky"
    };

    srand((unsigned int)time(0));
    for(uint32_t i = 0; i < 25; ++i) {
        error = pkmn_calculations_nature(
                    (uint32_t)(((rand() % 50000) * 1000) + i),
                    strbuffer,
                    BUFFER_SIZE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(natures[i], strbuffer);
    }
}

static void gen2_shiny_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_shiny(
                -1, 0, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_shiny(
                16, 0, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_shiny(
                0, -1, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_shiny(
                0, 16, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_shiny(
                0, 0, -1, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_shiny(
                0, 0, 16, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_calculations_gen2_shiny(
                0, 0, 0, -1,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gen2_shiny(
                0, 0, 0, 16,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
     */
    error = pkmn_calculations_gen2_shiny(
                7, 10, 10, 10,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(bool_result);
    error = pkmn_calculations_gen2_shiny(
                6, 15, 7, 15,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT(!bool_result);
}

static void modern_shiny_test() {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
     *         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
     */
    error = pkmn_calculations_modern_shiny(
                2814471828,
                2545049318,
                &bool_result
            );
    TEST_ASSERT(bool_result);
    error = pkmn_calculations_modern_shiny(
                0xB58F0B2A,
                398174488,
                &bool_result
            );
    TEST_ASSERT(bool_result);
}

static void pokemon_size_test() {
    float size = 0.0f;
    pkmn_database_pokemon_entry_t entry;
    srand((unsigned int)time(NULL));

    // Test input validation.
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, -1, 0, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 32, 0, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, -1, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 32, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, -1, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 32, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, -1, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, 32, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, -1, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 32, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, -1, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, 32, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * There are no known good calculations, so just check for reasonable values
     * for each relevant Pokémon.
     */
    static const char* pokemon_with_size_checks[] = {
        "Barboach", "Shroomish", "Seedot", "Lotad", "Magikarp", "Heracross"
    };
    static const char* games[] = {
        "Ruby", "Ruby", "Emerald", "Emerald", "FireRed", "FireRed"
    };
    for(int i = 0; i < 6; ++i) {
        error = pkmn_database_get_pokemon_entry(
                    pokemon_with_size_checks[i],
                    games[i],
                    "",
                    &entry
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(int j = 0; j < 10; ++j) {
            error = pkmn_calculations_pokemon_size(
                        entry.name,
                        (uint32_t)rand(),
                        (rand() % 32),
                        (rand() % 32),
                        (rand() % 32),
                        (rand() % 32),
                        (rand() % 32),
                        (rand() % 32),
                        &size
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            TEST_ASSERT(fabs(size-entry.height) < entry.height);
        }

        error = pkmn_database_pokemon_entry_free(&entry);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
}

static void spinda_spot_test() {
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: https://github.com/magical/spinda
     */
    static const pkmn_spinda_spots_t SPOTS1 = {
                     .left_ear = {
                         7,
                         5
                     },
                     .right_ear = {
                         10,
                         0
                     },
                     .left_face = {
                         1,
                         4
                     },
                     .right_face = {
                         2,
                         15
                     },
                };

    error = pkmn_calculations_spinda_spot_offset(
                4064348759,
                &pkmn_spinda_spots_t_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(SPOTS1.left_ear.x, pkmn_spinda_spots_t_result.left_ear.x);
    TEST_ASSERT_EQUAL(SPOTS1.left_ear.y, pkmn_spinda_spots_t_result.left_ear.y);
    TEST_ASSERT_EQUAL(SPOTS1.right_ear.x, pkmn_spinda_spots_t_result.right_ear.x);
    TEST_ASSERT_EQUAL(SPOTS1.right_ear.y, pkmn_spinda_spots_t_result.right_ear.y);
    TEST_ASSERT_EQUAL(SPOTS1.left_face.x, pkmn_spinda_spots_t_result.left_face.x);
    TEST_ASSERT_EQUAL(SPOTS1.left_face.y, pkmn_spinda_spots_t_result.left_face.y);
    TEST_ASSERT_EQUAL(SPOTS1.right_face.x, pkmn_spinda_spots_t_result.right_face.x);
    TEST_ASSERT_EQUAL(SPOTS1.right_face.y, pkmn_spinda_spots_t_result.right_face.y);
}

static void gb_stat_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_ATTACK,
                1, 1, 123456, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_ATTACK,
                1, 1, 1, 12345,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
                
    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
     */
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_HP,
                81, 35, 22850, 7,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 189);
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_ATTACK,
                81, 55, 23140, 8,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 137);
}

static void modern_stat_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_modern_stat(
                (pkmn_stat_t)(100), 1, 1.0f, 1, 1, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_SPECIAL, 1, 1.0f, 1, 1, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK, 1, 0.666f, 1, 1, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_DOMAIN_ERROR, error);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK, 1, 1.0f, 1, 12345, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK, 1, 1, 1, 1, 12345,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * Test with known good inputs.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
     */
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_HP,
                78, 1.0f, 108, 74, 24,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 289);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK,
                78, 1.1f, 130, 195, 12,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 280);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(gen2_unown_form_test)
    PKMN_C_TEST(gen3_unown_form_test)
    PKMN_C_TEST(wurmple_becomes_silcoon_test)
    PKMN_C_TEST(gen2_gender_test)
    PKMN_C_TEST(modern_gender_test)
    PKMN_C_TEST(gen2_hidden_power_test)
    PKMN_C_TEST(modern_hidden_power_test)
    PKMN_C_TEST(_nature_test)
    PKMN_C_TEST(gen2_shiny_test)
    PKMN_C_TEST(modern_shiny_test)
    PKMN_C_TEST(pokemon_size_test)
    PKMN_C_TEST(spinda_spot_test)
    PKMN_C_TEST(gb_stat_test)
    PKMN_C_TEST(modern_stat_test)
)
