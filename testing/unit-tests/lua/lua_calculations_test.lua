--
-- Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local luaunit = require("luaunit")
local pkmn = require("pkmn")

math.randomseed(os.time())

utils = {}

function utils.to_lua_table(vec)
    ret = {}
    for i = 1, #vec
    do
        ret[i] = vec[i]
    end

    return ret
end

function test_stab_modifier()
    luaunit.assertEquals(pkmn.calculations.STAB_MODIFIER, 1.5)
end

function test_brine_power()
    -- Test invalid inputs.

    luaunit.assertError(pkmn.calculations.brine_power, 0, 2)
    luaunit.assertError(pkmn.calculations.brine_power, 0, 0)
    luaunit.assertError(pkmn.calculations.brine_power, -2, -1)
    luaunit.assertError(pkmn.calculations.brine_power, 2, 1)

    -- Test expected results.

    local max_hp = 10
    local half_hp = max_hp / 2

    luaunit.assertEquals(
        pkmn.calculations.brine_power(half_hp - 1, max_hp),
        130
    )
    luaunit.assertEquals(
        pkmn.calculations.brine_power(half_hp, max_hp),
        130
    )
    luaunit.assertEquals(
        pkmn.calculations.brine_power(half_hp + 1, max_hp),
        65
    )
end

function test_crush_grip_power()
    -- Test invalid inputs.

    luaunit.assertError(pkmn.calculations.crush_grip_power, 0, 1, 5)
    luaunit.assertError(pkmn.calculations.crush_grip_power, 2, 1, 5)
    luaunit.assertError(pkmn.calculations.crush_grip_power, 1, 2, 3)
    luaunit.assertError(pkmn.calculations.crush_grip_power, 1, 2, 7)

    -- Test expected results.

    luaunit.assertEquals(
        pkmn.calculations.crush_grip_power(1, 200, 4),
        1
    )
    luaunit.assertEquals(
        pkmn.calculations.crush_grip_power(200, 200, 4),
        121
    )

    for generation = 5, 6
    do
        luaunit.assertEquals(
            pkmn.calculations.crush_grip_power(1, 200, generation),
            1
        )
        luaunit.assertEquals(
            pkmn.calculations.crush_grip_power(200, 200, generation),
            120
        )
    end
end

function test_echoed_voice_powers()
    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.echoed_voice_powers()),
        {40, 80, 120, 160, 200}
    )
end

function test_electro_ball_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.electro_ball_power, 0, 100)
    luaunit.assertError(pkmn.calculations.electro_ball_power, 100, 0)

    -- Test expected results.

    local attacker_speeds = {100, 100, 100, 100, 100, 100, 100, 100}
    local target_speeds = {101, 100, 51, 50, 34, 33, 32, 25}
    local expected_powers = {40, 60, 60, 80, 80, 120, 120, 150}

    for test_case_index = 1, #attacker_speeds
    do
        luaunit.assertEquals(
            pkmn.calculations.electro_ball_power(
                attacker_speeds[test_case_index],
                target_speeds[test_case_index]
            ),
            expected_powers[test_case_index]
        )
    end
end

function test_eruption_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.eruption_power, 0, 1)
    luaunit.assertError(pkmn.calculations.eruption_power, 1, 0)
    luaunit.assertError(pkmn.calculations.eruption_power, 2, 1)

    -- Test expected results.

    luaunit.assertEquals(pkmn.calculations.eruption_power(1, 500), 1)
    luaunit.assertEquals(pkmn.calculations.eruption_power(250, 500), 75)
    luaunit.assertEquals(pkmn.calculations.eruption_power(500, 500), 150)
end

function test_flail_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.flail_power, 0, 1)
    luaunit.assertError(pkmn.calculations.flail_power, 1, 0)
    luaunit.assertError(pkmn.calculations.flail_power, 2, 1)

    -- Test expected results.

    local attacker_current_HPs = {1, 20, 21, 52, 53, 104, 105, 177, 178, 343, 344, 500}
    local attacker_max_HPs = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
    local expected_powers = {200, 200, 150, 150, 100, 100, 80, 80, 40, 40, 20, 20}

    for test_case_index = 1, #attacker_current_HPs
    do
        luaunit.assertEquals(
            pkmn.calculations.flail_power(
                attacker_current_HPs[test_case_index],
                attacker_max_HPs[test_case_index]
            ),
            expected_powers[test_case_index]
        )
    end
end

