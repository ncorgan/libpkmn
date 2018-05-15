/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpCalculationsTest {

    private static Random rng = new Random();

    [Test]
    public void STABModifierTest()
    {
        Assert.AreEqual(1.5f, PKMN.Calculations.STABModifier);
    }

    [Test]
    public void BrinePowerTest()
    {
        // Test invalid inputs.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.BrinePower(0, 2);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.BrinePower(0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.BrinePower(-2, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.BrinePower(2, 1);
            }
        );

        const int maxHP = 10;
        const int halfHP = maxHP / 2;

        // Test expected behavior.

        Assert.AreEqual(130, PKMN.Calculations.BrinePower(halfHP - 1, maxHP));
        Assert.AreEqual(130, PKMN.Calculations.BrinePower(halfHP, maxHP));
        Assert.AreEqual(65, PKMN.Calculations.BrinePower(halfHP + 1, maxHP));
    }

    [Test]
    public void CrushGripPowerTest()
    {
        // Test invalid inputs.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CrushGripPower(0, 1, 5);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CrushGripPower(2, 1, 5);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CrushGripPower(1, 2, 3);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CrushGripPower(1, 2, 7);
            }
        );

        // Test expected results.

        Assert.AreEqual(1, PKMN.Calculations.CrushGripPower(1, 200, 4));
        Assert.AreEqual(121, PKMN.Calculations.CrushGripPower(200, 200, 4));

        for(int generation = 5; generation <= 6; ++generation)
        {
            Assert.AreEqual(
                1,
                PKMN.Calculations.CrushGripPower(1, 200, generation)
            );
            Assert.AreEqual(
                120,
                PKMN.Calculations.CrushGripPower(200, 200, generation)
            );
        }
    }

    [Test]
    public void EchoedVoicePowersTest()
    {
        int[] expectedResults = {40, 80, 120, 160, 200};

        PKMN.IntList echoedVoicePowers = PKMN.Calculations.EchoedVoicePowers();
        int[] resultsFromFunction = new int[echoedVoicePowers.Count];
        echoedVoicePowers.CopyTo(resultsFromFunction, 0);

        Assert.AreEqual(expectedResults, resultsFromFunction);
    }

    [Test]
    public void ElectroBallPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ElectroBallPower(0, 100);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ElectroBallPower(100, 0);
            }
        );

        // Test expected results.

        int[] attackerSpeeds = {100, 100, 100, 100, 100, 100, 100, 100};
        int[] targetSpeeds = {101, 100, 51, 50, 34, 33, 32, 25};
        int[] expectedPowers = {40, 60, 60, 80, 80, 120, 120, 150};

        for(int testCaseIndex = 0; testCaseIndex < attackerSpeeds.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.ElectroBallPower(
                    attackerSpeeds[testCaseIndex],
                    targetSpeeds[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void EruptionPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.EruptionPower(0, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.EruptionPower(1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.EruptionPower(2, 1);
            }
        );

        // Test expected results.

        Assert.AreEqual(1, PKMN.Calculations.EruptionPower(1, 500));
        Assert.AreEqual(75, PKMN.Calculations.EruptionPower(250, 500));
        Assert.AreEqual(150, PKMN.Calculations.EruptionPower(500, 500));
    }

    [Test]
    public void FlailPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FlailPower(0, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FlailPower(1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FlailPower(2, 1);
            }
        );

        // Test expected results.

        int[] attackerCurrentHPs = {1, 20, 21, 52, 53, 104, 105, 177, 178, 343, 344, 500};
        int[] attackerMaxHPs = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
        int[] expectedPowers = {200, 200, 150, 150, 100, 100, 80, 80, 40, 40, 20, 20};

        for(int testCaseIndex = 0; testCaseIndex < attackerCurrentHPs.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.FlailPower(
                    attackerCurrentHPs[testCaseIndex],
                    attackerMaxHPs[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void FlingPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FlingPower("Not an item");
            }
        );

        string[] itemNames =
        {
            "Oran Berry", "Health Wing", "Potion",
            "Icy Rock", "Dubious Disc", "Damp Rock",
            "Dragon Fang", "Dusk Stone", "Thick Club",
            "Rare Bone", "Iron Ball"
        };
        int[] expectedPowers = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 130};

        for(int testCaseIndex = 0; testCaseIndex < itemNames.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.FlingPower(itemNames[testCaseIndex])
            );
        }
    }

    [Test]
    public void FrustrationPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FrustrationPower(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FrustrationPower(256);
            }
        );

        // Test expected values.

        Assert.AreEqual(102, PKMN.Calculations.FrustrationPower(0));
        Assert.AreEqual(1, PKMN.Calculations.FrustrationPower(255));
    }

    [Test]
    public void FuryCutterPowersTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FuryCutterPowers(1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.FuryCutterPowers(10);
            }
        );

        // Test expected results.

        int[] expectedGen_2_4_Powers = {10, 20, 40, 80, 160};
        int[] expectedGen5Powers = {20, 40, 80, 160};
        int[] expectedGen6Powers = {40, 80, 160};

        PKMN.IntList furyCutterPowers;
        int[] resultsFromFunction;

        for(int generation = 2; generation <= 4; ++generation)
        {
            furyCutterPowers = PKMN.Calculations.FuryCutterPowers(generation);
            resultsFromFunction = new int[furyCutterPowers.Count];
            furyCutterPowers.CopyTo(resultsFromFunction, 0);
            Assert.AreEqual(expectedGen_2_4_Powers, resultsFromFunction);
        }

        furyCutterPowers = PKMN.Calculations.FuryCutterPowers(5);
        resultsFromFunction = new int[furyCutterPowers.Count];
        furyCutterPowers.CopyTo(resultsFromFunction, 0);
        Assert.AreEqual(expectedGen5Powers, resultsFromFunction);

        furyCutterPowers = PKMN.Calculations.FuryCutterPowers(6);
        resultsFromFunction = new int[furyCutterPowers.Count];
        furyCutterPowers.CopyTo(resultsFromFunction, 0);
        Assert.AreEqual(expectedGen6Powers, resultsFromFunction);
    }

    [Test]
    public void GrassKnotPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.GrassKnotPower(-1.0f);
            }
        );

        // Test expected results.

        float[] targetWeights = {5.0f, 15.0f, 35.0f, 75.0f, 150.0f, 250.0f};
        int[] expectedPowers = {20, 40, 60, 80, 100, 120};

        for(int testCaseIndex = 0; testCaseIndex < targetWeights.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.GrassKnotPower(targetWeights[testCaseIndex])
            );
        }
    }

    [Test]
    public void GyroBallPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.GyroBallPower(0, 50);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.GyroBallPower(50, 0);
            }
        );

        // Test expected values.

        Assert.AreEqual(150, PKMN.Calculations.GyroBallPower(1, 500));
        Assert.AreEqual(50, PKMN.Calculations.GyroBallPower(100, 200));
        Assert.AreEqual(1, PKMN.Calculations.GyroBallPower(500, 1));
    }

    [Test]
    public void HeatCrashPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.HeatCrashPower(0.0f, 1.0f);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.HeatCrashPower(1.0f, 0.0f);
            }
        );

        // Test expected values.

        float[] attackerWeights =
        {
            200.0f, 200.0f, 200.0f,
            200.0f, 200.0f, 200.0f,
            200.0f, 200.0f, 200.0f
        };
        float[] targetWeights =
        {
            200.0f, 100.1f, 100.0f,
            66.7f, 66.6f, 50.1f,
            50.0f, 40.1f, 40.0f
        };
        int[] expectedPowers =
        {
            40, 40, 60,
            60, 80, 80,
            100, 100, 120
        };

        for(int testCaseIndex = 0; testCaseIndex < attackerWeights.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.HeatCrashPower(
                    attackerWeights[testCaseIndex],
                    targetWeights[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void HeavySlamPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.HeavySlamPower(0.0f, 1.0f);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.HeavySlamPower(1.0f, 0.0f);
            }
        );

        // Test expected values.

        float[] attackerWeights =
        {
            200.0f, 200.0f, 200.0f,
            200.0f, 200.0f, 200.0f,
            200.0f, 200.0f, 200.0f
        };
        float[] targetWeights =
        {
            200.0f, 100.1f, 100.0f,
            66.7f, 66.6f, 50.1f,
            50.0f, 40.1f, 40.0f
        };
        int[] expectedPowers =
        {
            40, 40, 60,
            60, 80, 80,
            100, 100, 120
        };

        for(int testCaseIndex = 0; testCaseIndex < attackerWeights.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.HeavySlamPower(
                    attackerWeights[testCaseIndex],
                    targetWeights[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void IceBallPowersTest()
    {
        int[] expectedResults = {30, 60, 120, 240, 480};

        PKMN.IntList iceBallPowers = PKMN.Calculations.IceBallPowers();
        int[] resultsFromFunction = new int[iceBallPowers.Count];
        iceBallPowers.CopyTo(resultsFromFunction, 0);

        Assert.AreEqual(expectedResults, resultsFromFunction);
    }

    [Test]
    public void LowKickPowerTest()
    {
        // Test invalid inputs.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.LowKickPower(-1.0f, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.LowKickPower(1.0f, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.LowKickPower(1.0f, 10);
            }
        );

        // Test expected values.

        float[] targetWeights =
        {
            0.1f, 9.9f, 10.0f, 24.9f, 25.0f, 49.9f,
            50.0f, 99.9f, 100.0f, 199.9f, 200.0f, 1000.0f
        };
        int[] expectedPowers =
        {
            20, 20, 40, 40, 60, 60,
            80, 80, 100, 100, 120, 120
        };

        for(int testCaseIndex = 0; testCaseIndex < targetWeights.Length; ++testCaseIndex)
        {
            for(int generation = 1; generation <= 6; ++generation)
            {
                if(generation <= 2)
                {
                    // Generation I-II only have one set power,
                    // regardless of target weight.
                    Assert.AreEqual(
                        50,
                        PKMN.Calculations.LowKickPower(
                            targetWeights[testCaseIndex],
                            generation
                        )
                    );
                }
                else
                {
                    Assert.AreEqual(
                        expectedPowers[testCaseIndex],
                        PKMN.Calculations.LowKickPower(
                            targetWeights[testCaseIndex],
                            generation
                        )
                    );
                }
            }
        }
    }

    [Test]
    public void PowerTripPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(-1, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(7, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, -1, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 7, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, -1, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 7, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, -1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 7, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 7, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 0, 7, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 0, 0, 7);
            }
        );

        // Test expected values.

        Assert.AreEqual(
            20,
            PKMN.Calculations.PowerTripPower(0, 0, 0, 0, 0, 0, 0)
        );
        Assert.AreEqual(
            440,
            PKMN.Calculations.PowerTripPower(0, 1, 2, 3, 4, 5, 6)
        );
        Assert.AreEqual(
            860,
            PKMN.Calculations.PowerTripPower(6, 6, 6, 6, 6, 6, 6)
        );
    }

    [Test]
    public void PunishmentPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(-1, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(7, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, -1, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 7, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, -1, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 7, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, -1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 7, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 7, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 0, 7, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 0, 0, 7);
            }
        );

        // Test expected values.

        Assert.AreEqual(
            60,
            PKMN.Calculations.PunishmentPower(0, 0, 0, 0, 0, 0, 0)
        );
        Assert.AreEqual(
            200,
            PKMN.Calculations.PunishmentPower(0, 1, 2, 3, 4, 5, 6)
        );
        Assert.AreEqual(
            200,
            PKMN.Calculations.PunishmentPower(6, 6, 6, 6, 6, 6, 6)
        );
    }

    [Test]
    public void ReturnPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ReturnPower(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ReturnPower(256);
            }
        );

        // Test expected values.

        Assert.AreEqual(1, PKMN.Calculations.ReturnPower(0));
        Assert.AreEqual(102, PKMN.Calculations.ReturnPower(255));
    }

    [Test]
    public void ReversalPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ReversalPower(0, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ReversalPower(1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.ReversalPower(2, 1);
            }
        );

        // Test expected values.

        int[] attackerCurrentHPs =
        {
            1, 20, 21, 52, 53, 104,
            105, 177, 178, 343, 344, 500
        };
        int[] attackerMaxHPs =
        {
            500, 500, 500, 500, 500, 500,
            500, 500, 500, 500, 500, 500
        };
        int[] expectedPowers =
        {
            200, 200, 150, 150, 100, 100,
            80, 80, 40, 40, 20, 20
        };

        for(int testCaseIndex = 0; testCaseIndex < attackerCurrentHPs.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedPowers[testCaseIndex],
                PKMN.Calculations.ReversalPower(
                    attackerCurrentHPs[testCaseIndex],
                    attackerMaxHPs[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void RolloutPowersTest()
    {
        int[] expectedResults = {30, 60, 120, 240, 480};

        PKMN.IntList rolloutPowers = PKMN.Calculations.RolloutPowers();
        int[] resultsFromFunction = new int[rolloutPowers.Count];
        rolloutPowers.CopyTo(resultsFromFunction, 0);

        Assert.AreEqual(expectedResults, resultsFromFunction);
    }

    [Test]
    public void SpitUpPowerTest()
    {
        // Test invalid inputs.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.SpitUpPower(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.SpitUpPower(4);
            }
        );

        Assert.AreEqual(0, PKMN.Calculations.SpitUpPower(0));
        Assert.AreEqual(100, PKMN.Calculations.SpitUpPower(1));
        Assert.AreEqual(200, PKMN.Calculations.SpitUpPower(2));
        Assert.AreEqual(300, PKMN.Calculations.SpitUpPower(3));
    }

    [Test]
    public void StoredPowerPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(-1, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(7, 0, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, -1, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 7, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, -1, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 7, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, -1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 7, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 7, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 0, 7, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 0, 0, 7);
            }
        );

        // Test expected values.

        Assert.AreEqual(
            20,
            PKMN.Calculations.StoredPowerPower(0, 0, 0, 0, 0, 0, 0)
        );
        Assert.AreEqual(
            440,
            PKMN.Calculations.StoredPowerPower(0, 1, 2, 3, 4, 5, 6)
        );
        Assert.AreEqual(
            860,
            PKMN.Calculations.StoredPowerPower(6, 6, 6, 6, 6, 6, 6)
        );
    }

    [Test]
    public void TripleKickPowersTest()
    {
        int[] expectedResults = {10, 20, 30};

        PKMN.IntList tripleKickPowers = PKMN.Calculations.TripleKickPowers();
        int[] resultsFromFunction = new int[tripleKickPowers.Count];
        tripleKickPowers.CopyTo(resultsFromFunction, 0);

        Assert.AreEqual(expectedResults, resultsFromFunction);
    }

    [Test]
    public void TrumpCardPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.TrumpCardPower(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.TrumpCardPower(9);
            }
        );

        int[] expectedResults = {200, 80, 60, 50, 40, 40, 40, 40, 40};

        for(int remainingPP = 0; remainingPP < expectedResults.Length; ++remainingPP)
        {
            Assert.AreEqual(
                expectedResults[remainingPP],
                PKMN.Calculations.TrumpCardPower(remainingPP)
            );
        }
    }

    [Test]
    public void WaterSpoutPowerTest()
    {
        // Test invalid inputs.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WaterSpoutPower(0, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WaterSpoutPower(1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WaterSpoutPower(2, 1);
            }
        );

        // Test expected results.

        Assert.AreEqual(1, PKMN.Calculations.WaterSpoutPower(1, 500));
        Assert.AreEqual(75, PKMN.Calculations.WaterSpoutPower(250, 500));
        Assert.AreEqual(150, PKMN.Calculations.WaterSpoutPower(500, 500));
    }

    [Test]
    public void WringOutPowerTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WringOutPower(0, 1, 5);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WringOutPower(2, 1, 5);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WringOutPower(1, 2, 3);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.WringOutPower(1, 2, 7);
            }
        );

        // Test expected values.

        Assert.AreEqual(1, PKMN.Calculations.WringOutPower(1, 200, 4));
        Assert.AreEqual(121, PKMN.Calculations.WringOutPower(200, 200, 4));

        for(int generation = 5; generation <= 6; ++generation)
        {
            Assert.AreEqual(
                1,
                PKMN.Calculations.WringOutPower(1, 200, generation)
            );
            Assert.AreEqual(
                120,
                PKMN.Calculations.WringOutPower(200, 200, generation)
            );
        }
    }

    [Test]
    public void Gen1CriticalHitChanceTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.Gen1CriticalHitChance(0, false, false);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.Gen1CriticalHitChance(256, false, false);
            }
        );

        // In Generation I, make sure there is never a guaranteed chance for
        // a critical hit.
        Assert.LessOrEqual(
            PKMN.Calculations.Gen1CriticalHitChance(255, true, true),
            (255.0f / 256.0f)
        );
    }

    [Test]
    public void CriticalHitChanceTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CriticalHitChance(1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CriticalHitChance(10, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CriticalHitChance(2, -1);
            }
        );

        // Test expected values.

        int[] generations =
        {
            2, 2, 2, 2, 2,
            3, 3, 3, 3, 3,
            4, 4, 4, 4, 4,
            5, 5, 5, 5, 5,
            6, 6, 6, 6, 6,
        };
        int[] stages =
        {
            0, 1, 2, 3, 4,
            0, 1, 2, 3, 4,
            0, 1, 2, 3, 4,
            0, 1, 2, 3, 4,
            0, 1, 2, 3, 4,
        };
        float[] expectedResults =
        {
            0.0625f, 0.125f, 0.25f, 0.333f, 0.5f,
            0.0625f, 0.125f, 0.25f, 0.333f, 0.5f,
            0.0625f, 0.125f, 0.25f, 0.333f, 0.5f,
            0.0625f, 0.125f, 0.25f, 0.333f, 0.5f,
            0.0625f, 0.125f, 0.5f,  1.0f,   1.0f
        };

        for(int testCaseIndex = 0; testCaseIndex < generations.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                expectedResults[testCaseIndex],
                PKMN.Calculations.CriticalHitChance(
                    generations[testCaseIndex],
                    stages[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void Gen1CriticalHitModifierTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.Gen1CriticalHitModifier(-1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.Gen1CriticalHitModifier(256);
            }
        );

        // Test expected values.

        Assert.AreEqual(1.5f, PKMN.Calculations.Gen1CriticalHitModifier(5));
        Assert.AreEqual(1.8f, PKMN.Calculations.Gen1CriticalHitModifier(20));
        Assert.AreEqual(1.95f, PKMN.Calculations.Gen1CriticalHitModifier(95));
    }

    [Test]
    public void CriticalHitModifierTest()
    {
        // Test invalid parameters.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CriticalHitModifier(1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.CriticalHitModifier(10);
            }
        );

        // Test expected values.

        // Past Generation I, the modifier is constant, depending on the
        // generation.
        Assert.AreEqual(2.0f, PKMN.Calculations.CriticalHitModifier(2));
        Assert.AreEqual(2.0f, PKMN.Calculations.CriticalHitModifier(3));
        Assert.AreEqual(2.0f, PKMN.Calculations.CriticalHitModifier(4));
        Assert.AreEqual(2.0f, PKMN.Calculations.CriticalHitModifier(5));
        Assert.AreEqual(1.5f, PKMN.Calculations.CriticalHitModifier(6));
    }

    [Test]
    public void DamageTest()
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
        const int level = 75;
        const int iceFangBasePower = 65;
        const float modifierAgainstDragonGround = 6.0f;
        const int glaceonL75Attack = 123;
        const int garchompL75Defense = 163;

        int damage = PKMN.Calculations.Damage(
                         level,
                         iceFangBasePower,
                         glaceonL75Attack,
                         garchompL75Defense,
                         modifierAgainstDragonGround
                     );
        Assert.AreEqual(200, damage);
    }

    [Test]
    public void TypeDamageModifierTest()
    {
        // Invalid generation
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.TypeDamageModifier(
                    -1, "Normal", "Normal"
                );
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.TypeDamageModifier(
                    8, "Normal", "Normal"
                );
            }
        );

        // Invalid types for a given generation
        int[] generations = {1, 1, 5, 3, 5, 2, 4};
        string[] types = {"Dark", "Steel", "Fairy", "???", "???", "Shadow", "Shadow"};

        for(int testCaseIndex = 0; testCaseIndex < generations.Length; ++testCaseIndex)
        {
            // Invalid attacking type
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    PKMN.Calculations.TypeDamageModifier(
                        generations[testCaseIndex],
                        types[testCaseIndex],
                        "Normal"
                    );
                }
            );

            // Invalid defending type
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    PKMN.Calculations.TypeDamageModifier(
                        generations[testCaseIndex],
                        "Normal",
                        types[testCaseIndex]
                    );
                }
            );
        }

        // Check that changes between generations are properly implemented.
        string[] attackingTypes = {"Bug", "Poison", "Ghost", "Ice", "Ghost", "Dark"};
        string[] defendingTypes = {"Poison", "Bug", "Psychic", "Fire", "Steel", "Steel"};
        int[] oldGenerations = {1, 1, 1, 1, 5, 5};
        float[] oldModifiers = {2.0f, 2.0f, 0.0f, 1.0f, 0.5f, 0.5f};
        int[] newGenerations = {2, 2, 2, 2, 6, 6};
        float[] newModifiers = {0.5f, 1.0f, 2.0f, 0.5f, 1.0f, 1.0f};

        for(int testCaseIndex = 0; testCaseIndex < attackingTypes.Length; ++testCaseIndex)
        {
            Assert.AreEqual(
                oldModifiers[testCaseIndex],
                PKMN.Calculations.TypeDamageModifier(
                    oldGenerations[testCaseIndex],
                    attackingTypes[testCaseIndex],
                    defendingTypes[testCaseIndex]
                )
            );
            Assert.AreEqual(
                newModifiers[testCaseIndex],
                PKMN.Calculations.TypeDamageModifier(
                    newGenerations[testCaseIndex],
                    attackingTypes[testCaseIndex],
                    defendingTypes[testCaseIndex]
                )
            );
        }
    }

    [Test]
    public void Gen2UnownFormTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(-1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(16, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, 16, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, 0, 16, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2UnownForm(0, 0, 0, 16);
            }
        );

        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
         */
        Assert.AreEqual("G", PKMN.Calculations.Gen2UnownForm(10, 9, 1, 14));
        Assert.AreEqual("S", PKMN.Calculations.Gen2UnownForm(5, 15, 10, 5));
    }

    [Test]
    public void Gen3UnownFormTest() {
        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
         */
        Assert.AreEqual("B", PKMN.Calculations.Gen3UnownForm(0x4C07DE71));
    }

    [Test]
    public void WurmpleBecomesSilcoonTest() {
        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
         */
        Assert.IsTrue(PKMN.Calculations.WurmpleBecomesSilcoon(0x5CF4091C, true));
        Assert.IsFalse(PKMN.Calculations.WurmpleBecomesSilcoon(0x091C5CF4, true));
        Assert.IsTrue(PKMN.Calculations.WurmpleBecomesSilcoon(0x091C5CF4, false));
        Assert.IsFalse(PKMN.Calculations.WurmpleBecomesSilcoon(0x5CF4091C, false));
    }

    [Test]
    public void Gen2PokemonGenderTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2PokemonGender("Not a species", 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2PokemonGender("Bulbasaur", -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2PokemonGender("Bulbasaur", 16);
            }
        );

        // Make sure known good inputs result in expected results.

        // All male
        Assert.AreEqual("Male", PKMN.Calculations.Gen2PokemonGender("Nidorino", 0));
        Assert.AreEqual("Male", PKMN.Calculations.Gen2PokemonGender("Nidorino", 15));

        // 25% male, 75% female
        Assert.AreEqual("Female", PKMN.Calculations.Gen2PokemonGender("Vulpix", 11));
        Assert.AreEqual("Male", PKMN.Calculations.Gen2PokemonGender("Vulpix", 12));

        // All female
        Assert.AreEqual("Female", PKMN.Calculations.Gen2PokemonGender("Nidorina", 0));
        Assert.AreEqual("Female", PKMN.Calculations.Gen2PokemonGender("Nidorina", 15));

        // Genderless
        Assert.AreEqual("Genderless", PKMN.Calculations.Gen2PokemonGender("Magnemite", 0));
        Assert.AreEqual("Genderless", PKMN.Calculations.Gen2PokemonGender("Magnemite", 15));
    }

    [Test]
    public void ModernPokemonGenderTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernPokemonGender("Not a species", 0);
            }
        );

        // Make sure known good inputs result in expected results.

        // All male
        Assert.AreEqual("Male", PKMN.Calculations.ModernPokemonGender("Nidorino", 0));
        Assert.AreEqual("Male", PKMN.Calculations.ModernPokemonGender("Nidorino", 0xFFFFFFFF));

        // 25% male, 75% female
        Assert.AreEqual("Female", PKMN.Calculations.ModernPokemonGender("Vulpix", 190));
        Assert.AreEqual("Male", PKMN.Calculations.ModernPokemonGender("Vulpix", 191));

        // All female
        Assert.AreEqual("Female", PKMN.Calculations.ModernPokemonGender("Nidorina", 0));
        Assert.AreEqual("Female", PKMN.Calculations.ModernPokemonGender("Nidorina", 0xFFFFFFFF));

        // Genderless
        Assert.AreEqual("Genderless", PKMN.Calculations.ModernPokemonGender("Magnemite", 0));
        Assert.AreEqual("Genderless", PKMN.Calculations.ModernPokemonGender("Magnemite", 0xFFFFFFFF));
    }

    [Test]
    public void Gen2HiddenPowerTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(-1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(16, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, 16, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, 0, 16, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2HiddenPower(0, 0, 0, 16);
            }
        );

        /*
         * Make sure known good inputs result in expected results, and test (in)equality functions.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
         */
        PKMN.HiddenPower expectedHiddenPower = new PKMN.HiddenPower(
                                                       "Dark", 69
                                                   );
        PKMN.HiddenPower hiddenPower = PKMN.Calculations.Gen2HiddenPower(15, 15, 15, 14);

        Assert.AreEqual(expectedHiddenPower, hiddenPower);
    }

    [Test]
    public void ModernHiddenPowerTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(-1, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(32, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, -1, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 32, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, -1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 32, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, 32, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, 0, 32, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.ModernHiddenPower(0, 0, 0, 0, 0, 32);
            }
        );

        /*
         * Make sure known good inputs result in expected results, and test equality functions.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
         */
        PKMN.HiddenPower expectedHiddenPower = new PKMN.HiddenPower(
                                                       "Grass", 70
                                                   );
        PKMN.HiddenPower hiddenPower = PKMN.Calculations.ModernHiddenPower(30, 31, 31, 31, 30, 31);

        Assert.AreEqual(hiddenPower, expectedHiddenPower);
    }

    [Test]
    public void NaturalGiftTest()
    {
        // Test invalid generations.

        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.NaturalGiftStats("Cheri Berry", 3);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.NaturalGiftStats("Cheri Berry", 10);
            }
        );

        // Test invalid items.

        Assert.Throws<ArgumentOutOfRangeException>(
            delegate
            {
                PKMN.Calculations.NaturalGiftStats("Potion", 4);
            }
        );

        // Test expected values. Make sure differences between
        // generations are reflected.

        string[] items = {"Cheri Berry", "Nanab Berry", "Belue Berry"};
        string[] types = {"Fire", "Water", "Electric"};
        int[] gen4Powers = {60, 70, 80};
        int[] gen5Powers = {60, 70, 80};
        int[] gen6Powers = {80, 90, 100};

        for(int testCaseIndex = 0; testCaseIndex < items.Length; ++testCaseIndex)
        {
            PKMN.NaturalGift naturalGift;

            naturalGift = PKMN.Calculations.NaturalGiftStats(
                              items[testCaseIndex],
                              4
                          );
            Assert.AreEqual(types[testCaseIndex], naturalGift.Type);
            Assert.AreEqual(gen4Powers[testCaseIndex], naturalGift.BasePower);

            naturalGift = PKMN.Calculations.NaturalGiftStats(
                              items[testCaseIndex],
                              5
                          );
            Assert.AreEqual(types[testCaseIndex], naturalGift.Type);
            Assert.AreEqual(gen5Powers[testCaseIndex], naturalGift.BasePower);

            naturalGift = PKMN.Calculations.NaturalGiftStats(
                              items[testCaseIndex],
                              6
                          );
            Assert.AreEqual(types[testCaseIndex], naturalGift.Type);
            Assert.AreEqual(gen6Powers[testCaseIndex], naturalGift.BasePower);
        }
    }

    [Test]
    public void NatureTest() {
        string[] natures = {
            "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
            "Bold", "Docile", "Relaxed", "Impish", "Lax",
            "Timid", "Hasty", "Serious", "Jolly", "Naive",
            "Modest", "Mild", "Quiet", "Bashful", "Rash",
            "Calm", "Gentle", "Sassy", "Careful", "Quirky"
        };

        for(uint i = 0; i < natures.Length; ++i) {
            Assert.AreEqual(
                natures[i],
                PKMN.Calculations.Nature((uint)((rng.Next(0,50001) * 1000) + i))
            );
        }
    }

    [Test]
    public void PersonalityTest() {
        /*
         * Given the amount of time the C++ test takes, this will just verify
         * the API wrapper.
         */

        // Test invalid ability.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GeneratePersonality(
                    "Charmander",
                    PKMN.Pokemon.DefaultTrainerID,
                    true,
                    "Torrent",
                    "Male",
                    "Quiet"
                );
            }
        );

        // Test invalid gender.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GeneratePersonality(
                    "Charmander",
                    PKMN.Pokemon.DefaultTrainerID,
                    true,
                    "Blaze",
                    "Not a gender",
                    "Quiet"
                );
            }
        );

        // Test invalid nature.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GeneratePersonality(
                    "Charmander",
                    PKMN.Pokemon.DefaultTrainerID,
                    true,
                    "Blaze",
                    "Male",
                    "Not a nature"
                );
            }
        );

        // Test and validate a valid call.
        uint personality = PKMN.Calculations.GeneratePersonality(
                               "Charmander",
                               PKMN.Pokemon.DefaultTrainerID,
                               true,
                               "Blaze",
                               "Male",
                               "Quiet"
                           );
        Assert.AreEqual(
            "Male",
            PKMN.Calculations.ModernPokemonGender(
                "Charmander", personality
            )
        );
        Assert.IsTrue(
            PKMN.Calculations.ModernShiny(
                personality, PKMN.Pokemon.DefaultTrainerID
            )
        );
    }

    [Test]
    public void Gen2ShinyTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(-1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(16, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, 16, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, 0, 16, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.Gen2Shiny(0, 0, 0, 16);
            }
        );

        /*
         * Make sure known good inputs result in known results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
         */
        Assert.IsTrue(PKMN.Calculations.Gen2Shiny(7, 10, 10, 10));
        Assert.IsFalse(PKMN.Calculations.Gen2Shiny(6, 15, 7, 15));
    }

    [Test]
    public void ModernShinyTest() {
        /*
         * Make sure known good inputs result in known results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
         *         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
         */
        Assert.IsTrue(PKMN.Calculations.ModernShiny(2814471828, 2545049318));
        Assert.IsTrue(PKMN.Calculations.ModernShiny(0xB58F0B2A, 398174488));
    }

    [Test]
    public void PokemonSizeTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, -1, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 32, 0, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, -1, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 32, 0, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, -1, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 32, 0, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, -1, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, 32, 0, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, 0, -1, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, 0, 32, 0);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, 0, 0, -1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.PokemonSize("Magikarp", 0, 0, 0, 0, 0, 0, 32);
            }
        );

        /*
         * There are no known good calculations, so just check for reasonable values
         * for each relevant Pokémon.
         */
        PKMN.Database.PokemonEntry[] pokemonWithSizeChecks = {
            new PKMN.Database.PokemonEntry("Barboach", "Ruby", ""),
            new PKMN.Database.PokemonEntry("Shroomish", "Ruby", ""),
            new PKMN.Database.PokemonEntry("Seedot", "Emerald", ""),
            new PKMN.Database.PokemonEntry("Lotad", "Emerald", ""),
            new PKMN.Database.PokemonEntry("Magikarp", "FireRed", ""),
            new PKMN.Database.PokemonEntry("Heracross", "FireRed", "")
        };

        for(int i = 0; i < pokemonWithSizeChecks.Length; ++i) {
            for(int j = 0; j < 10; ++j) {
                float size = PKMN.Calculations.PokemonSize(
                                 pokemonWithSizeChecks[i].Name,
                                 (uint)rng.Next(0, Int32.MaxValue),
                                 rng.Next(0, 32),
                                 rng.Next(0, 32),
                                 rng.Next(0, 32),
                                 rng.Next(0, 32),
                                 rng.Next(0, 32),
                                 rng.Next(0, 32)
                             );
                Assert.LessOrEqual(
                    Math.Abs(size-pokemonWithSizeChecks[i].Height),
                    pokemonWithSizeChecks[i].Height
                );
            }
        }
    }

    [Test]
    public void SpindaCoordsTest() {
        PKMN.SpindaCoords spindaCoords1 = new PKMN.SpindaCoords(123, 456);
        PKMN.SpindaCoords spindaCoords2 = new PKMN.SpindaCoords(123, 456);
        PKMN.SpindaCoords spindaCoords3 = new PKMN.SpindaCoords(456, 123);

        // Check addition operators.
        PKMN.SpindaCoords addedCoords = spindaCoords1 + spindaCoords2;
        Assert.AreEqual(246, addedCoords.X);
        Assert.AreEqual(912, addedCoords.Y);

        addedCoords += spindaCoords3;
        Assert.AreEqual(702, addedCoords.X);
        Assert.AreEqual(1035, addedCoords.Y);
    }

    [Test]
    public void SpindaSpotsTest() {
        PKMN.SpindaSpots spindaSpots1 = new PKMN.SpindaSpots(
                                                new PKMN.SpindaCoords(7, 5),
                                                new PKMN.SpindaCoords(10, 0),
                                                new PKMN.SpindaCoords(1, 4),
                                                new PKMN.SpindaCoords(2, 15)
                                            );
        PKMN.SpindaSpots spindaSpots2 = new PKMN.SpindaSpots(
                                                new PKMN.SpindaCoords(7, 5),
                                                new PKMN.SpindaCoords(10, 0),
                                                new PKMN.SpindaCoords(1, 4),
                                                new PKMN.SpindaCoords(2, 15)
                                            );
        PKMN.SpindaSpots spindaSpots3 = new PKMN.SpindaSpots(
                                                new PKMN.SpindaCoords(7, 5),
                                                new PKMN.SpindaCoords(10, 0),
                                                new PKMN.SpindaCoords(2, 15),
                                                new PKMN.SpindaCoords(1, 4)
                                            );

        /*
         * Make sure known good inputs result in known results.
         *
         * Source: https://github.com/magical/spinda
         */
        PKMN.SpindaSpots spindaSpots = PKMN.Calculations.SpindaSpotOffset(4064348759);
        Assert.AreEqual(spindaSpots, spindaSpots1);

        // Test addition operators.
        spindaSpots = spindaSpots + spindaSpots2;
        Assert.AreEqual(new PKMN.SpindaCoords(14, 10), spindaSpots.LeftEar);
        Assert.AreEqual(new PKMN.SpindaCoords(20, 0), spindaSpots.RightEar);
        Assert.AreEqual(new PKMN.SpindaCoords(2, 8), spindaSpots.LeftFace);
        Assert.AreEqual(new PKMN.SpindaCoords(4, 30), spindaSpots.RightFace);

        spindaSpots += spindaSpots3;
        Assert.AreEqual(new PKMN.SpindaCoords(21, 15), spindaSpots.LeftEar);
        Assert.AreEqual(new PKMN.SpindaCoords(30, 0), spindaSpots.RightEar);
        Assert.AreEqual(new PKMN.SpindaCoords(4, 23), spindaSpots.LeftFace);
        Assert.AreEqual(new PKMN.SpindaCoords(5, 34), spindaSpots.RightFace);

        spindaSpots = spindaSpots + new PKMN.SpindaCoords(2, 5);
        Assert.AreEqual(new PKMN.SpindaCoords(23, 20), spindaSpots.LeftEar);
        Assert.AreEqual(new PKMN.SpindaCoords(32, 5), spindaSpots.RightEar);
        Assert.AreEqual(new PKMN.SpindaCoords(6, 28), spindaSpots.LeftFace);
        Assert.AreEqual(new PKMN.SpindaCoords(7, 39), spindaSpots.RightFace);

        spindaSpots += new PKMN.SpindaCoords(5, 2);
        Assert.AreEqual(new PKMN.SpindaCoords(28, 22), spindaSpots.LeftEar);
        Assert.AreEqual(new PKMN.SpindaCoords(37, 7), spindaSpots.RightEar);
        Assert.AreEqual(new PKMN.SpindaCoords(11, 30), spindaSpots.LeftFace);
        Assert.AreEqual(new PKMN.SpindaCoords(12, 41), spindaSpots.RightFace);
    }

    [Test]
    public void GBStatsTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetGBStat("Not a stat", 1, 1, 1, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetGBStat("Attack", 1, 1, 123456, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetGBStat("Attack", 1, 1, 1, 12345);
            }
        );

        /*
         * Make sure known good inputs result in known results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
         */
        Assert.That(PKMN.Calculations.GetGBStat("HP", 81, 35, 22850, 7), Is.EqualTo(189).Within(1));
        Assert.That(PKMN.Calculations.GetGBStat("Attack", 81, 55, 23140, 8), Is.EqualTo(137).Within(1));
    }

    [Test]
    public void ModernStatsTest() {
        // Make sure expected exceptions are thrown.
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetModernStat("Not a stat", 1, 1.0f, 1, 1, 1);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetModernStat("Special", 1, 1.0f, 1, 1, 1);
            }
        );
        Assert.Throws<ArgumentOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetModernStat("Attack", 1, 0.666f, 1, 1, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetModernStat("Attack", 1, 1.0f, 1, 12345, 1);
            }
        );
        Assert.Throws<IndexOutOfRangeException>(
            delegate {
                PKMN.Calculations.GetModernStat("Attack", 1, 1.0f, 1, 1, 12345);
            }
        );

        /*
         * Make sure known good inputs result in known results.
         *
         * Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
         */
        Assert.That(PKMN.Calculations.GetModernStat("HP", 78, 1.0f, 108, 74, 24), Is.EqualTo(289).Within(1));
        Assert.That(PKMN.Calculations.GetModernStat("Attack", 78, 1.1f, 130, 195, 12), Is.EqualTo(280).Within(1));
    }
}
