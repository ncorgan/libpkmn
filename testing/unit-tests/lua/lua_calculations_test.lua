--
-- Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

    luaunit.assertError(pkmn.calculations.fling_power, "Not an item")

    -- Test expected results.

    local item_names =
    {
        "Oran Berry", "Health Wing", "Potion",
        "Icy Rock", "Dubious Disc", "Damp Rock",
        "Dragon Fang", "Dusk Stone", "Thick Club",
        "Rare Bone", "Iron Ball"
    }
    local expected_powers = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 130};

    for test_case_index = 1, #item_names
    do
        luaunit.assertEquals(
            pkmn.calculations.fling_power(item_names[test_case_index]),
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

function test_spitup_power()
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

    luaunit.assertTrue(pkmn.calculations.gen1_critical_hit_chance(255, true, true) < (255.0/256.0))
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
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Not a species", 0)
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Bulbasaur", -1)
    luaunit.assertError(pkmn.calculations.gen2_pokemon_gender, "Bulbasaur", 16)

    -- Make sure known good inputs result in expected results.

    -- All male
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 0), "Male")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 15), "Male")

    -- 25% male, 75% female
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 11), "Female")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 12), "Male")

    -- All female
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 0), "Female")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 15), "Female")

    -- Genderless
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 0), "Genderless")
    luaunit.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 15), "Genderless")
end

function test_modern_gender()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.modern_pokemon_gender, "Not a species", 0)

    -- All male
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0), "Male")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0xFFFFFFFF), "Male")

    -- 25% male, 75% female
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 190), "Female")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 191), "Male")

    -- All female
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0), "Female")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0xFFFFFFFF), "Female")

    -- Genderless
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0), "Genderless")
    luaunit.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0xFFFFFFFF), "Genderless")
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
    -- Make sure known good inputs result in expected results, and test (in)equality operators.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
    --
    expected_hidden_power = pkmn.calculations.hidden_power("Dark", 69)
    hidden_power_different_type = pkmn.calculations.hidden_power("Normal", 69)
    hidden_power_different_base_power = pkmn.calculations.hidden_power("Dark", 50)

    hidden_power = pkmn.calculations.gen2_hidden_power(15, 15, 15, 14)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_type)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_base_power)
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
    expected_hidden_power = pkmn.calculations.hidden_power("Grass", 70)
    hidden_power_different_type = pkmn.calculations.hidden_power("Steel", 70)
    hidden_power_different_base_power = pkmn.calculations.hidden_power("Grass", 10)

    hidden_power = pkmn.calculations.modern_hidden_power(30, 31, 31, 31, 30, 31)
    luaunit.assertEquals(hidden_power, expected_hidden_power)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_type)
    luaunit.assertNotEquals(hidden_power, hidden_power_different_base_power)
end

function test_natural_gift()
    -- Test invalid generations.

    luaunit.assertError(pkmn.calculations.natural_gift_stats, "Cheri Berry", 3)
    luaunit.assertError(pkmn.calculations.natural_gift_stats, "Cheri Berry", 10)

    -- Test invalid items.

    luaunit.assertError(pkmn.calculations.natural_gift_stats, "Potion", 4)

    local items = {"Cheri Berry", "Nanab Berry", "Belue Berry"}
    local types = {"Fire", "Water", "Electric"}
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

    local natures = {
        "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
        "Bold", "Docile", "Relaxed", "Impish", "Lax",
        "Timid", "Hasty", "Serious", "Jolly", "Naive",
        "Modest", "Mild", "Quiet", "Bashful", "Rash",
        "Calm", "Gentle", "Sassy", "Careful", "Quirky",
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
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Torrent",
        "Male",
        "Quiet"
    )

    -- Test invalid gender.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Blaze",
        "Not a gender",
        "Quiet"
    )

    -- Test invalid nature.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        pkmn.DEFAULT_TRAINER_ID,
        true,
        "Blaze",
        "Male",
        "Not a nature"
    )

    -- Make sure Lua+SWIG catch invalid trainer IDs.
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        -1,
        true,
        "Blaze",
        "Male",
        "Quiet"
    )
    luaunit.assertError(
        pkmn.calculations.generate_personality,
        "Charmander",
        0xFFFFFFFF+1,
        true,
        "Blaze",
        "Male",
        "Quiet"
    )

    -- Test and validate a valid call.
    local personality = pkmn.calculations.generate_personality(
                            "Charmander",
                            pkmn.DEFAULT_TRAINER_ID,
                            true,
                            "Blaze",
                            "Male",
                            "Quiet"
                        )
    luaunit.assertEquals("Male", pkmn.calculations.modern_pokemon_gender("Charmander", personality))
    luaunit.assertTrue(pkmn.calculations.modern_shiny(pkmn.DEFAULT_TRAINER_ID, personality))
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
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", -1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, -1, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 32, 0, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, -1, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 32, 0, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, -1, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 32, 0, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, -1, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 32, 0, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, -1, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 32, 0)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 0, -1)
    luaunit.assertError(pkmn.calculations.pokemon_size, "Magikarp", 0, 0, 0, 0, 0, 0, 32)

    --
    -- There are no known good calculations, so just check for reasonable values
    -- for each relevant Pokémon.
    --
    local pokemon_with_size_checks = {
        pkmn.database.pokemon_entry("Barboach", "Ruby", ""),
        pkmn.database.pokemon_entry("Shroomish", "Ruby", ""),
        pkmn.database.pokemon_entry("Seedot", "Emerald", ""),
        pkmn.database.pokemon_entry("Lotad", "Emerald", ""),
        pkmn.database.pokemon_entry("Magikarp", "FireRed", ""),
        pkmn.database.pokemon_entry("Heracross", "FireRed", "")
    }

    for i = 1, #pokemon_with_size_checks
    do
        local height = pokemon_with_size_checks[i]:get_height()
        local species = pokemon_with_size_checks[i]:get_name()

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
    luaunit.assertError(pkmn.calculations.get_gb_stat, "Attack", 1, 1, 123456, 1)
    luaunit.assertError(pkmn.calculations.get_gb_stat, "Attack", 1, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
    --
    luaunit.assertAlmostEquals(pkmn.calculations.get_gb_stat("HP", 81, 35, 22850, 7), 189, 1)
    luaunit.assertAlmostEquals(pkmn.calculations.get_gb_stat("Attack", 81, 55, 23140, 8), 137, 1)
end

function test_modern_stats()
    -- Make sure expected errors are thrown.
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Not a stat", 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Special", 1, 1.0, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 0.666, 1, 1, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 1.0, 1, 12345, 1)
    luaunit.assertError(pkmn.calculations.get_modern_stat, "Attack", 1, 1.0, 1, 1, 12345)

    --
    -- Test with known good inputs.
    --
    -- Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
    --
    luaunit.assertAlmostEquals(pkmn.calculations.get_modern_stat("HP", 78, 1.0, 108, 74, 24), 289, 1)
    luaunit.assertAlmostEquals(pkmn.calculations.get_modern_stat("Attack", 78, 1.1, 130, 195, 12), 280, 1)
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