function test_fling_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.fling_power, pkmn.item.NONE)

    -- Test expected results.

    local items =
    {
        pkmn.item.ORAN_BERRY,
        pkmn.item.HEALTH_WING,
        pkmn.item.POTION,
        pkmn.item.ICY_ROCK,
        pkmn.item.DUBIOUS_DISC,
        pkmn.item.DAMP_ROCK,
        pkmn.item.DRAGON_FANG,
        pkmn.item.DUSK_STONE,
        pkmn.item.THICK_CLUB,
        pkmn.item.RARE_BONE,
        pkmn.item.IRON_BALL
    }
    local expected_powers = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 130};

    for test_case_index = 1, #items
    do
        luaunit.assertEquals(
            pkmn.calculations.fling_power(items[test_case_index]),
            expected_powers[test_case_index]
        )
    end
end

function test_frustration_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.frustration_power, -1)
    luaunit.assertError(pkmn.calculations.frustration_power, 256)

    -- Test expected values.

    luaunit.assertEquals(pkmn.calculations.frustration_power(0), 102)
    luaunit.assertEquals(pkmn.calculations.frustration_power(255), 1)
end

function test_fury_cutter_powers()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.fury_cutter_powers, 1)
    luaunit.assertError(pkmn.calculations.fury_cutter_powers, 10)

    -- Test expected results.

    local expected_gen_2_4_powers = {10, 20, 40, 80, 160}
    local expected_gen5_powers = {20, 40, 80, 160}
    local expected_gen6_powers = {40, 80, 160}

    for generation = 2, 4
    do
        luaunit.assertEquals(
            utils.to_lua_table(pkmn.calculations.fury_cutter_powers(generation)),
            expected_gen_2_4_powers
        )
    end

    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.fury_cutter_powers(5)),
        expected_gen5_powers
    )
    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.fury_cutter_powers(6)),
        expected_gen6_powers
    )
end

function test_grass_knot_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.grass_knot_power, -1.0)

    -- Test expected results.

    local target_weights = {5.0, 15.0, 35.0, 75.0, 150.0, 250.0}
    local expected_powers = {20, 40, 60, 80, 100, 120}

    for test_case_index = 1, #target_weights
    do
        luaunit.assertEquals(
            pkmn.calculations.grass_knot_power(target_weights[test_case_index]),
            expected_powers[test_case_index]
        )
    end
end

function test_gyro_ball_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.gyro_ball_power, 0, 50)
    luaunit.assertError(pkmn.calculations.gyro_ball_power, 50, 0)

    -- Test expected values.

    luaunit.assertEquals(pkmn.calculations.gyro_ball_power(1, 500), 150)
    luaunit.assertEquals(pkmn.calculations.gyro_ball_power(100, 200), 50)
    luaunit.assertEquals(pkmn.calculations.gyro_ball_power(500, 1), 1)
end

function test_heat_crash_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.heat_crash_power, 0.0, 1.0)
    luaunit.assertError(pkmn.calculations.heat_crash_power, 1.0, 0.0)

    -- Test expected values.

    local attacker_weights =
    {
        200.0, 200.0, 200.0,
        200.0, 200.0, 200.0,
        200.0, 200.0, 200.0
    }
    local target_weights =
    {
        200.0, 100.1, 100.0,
        66.7, 66.6, 50.1,
        50.0, 40.1, 40.0
    }
    local expected_powers =
    {
        40, 40, 60,
        60, 80, 80,
        100, 100, 120
    }

    for test_case_index = 1, #attacker_weights
    do
        luaunit.assertEquals(
            pkmn.calculations.heat_crash_power(
                attacker_weights[test_case_index],
                target_weights[test_case_index]
            ),
            expected_powers[test_case_index]
        )
    end
end

function test_heavy_slam_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.heavy_slam_power, 0.0, 1.0)
    luaunit.assertError(pkmn.calculations.heavy_slam_power, 1.0, 0.0)

    -- Test expected values.

    local attacker_weights =
    {
        200.0, 200.0, 200.0,
        200.0, 200.0, 200.0,
        200.0, 200.0, 200.0
    }
    local target_weights =
    {
        200.0, 100.1, 100.0,
        66.7, 66.6, 50.1,
        50.0, 40.1, 40.0
    }
    local expected_powers =
    {
        40, 40, 60,
        60, 80, 80,
        100, 100, 120
    }

    for test_case_index = 1, #attacker_weights
    do
        luaunit.assertEquals(
            pkmn.calculations.heavy_slam_power(
                attacker_weights[test_case_index],
                target_weights[test_case_index]
            ),
            expected_powers[test_case_index]
        )
    end
end

function test_ice_ball_powers()
    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.ice_ball_powers()),
        {30, 60, 120, 240, 480}
    )
end

