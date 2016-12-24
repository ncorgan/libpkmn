/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpCalculationsTest {
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

        PKMN.HiddenPower expectedHiddenPower = new PKMN.HiddenPower(
                                                       "Dark", 69
                                                   );
        PKMN.HiddenPower hiddenPowerDifferentType = new PKMN.HiddenPower(
                                                            "Normal", 69
                                                        );
        PKMN.HiddenPower hiddenPowerDifferentBasePower = new PKMN.HiddenPower(
                                                                 "Dark", 50
                                                             );

        PKMN.HiddenPower hiddenPower = PKMN.Calculations.Gen2HiddenPower(15, 15, 15, 15);

        Assert.AreEqual(hiddenPower, expectedHiddenPower);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentType);
        Assert.AreNotEqual(hiddenPower, hiddenPowerDifferentBasePower);
    }
}
