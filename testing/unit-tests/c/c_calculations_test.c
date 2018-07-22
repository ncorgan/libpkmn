/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn-c.h>

#include <math.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 512
static char strbuffer[BUFFER_SIZE];
static enum pkmn_error error;
static int int_result = 0;
static float float_result = 0;
static bool bool_result = false;
static enum pkmn_gender gender_result = PKMN_GENDER_MALE;
static struct pkmn_hidden_power hidden_power_result = { NULL, 0 };
static struct pkmn_spinda_spots spinda_spots_result;

static void brine_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_brine_power(0, 2, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);

    error = pkmn_calculations_brine_power(-2, -1, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);

    error = pkmn_calculations_brine_power(2, 1, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_OUT_OF_RANGE);

    // Test expected behavior.

    const int max_hp = 10;
    const int half_hp = max_hp / 2;

    error = pkmn_calculations_brine_power((half_hp - 1), max_hp, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(130, int_result);

    error = pkmn_calculations_brine_power(half_hp, max_hp, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(130, int_result);

    error = pkmn_calculations_brine_power((half_hp + 1), max_hp, &int_result);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(65, int_result);
}

static void crush_grip_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_crush_grip_power(0, 1, 5, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_crush_grip_power(2, 1, 5, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_crush_grip_power(1, 2, 3, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_crush_grip_power(1, 2, 7, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_crush_grip_power(1, 200, 4, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);

    error = pkmn_calculations_crush_grip_power(200, 200, 4, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(121, int_result);

    for(int generation = 5; generation <= 6; ++generation)
    {
        error = pkmn_calculations_crush_grip_power(
                    1, 200, generation, &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(1, int_result);

        error = pkmn_calculations_crush_grip_power(
                    200, 200, generation, &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(120, int_result);
    }
}

static void echoed_voice_powers_test()
{
    const int expected_results[5] = {40, 80, 120, 160, 200};

    int int_buffer[BUFFER_SIZE] = {0};
    size_t num_powers = 0;

    error = pkmn_calculations_echoed_voice_powers(
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(5ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_results, int_buffer, num_powers);
}

static void electro_ball_power_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_electro_ball_power(0, 100, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_electro_ball_power(100, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct electro_ball_power_test_params
    {
        int attacker_speed;
        int target_speed;
        int expected_power;
    };
    static const struct electro_ball_power_test_params test_cases[] =
    {
        {100, 101, 40},
        {100, 100, 60},
        {100, 51, 60},
        {100, 50, 80},
        {100, 34, 80},
        {100, 33, 120},
        {100, 32, 120},
        {100, 25, 150}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_electro_ball_power(
                    test_cases[test_case_index].attacker_speed,
                    test_cases[test_case_index].target_speed,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].expected_power,
            int_result
        );
    }
}

static void eruption_power_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_eruption_power(0, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_eruption_power(1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_eruption_power(2, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected values.

    error = pkmn_calculations_eruption_power(1, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);

    error = pkmn_calculations_eruption_power(250, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(75, int_result);

    error = pkmn_calculations_eruption_power(500, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(150, int_result);
}
/*
    EXPECT_THROW(
        pkmn::calculations::eruption_power(0, 1);
    , std::out_of_range);
    EXPECT_THROW(
        pkmn::calculations::eruption_power(1, 0);
    , std::out_of_range);
    EXPECT_THROW(
        pkmn::calculations::eruption_power(2, 1);
    , std::out_of_range);

    EXPECT_EQ(
        1,
        pkmn::calculations::eruption_power(1, 500)
    );
    EXPECT_EQ(
        75,
        pkmn::calculations::eruption_power(250, 500)
    );
    EXPECT_EQ(
        150,
        pkmn::calculations::eruption_power(500, 500)
    );
}

TEST(cpp_calculations_test, flail_power_test)
{
 */

static void flail_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_flail_power(0, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_flail_power(1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_flail_power(2, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected behavior.

    struct flail_power_test_params
    {
        int attacker_current_hp;
        int attacker_max_hp;
        int expected_power;
    };
    static const struct flail_power_test_params test_cases[] =
    {
        {1, 500, 200},
        {20, 500, 200},

        {21, 500, 150},
        {52, 500, 150},

        {53, 500, 100},
        {104, 500, 100},

        {105, 500, 80},
        {177, 500, 80},

        {178, 500, 40},
        {343, 500, 40},

        {344, 500, 20},
        {500, 500, 20}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_flail_power(
                    test_cases[test_case_index].attacker_current_hp,
                    test_cases[test_case_index].attacker_max_hp,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(test_cases[test_case_index].expected_power, int_result);
    }
}

static void fling_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_fling_power("Not an item", &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    struct fling_power_test_params
    {
        const char* held_item;
        int expected_power;
    };

    static const struct fling_power_test_params test_cases[] =
    {
        {"Oran Berry", 10},
        {"Health Wing", 20},
        {"Potion", 30},
        {"Icy Rock", 40},
        {"Dubious Disc", 50},
        {"Damp Rock", 60},
        {"Dragon Fang", 70},
        {"Dusk Stone", 80},
        {"Thick Club", 90},
        {"Rare Bone", 100},
        {"Iron Ball", 130}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_fling_power(
                    test_cases[test_case_index].held_item,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(test_cases[test_case_index].expected_power, int_result);
    }
}

static void frustration_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_frustration_power(-1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_frustration_power(256, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected behavior.

    error = pkmn_calculations_frustration_power(0, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(102, int_result);

    error = pkmn_calculations_frustration_power(255, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);
}

static void fury_cutter_powers_test()
{
    int int_buffer[BUFFER_SIZE] = {0};
    size_t num_powers = 0;

    // Test invalid inputs.

    error = pkmn_calculations_fury_cutter_powers(
                1,
                int_buffer,
                BUFFER_SIZE,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_fury_cutter_powers(
                10,
                int_buffer,
                BUFFER_SIZE,
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    static const int gen_2_4_expected_results[5] = {10, 20, 40, 80, 160};
    static const int gen5_expected_results[4] = {20, 40, 80, 160};
    static const int gen6_expected_results[3] = {40, 80, 160};

    for(int generation = 2; generation <= 4; ++generation)
    {
        error = pkmn_calculations_fury_cutter_powers(
                    generation,
                    int_buffer,
                    BUFFER_SIZE,
                    &num_powers
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(5ULL, num_powers);
        TEST_ASSERT_EQUAL_INT_ARRAY(
            gen_2_4_expected_results,
            int_buffer,
            num_powers
        );
    }

    error = pkmn_calculations_fury_cutter_powers(
                5,
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(
        gen5_expected_results,
        int_buffer,
        num_powers
    );

    error = pkmn_calculations_fury_cutter_powers(
                6,
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(
        gen6_expected_results,
        int_buffer,
        num_powers
    );
}

static void grass_knot_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_grass_knot_power(-1.0f, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct grass_knot_power_test_params
    {
        float target_weight;
        int expected_power;
    };

    static const struct grass_knot_power_test_params test_cases[] =
    {
        {5.0f, 20},
        {15.0f, 40},
        {35.0f, 60},
        {75.0f, 80},
        {150.0f, 100},
        {250.0f, 120}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_grass_knot_power(
                    test_cases[test_case_index].target_weight,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].expected_power,
            int_result
        );
    }
}

static void gyro_ball_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_gyro_ball_power(0, 50, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gyro_ball_power(50, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_gyro_ball_power(1, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(150, int_result);

    error = pkmn_calculations_gyro_ball_power(100, 200, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(50, int_result);

    error = pkmn_calculations_gyro_ball_power(500, 1, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);
}

static void heat_crash_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_heat_crash_power(0.0f, 1.0f, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_heat_crash_power(1.0f, 0.0f, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct heat_crash_power_test_params
    {
        float attacker_weight;
        float target_weight;
        int expected_power;
    };

    static const struct heat_crash_power_test_params test_cases[] =
    {
        {200.0f, 200.0f, 40},
        {200.0f, 100.1f, 40},
        {200.0f, 100.0f, 60},
        {200.0f, 66.7f, 60},
        {200.0f, 66.6f, 80},
        {200.0f, 50.1f, 80},
        {200.0f, 50.0f, 100},
        {200.0f, 40.1f, 100},
        {200.0f, 40.0f, 120}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_heat_crash_power(
                    test_cases[test_case_index].attacker_weight,
                    test_cases[test_case_index].target_weight,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].expected_power,
            int_result
        );
    }
}

static void heavy_slam_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_heavy_slam_power(0.0f, 1.0f, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_heavy_slam_power(1.0f, 0.0f, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct heavy_slam_power_test_params
    {
        float attacker_weight;
        float target_weight;
        int expected_power;
    };

    static const struct heavy_slam_power_test_params test_cases[] =
    {
        {200.0f, 200.0f, 40},
        {200.0f, 100.1f, 40},
        {200.0f, 100.0f, 60},
        {200.0f, 66.7f, 60},
        {200.0f, 66.6f, 80},
        {200.0f, 50.1f, 80},
        {200.0f, 50.0f, 100},
        {200.0f, 40.1f, 100},
        {200.0f, 40.0f, 120}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_heavy_slam_power(
                    test_cases[test_case_index].attacker_weight,
                    test_cases[test_case_index].target_weight,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].expected_power,
            int_result
        );
    }
}

static void ice_ball_power_test()
{
    const int expected_results[5] = {30, 60, 120, 240, 480};

    int int_buffer[BUFFER_SIZE] = {0};
    size_t num_powers = 0;

    error = pkmn_calculations_ice_ball_powers(
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(5ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_results, int_buffer, num_powers);
}

static void low_kick_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_low_kick_power(-1.0f, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_low_kick_power(1.0f, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_low_kick_power(1.0f, 10, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct low_kick_power_test_params
    {
        float target_weight;
        int expected_power;
    };
    static const struct low_kick_power_test_params test_cases[] =
    {
        {0.1f, 20},
        {9.9f, 20},
        {10.0f, 40},
        {24.9f, 40},
        {25.0f, 60},
        {49.9f, 60},
        {50.0f, 80},
        {99.9f, 80},
        {100.0f, 100},
        {199.9f, 100},
        {200.0f, 120},
        {1000.0f, 120}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        for(int generation = 1; generation <= 6; ++generation)
        {
            if(generation <= 2)
            {
                // Generation I-II only have one set power, regardless of
                // weight.
                error = pkmn_calculations_low_kick_power(
                            test_cases[test_case_index].target_weight,
                            generation,
                            &int_result
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL(50, int_result);
            }
            else
            {
                error = pkmn_calculations_low_kick_power(
                            test_cases[test_case_index].target_weight,
                            generation,
                            &int_result
                        );
                PKMN_TEST_ASSERT_SUCCESS(error);
                TEST_ASSERT_EQUAL(
                    test_cases[test_case_index].expected_power,
                    int_result
                );
            }
        }
    }
}

static void power_trip_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_power_trip_power(-1, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(7, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, -1, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 7, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, -1, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 7, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, -1, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 7, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, -1, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 7, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 0, -1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 0, 7, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 0, 0, -1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 0, 0, 7, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_power_trip_power(0, 0, 0, 0, 0, 0, 0, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(20, int_result);

    error = pkmn_calculations_power_trip_power(0, 1, 2, 3, 4, 5, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(440, int_result);

    error = pkmn_calculations_power_trip_power(6, 6, 6, 6, 6, 6, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(860, int_result);
}

static void punishment_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_punishment_power(-1, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(7, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, -1, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 7, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, -1, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 7, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, -1, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 7, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, -1, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 7, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 0, -1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 0, 7, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 0, 0, -1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 0, 0, 7, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_punishment_power(0, 0, 0, 0, 0, 0, 0, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(60, int_result);

    error = pkmn_calculations_punishment_power(0, 1, 2, 3, 4, 5, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(200, int_result);

    error = pkmn_calculations_punishment_power(6, 6, 6, 6, 6, 6, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(200, int_result);
}

static void return_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_return_power(-1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_return_power(256, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_return_power(0, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);

    error = pkmn_calculations_return_power(255, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(102, int_result);
}

static void reversal_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_reversal_power(0, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_reversal_power(1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_reversal_power(2, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    struct reversal_power_test_params
    {
        int attacker_current_hp;
        int attacker_max_hp;
        int expected_power;
    };
    static const struct reversal_power_test_params test_cases[] =
    {
        {1, 500, 200},
        {20, 500, 200},

        {21, 500, 150},
        {52, 500, 150},

        {53, 500, 100},
        {104, 500, 100},

        {105, 500, 80},
        {177, 500, 80},

        {178, 500, 40},
        {343, 500, 40},

        {344, 500, 20},
        {500, 500, 20}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_reversal_power(
                    test_cases[test_case_index].attacker_current_hp,
                    test_cases[test_case_index].attacker_max_hp,
                    &int_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].expected_power,
            int_result
        );
    }
}

static void rollout_powers_test()
{
    const int expected_results[5] = {30, 60, 120, 240, 480};

    int int_buffer[BUFFER_SIZE] = {0};
    size_t num_powers = 0;

    error = pkmn_calculations_rollout_powers(
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(5ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_results, int_buffer, num_powers);
}

static void spit_up_power_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_spit_up_power(-1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_spit_up_power(4, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    static const int expected_results[4] = {0, 100, 200, 300};
    static const size_t num_test_cases = sizeof(expected_results)/sizeof(expected_results[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_spit_up_power((int)test_case_index, &int_result);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(expected_results[test_case_index], int_result);
    }
}

static void stored_power_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_stored_power_power(-1, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(7, 0, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, -1, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 7, 0, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, -1, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 7, 0, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, -1, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 7, 0, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, -1, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 7, 0, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 0, -1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 0, 7, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 0, 0, -1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 0, 0, 7, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_stored_power_power(0, 0, 0, 0, 0, 0, 0, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(20, int_result);

    error = pkmn_calculations_stored_power_power(0, 1, 2, 3, 4, 5, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(440, int_result);

    error = pkmn_calculations_stored_power_power(6, 6, 6, 6, 6, 6, 6, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(860, int_result);
}

static void triple_kick_powers_test()
{
    const int expected_results[3] = {10, 20, 30};

    int int_buffer[BUFFER_SIZE] = {0};
    size_t num_powers = 0;

    error = pkmn_calculations_triple_kick_powers(
                int_buffer,
                BUFFER_SIZE,
                &num_powers
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3ULL, num_powers);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_results, int_buffer, num_powers);
}

static void trump_card_power_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_trump_card_power(-1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_trump_card_power(9, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    static const int expected_results[] =
    {
        200, 80, 60, 50, 40, 40, 40, 40, 40
    };
    static const size_t num_test_cases = sizeof(expected_results)/sizeof(expected_results[0]);

    for(int remaining_pp = 0;
        remaining_pp < (int)num_test_cases;
        ++remaining_pp
    )
    {
        error = pkmn_calculations_trump_card_power(remaining_pp, &int_result);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(expected_results[remaining_pp], int_result);
    }
}

static void water_spout_power_test()
{
    // Test invalid inputs.

    error = pkmn_calculations_water_spout_power(0, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_water_spout_power(1, 0, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_water_spout_power(2, 1, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_water_spout_power(1, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);

    error = pkmn_calculations_water_spout_power(250, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(75, int_result);

    error = pkmn_calculations_water_spout_power(500, 500, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(150, int_result);
}

static void wring_out_power_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_wring_out_power(0, 1, 5, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_wring_out_power(2, 1, 5, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_wring_out_power(1, 2, 3, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_wring_out_power(1, 2, 7, &int_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    error = pkmn_calculations_wring_out_power(1, 200, 4, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, int_result);

    error = pkmn_calculations_wring_out_power(200, 200, 4, &int_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(121, int_result);

    for(int generation = 5; generation <= 6; ++generation)
    {
        error = pkmn_calculations_wring_out_power(1, 200, generation, &int_result);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(1, int_result);

        error = pkmn_calculations_wring_out_power(200, 200, generation, &int_result);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(120, int_result);
    }
}

static void gen1_critical_hit_chance_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_gen1_critical_hit_chance(
                0, false, false, &float_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen1_critical_hit_chance(
                256, false, false, &float_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    // In Generation I, make sure there is never a guaranteed chance
    // for a critical hit.
    error = pkmn_calculations_gen1_critical_hit_chance(
                255, true, true, &float_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(float_result <= (255.0f / 256.0f));
}

static void critical_hit_chance_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_critical_hit_chance(1, 0, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_critical_hit_chance(10, 0, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_critical_hit_chance(2, -1, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    struct critical_hit_chance_test_params
    {
        int generation;
        int stage;
        float expected_result;
    };
    static const struct critical_hit_chance_test_params test_cases[] =
    {
        {2, 0, 0.0625f}, {2, 1, 0.125f}, {2, 2, 0.25f}, {2, 3, 0.333f}, {2, 4, 0.5f},
        {3, 0, 0.0625f}, {3, 1, 0.125f}, {3, 2, 0.25f}, {3, 3, 0.333f}, {3, 4, 0.5f},
        {4, 0, 0.0625f}, {4, 1, 0.125f}, {4, 2, 0.25f}, {4, 3, 0.333f}, {4, 4, 0.5f},
        {5, 0, 0.0625f}, {5, 1, 0.125f}, {5, 2, 0.25f}, {5, 3, 0.333f}, {5, 4, 0.5f},
        {6, 0, 0.0625f}, {6, 1, 0.125f}, {6, 2, 0.5f},  {6, 3, 1.0f},   {6, 4, 1.0f},
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        error = pkmn_calculations_critical_hit_chance(
                    test_cases[test_case_index].generation,
                    test_cases[test_case_index].stage,
                    &float_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_FLOAT(
            test_cases[test_case_index].expected_result,
            float_result
        );
    }
}

static void gen1_critical_hit_modifier_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_gen1_critical_hit_modifier(-1, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen1_critical_hit_modifier(256, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    // The critical hit modifier is level-dependent in Generation I.

    error = pkmn_calculations_gen1_critical_hit_modifier(5, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, float_result);

    error = pkmn_calculations_gen1_critical_hit_modifier(20, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(1.8f, float_result);

    error = pkmn_calculations_gen1_critical_hit_modifier(95, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(1.95f, float_result);
}

static void critical_hit_modifier_test()
{
    // Test invalid parameters.

    error = pkmn_calculations_critical_hit_modifier(1, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_critical_hit_modifier(10, &float_result);
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Test expected results.

    // Past Generation I, the modifier is constant, depending on the generation.

    error = pkmn_calculations_critical_hit_modifier(2, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, float_result);

    error = pkmn_calculations_critical_hit_modifier(3, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, float_result);

    error = pkmn_calculations_critical_hit_modifier(4, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, float_result);

    error = pkmn_calculations_critical_hit_modifier(5, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, float_result);

    error = pkmn_calculations_critical_hit_modifier(6, &float_result);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, float_result);
}

static void damage_test()
{
    // Source: https://bulbapedia.bulbagarden.net/wiki/Damage#Example

    // Only taking types into account
    //
    // "Imagine a level 75 Glaceon...with an effective Attack stat of 123
    // uses Ice Fang (an Ice-type physical move with a power of 65) against
    // a Garchomp with an effective Defense stat of 163 in Generation VI,
    // and does not land a critical hit."
    //
    // The article itself results in the wrong value, but the value I'm
    // testing for below was based on its equations.
    static const int level = 75;
    static const int ice_fang_base_power = 65;
    static const float modifier_against_dragon_ground = 6.0f;
    static const int glaceon_l75_attack = 123;
    static const int garchomp_l75_defense = 163;

    error = pkmn_calculations_damage(
                level,
                ice_fang_base_power,
                glaceon_l75_attack,
                garchomp_l75_defense,
                modifier_against_dragon_ground,
                &int_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(200, int_result);
}

static void type_damage_modifier_test()
{
    // Test invalid parameters.

    // Invalid generation

    error = pkmn_calculations_type_damage_modifier(
                -1,
                "Normal",
                "Normal",
                &float_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_type_damage_modifier(
                10,
                "Normal",
                "Normal",
                &float_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Invalid type for a given generation

    struct invalid_type_for_generation_test_params
    {
        int generation;
        const char* type;
    };
    static const struct invalid_type_for_generation_test_params invalid_type_test_cases[] =
    {
        {1, "Dark"}, {1, "Steel"},
        {5, "Fairy"},
        {3, "???"},{5, "???"},
        {2, "Shadow"},{4, "Shadow"}
    };
    static const size_t num_invalid_type_test_cases =
        sizeof(invalid_type_test_cases)/sizeof(invalid_type_test_cases[0]);

    for(size_t test_case_index = 0;
        test_case_index < num_invalid_type_test_cases;
        ++test_case_index
       )
    {
        // Invalid attacking type
        error = pkmn_calculations_type_damage_modifier(
                    invalid_type_test_cases[test_case_index].generation,
                    invalid_type_test_cases[test_case_index].type,
                    "Normal",
                    &float_result
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        // Invalid defending type
        error = pkmn_calculations_type_damage_modifier(
                    invalid_type_test_cases[test_case_index].generation,
                    "Normal",
                    invalid_type_test_cases[test_case_index].type,
                    &float_result
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }

    // Test expected results.

    // Check that changes between generations are properly implemented.

    struct modifier_changes_test_params
    {
        const char* attacking_type;
        const char* defending_type;

        int old_generation;
        float old_modifier;

        int new_generation;
        float new_modifier;
    };

    static const struct modifier_changes_test_params modifier_changes_test_cases[] =
    {
        {"Bug",    "Poison",  1, 2.0f, 2, 0.5f},
        {"Poison", "Bug",     1, 2.0f, 2, 1.0f},
        {"Ghost",  "Psychic", 1, 0.0f, 2, 2.0f},
        {"Ice",    "Fire",    1, 1.0f, 2, 0.5f},
        {"Ghost",  "Steel",   5, 0.5f, 6, 1.0f},
        {"Dark",   "Steel",   5, 0.5f, 6, 1.0f},
    };
    static const size_t num_modifier_changes_test_cases =
        sizeof(modifier_changes_test_cases)/sizeof(modifier_changes_test_cases[0]);

    for(size_t test_case_index = 0;
        test_case_index < num_modifier_changes_test_cases;
        ++test_case_index
       )
    {
        error = pkmn_calculations_type_damage_modifier(
                    modifier_changes_test_cases[test_case_index].old_generation,
                    modifier_changes_test_cases[test_case_index].attacking_type,
                    modifier_changes_test_cases[test_case_index].defending_type,
                    &float_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_FLOAT(
            modifier_changes_test_cases[test_case_index].old_modifier,
            float_result
        );

        error = pkmn_calculations_type_damage_modifier(
                    modifier_changes_test_cases[test_case_index].new_generation,
                    modifier_changes_test_cases[test_case_index].attacking_type,
                    modifier_changes_test_cases[test_case_index].defending_type,
                    &float_result
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_FLOAT(
            modifier_changes_test_cases[test_case_index].new_modifier,
            float_result
        );
    }
}

static void gen2_unown_form_test()
{
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_unown_form(
                -1, 0, 0, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_unown_form(
                16, 0, 0, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_unown_form(
                0, -1, 0, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_unown_form(
                0, 16, 0, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_unown_form(
                0, 0, -1, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_unown_form(
                -1, 0, 16, 0,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_unown_form(
                0, 0, 0, -1,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_unown_form(
                0, 0, 0, 16,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
     */
    error = pkmn_calculations_gen2_unown_form(
                10, 9, 1, 14,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "G");

    error = pkmn_calculations_gen2_unown_form(
                5, 15, 10, 5,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "S");
}

static void gen3_unown_form_test()
{
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
     */
    error = pkmn_calculations_gen3_unown_form(
                0x4C07DE71,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x091C5CF4, true,
                &bool_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(!bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x091C5CF4, false,
                &bool_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(bool_result);

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0x5CF4091C, false,
                &bool_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(!bool_result);
}

static void gen2_gender_test()
{
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_NONE,
                0,
                &gender_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_BULBASAUR,
                -1,
                &gender_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_BULBASAUR,
                16,
                &gender_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_NIDORINO,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_NIDORINO,
                15,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);

    /*
     * 25% male, 75% female
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_VULPIX,
                11,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_VULPIX,
                12,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);

    /*
     * All female
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_NIDORINA,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_NIDORINA,
                15,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);

    /*
     * Genderless
     */
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_MAGNEMITE,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender_result);
    error = pkmn_calculations_gen2_pokemon_gender(
                PKMN_SPECIES_MAGNEMITE,
                15,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender_result);
}

static void modern_gender_test()
{
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_NONE,
                0,
                &gender_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    /*
     * Make sure known good inputs result in expected results.
     */

    /*
     * All male
     */
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_NIDORINO,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_NIDORINO,
                0xFFFFFFFF,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);

    /*
     * 25% male, 75% female
     */
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_VULPIX,
                190,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_VULPIX,
                191,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender_result);

    /*
     * All female
     */
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_NIDORINA,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_NIDORINA,
                0xFFFFFFFF,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender_result);

    /*
     * Genderless
     */
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_MAGNEMITE,
                0,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender_result);
    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_MAGNEMITE,
                0xFFFFFFFF,
                &gender_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender_result);
}

static void gen2_hidden_power_test()
{
    /*
     * Make sure expected errors are returned and no new memory is allocated.
     */
    error = pkmn_calculations_gen2_hidden_power(
                -1, 0, 0, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_gen2_hidden_power(
                16, 0, 0, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_gen2_hidden_power(
                0, -1, 0, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 16, 0, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_gen2_hidden_power(
                0, 0, -1, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 16, 0,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 0, -1,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_gen2_hidden_power(
                0, 0, 0, 16,
                &hidden_power_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
     */
    error = pkmn_calculations_gen2_hidden_power(
                15, 15, 15, 14,
                &hidden_power_result
            );
    TEST_ASSERT_NOT_NULL(hidden_power_result.p_type);
    TEST_ASSERT_EQUAL_STRING(hidden_power_result.p_type, "Dark");
    TEST_ASSERT_EQUAL(69, hidden_power_result.base_power);
    pkmn_hidden_power_free(&hidden_power_result);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
}

static void modern_hidden_power_test()
{
    /*
     * Make sure expected exceptions are thrown and no new memory is allocated.
     */
    error = pkmn_calculations_modern_hidden_power(
        -1, 0, 0, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        32, 0, 0, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_modern_hidden_power(
        0, -1, 0, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        0, 32, 0, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, -1, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 32, 0, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, -1, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 32, 0, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, -1, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, 32, 0,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    error = pkmn_calculations_modern_hidden_power(
        0, 0, 0, 0, 0, -1,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);
    error = pkmn_calculations_modern_hidden_power(
        0, -1, 0, 0, 0, 32,
        &hidden_power_result
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(hidden_power_result.p_type);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
     */
    error = pkmn_calculations_modern_hidden_power(
                30, 31, 31, 31, 30, 31,
                &hidden_power_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(hidden_power_result.p_type);
    TEST_ASSERT_EQUAL_STRING(hidden_power_result.p_type, "Grass");
    TEST_ASSERT_EQUAL(70, hidden_power_result.base_power);
}

static void natural_gift_test()
{
    struct pkmn_natural_gift natural_gift = { NULL, 0 };

    // Test invalid generations.

    error = pkmn_calculations_natural_gift_stats(
                "Cheri Berry",
                3,
                &natural_gift
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(natural_gift.p_type);
    TEST_ASSERT_EQUAL(0, natural_gift.base_power);

    error = pkmn_calculations_natural_gift_stats(
                "Cheri Berry",
                10,
                &natural_gift
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    TEST_ASSERT_NULL(natural_gift.p_type);
    TEST_ASSERT_EQUAL(0, natural_gift.base_power);

    // Test invalid types.

    error = pkmn_calculations_natural_gift_stats(
                "Potion",
                4,
                &natural_gift
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    TEST_ASSERT_NULL(natural_gift.p_type);
    TEST_ASSERT_EQUAL(0, natural_gift.base_power);

    // Make sure differences between generations are reflected.
    struct natural_gift_test_params
    {
        const char* p_item;

        const char* p_type;
        int gen4_power;
        int gen5_power;
        int gen6_power;
    };
    static const struct natural_gift_test_params test_cases[] =
    {
        {"Cheri Berry", "Fire",     60, 60, 80},
        {"Nanab Berry", "Water",    70, 70, 90},
        {"Belue Berry", "Electric", 80, 80, 100}
    };
    static const size_t num_test_cases = sizeof(test_cases)/sizeof(test_cases[0]);

    for(size_t test_case_index = 0; test_case_index < num_test_cases; ++test_case_index)
    {
        // Generation IV

        error = pkmn_calculations_natural_gift_stats(
                    test_cases[test_case_index].p_item,
                    4,
                    &natural_gift
                );
        TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            test_cases[test_case_index].p_type,
            natural_gift.p_type
        );
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].gen4_power,
            natural_gift.base_power
        );

        error = pkmn_natural_gift_free(&natural_gift);
        TEST_ASSERT_NULL(natural_gift.p_type);
        TEST_ASSERT_EQUAL(0, natural_gift.base_power);

        // Generation V

        error = pkmn_calculations_natural_gift_stats(
                    test_cases[test_case_index].p_item,
                    5,
                    &natural_gift
                );
        TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            test_cases[test_case_index].p_type,
            natural_gift.p_type
        );
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].gen5_power,
            natural_gift.base_power
        );

        error = pkmn_natural_gift_free(&natural_gift);
        TEST_ASSERT_NULL(natural_gift.p_type);
        TEST_ASSERT_EQUAL(0, natural_gift.base_power);

        // Generation VI

        error = pkmn_calculations_natural_gift_stats(
                    test_cases[test_case_index].p_item,
                    6,
                    &natural_gift
                );
        TEST_ASSERT_EQUAL_STRING(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            test_cases[test_case_index].p_type,
            natural_gift.p_type
        );
        TEST_ASSERT_EQUAL(
            test_cases[test_case_index].gen6_power,
            natural_gift.base_power
        );

        error = pkmn_natural_gift_free(&natural_gift);
        TEST_ASSERT_NULL(natural_gift.p_type);
        TEST_ASSERT_EQUAL(0, natural_gift.base_power);
    }
}

static void nature_test()
{
    for(enum pkmn_nature expected_nature = PKMN_NATURE_HARDY;
        expected_nature <= PKMN_NATURE_QUIRKY;
        ++expected_nature)
    {
        enum pkmn_nature nature = PKMN_NATURE_NONE;

        error = pkmn_calculations_nature(
                    (uint32_t)(((rand() % 50000) * 1000) + expected_nature - 1),
                    &nature
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(expected_nature, nature);
    }
}

/*
 * Given the amount of time the C++ test takes, this will just verify
 * the API wrapper.
 */
static void personality_test()
{
    uint32_t personality = 0;
    bool shiny = false;
    enum pkmn_gender gender = PKMN_GENDER_GENDERLESS;

    // Test invalid ability.
    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_CHARMANDER,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_TORRENT,
                PKMN_GENDER_MALE,
                PKMN_NATURE_QUIET,
                &personality
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Test invalid gender.
    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_CHARMANDER,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_BLAZE,
                PKMN_GENDER_GENDERLESS,
                PKMN_NATURE_QUIET,
                &personality
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Test invalid nature.
    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_CHARMANDER,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_BLAZE,
                PKMN_GENDER_MALE,
                PKMN_NATURE_NONE,
                &personality
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    // Test and validate calls with each gender.
    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_CHARMANDER,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_BLAZE,
                PKMN_GENDER_FEMALE,
                PKMN_NATURE_QUIET,
                &personality
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(0, personality);

    error = pkmn_calculations_modern_shiny(
                personality,
                pkmn_pokemon_default_trainer_id().id,
                &shiny
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(shiny);

    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_CHARMANDER,
                personality,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_CHARMANDER,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_BLAZE,
                PKMN_GENDER_MALE,
                PKMN_NATURE_QUIET,
                &personality
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(0, personality);

    error = pkmn_calculations_modern_shiny(
                personality,
                pkmn_pokemon_default_trainer_id().id,
                &shiny
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(shiny);

    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_CHARMANDER,
                personality,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

    error = pkmn_calculations_generate_personality(
                PKMN_SPECIES_MAGNEMITE,
                pkmn_pokemon_default_trainer_id().id,
                true,
                PKMN_ABILITY_MAGNET_PULL,
                PKMN_GENDER_GENDERLESS,
                PKMN_NATURE_QUIET,
                &personality
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_EQUAL(0, personality);

    error = pkmn_calculations_modern_shiny(
                personality,
                pkmn_pokemon_default_trainer_id().id,
                &shiny
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_TRUE(shiny);

    error = pkmn_calculations_modern_pokemon_gender(
                PKMN_SPECIES_MAGNEMITE,
                personality,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender);
}

static void gen2_shiny_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_gen2_shiny(
                -1, 0, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_shiny(
                16, 0, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_shiny(
                0, -1, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_shiny(
                0, 16, 0, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_shiny(
                0, 0, -1, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_shiny(
                0, 0, 16, 0,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_calculations_gen2_shiny(
                0, 0, 0, -1,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gen2_shiny(
                0, 0, 0, 16,
                &bool_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
     */
    error = pkmn_calculations_gen2_shiny(
                7, 10, 10, 10,
                &bool_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(bool_result);
    error = pkmn_calculations_gen2_shiny(
                6, 15, 7, 15,
                &bool_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
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
    struct pkmn_database_pokemon_entry entry;
    srand((unsigned int)time(NULL));

    // Test input validation.
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, -1, 0, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 32, 0, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, -1, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 32, 0, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, -1, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 32, 0, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, -1, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, 32, 0, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, 0, -1, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, 0, 32, 0, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, 0, 0, -1, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_pokemon_size(
                PKMN_SPECIES_MAGIKARP, 0, 0, 0, 0, 0, 0, 32, &size
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * There are no known good calculations, so just check for reasonable values
     * for each relevant Pokémon.
     */
    struct size_test_params
    {
        enum pkmn_species species;
        enum pkmn_game game;
    };
    static const struct size_test_params test_params[] =
    {
        {PKMN_SPECIES_BARBOACH, PKMN_GAME_RUBY},
        {PKMN_SPECIES_SHROOMISH, PKMN_GAME_RUBY},
        {PKMN_SPECIES_SEEDOT, PKMN_GAME_EMERALD},
        {PKMN_SPECIES_LOTAD, PKMN_GAME_EMERALD},
        {PKMN_SPECIES_MAGIKARP, PKMN_GAME_FIRERED},
        {PKMN_SPECIES_HERACROSS, PKMN_GAME_FIRERED},
    };

    for(int i = 0; i < 6; ++i) {
        error = pkmn_database_get_pokemon_entry(
                    test_params[i].species,
                    test_params[i].game,
                    "",
                    &entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

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
            PKMN_TEST_ASSERT_SUCCESS(error);

            TEST_ASSERT(fabs(size-entry.height) < entry.height);
        }

        error = pkmn_database_pokemon_entry_free(&entry);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
}

static void spinda_spot_test()
{
    /*
     * Make sure known good inputs result in expected results.
     *
     * Source: https://github.com/magical/spinda
     */
    static const struct pkmn_spinda_spots SPOTS1 =
    {
        .left_ear =
        {
            7,
            5
        },
        .right_ear =
        {
            10,
            0
        },
        .left_face =
        {
            1,
            4
        },
        .right_face =
        {
            2,
            15
        },
    };

    error = pkmn_calculations_spinda_spot_offset(
                4064348759,
                &spinda_spots_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(SPOTS1.left_ear.x, spinda_spots_result.left_ear.x);
    TEST_ASSERT_EQUAL(SPOTS1.left_ear.y, spinda_spots_result.left_ear.y);
    TEST_ASSERT_EQUAL(SPOTS1.right_ear.x, spinda_spots_result.right_ear.x);
    TEST_ASSERT_EQUAL(SPOTS1.right_ear.y, spinda_spots_result.right_ear.y);
    TEST_ASSERT_EQUAL(SPOTS1.left_face.x, spinda_spots_result.left_face.x);
    TEST_ASSERT_EQUAL(SPOTS1.left_face.y, spinda_spots_result.left_face.y);
    TEST_ASSERT_EQUAL(SPOTS1.right_face.x, spinda_spots_result.right_face.x);
    TEST_ASSERT_EQUAL(SPOTS1.right_face.y, spinda_spots_result.right_face.y);
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_ATTACK,
                1, 1, 1, 12345,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 189);
    error = pkmn_calculations_gb_stat(
                PKMN_STAT_ATTACK,
                81, 55, 23140, 8,
                &int_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 137);
}

static void modern_stat_test() {
    /*
     * Make sure expected errors are returned.
     */
    error = pkmn_calculations_modern_stat(
                (enum pkmn_stat)(100), 1, 1.0f, 1, 1, 1,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK, 1, 1, 1, 1, 12345,
                &int_result
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

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
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 289);
    error = pkmn_calculations_modern_stat(
                PKMN_STAT_ATTACK,
                78, 1.1f, 130, 195, 12,
                &int_result
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_INT_WITHIN(1, int_result, 280);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(brine_power_test)
    PKMN_C_TEST(crush_grip_power_test)
    PKMN_C_TEST(echoed_voice_powers_test)
    PKMN_C_TEST(electro_ball_power_test)
    PKMN_C_TEST(eruption_power_test)
    PKMN_C_TEST(flail_power_test)
    PKMN_C_TEST(fling_power_test)
    PKMN_C_TEST(frustration_power_test)
    PKMN_C_TEST(fury_cutter_powers_test)
    PKMN_C_TEST(grass_knot_power_test)
    PKMN_C_TEST(gyro_ball_power_test)
    PKMN_C_TEST(heat_crash_power_test)
    PKMN_C_TEST(heavy_slam_power_test)
    PKMN_C_TEST(ice_ball_power_test)
    PKMN_C_TEST(low_kick_power_test)
    PKMN_C_TEST(power_trip_power_test)
    PKMN_C_TEST(punishment_power_test)
    PKMN_C_TEST(return_power_test)
    PKMN_C_TEST(reversal_power_test)
    PKMN_C_TEST(rollout_powers_test)
    PKMN_C_TEST(spit_up_power_test)
    PKMN_C_TEST(stored_power_power_test)
    PKMN_C_TEST(triple_kick_powers_test)
    PKMN_C_TEST(trump_card_power_test)
    PKMN_C_TEST(water_spout_power_test)
    PKMN_C_TEST(wring_out_power_test)
    PKMN_C_TEST(gen1_critical_hit_chance_test)
    PKMN_C_TEST(critical_hit_chance_test)
    PKMN_C_TEST(gen1_critical_hit_modifier_test)
    PKMN_C_TEST(critical_hit_modifier_test)
    PKMN_C_TEST(damage_test)
    PKMN_C_TEST(type_damage_modifier_test)
    PKMN_C_TEST(gen2_unown_form_test)
    PKMN_C_TEST(gen3_unown_form_test)
    PKMN_C_TEST(wurmple_becomes_silcoon_test)
    PKMN_C_TEST(gen2_gender_test)
    PKMN_C_TEST(modern_gender_test)
    PKMN_C_TEST(gen2_hidden_power_test)
    PKMN_C_TEST(modern_hidden_power_test)
    PKMN_C_TEST(natural_gift_test)
    PKMN_C_TEST(nature_test)
    PKMN_C_TEST(personality_test)
    PKMN_C_TEST(gen2_shiny_test)
    PKMN_C_TEST(modern_shiny_test)
    PKMN_C_TEST(pokemon_size_test)
    PKMN_C_TEST(spinda_spot_test)
    PKMN_C_TEST(gb_stat_test)
    PKMN_C_TEST(modern_stat_test)
)