function test_low_kick_power()
    -- Test invalid inputs.

    luaunit.assertError(pkmn.calculations.low_kick_power, -1.0, 1)
    luaunit.assertError(pkmn.calculations.low_kick_power, 1.0, 0)
    luaunit.assertError(pkmn.calculations.low_kick_power, 1.0, 10)

    -- Test expected values.

    local target_weights =
    {
        0.1, 9.9, 10.0, 24.9, 25.0, 49.9,
        50.0, 99.9, 100.0, 199.9, 200.0, 1000.0
    };
    local expected_powers =
    {
        20, 20, 40, 40, 60, 60,
        80, 80, 100, 100, 120, 120
    };

    for test_case_index = 1, #target_weights
    do
        for generation = 1, 6
        do
            if generation <= 2
            then
                -- Generation I-II has only one set power, regardless of
                -- weight.
                luaunit.assertEquals(
                    pkmn.calculations.low_kick_power(
                        target_weights[test_case_index],
                        generation
                    ),
                    50
                )
            else
                luaunit.assertEquals(
                    pkmn.calculations.low_kick_power(
                        target_weights[test_case_index],
                        generation
                    ),
                    expected_powers[test_case_index]
                )
            end
        end
    end
end

function test_power_trip_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.power_trip_power, -1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 7, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 7, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 7, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 7, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, 7, 0, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, 0, 7, 0)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.power_trip_power, 0, 0, 0, 0, 0, 0, 7)

    -- Test expected values.

    luaunit.assertEquals(
        pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, 0, 0),
        20
    )
    luaunit.assertEquals(
        pkmn.calculations.power_trip_power(0, 1, 2, 3, 4, 5, 6),
        440
    )
    luaunit.assertEquals(
        pkmn.calculations.power_trip_power(6, 6, 6, 6, 6, 6, 6),
        860
    )
end

function test_punishment_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.punishment_power, -1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 7, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 7, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 7, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 7, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, 7, 0, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, 0, 7, 0)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.punishment_power, 0, 0, 0, 0, 0, 0, 7)

    -- Test expected values.

    luaunit.assertEquals(
        pkmn.calculations.punishment_power(0, 0, 0, 0, 0, 0, 0),
        60
    )
    luaunit.assertEquals(
        pkmn.calculations.punishment_power(0, 1, 2, 3, 4, 5, 6),
        200
    )
    luaunit.assertEquals(
        pkmn.calculations.punishment_power(6, 6, 6, 6, 6, 6, 6),
        200
    )
end

function test_return_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.return_power, -1)
    luaunit.assertError(pkmn.calculations.return_power, 256)

    -- Test expected values.

    luaunit.assertEquals(pkmn.calculations.return_power(0), 1)
    luaunit.assertEquals(pkmn.calculations.return_power(255), 102)
end

function test_reversal_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.reversal_power, 0, 1)
    luaunit.assertError(pkmn.calculations.reversal_power, 1, 0)
    luaunit.assertError(pkmn.calculations.reversal_power, 2, 1)

    -- Test expected values.

    local attacker_current_HPs =
    {
        1, 20, 21, 52, 53, 104,
        105, 177, 178, 343, 344, 500
    }
    local attacker_max_HPs =
    {
        500, 500, 500, 500, 500, 500,
        500, 500, 500, 500, 500, 500
    }
    local expected_powers =
    {
        200, 200, 150, 150, 100, 100,
        80, 80, 40, 40, 20, 20
    }

    for test_case_index = 1, #attacker_current_HPs
    do
        luaunit.assertEquals(
            pkmn.calculations.reversal_power(
                attacker_current_HPs[test_case_index],
                attacker_max_HPs[test_case_index]
            ),
            expected_powers[test_case_index]
        )
    end
end

function test_rollout_powers()
    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.rollout_powers()),
        {30, 60, 120, 240, 480}
    )
end

function test_spit_up_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.spit_up_power, -1)
    luaunit.assertError(pkmn.calculations.spit_up_power, 4)

    -- Test expected results.

    luaunit.assertEquals(pkmn.calculations.spit_up_power(0), 0)
    luaunit.assertEquals(pkmn.calculations.spit_up_power(1), 100)
    luaunit.assertEquals(pkmn.calculations.spit_up_power(2), 200)
    luaunit.assertEquals(pkmn.calculations.spit_up_power(3), 300)
end

function test_stored_power_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.stored_power_power, -1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 7, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 7, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 7, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 7, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, 7, 0, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, 0, 7, 0)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.stored_power_power, 0, 0, 0, 0, 0, 0, 7)

    -- Test expected values.

    luaunit.assertEquals(
        pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, 0, 0),
        20
    )
    luaunit.assertEquals(
        pkmn.calculations.stored_power_power(0, 1, 2, 3, 4, 5, 6),
        440
    )
    luaunit.assertEquals(
        pkmn.calculations.stored_power_power(6, 6, 6, 6, 6, 6, 6),
        860
    )
end

function test_triple_kick_powers()
    luaunit.assertEquals(
        utils.to_lua_table(pkmn.calculations.triple_kick_powers()),
        {10, 20, 30}
    )
end

