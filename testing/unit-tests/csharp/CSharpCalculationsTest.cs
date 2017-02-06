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
        Assert.AreEqual(PKMN.Calculations.Gen2UnownForm(10, 9, 1, 14), "G");
        Assert.AreEqual(PKMN.Calculations.Gen2UnownForm(5, 15, 10, 5), "S");
    }

    [Test]
    public void Gen3UnownFormTest() {
        /*
         * Make sure known good inputs result in expected results.
         *
         * Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
         */
        Assert.AreEqual(PKMN.Calculations.Gen3UnownForm(0x4C07DE71), "B");
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
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Nidorino", 0), "Male");
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Nidorino", 15), "Male");

        // 25% male, 75% female
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Vulpix", 11), "Female");
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Vulpix", 12), "Male");

        // All female
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Nidorina", 0), "Female");
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Nidorina", 15), "Female");

        // Genderless
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Magnemite", 0), "Genderless");
        Assert.AreEqual(PKMN.Calculations.Gen2PokemonGender("Magnemite", 15), "Genderless");
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
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Nidorino", 0), "Male");
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Nidorino", 0xFFFFFFFF), "Male");

        // 25% male, 75% female
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Vulpix", 190), "Female");
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Vulpix", 191), "Male");

        // All female
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Nidorina", 0), "Female");
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Nidorina", 0xFFFFFFFF), "Female");

        // Genderless
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Magnemite", 0), "Genderless");
        Assert.AreEqual(PKMN.Calculations.ModernPokemonGender("Magnemite", 0xFFFFFFFF), "Genderless");
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
        PKMN.HiddenPower hiddenPowerDifferentType = new PKMN.HiddenPower(
                                                            "Normal", 69
                                                        );
        PKMN.HiddenPower hiddenPowerDifferentBasePower = new PKMN.HiddenPower(
                                                                 "Dark", 50
                                                             );

        PKMN.HiddenPower hiddenPower = PKMN.Calculations.Gen2HiddenPower(15, 15, 15, 14);

        Assert.AreEqual(hiddenPower, expectedHiddenPower);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentType);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentBasePower);
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
        PKMN.HiddenPower hiddenPowerDifferentType = new PKMN.HiddenPower(
                                                            "Steel", 70
                                                        );
        PKMN.HiddenPower hiddenPowerDifferentBasePower = new PKMN.HiddenPower(
                                                                 "Grass", 10
                                                             );

        PKMN.HiddenPower hiddenPower = PKMN.Calculations.ModernHiddenPower(30, 31, 31, 31, 30, 31);

        Assert.AreEqual(hiddenPower, expectedHiddenPower);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentType);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentBasePower);
        Assert.AreEqual(hiddenPower.GetHashCode(), expectedHiddenPower.GetHashCode());
        Assert.AreNotEqual(hiddenPower.GetHashCode(), hiddenPowerDifferentType.GetHashCode());
        Assert.AreNotEqual(hiddenPower.GetHashCode(), hiddenPowerDifferentBasePower.GetHashCode());
    }

    [Test]
    public void Gen3Gen4NatureTest() {
        string[] natures = {
            "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
            "Bold", "Docile", "Relaxed", "Impish", "Lax",
            "Timid", "Hasty", "Serious", "Jolly", "Naive",
            "Modest", "Mild", "Quiet", "Bashful", "Rash",
            "Calm", "Gentle", "Sassy", "Careful", "Quirky"
        };

        for(uint i = 0; i < natures.Length; ++i) {
            Assert.AreEqual(
                PKMN.Calculations.Gen3Gen4Nature((uint)((rng.Next(0,50001) * 1000) + i)),
                natures[i]
            );
        }
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
    public void SpindaCoordsTest() {
        // Check equality functions.
        PKMN.SpindaCoords spindaCoords1 = new PKMN.SpindaCoords(123, 456);
        PKMN.SpindaCoords spindaCoords2 = new PKMN.SpindaCoords(123, 456);
        PKMN.SpindaCoords spindaCoords3 = new PKMN.SpindaCoords(456, 123);

        Assert.AreEqual(spindaCoords1, spindaCoords2);
        Assert.AreNotEqual(spindaCoords1, spindaCoords3);
        Assert.AreEqual(spindaCoords1.GetHashCode(), spindaCoords2.GetHashCode());
        Assert.AreNotEqual(spindaCoords1.GetHashCode(), spindaCoords3.GetHashCode());
    }

    [Test]
    public void SpindaSpotsTest() {
        // Check equality functions.
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

        Assert.AreEqual(spindaSpots1, spindaSpots2);
        Assert.AreNotEqual(spindaSpots1, spindaSpots3);
        Assert.AreEqual(spindaSpots1.GetHashCode(), spindaSpots2.GetHashCode());
        Assert.AreNotEqual(spindaSpots1.GetHashCode(), spindaSpots3.GetHashCode());

        /*
         * Make sure known good inputs result in known results.
         *
         * Source: https://github.com/magical/spinda
         */
        PKMN.SpindaSpots spindaSpots = PKMN.Calculations.SpindaSpotOffset(4064348759);
        Assert.AreEqual(spindaSpots, spindaSpots1);
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
