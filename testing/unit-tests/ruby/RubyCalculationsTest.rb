#!/usr/bin/ruby
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "test/unit"

class RubyCalculationsTest < Test::Unit::TestCase

    def test_stab_modifier
        assert_equal(1.5, PKMN::Calculations.STAB_MODIFIER)
    end

    def test_brine_power
        # Test invalid inputs

        assert_raise IndexError do
            PKMN::Calculations::brine_power(0, 2)
        end
        assert_raise IndexError do
            PKMN::Calculations::brine_power(0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::brine_power(-2, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::brine_power(2, 1)
        end

        # Test expected results.

        max_hp = 10
        half_hp = max_hp / 2

        assert_equal(
            130,
            PKMN::Calculations::brine_power(half_hp-1, max_hp)
        )
        assert_equal(
            130,
            PKMN::Calculations::brine_power(half_hp, max_hp)
        )
        assert_equal(
            65,
            PKMN::Calculations::brine_power(half_hp+1, max_hp)
        )
    end

    def test_crush_grip_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::crush_grip_power(0, 1, 5)
        end
        assert_raise IndexError do
            PKMN::Calculations::crush_grip_power(2, 1, 5)
        end
        assert_raise IndexError do
            PKMN::Calculations::crush_grip_power(1, 2, 3)
        end
        assert_raise IndexError do
            PKMN::Calculations::crush_grip_power(1, 2, 7)
        end

        # Test expected results.

        assert_equal(
            1,
            PKMN::Calculations::crush_grip_power(1, 200, 4)
        )
        assert_equal(
            121,
            PKMN::Calculations::crush_grip_power(200, 200, 4)
        )

        [5, 6].each do |generation|
            assert_equal(
                1,
                PKMN::Calculations::crush_grip_power(1, 200, generation)
            )
            assert_equal(
                120,
                PKMN::Calculations::crush_grip_power(200, 200, generation)
            )
        end
    end

    def test_echoed_voice_powers
        # For some reason, the comparison only works this way in Ruby.
        assert_equal(
            PKMN::Calculations::echoed_voice_powers(),
            [40, 80, 120, 160, 200]
        )
    end

    def test_electro_ball_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::electro_ball_power(0, 100)
        end
        assert_raise IndexError do
            PKMN::Calculations::electro_ball_power(100, 0)
        end

        # Test expected results.
        attacker_speeds = [100, 100, 100, 100, 100, 100, 100, 100]
        target_speeds = [101, 100, 51, 50, 34, 33, 32, 25]
        expected_powers = [40, 60, 60, 80, 80, 120, 120, 150]
        test_cases = attacker_speeds.zip(target_speeds, expected_powers)

        test_cases.each do |attacker_speed, target_speed, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::electro_ball_power(
                    attacker_speed,
                    target_speed
                )
            )
        end
    end

    def test_eruption_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::eruption_power(0, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::eruption_power(1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::eruption_power(2, 1)
        end

        # Test expected results.

        assert_equal(1, PKMN::Calculations::eruption_power(1, 500))
        assert_equal(75, PKMN::Calculations::eruption_power(250, 500))
        assert_equal(150, PKMN::Calculations::eruption_power(500, 500))
    end

    def test_flail_power
        # Test invalid parameters.

        assert_raises IndexError do
            PKMN::Calculations::flail_power(0, 1)
        end
        assert_raises IndexError do
            PKMN::Calculations::flail_power(1, 0)
        end
        assert_raises IndexError do
            PKMN::Calculations::flail_power(2, 1)
        end

        # Test expected results.

        attacker_current_HPs = [1, 20, 21, 52, 53, 104, 105, 177, 178, 343, 344, 500]
        attacker_max_HPs = [500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500]
        expected_powers = [200, 200, 150, 150, 100, 100, 80, 80, 40, 40, 20, 20]
        test_cases = attacker_current_HPs.zip(attacker_max_HPs, expected_powers)

        test_cases.each do |attacker_current_hp, attacker_max_hp, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::flail_power(
                    attacker_current_hp,
                    attacker_max_hp
                )
            )
        end
    end

    def test_fling_power
        # Test invalid parameters.

        assert_raise ArgumentError do
            PKMN::Calculations::fling_power(PKMN::Item::NONE)
        end

        # Test expected results.

        items =
        [
            PKMN::Item::ORAN_BERRY,
            PKMN::Item::HEALTH_WING,
            PKMN::Item::POTION,
            PKMN::Item::ICY_ROCK,
            PKMN::Item::DUBIOUS_DISC,
            PKMN::Item::DAMP_ROCK,
            PKMN::Item::DRAGON_FANG,
            PKMN::Item::DUSK_STONE,
            PKMN::Item::THICK_CLUB,
            PKMN::Item::RARE_BONE,
            PKMN::Item::IRON_BALL
        ]
        expected_powers = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 130]
        test_cases = items.zip(expected_powers)

        test_cases.each do |item_name, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::fling_power(item_name)
            )
        end
    end

    def test_frustration_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::frustration_power(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::frustration_power(256)
        end

        # Test expected results.

        assert_equal(102, PKMN::Calculations::frustration_power(0))
        assert_equal(1, PKMN::Calculations::frustration_power(255))
    end

    def test_fury_cutter_powers
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::fury_cutter_powers(1)
        end
        assert_raise IndexError do
            PKMN::Calculations::fury_cutter_powers(10)
        end

        # Test expected results.

        expected_gen_2_4_powers = [10, 20, 40, 80, 160]
        expected_gen5_powers = [20, 40, 80, 160]
        expected_gen6_powers = [40, 80, 160]

        # For some reason, the comparison only works one way in Ruby.

        (2..4).each do |generation|
            assert_equal(
                PKMN::Calculations::fury_cutter_powers(generation),
                expected_gen_2_4_powers
            )
        end

        assert_equal(
            PKMN::Calculations::fury_cutter_powers(5),
            expected_gen5_powers
        )
        assert_equal(
            PKMN::Calculations::fury_cutter_powers(6),
            expected_gen6_powers
        )
    end

    def test_grass_knot_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::grass_knot_power(-1.0)
        end

        # Test expected results.

        target_weights = [5.0, 15.0, 35.0, 75.0, 150.0, 250.0]
        expected_powers = [20, 40, 60, 80, 100, 120]
        test_cases = target_weights.zip(expected_powers)

        test_cases.each do |target_weight, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::grass_knot_power(target_weight)
            )
        end
    end

    def test_gyro_ball_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::gyro_ball_power(0, 50)
        end
        assert_raise IndexError do
            PKMN::Calculations::gyro_ball_power(50, 0)
        end

        # Test expected results.

        assert_equal(150, PKMN::Calculations::gyro_ball_power(1, 500))
        assert_equal(50, PKMN::Calculations::gyro_ball_power(100, 200))
        assert_equal(1, PKMN::Calculations::gyro_ball_power(500, 1))
    end

    def test_heat_crash_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::heat_crash_power(0.0, 1.0)
        end
        assert_raise IndexError do
            PKMN::Calculations::heat_crash_power(1.0, 0.0)
        end

        # Test expected results.

        attacker_weights = [200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0]
        target_weights = [200.0, 100.1, 100.0,
                          66.7, 66.6, 50.1,
                          50.0, 40.1, 40.0]
        expected_powers = [40, 40, 60,
                           60, 80, 80,
                           100, 100, 120]
        test_cases = attacker_weights.zip(target_weights, expected_powers)

        test_cases.each do |attacker_weight, target_weight, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::heat_crash_power(
                    attacker_weight,
                    target_weight
                )
            )
        end
    end

    def test_heavy_slam_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::heavy_slam_power(0.0, 1.0)
        end
        assert_raise IndexError do
            PKMN::Calculations::heavy_slam_power(1.0, 0.0)
        end

        # Test expected results.

        attacker_weights = [200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0]
        target_weights = [200.0, 100.1, 100.0,
                          66.7, 66.6, 50.1,
                          50.0, 40.1, 40.0]
        expected_powers = [40, 40, 60,
                           60, 80, 80,
                           100, 100, 120]
        test_cases = attacker_weights.zip(target_weights, expected_powers)

        test_cases.each do |attacker_weight, target_weight, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::heavy_slam_power(
                    attacker_weight,
                    target_weight
                )
            )
        end
    end

    def test_ice_ball_powers
        # For some reason, the comparison only works this way in Ruby.
        assert_equal(
            PKMN::Calculations::ice_ball_powers(),
            [30, 60, 120, 240, 480]
        )
    end

    def test_low_kick_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::low_kick_power(-1.0, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::low_kick_power(1.0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::low_kick_power(1.0, 10)
        end

        # Test expected results.

        target_weights = [0.1, 9.9, 10.0, 24.9, 25.0, 49.9,
                          50.0, 99.9, 100.0, 199.9, 200.0, 1000.0];
        expected_powers = [20, 20, 40, 40, 60, 60,
                           80, 80, 100, 100, 120, 120];
        test_cases = target_weights.zip(expected_powers)

        test_cases.each do |target_weight, expected_power|
            (1..6).each do |generation|
                if generation <= 2
                    # Generation I has only one set power, regardless of
                    # weight.
                    assert_equal(
                        50,
                        PKMN::Calculations::low_kick_power(
                            target_weight,
                            generation
                        )
                    )
                else
                    assert_equal(
                        expected_power,
                        PKMN::Calculations::low_kick_power(
                            target_weight,
                            generation
                        )
                    )
                end
            end
        end
    end

    def test_power_trip_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(-1, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(7, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, -1, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 7, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, -1, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 7, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, -1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 7, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 7, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 0, 7, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 0, 0, 7)
        end

        # Test expected results.

        assert_equal(
            20,
            PKMN::Calculations::power_trip_power(0, 0, 0, 0, 0, 0, 0)
        )
        assert_equal(
            440,
            PKMN::Calculations::power_trip_power(0, 1, 2, 3, 4, 5, 6)
        )
        assert_equal(
            860,
            PKMN::Calculations::power_trip_power(6, 6, 6, 6, 6, 6, 6)
        )
    end

    def test_punishment_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::punishment_power(-1, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(7, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, -1, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 7, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, -1, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 7, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, -1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 7, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 7, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 0, 7, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 0, 0, 7)
        end

        # Test expected results.

        assert_equal(
            60,
            PKMN::Calculations::punishment_power(0, 0, 0, 0, 0, 0, 0)
        )
        assert_equal(
            200,
            PKMN::Calculations::punishment_power(0, 1, 2, 3, 4, 5, 6)
        )
        assert_equal(
            200,
            PKMN::Calculations::punishment_power(6, 6, 6, 6, 6, 6, 6)
        )
    end

    def test_return_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::return_power(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::return_power(256)
        end

        # Test expected results.

        assert_equal(1, PKMN::Calculations::return_power(0))
        assert_equal(102, PKMN::Calculations::return_power(255))
    end

    def test_reversal_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::reversal_power(0, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::reversal_power(1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::reversal_power(2, 1)
        end

        # Test expected results.

        attacker_current_HPs = [1, 20, 21, 52, 53, 104,
                                105, 177, 178, 343, 344, 500]
        attacker_max_HPs = [500, 500, 500, 500, 500, 500,
                            500, 500, 500, 500, 500, 500]
        expected_powers = [200, 200, 150, 150, 100, 100,
                           80, 80, 40, 40, 20, 20]
        test_cases = attacker_current_HPs.zip(attacker_max_HPs, expected_powers)

        test_cases.each do |attacker_current_hp, attacker_max_hp, expected_power|
            assert_equal(
                expected_power,
                PKMN::Calculations::reversal_power(
                    attacker_current_hp,
                    attacker_max_hp
                )
            )
        end
    end

    def test_rollout_powers
        # For some reason, the comparison only works this direction in Ruby.
        assert_equal(
            PKMN::Calculations::rollout_powers(),
            [30, 60, 120, 240, 480]
        )
    end

    def test_spit_up_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::spit_up_power(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::spit_up_power(4)
        end

        # Test expected results.

        assert_equal(0, PKMN::Calculations::spit_up_power(0))
        assert_equal(100, PKMN::Calculations::spit_up_power(1))
        assert_equal(200, PKMN::Calculations::spit_up_power(2))
        assert_equal(300, PKMN::Calculations::spit_up_power(3))
    end

    def test_stored_power_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(-1, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(7, 0, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, -1, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 7, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, -1, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 7, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, -1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 7, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 7, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 0, 7, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 0, 0, 7)
        end

        # Test expected results.

        assert_equal(
            20,
            PKMN::Calculations::stored_power_power(0, 0, 0, 0, 0, 0, 0)
        )
        assert_equal(
            440,
            PKMN::Calculations::stored_power_power(0, 1, 2, 3, 4, 5, 6)
        )
        assert_equal(
            860,
            PKMN::Calculations::stored_power_power(6, 6, 6, 6, 6, 6, 6)
        )
    end

    def test_triple_kick_powers
        assert_equal(
            PKMN::Calculations::triple_kick_powers(),
            [10, 20, 30]
        )
    end

    def test_trump_card_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::trump_card_power(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::trump_card_power(9)
        end

        # Text expected results.

        expected_results = [200, 80, 60, 50, 40, 40, 40, 40, 40]

        (0..(expected_results.length-1)).each do |remaining_pp|
            assert_equal(
                expected_results[remaining_pp],
                PKMN::Calculations::trump_card_power(remaining_pp)
            )
        end
    end

    def test_water_spout_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::water_spout_power(0, 1)
        end
        assert_raise IndexError do
            PKMN::Calculations::water_spout_power(1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::water_spout_power(2, 1)
        end

        # Test expected results.

        assert_equal(1, PKMN::Calculations::water_spout_power(1, 500))
        assert_equal(75, PKMN::Calculations::water_spout_power(250, 500))
        assert_equal(150, PKMN::Calculations::water_spout_power(500, 500))
    end

    def test_wring_out_power
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::wring_out_power(0, 1, 5)
        end
        assert_raise IndexError do
            PKMN::Calculations::wring_out_power(2, 1, 5)
        end
        assert_raise IndexError do
            PKMN::Calculations::wring_out_power(1, 2, 3)
        end
        assert_raise IndexError do
            PKMN::Calculations::wring_out_power(1, 2, 7)
        end

        # Test expected results.

        assert_equal(
            1,
            PKMN::Calculations::wring_out_power(1, 200, 4)
        )
        assert_equal(
            121,
            PKMN::Calculations::wring_out_power(200, 200, 4)
        )

        (5..6).each do |generation|
            assert_equal(
                1,
                PKMN::Calculations::wring_out_power(1, 200, generation)
            )
            assert_equal(
                120,
                PKMN::Calculations::wring_out_power(200, 200, generation)
            )
        end
    end

    def test_gen1_critical_hit_chance
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::gen1_critical_hit_chance(0, false, false)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen1_critical_hit_chance(256, false, false)
        end

        # In Generation I, make sure there is never a guaranteed chance for
        # a critical hit.

        assert_operator(
            PKMN::Calculations::gen1_critical_hit_chance(255, true, true),
            :<=,
            (255.0 / 256.0)
        )
    end

    def test_critical_hit_chance
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::critical_hit_chance(1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::critical_hit_chance(10, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::critical_hit_chance(2, -1)
        end

        # Test expected values.

        generations = [2, 2, 2, 2, 2,
                       3, 3, 3, 3, 3,
                       4, 4, 4, 4, 4,
                       5, 5, 5, 5, 5,
                       6, 6, 6, 6, 6]
        stages = [0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4]
        expected_results = [0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.5,  1.0,   1.0]
        test_cases = generations.zip(stages, expected_results)

        test_cases.each do |generation, stage, expected_result|
            assert_in_delta(
                expected_result,
                PKMN::Calculations::critical_hit_chance(generation, stage),
                0.0001
            )
        end
    end

    def test_gen1_critical_hit_modifier
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::gen1_critical_hit_modifier(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen1_critical_hit_modifier(256)
        end

        # Test expected results.

        assert_in_delta(
            1.5,
            PKMN::Calculations::gen1_critical_hit_modifier(5),
            0.0001
        )
        assert_in_delta(
            1.8,
            PKMN::Calculations::gen1_critical_hit_modifier(20),
            0.0001
        )
        assert_in_delta(
            1.95,
            PKMN::Calculations::gen1_critical_hit_modifier(95),
            0.0001
        )
    end

    def test_critical_hit_modifier
        # Test invalid parameters.

        assert_raise IndexError do
            PKMN::Calculations::critical_hit_modifier(-1)
        end
        assert_raise IndexError do
            PKMN::Calculations::critical_hit_modifier(10)
        end

        # Test expected results.

        (2..5).each do |generation|
            assert_in_delta(
                2.0,
                PKMN::Calculations::critical_hit_modifier(generation),
                0.0001
            )
        end

        assert_in_delta(
            1.5,
            PKMN::Calculations::critical_hit_modifier(6),
            0.0001
        )
    end

    def test_damage
        # Source: https://bulbapedia.bulbagarden.net/wiki/Damage#Example

        # Only taking types into account
        #
        # "Imagine a level 75 Glaceon...with an effective Attack stat of 123
        # uses Ice Fang (an Ice-type physical move with a power of 65) against
        # a Garchomp with an effective Defense stat of 163 in Generation VI,
        # and does not land a critical hit."
        #
        # The article itself results in the wrong value, but the value I'm
        # testing for below was based on its equations.

        level = 75
        ice_fang_base_power = 65
        modifier_against_dragon_ground = 6.0
        glaceon_l75_attack = 123
        garchomp_l75_defense = 163

        assert_equal(
            200,
            PKMN::Calculations::damage(
                level,
                ice_fang_base_power,
                glaceon_l75_attack,
                garchomp_l75_defense,
                modifier_against_dragon_ground
            )
        )
    end

    def test_type_damage_modifier
        # Test invalid inputs.

        # Invalid generation
        assert_raise IndexError do
            PKMN::Calculations::type_damage_modifier(
                -1,
                PKMN::Type::NORMAL,
                PKMN::Type::NORMAL
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::type_damage_modifier(
                8,
                PKMN::Type::NORMAL,
                PKMN::Type::NORMAL
            )
        end

        # Invalid types for a given generation
        generations = [1, 1, 5, 3, 5, 2, 4]
        types =
        [
            PKMN::Type::DARK,
            PKMN::Type::STEEL,
            PKMN::Type::FAIRY,
            PKMN::Type::QUESTION_MARK,
            PKMN::Type::QUESTION_MARK,
            PKMN::Type::SHADOW,
            PKMN::Type::SHADOW
        ]
        test_cases = generations.zip(types)

        test_cases.each do |generation, type|
            # Invalid attacking type
            assert_raise ArgumentError do
                PKMN::Calculations::type_damage_modifier(
                    generation,
                    type,
                    PKMN::Type::NORMAL
                )
            end

            # Invalid defending type
            assert_raise ArgumentError do
                PKMN::Calculations::type_damage_modifier(
                    generation,
                    PKMN::Type::NORMAL,
                    type
                )
            end
        end

        # Check that changes between generations are properly implemented.

        attacking_types =
        [
            PKMN::Type::BUG,
            PKMN::Type::POISON,
            PKMN::Type::GHOST,
            PKMN::Type::ICE,
            PKMN::Type::GHOST,
            PKMN::Type::DARK
        ]
        defending_types =
        [
            PKMN::Type::POISON,
            PKMN::Type::BUG,
            PKMN::Type::PSYCHIC,
            PKMN::Type::FIRE,
            PKMN::Type::STEEL,
            PKMN::Type::STEEL
        ]
        old_generations = [1, 1, 1, 1, 5, 5]
        old_modifiers = [2.0, 2.0, 0.0, 1.0, 0.5, 0.5]
        new_generations = [2, 2, 2, 2, 6, 6]
        new_modifiers = [0.5, 1.0, 2.0, 0.5, 1.0, 1.0]
        test_cases = attacking_types.zip(defending_types, old_generations,
                                         old_modifiers, new_generations,
                                         new_modifiers)

        test_cases.each do |attacking_type, defending_type, old_generation,
                            old_modifier, new_generation, new_modifier|
            assert_in_delta(
                old_modifier,
                PKMN::Calculations::type_damage_modifier(
                    old_generation,
                    attacking_type,
                    defending_type
                )
            )

            assert_in_delta(
                new_modifier,
                PKMN::Calculations::type_damage_modifier(
                    new_generation,
                    attacking_type,
                    defending_type
                )
            )
        end
    end

    def test_gen2_unown_forms
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_unown_form(0, 0, 0, 16)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
        #
        assert_equal("G", PKMN::Calculations::gen2_unown_form(10, 9, 1, 14))
        assert_equal("S", PKMN::Calculations::gen2_unown_form(5, 15, 10, 5))
    end

    def test_gen3_unown_forms
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::gen3_unown_form(-1)
        end
        assert_raise RangeError do
            PKMN::Calculations::gen3_unown_form(0xFFFFFFFF+1)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
        #
        assert_equal("B", PKMN::Calculations::gen3_unown_form(0x4C07DE71))
    end

    def test_wurmple_becomes_silcoon
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::wurmple_becomes_silcoon(-1, false)
        end
        assert_raise RangeError do
            PKMN::Calculations::wurmple_becomes_silcoon(0xFFFFFFFF+1, false)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
        #
        assert(PKMN::Calculations::wurmple_becomes_silcoon(0x5CF4091C, true))
        assert_equal(false, PKMN::Calculations::wurmple_becomes_silcoon(0x091C5CF4, true))
        assert(PKMN::Calculations::wurmple_becomes_silcoon(0x091C5CF4, false))
        assert_equal(false, PKMN::Calculations::wurmple_becomes_silcoon(0x5CF4091C, false))
    end

    def test_gen2_gender
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::gen2_pokemon_gender(
                PKMN::Species::NONE,
                0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_pokemon_gender(
                PKMN::Species::BULBASAUR,
                -1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_pokemon_gender(
                PKMN::Species::BULBASAUR,
                16
            )
        end

        # Make sure known good inputs result in expected results.

        # All male
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::NIDORINO, 0)
        )
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::NIDORINO, 15)
        )

        # 25% male, 75% female
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::VULPIX, 11)
        )
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::VULPIX, 12)
        )

        # All female
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::NIDORINA, 0)
        )
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::NIDORINA, 15)
        )

        # Genderless
        assert_equal(
            PKMN::Gender::GENDERLESS,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::MAGNEMITE, 0)
        )
        assert_equal(
            PKMN::Gender::GENDERLESS,
            PKMN::Calculations::gen2_pokemon_gender(PKMN::Species::MAGNEMITE, 15)
        )
    end

    def test_modern_gender
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::modern_pokemon_gender(PKMN::Species::NONE, 0)
        end

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::modern_pokemon_gender(PKMN::Species::BULBASAUR, -1)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::BULBASAUR,
                0xFFFFFFFF+1
            )
        end

        # All male
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::NIDORINO,
                0
            )
        )
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::NIDORINO,
                0xFFFFFFFF
            )
        )

        # 25% male, 75% female
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::VULPIX,
                190
            )
        )
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::VULPIX,
                191
            )
        )

        # All female
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::NIDORINA,
                0
            )
        )
        assert_equal(
            PKMN::Gender::FEMALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::NIDORINA,
                0xFFFFFFFF
            )
        )

        # Genderless
        assert_equal(
            PKMN::Gender::GENDERLESS,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::MAGNEMITE,
                0
            )
        )
        assert_equal(
            PKMN::Gender::GENDERLESS,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::MAGNEMITE,
                0xFFFFFFFF
            )
        )
    end

    def test_gen2_hidden_power
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_hidden_power(0, 0, 0, 16)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
        #
        expected_hidden_power = PKMN::Calculations::HiddenPower.new(
                                    PKMN::Type::DARK,
                                    69
                                )
        hidden_power = PKMN::Calculations::gen2_hidden_power(15, 15, 15, 14)
        assert_equal(expected_hidden_power, hidden_power)
    end

    def test_modern_hidden_power
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(-1, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(32, 0, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, -1, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 32, 0, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, -1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 32, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 32, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 32, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::modern_hidden_power(0, 0, 0, 0, 0, 32)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
        #
        expected_hidden_power = PKMN::Calculations::HiddenPower.new(
                                    PKMN::Type::GRASS,
                                    70
                                )
        hidden_power = PKMN::Calculations::modern_hidden_power(30, 31, 31, 31, 30, 31)
        assert_equal(expected_hidden_power, hidden_power)
    end

    def test_natural_gift
        # Test invalid generations.

        assert_raise IndexError do
            PKMN::Calculations::natural_gift_stats(PKMN::Item::CHERI_BERRY, 3)
        end
        assert_raise IndexError do
            PKMN::Calculations::natural_gift_stats(PKMN::Item::CHERI_BERRY, 10)
        end

        # Test invalid items.

        assert_raise ArgumentError do
            PKMN::Calculations::natural_gift_stats(PKMN::Item::POTION, 4)
        end

        # Test expected values.

        items = [PKMN::Item::CHERI_BERRY, PKMN::Item::NANAB_BERRY, PKMN::Item::BELUE_BERRY]
        types = [PKMN::Type::FIRE, PKMN::Type::WATER, PKMN::Type::ELECTRIC]
        gen4_powers = [60, 70, 80]
        gen5_powers = [60, 70, 80]
        gen6_powers = [80, 90, 100]
        test_cases = items.zip(types, gen4_powers, gen5_powers, gen6_powers)

        test_cases.each do |item, type, gen4_power, gen5_power, gen6_power|
            natural_gift = PKMN::Calculations::natural_gift_stats(item, 4)
            assert_equal(type, natural_gift.type)
            assert_equal(gen4_power, natural_gift.base_power)

            natural_gift = PKMN::Calculations::natural_gift_stats(item, 5)
            assert_equal(type, natural_gift.type)
            assert_equal(gen5_power, natural_gift.base_power)

            natural_gift = PKMN::Calculations::natural_gift_stats(item, 6)
            assert_equal(type, natural_gift.type)
            assert_equal(gen6_power, natural_gift.base_power)
        end
    end

    def test_nature
        natures =
        [
            PKMN::Nature::HARDY,
            PKMN::Nature::LONELY,
            PKMN::Nature::BRAVE,
            PKMN::Nature::ADAMANT,
            PKMN::Nature::NAUGHTY,
            PKMN::Nature::BOLD,
            PKMN::Nature::DOCILE,
            PKMN::Nature::RELAXED,
            PKMN::Nature::IMPISH,
            PKMN::Nature::LAX,
            PKMN::Nature::TIMID,
            PKMN::Nature::HASTY,
            PKMN::Nature::SERIOUS,
            PKMN::Nature::JOLLY,
            PKMN::Nature::NAIVE,
            PKMN::Nature::MODEST,
            PKMN::Nature::MILD,
            PKMN::Nature::QUIET,
            PKMN::Nature::BASHFUL,
            PKMN::Nature::RASH,
            PKMN::Nature::CALM,
            PKMN::Nature::GENTLE,
            PKMN::Nature::SASSY,
            PKMN::Nature::CAREFUL,
            PKMN::Nature::QUIRKY
        ]

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::nature(-1)
        end
        assert_raise RangeError do
            PKMN::Calculations::nature(0xFFFFFFFF+1)
        end

        (0..natures.length-1).each do |i|
            assert_equal(
                natures[i],
                PKMN::Calculations::nature((rand(0...50000) * 1000) + i)
            )
        end
    end

    #
    # Given the amount of time the C++ test takes, this will just verify
    # the API wrapper.
    #
    def test_personality
        # Test invalid ability.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                PKMN::Species::CHARMANDER,
                PKMN::Pokemon.DEFAULT_TRAINER_ID,
                true,
                PKMN::Ability::TORRENT,
                PKMN::Gender::MALE,
                PKMN::Nature::QUIET
            )
        end

        # Test invalid gender.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                PKMN::Species::CHARMANDER,
                PKMN::Pokemon.DEFAULT_TRAINER_ID,
                true,
                PKMN::Ability::BLAZE,
                PKMN::Gender::NONE,
                PKMN::Nature::QUIET
            )
        end

        # Test invalid nature.
        assert_raise ArgumentError do
            PKMN::Calculations::generate_personality(
                PKMN::Species::CHARMANDER,
                PKMN::Pokemon.DEFAULT_TRAINER_ID,
                true,
                PKMN::Ability::BLAZE,
                PKMN::Gender::MALE,
                PKMN::Nature::NONE
            )
        end

        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::generate_personality(
                PKMN::Species::CHARMANDER,
                -1,
                true,
                PKMN::Ability::BLAZE,
                PKMN::Gender::MALE,
                PKMN::Nature::QUIET
            )
        end
        assert_raise RangeError do
            PKMN::Calculations::generate_personality(
                PKMN::Species::CHARMANDER,
                0xFFFFFFFF+1,
                true,
                PKMN::Ability::BLAZE,
                PKMN::Gender::MALE,
                PKMN::Nature::QUIET
            )
        end

        # Test and validate a valid call.
        personality = PKMN::Calculations::generate_personality(
                          PKMN::Species::CHARMANDER,
                          PKMN::Pokemon.DEFAULT_TRAINER_ID,
                          true,
                          PKMN::Ability::BLAZE,
                          PKMN::Gender::MALE,
                          PKMN::Nature::QUIET
                      )
        assert(
            PKMN::Calculations::modern_shiny(
                personality, PKMN::Pokemon.DEFAULT_TRAINER_ID
            )
        )
        assert_equal(
            PKMN::Gender::MALE,
            PKMN::Calculations::modern_pokemon_gender(
                PKMN::Species::CHARMANDER,
                personality
            )
        )
    end

    def test_gen2_shiny
        # Make sure expected errors are raised.
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(-1, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(16, 0, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, -1, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 16, 0, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, -1, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 16, 0)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 0, -1)
        end
        assert_raise IndexError do
            PKMN::Calculations::gen2_shiny(0, 0, 0, 16)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
        #
        assert(PKMN::Calculations::gen2_shiny(7, 10, 10, 10))
        assert_equal(false, PKMN::Calculations::gen2_shiny(6, 15, 7, 15))
    end

    def test_modern_shiny
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(-1, 0)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0xFFFFFFFF+1, 0)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0, -1)
        end
        assert_raise RangeError do
            PKMN::Calculations::modern_shiny(0, 0xFFFFFFFF+1)
        end

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
        #         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
        #
        assert(PKMN::Calculations::modern_shiny(2814471828, 2545049318))
        assert(PKMN::Calculations::modern_shiny(0xB58F0B2A, 398174488))
    end

    def test_pokemon_size
        # Make sure SWIG+Ruby catches values outside the uint32_t bounds.
        assert_raise RangeError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                -1, 0, 0, 0, 0, 0, 0
            )
        end
        assert_raise RangeError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
            )
        end

        # Test input validation.
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, -1, 0, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 32, 0, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, -1, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 32, 0, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, -1, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 32, 0, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, -1, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, 32, 0, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, 0, -1, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, 0, 32, 0
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, 0, 0, -1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::pokemon_size(
                PKMN::Species::MAGIKARP,
                0, 0, 0, 0, 0, 0, 32
            )
        end

        # There are no known good calculations, so just check for reasonable values
        # for each relevant Pokemon.
        pokemon_with_size_checks = [
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::BARBOACH,
                PKMN::Game::RUBY,
                ""
            ),
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::SHROOMISH,
                PKMN::Game::RUBY,
                ""
            ),
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::SEEDOT,
                PKMN::Game::EMERALD,
                ""
            ),
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::LOTAD,
                PKMN::Game::EMERALD,
                ""
            ),
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::MAGIKARP,
                PKMN::Game::FIRERED,
                ""
            ),
            PKMN::Database::PokemonEntry.new(
                PKMN::Species::HERACROSS,
                PKMN::Game::FIRERED,
                ""
            )
        ]

        (0..pokemon_with_size_checks.length-1).each do |i|
            height = pokemon_with_size_checks[i].height
            species = pokemon_with_size_checks[i].species

            (0..9).each do
                size = PKMN::Calculations::pokemon_size(
                           species,
                           rand(0...0xFFFFFF),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31),
                           rand(0...31)
                       )
                assert((size-height).abs <= height)
            end
        end
    end

    def test_spinda_coords
        # Check (in)equality operators.
        spinda_coords1 = PKMN::Calculations::SpindaCoords.new(123, 456)
        spinda_coords2 = PKMN::Calculations::SpindaCoords.new(123, 456)
        spinda_coords3 = PKMN::Calculations::SpindaCoords.new(456, 123)

        assert_equal(spinda_coords1, spinda_coords2)
        assert_not_equal(spinda_coords1, spinda_coords3)
    end

    def test_spinda_spots
        # Check comparisons.
        spots1 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(1, 4),
                     PKMN::Calculations::SpindaCoords.new(2, 15)
                 )
        spots2 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(1, 4),
                     PKMN::Calculations::SpindaCoords.new(2, 15)
                 )
        spots3 = PKMN::Calculations::SpindaSpots.new(
                     PKMN::Calculations::SpindaCoords.new(7, 5),
                     PKMN::Calculations::SpindaCoords.new(10, 0),
                     PKMN::Calculations::SpindaCoords.new(2, 15),
                     PKMN::Calculations::SpindaCoords.new(1, 4)
                 )
        assert_equal(spots1, spots2)
        assert_not_equal(spots1, spots3)

        #
        # Make sure known good inputs result in expected results, and test comparisons.
        #
        # Source: https://github.com/magical/spinda
        #
        spots = PKMN::Calculations::spinda_spot_offset(4064348759)
        assert_equal(spots1, spots)
    end

    def assert_ints_almost_equal(a,b)
        return (a == b or (a-1) == b or (a+1) == b)
    end

    def test_gb_stats
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::get_gb_stat(
                PKMN::Stat::NONE,
                1, 1, 1, 1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::get_gb_stat(
                PKMN::Stat::ATTACK,
                1, 1, 123456, 1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::get_gb_stat(
                PKMN::Stat::ATTACK,
                1, 1, 1, 12345
            )
        end

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
        #
        stat = PKMN::Calculations::get_gb_stat(
            PKMN::Stat::HP,
            81, 35, 22850, 7
        )
        assert_ints_almost_equal(stat, 189)
        stat = PKMN::Calculations::get_gb_stat(
            PKMN::Stat::ATTACK,
            81, 55, 23140, 8
        )
        assert_ints_almost_equal(stat, 137)
    end

    def test_modern_stats
        # Make sure expected errors are raised.
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat(
                PKMN::Stat::NONE,
                1, 1.0, 1, 1, 1
            )
        end
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat(
                PKMN::Stat::SPECIAL,
                1, 1.0, 1, 1, 1
            )
        end
        assert_raise ArgumentError do
            PKMN::Calculations::get_modern_stat(
                PKMN::Stat::ATTACK,
                1, 0.666, 1, 1, 1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::get_modern_stat(
                PKMN::Stat::ATTACK,
                1, 1.0, 1, 12345, 1
            )
        end
        assert_raise IndexError do
            PKMN::Calculations::get_modern_stat(
                PKMN::Stat::ATTACK,
                1, 1.0, 1, 1, 12345
            )
        end

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
        #
        stat = PKMN::Calculations::get_modern_stat(
                   PKMN::Stat::HP,
                   78, 1.0, 108, 74, 24
               )
        assert_ints_almost_equal(stat, 289)
        stat = PKMN::Calculations::get_modern_stat(
                   PKMN::Stat::ATTACK,
                   78, 1.1, 130, 195, 12
               )
        assert_ints_almost_equal(stat, 280)
    end
end