function test_trump_card_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.trump_card_power, -1)
    luaunit.assertError(pkmn.calculations.trump_card_power, 9)

    -- Test expected values.

    local expected_results = {200, 80, 60, 50, 40, 40, 40, 40, 40}

    for remaining_pp = 0, #expected_results-1
    do
        luaunit.assertEquals(
            pkmn.calculations.trump_card_power(remaining_pp),
            expected_results[remaining_pp+1]
        )
    end
end

function test_water_spout_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.water_spout_power, 0, 1)
    luaunit.assertError(pkmn.calculations.water_spout_power, 1, 0)
    luaunit.assertError(pkmn.calculations.water_spout_power, 2, 1)

    -- Test expected results.

    luaunit.assertEquals(pkmn.calculations.water_spout_power(1, 500), 1)
    luaunit.assertEquals(pkmn.calculations.water_spout_power(250, 500), 75)
    luaunit.assertEquals(pkmn.calculations.water_spout_power(500, 500), 150)
end

function test_wring_out_power()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.wring_out_power, 0, 1, 5)
    luaunit.assertError(pkmn.calculations.wring_out_power, 2, 1, 5)
    luaunit.assertError(pkmn.calculations.wring_out_power, 1, 2, 3)
    luaunit.assertError(pkmn.calculations.wring_out_power, 1, 2, 7)

    -- Test expected results.

    luaunit.assertEquals(pkmn.calculations.wring_out_power(1, 200, 4), 1)
    luaunit.assertEquals(pkmn.calculations.wring_out_power(200, 200, 4), 121)

    for generation = 5, 6
    do
        luaunit.assertEquals(
            pkmn.calculations.wring_out_power(1, 200, generation),
            1
        )
        luaunit.assertEquals(
            pkmn.calculations.wring_out_power(200, 200, generation),
            120
        )
    end
end

function test_gen1_critical_hit_chance()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.gen1_critical_hit_chance, 0, false, false)
    luaunit.assertError(pkmn.calculations.gen1_critical_hit_chance, 256, false, false)

    -- In Generation I, make sure there is never a guaranteed chance for a
    -- critical hit.

    luaunit.assertTrue(pkmn.calculations.gen1_critical_hit_chance(255, true, true) <= (255.0/256.0))
end

function test_critical_hit_chance()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.critical_hit_chance, 1, 0)
    luaunit.assertError(pkmn.calculations.critical_hit_chance, 10, 0)
    luaunit.assertError(pkmn.calculations.critical_hit_chance, 2, -1)

    -- Test expected values.

    local generations =
    {
        2, 2, 2, 2, 2,
        3, 3, 3, 3, 3,
        4, 4, 4, 4, 4,
        5, 5, 5, 5, 5,
        6, 6, 6, 6, 6,
    };
    local stages =
    {
        0, 1, 2, 3, 4,
        0, 1, 2, 3, 4,
        0, 1, 2, 3, 4,
        0, 1, 2, 3, 4,
        0, 1, 2, 3, 4,
    };
    local expected_results =
    {
        0.0625, 0.125, 0.25, 0.333, 0.5,
        0.0625, 0.125, 0.25, 0.333, 0.5,
        0.0625, 0.125, 0.25, 0.333, 0.5,
        0.0625, 0.125, 0.25, 0.333, 0.5,
        0.0625, 0.125, 0.5,  1.0,   1.0
    };

    for test_case_index = 1, #generations
    do
        luaunit.assertAlmostEquals(
            pkmn.calculations.critical_hit_chance(
                generations[test_case_index],
                stages[test_case_index]
            ),
            expected_results[test_case_index],
            0.0001
        )
    end
end

function test_gen1_critical_hit_modifier()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.gen1_critical_hit_modifier, -1)
    luaunit.assertError(pkmn.calculations.gen1_critical_hit_modifier, 256)

    -- Test expected results.

    luaunit.assertAlmostEquals(
        pkmn.calculations.gen1_critical_hit_modifier(5),
        1.5,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.gen1_critical_hit_modifier(20),
        1.8,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.gen1_critical_hit_modifier(95),
        1.95,
        0.0001
    )
end

function test_critical_hit_modifier()
    -- Test invalid parameters.

    luaunit.assertError(pkmn.calculations.critical_hit_modifier, -1)
    luaunit.assertError(pkmn.calculations.critical_hit_modifier, 10)

    -- Test expected results.

    luaunit.assertAlmostEquals(
        pkmn.calculations.critical_hit_modifier(2),
        2.0,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.critical_hit_modifier(3),
        2.0,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.critical_hit_modifier(4),
        2.0,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.critical_hit_modifier(5),
        2.0,
        0.0001
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.critical_hit_modifier(6),
        1.5,
        0.0001
    )
end

function test_damage()
    -- Source: https://bulbapedia.bulbagarden.net/wiki/Damage#Example

    -- Only taking types into account
    --
    -- "Imagine a level 75 Glaceon...with an effective Attack stat of 123
    -- uses Ice Fang (an Ice-type physical move with a power of 65) against
    -- a Garchomp with an effective Defense stat of 163 in Generation VI,
    -- and does not land a critical hit."
    --
    -- The article itself results in the wrong value, but the value I'm
    -- testing for below was based on its equations.

    local level = 75
    local ice_fang_base_power = 65
    local modifier_against_dragon_ground = 6.0
    local glaceon_l75_attack = 123
    local garchomp_l75_defense = 163

    luaunit.assertEquals(
        pkmn.calculations.damage(
            level,
            ice_fang_base_power,
            glaceon_l75_attack,
            garchomp_l75_defense,
            modifier_against_dragon_ground
        ),
        200
    )
end

function test_type_damage_modifier()
    -- Test invalid inputs.

    -- Invalid generation
    luaunit.assertError(
        pkmn.calculations.type_damage_modifier,
        -1,
        pkmn.type.NORMAL,
        pkmn.type.NORMAL
    )
    luaunit.assertError(
        pkmn.calculations.type_damage_modifier,
        8,
        pkmn.type.NORMAL,
        pkmn.type.NORMAL
    )

    -- Invalid types for a given generation
    local generations = {1, 1, 5, 3, 5, 2, 4}
    local types =
    {
        pkmn.type.DARK,
        pkmn.type.STEEL,
        pkmn.type.FAIRY,
        pkmn.type.QUESTION_MARK,
        pkmn.type.QUESTION_MARK,
        pkmn.type.SHADOW,
        pkmn.type.SHADOW
    }

    for test_case_index = 1, #generations
    do
        -- Invalid attacking type
        luaunit.assertError(
            pkmn.calculations.type_damage_modifier,
            generations[test_case_index],
            types[test_case_index],
            pkmn.type.NORMAL
        )

        -- Invalid defending type
        luaunit.assertError(
            pkmn.calculations.type_damage_modifier,
            generations[test_case_index],
            pkmn.type.NORMAL,
            types[test_case_index]
        )
    end

    -- Check that changes between generations are properly implemented.

    local attacking_types =
    {
        pkmn.type.BUG,
        pkmn.type.POISON,
        pkmn.type.GHOST,
        pkmn.type.ICE,
        pkmn.type.GHOST,
        pkmn.type.DARK
    }
    local defending_types =
    {
        pkmn.type.POISON,
        pkmn.type.BUG,
        pkmn.type.PSYCHIC,
        pkmn.type.FIRE,
        pkmn.type.STEEL,
        pkmn.type.STEEL
    }
    local old_generations = {1, 1, 1, 1, 5, 5}
    local old_modifiers = {2.0, 2.0, 0.0, 1.0, 0.5, 0.5}
    local new_generations = {2, 2, 2, 2, 6, 6}
    local new_modifiers = {0.5, 1.0, 2.0, 0.5, 1.0, 1.0}

    for test_case_index = 1, #attacking_types
    do
        luaunit.assertAlmostEquals(
            pkmn.calculations.type_damage_modifier(
                old_generations[test_case_index],
                attacking_types[test_case_index],
                defending_types[test_case_index]
            ),
            old_modifiers[test_case_index],
            0.0001
        )
        luaunit.assertAlmostEquals(
            pkmn.calculations.type_damage_modifier(
                new_generations[test_case_index],
                attacking_types[test_case_index],
                defending_types[test_case_index]
            ),
            new_modifiers[test_case_index],
            0.0001
        )
    end
end

function test_gen2_unown_form()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_unown_form, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_unown_form, 0, 0, 0, 16)

    --
    -- Make sure known inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
    --
    luaunit.assertEquals(pkmn.calculations.gen2_unown_form(10, 9, 1, 14), "G")
    luaunit.assertEquals(pkmn.calculations.gen2_unown_form(5, 15, 10, 5), "S")
end

function test_gen3_unown_form()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
    --
    luaunit.assertError(pkmn.calculations.gen3_unown_form(0x4C07DE71), "B")
end

function test_wurmple_becomes_silcoon()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
    --
    luaunit.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, true))
    luaunit.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, true))
    luaunit.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, false))
    luaunit.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, false))
end

function test_gen2_gender()
    -- Make sure expected errors are thrown.
    luaunit.assertError(
        pkmn.calculations.gen2_pokemon_gender,
        pkmn.species.NONE,
        0
    )
    luaunit.assertError(
        pkmn.calculations.gen2_pokemon_gender,
        pkmn.species.BULBASAUR,
        -1
    )
    luaunit.assertError(
        pkmn.calculations.gen2_pokemon_gender,
        pkmn.species.BULBASAUR,
        16
    )

    -- Make sure known good inputs result in expected results.

    -- All male
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINO, 0),
        pkmn.gender.MALE
    )
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINO, 15),
        pkmn.gender.MALE
    )

    -- 25% male, 75% female
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.VULPIX, 11),
        pkmn.gender.FEMALE
    )
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.VULPIX, 12),
        pkmn.gender.MALE
    )

    -- All female
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINA, 0),
        pkmn.gender.FEMALE
    )
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINA, 15),
        pkmn.gender.FEMALE
    )

    -- Genderless
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.MAGNEMITE, 0),
        pkmn.gender.GENDERLESS
    )
    luaunit.assertEquals(
        pkmn.calculations.gen2_pokemon_gender(pkmn.species.MAGNEMITE, 15),
        pkmn.gender.GENDERLESS
    )
end

function test_modern_gender()
    -- Make sure expected errors are thrown.
    luaunit.assertError(
        pkmn.calculations.modern_pokemon_gender,
        pkmn.species.NONE,
        0
    )

    -- All male
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINO, 0),
        pkmn.gender.MALE
    )
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINO, 0xFFFFFFFF),
        pkmn.gender.MALE
    )

    -- 25% male, 75% female
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.VULPIX, 190),
        pkmn.gender.FEMALE
    )
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.VULPIX, 191),
        pkmn.gender.MALE
    )

    -- All female
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINA, 0),
        pkmn.gender.FEMALE
    )
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINA, 0xFFFFFFFF),
        pkmn.gender.FEMALE
    )

    -- Genderless
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.MAGNEMITE, 0),
        pkmn.gender.GENDERLESS
    )
    luaunit.assertEquals(
        pkmn.calculations.modern_pokemon_gender(pkmn.species.MAGNEMITE, 0xFFFFFFFF),
        pkmn.gender.GENDERLESS
    )
end

function test_gen2_hidden_power()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_hidden_power, 0, 0, 0, 16)

    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
    --
    expected_hidden_power = pkmn.calculations.hidden_power(pkmn.type.DARK, 69)

    hidden_power = pkmn.calculations.gen2_hidden_power(15, 15, 15, 14)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
end

function test_modern_hidden_power()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.modern_hidden_power, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 32, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 32, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 32, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 32, 0, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 32, 0)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.modern_hidden_power, 0, 0, 0, 0, 0, 32)

    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
    --
    expected_hidden_power = pkmn.calculations.hidden_power(pkmn.type.GRASS, 70)

    hidden_power = pkmn.calculations.modern_hidden_power(30, 31, 31, 31, 30, 31)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
end

function test_natural_gift()
    -- Test invalid generations.

    luaunit.assertError(
        pkmn.calculations.natural_gift_stats,
        pkmn.item.CHERI_BERRY,
        3
    )
    luaunit.assertError(
        pkmn.calculations.natural_gift_stats,
        pkmn.item.CHERI_BERRY,
        10
    )

    -- Test invalid items.

    luaunit.assertError(
        pkmn.calculations.natural_gift_stats,
        pkmn.item.POTION,
        4
    )

    local items =
    {
        pkmn.item.CHERI_BERRY,
        pkmn.item.NANAB_BERRY,
        pkmn.item.BELUE_BERRY
    }
    local types =
    {
        pkmn.type.FIRE,
        pkmn.type.WATER,
        pkmn.type.ELECTRIC
    }
    local gen4_powers = {60, 70, 80}
    local gen5_powers = {60, 70, 80}
    local gen6_powers = {80, 90, 100}

    for test_case_index = 1, #items
    do
        local natural_gift = pkmn.calculations.natural_gift_stats(
                                 items[test_case_index],
                                 4
                             )
        luaunit.assertEquals(natural_gift.type, types[test_case_index])
        luaunit.assertEquals(natural_gift.base_power, gen4_powers[test_case_index])

        local natural_gift = pkmn.calculations.natural_gift_stats(
                                 items[test_case_index],
                                 5
                             )
        luaunit.assertEquals(natural_gift.type, types[test_case_index])
        luaunit.assertEquals(natural_gift.base_power, gen5_powers[test_case_index])

        local natural_gift = pkmn.calculations.natural_gift_stats(
                                 items[test_case_index],
                                 6
                             )
        luaunit.assertEquals(natural_gift.type, types[test_case_index])
        luaunit.assertEquals(natural_gift.base_power, gen6_powers[test_case_index])
    end
end

function test_nature()
    -- Make sure SWIG+Lua catches values outside the uint32_t range.
    luaunit.assertError(pkmn.calculations.nature, -1)
    luaunit.assertError(pkmn.calculations.nature, 0xFFFFFFFF+1)

    local natures =
    {
        pkmn.nature.HARDY,
        pkmn.nature.LONELY,
        pkmn.nature.BRAVE,
        pkmn.nature.ADAMANT,
        pkmn.nature.NAUGHTY,
        pkmn.nature.BOLD,
        pkmn.nature.DOCILE,
        pkmn.nature.RELAXED,
        pkmn.nature.IMPISH,
        pkmn.nature.LAX,
        pkmn.nature.TIMID,
        pkmn.nature.HASTY,
        pkmn.nature.SERIOUS,
        pkmn.nature.JOLLY,
        pkmn.nature.NAIVE,
        pkmn.nature.MODEST,
        pkmn.nature.MILD,
        pkmn.nature.QUIET,
        pkmn.nature.BASHFUL,
        pkmn.nature.RASH,
        pkmn.nature.CALM,
        pkmn.nature.GENTLE,
        pkmn.nature.SASSY,
        pkmn.nature.CAREFUL,
        pkmn.nature.QUIRKY
    }

    for i = 1, #natures
    do
        luaunit.assertEquals(
            pkmn.calculations.nature((math.random(0,50000) * 1000) + i-1),
            natures[i]
        )
    end
end

--
-- Given the amount of time the C++ test takes, this will just verify
-- the API wrapper.
--
function test_personality()
    -- Test invalid ability.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        pkmn.species.CHARMANDER,
        pkmn.DEFAULT_TRAINER_ID,
        true,
        pkmn.ability.TORRENT,
        pkmn.gender.MALE,
        pkmn.nature.QUIET
    )

    -- Test invalid gender.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        pkmn.species.CHARMANDER,
        pkmn.DEFAULT_TRAINER_ID,
        true,
        pkmn.ability.BLAZE,
        pkmn.gender.NONE,
        pkmn.nature.QUIET
    )

    -- Test invalid nature.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        pkmn.species.CHARMANDER,
        pkmn.DEFAULT_TRAINER_ID,
        true,
        pkmn.ability.BLAZE,
        pkmn.gender.MALE,
        pkmn.nature.NONE
    )

    -- Make sure Lua+SWIG catch invalid trainer IDs.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        pkmn.species.CHARMANDER,
        -1,
        true,
        pkmn.ability.BLAZE,
        pkmn.gender.MALE,
        pkmn.nature.QUIET
    )
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        pkmn.species.CHARMANDER,
        0xFFFFFFFF+1,
        true,
        pkmn.ability.BLAZE,
        pkmn.gender.MALE,
        pkmn.nature.QUIET
    )

    -- Test and validate a valid call.
    local personality = pkmn.calculations.generate_personality(
                            pkmn.species.CHARMANDER,
                            pkmn.pokemon.DEFAULT_TRAINER_ID,
                            true,
                            pkmn.ability.BLAZE,
                            pkmn.gender.MALE,
                            pkmn.nature.QUIET
                        )
    luaunit.assertEquals(
        pkmn.gender.MALE,
        pkmn.calculations.modern_pokemon_gender(
            pkmn.species.CHARMANDER,
            personality
        )
    )
    luaunit.assertTrue(
        pkmn.calculations.modern_shiny(
            pkmn.pokemon.DEFAULT_TRAINER_ID,
            personality
        )
    )
end

function test_gen2_shiny()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.gen2_shiny, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 16, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 16, 0, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 16, 0)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.gen2_shiny, 0, 0, 0, 16)

    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
    --
    luaunit.assertTrue(pkmn.calculations.gen2_shiny(7, 10, 10, 10))
    luaunit.assertFalse(pkmn.calculations.gen2_shiny(6, 15, 7, 15))
end

function test_modern_shiny()
    --
    -- Make sure known good inputs result in expected results.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
    --         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
    --
    luaunit.assertTrue(pkmn.calculations.modern_shiny(2814471828, 2545049318))
    luaunit.assertTrue(pkmn.calculations.modern_shiny(0xB58F0B2A, 398174488))
end

function test_pokemon_size()
    -- Test input validation.
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        -1, 0, 0, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, -1, 0, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 32, 0, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, -1, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 32, 0, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, -1, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 32, 0, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, -1, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, 32, 0, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, 0, -1, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, 0, 32, 0
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, 0, 0, -1
    )
    luaunit.assertError(
        pkmn.calculations.pokemon_size,
        pkmn.species.MAGIKARP,
        0, 0, 0, 0, 0, 0, 32
    )

    --
    -- There are no known good calculations, so just check for reasonable values
    -- for each relevant Pokémon.
    --
    local pokemon_with_size_checks =
    {
        pkmn.database.pokemon_entry(pkmn.species.BARBOACH, pkmn.game.RUBY, ""),
        pkmn.database.pokemon_entry(pkmn.species.SHROOMISH, pkmn.game.RUBY, ""),
        pkmn.database.pokemon_entry(pkmn.species.SEEDOT, pkmn.game.EMERALD, ""),
        pkmn.database.pokemon_entry(pkmn.species.LOTAD, pkmn.game.EMERALD, ""),
        pkmn.database.pokemon_entry(pkmn.species.MAGIKARP, pkmn.game.FIRERED, ""),
        pkmn.database.pokemon_entry(pkmn.species.HERACROSS, pkmn.game.FIRERED, "")
    }

    for i = 1, #pokemon_with_size_checks
    do
        local height = pokemon_with_size_checks[i].height
        local species = pokemon_with_size_checks[i].species

        for j = 1, 10
        do
            local size = pkmn.calculations.pokemon_size(
                             species,
                             math.random(0, 2147483646),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31),
                             math.random(0, 31)
                         )
            luaunit.assertTrue(math.abs(height-size) <= height)
        end
    end
end

function test_spinda_coords()
    -- Check (in)equality.
    spinda_coords1 = pkmn.calculations.spinda_coords(123, 456)
    spinda_coords2 = pkmn.calculations.spinda_coords(123, 456)
    spinda_coords3 = pkmn.calculations.spinda_coords(456, 123)

    luaunit.assertEquals(spinda_coords1, spinda_coords2)
    luaunit.assertNotEquals(spinda_coords1, spinda_coords3)

    -- Test addition operator.
    added_coords = spinda_coords1 + spinda_coords2
    luaunit.assertEquals(246, added_coords.x)
    luaunit.assertEquals(912, added_coords.y)
end

function test_spinda_spots()
    -- Check (in)equality.
    spots1 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(1, 4),
                 pkmn.calculations.spinda_coords(2, 15)
             )
    spots2 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(1, 4),
                 pkmn.calculations.spinda_coords(2, 15)
             )
    spots3 = pkmn.calculations.spinda_spots(
                 pkmn.calculations.spinda_coords(7, 5),
                 pkmn.calculations.spinda_coords(10, 0),
                 pkmn.calculations.spinda_coords(2, 15),
                 pkmn.calculations.spinda_coords(1, 4)
             )
    luaunit.assertEquals(spots1, spots2)
    luaunit.assertNotEquals(spots1, spots3)

    --
    -- Make sure known good inputs result in expected results, and test (in)equality operators.
    --
    -- Source: https://github.com/magical/spinda
    --
    spots = pkmn.calculations.spinda_spot_offset(4064348759)
    luaunit.assertEquals(spots, spots1)
    luaunit.assertNotEquals(spots, spots3)

    -- Test addition operators.
    added_spots = spots1 + spots2
    luaunit.assertEquals(14, added_spots.left_ear.x)
    luaunit.assertEquals(10, added_spots.left_ear.y)
    luaunit.assertEquals(20, added_spots.right_ear.x)
    luaunit.assertEquals(0, added_spots.right_ear.y)
    luaunit.assertEquals(2, added_spots.left_face.x)
    luaunit.assertEquals(8, added_spots.left_face.y)
    luaunit.assertEquals(4, added_spots.right_face.x)
    luaunit.assertEquals(30, added_spots.right_face.y)

    added_spots = added_spots + pkmn.calculations.spinda_coords(1, 5)
    luaunit.assertEquals(15, added_spots.left_ear.x)
    luaunit.assertEquals(15, added_spots.left_ear.y)
    luaunit.assertEquals(21, added_spots.right_ear.x)
    luaunit.assertEquals(5, added_spots.right_ear.y)
    luaunit.assertEquals(3, added_spots.left_face.x)
    luaunit.assertEquals(13, added_spots.left_face.y)
    luaunit.assertEquals(5, added_spots.right_face.x)
    luaunit.assertEquals(35, added_spots.right_face.y)
end

function test_gb_stats()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.get_gb_stat, pkmn.stat.ATTACK, 1, 1, 123456, 1)
    luaunit.assertError(pkmn.calculations.get_gb_stat, pkmn.stat.ATTACK, 1, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
    --
    luaunit.assertAlmostEquals(
        pkmn.calculations.get_gb_stat(pkmn.stat.HP, 81, 35, 22850, 7),
        189, 1
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.get_gb_stat(pkmn.stat.ATTACK, 81, 55, 23140, 8),
        137, 1
    )
end

function test_modern_stats()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.get_modern_stat, pkmn.stat.NONE, 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, pkmn.stat.SPECIAL, 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, pkmn.stat.ATTACK, 1, 0.666, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, pkmn.stat.ATTACK, 1, 1.0, 1, 12345, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, pkmn.stat.ATTACK, 1, 1.0, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
    --
    luaunit.assertAlmostEquals(
        pkmn.calculations.get_modern_stat(pkmn.stat.HP, 78, 1.0, 108, 74, 24),
        289, 1
    )
    luaunit.assertAlmostEquals(
        pkmn.calculations.get_modern_stat(pkmn.stat.ATTACK, 78, 1.1, 130, 195, 12),
        280, 1
    )
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
