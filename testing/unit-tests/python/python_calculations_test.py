#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import random
import sys
import unittest

class calculations_test(unittest.TestCase):

    def test_gen2_unown_form(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
        #
        self.assertEqual(pkmn.calculations.gen2_unown_form(10, 9, 1, 14), "G")
        self.assertEqual(pkmn.calculations.gen2_unown_form(5, 15, 10, 5), "S")

    def test_gen3_unown_form(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
        #
        self.assertEqual(pkmn.calculations.gen3_unown_form(0x4C07DE71), "B")

    def test_wurmple_becomes_silcoon(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
        #
        self.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, True))
        self.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, True))
        self.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, False))
        self.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, False))

    def test_gen2_gender(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            gender = pkmn.calculations.gen2_pokemon_gender("Not a species", 0)
        with self.assertRaises(IndexError):
            gender = pkmn.calculations.gen2_pokemon_gender("Bulbasaur", -1)
        with self.assertRaises(IndexError):
            gender = pkmn.calculations.gen2_pokemon_gender("Bulbasaur", 16)

        # Make sure known good inputs result in expected results.

        # All male
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 0), "Male")
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorino", 15), "Male")

        # 25% male, 75% female
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 11), "Female")
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Vulpix", 12), "Male")

        # All female
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 0), "Female")
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Nidorina", 15), "Female")

        # Genderless
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 0), "Genderless")
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender("Magnemite", 15), "Genderless")

    def test_modern_gender(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            gender = pkmn.calculations.modern_pokemon_gender("Not a species", 0)

        # All male
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0), "Male")
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorino", 0xFFFFFFFF), "Male")

        # 25% male, 75% female
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 190), "Female")
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Vulpix", 191), "Male")

        # All female
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0), "Female")
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Nidorina", 0xFFFFFFFF), "Female")

        # Genderless
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0), "Genderless")
        self.assertEquals(pkmn.calculations.modern_pokemon_gender("Magnemite", 0xFFFFFFFF), "Genderless")

    def test_gen2_hidden_power(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results, and test (in)equality operators.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
        #
        expected_hidden_power = pkmn.calculations.hidden_power("Dark", 69)
        hidden_power_different_type = pkmn.calculations.hidden_power("Normal", 69)
        hidden_power_different_base_power = pkmn.calculations.hidden_power("Dark", 50)

        hidden_power = pkmn.calculations.gen2_hidden_power(15, 15, 15, 14)
        self.assertTrue(hidden_power == expected_hidden_power)
        self.assertFalse(hidden_power != expected_hidden_power)
        self.assertFalse(hidden_power == hidden_power_different_type)
        self.assertTrue(hidden_power != hidden_power_different_type)
        self.assertFalse(hidden_power == hidden_power_different_base_power)
        self.assertTrue(hidden_power != hidden_power_different_base_power)

    def test_modern_hidden_power(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(-1, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(32, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, -1, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 32, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, -1, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 32, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, -1, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 32, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, -1, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 32, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 0, -1)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 0, 32)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
        #
        expected_hidden_power = pkmn.calculations.hidden_power("Grass", 70)
        hidden_power_different_type = pkmn.calculations.hidden_power("Steel", 70)
        hidden_power_different_base_power = pkmn.calculations.hidden_power("Grass", 10)

        hidden_power = pkmn.calculations.modern_hidden_power(30, 31, 31, 31, 30, 31)
        self.assertTrue(hidden_power == expected_hidden_power)
        self.assertFalse(hidden_power != expected_hidden_power)
        self.assertFalse(hidden_power == hidden_power_different_type)
        self.assertTrue(hidden_power != hidden_power_different_type)
        self.assertFalse(hidden_power == hidden_power_different_base_power)
        self.assertTrue(hidden_power != hidden_power_different_base_power)

    def test_gen3_gen4_nature(self):
        natures = [
            "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
            "Bold", "Docile", "Relaxed", "Impish", "Lax",
            "Timid", "Hasty", "Serious", "Jolly", "Naive",
            "Modest", "Mild", "Quiet", "Bashful", "Rash",
            "Calm", "Gentle", "Sassy", "Careful", "Quirky",
        ]

        # Make sure the SWIG wrapper keeps personality within the proper bounds.
        # Which error applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.gen3_gen4_nature(-1)
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.gen3_gen4_nature(0xFFFFFFFF+1)

        for i in range(len(natures)):
            self.assertEqual(
                pkmn.calculations.gen3_gen4_nature((random.randint(0, 50000) * 1000) + i),
                natures[i]
            )

    def test_gen2_shiny(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
        #
        self.assertTrue(pkmn.calculations.gen2_shiny(7, 10, 10, 10))
        self.assertFalse(pkmn.calculations.gen2_shiny(6, 15, 7, 15))

    def test_modern_shiny(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
        #         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
        #
        self.assertTrue(pkmn.calculations.modern_shiny(2814471828, 2545049318))
        self.assertTrue(pkmn.calculations.modern_shiny(0xB58F0B2A, 398174488))

    def test_pokemon_size(self):
        # There are no known good calculations, so just check for reasonable values
        # for each relevant Pokemon.
        pokemon_with_size_checks = [
            pkmn.database.pokemon_entry("Barboach", "Ruby", ""),
            pkmn.database.pokemon_entry("Shroomish", "Ruby", ""),
            pkmn.database.pokemon_entry("Seedot", "Emerald", ""),
            pkmn.database.pokemon_entry("Lotad", "Emerald", ""),
            pkmn.database.pokemon_entry("Magikarp", "FireRed", ""),
            pkmn.database.pokemon_entry("Heracross", "FireRed", "")
        ]

        # Make sure the SWIG wrapper keeps personality within the proper bounds.
        # Which error applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.pokemon_size(
                    "Magikarp", -1, 0, 0, 0, 0, 0, 0
                )
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.pokemon_size(
                    "Magikarp", -1, 0, 0, 0, 0, 0, 0
                )
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.pokemon_size(
                    "Magikarp", 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
                )
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.pokemon_size(
                    "Magikarp", 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
                )

        # Test input validation.
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, -1, 0, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 32, 0, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, -1, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 32, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, -1, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 32, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, -1, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, 32, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, -1, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 32, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, -1
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                "Magikarp", 0, 0, 0, 0, 0, 0, 32
            )

        for entry in pokemon_with_size_checks:
            height = entry.get_height()
            species = entry.get_name()

            for i in range(10):
                size = pkmn.calculations.pokemon_size(
                           species,
                           random.randint(0, 0xFFFFFF),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31)
                       )
                self.assertTrue(abs(size-height) <= height)

    def test_spinda_coords(self):
        # Check (in)equality operators.
        spinda_coords1 = pkmn.calculations.spinda_coords(123, 456)
        spinda_coords2 = pkmn.calculations.spinda_coords(123, 456)
        spinda_coords3 = pkmn.calculations.spinda_coords(456, 123)

        self.assertTrue(spinda_coords1 == spinda_coords2)
        self.assertFalse(spinda_coords1 != spinda_coords2)
        self.assertFalse(spinda_coords1 == spinda_coords3)
        self.assertTrue(spinda_coords1 != spinda_coords3)

    def test_spinda_spots(self):
        # Check (in)equality operators.
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
        self.assertTrue(spots1 == spots2)
        self.assertFalse(spots1 != spots2)
        self.assertFalse(spots1 == spots3)
        self.assertTrue(spots1 != spots3)

        #
        # Make sure known good inputs result in expected results, and test (in)equality operators.
        #
        # Source: https://github.com/magical/spinda
        #
        spots = pkmn.calculations.spinda_spot_offset(4064348759)
        self.assertTrue(spots == spots1)
        self.assertFalse(spots != spots1)

    def assertIntsAlmostEqual(self, a, b):
        a_minus_one = a-1
        a_plus_one = 1+1

        self.assertTrue(a == b or a-1 == b or a+1 == b)

    def test_gb_stats(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_gb_stat("Not a stat", 1, 1, 1, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_gb_stat("Attack", 1, 1, 123456, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_gb_stat("Attack", 1, 1, 1, 12345)

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
        #
        stat = pkmn.calculations.get_gb_stat("HP", 81, 35, 22850, 7)
        self.assertIntsAlmostEqual(stat, 189)
        stat = pkmn.calculations.get_gb_stat("Attack", 81, 55, 23140, 8)
        self.assertIntsAlmostEqual(stat, 137)

    def test_modern_stats(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat("Not a stat", 1, 1.0, 1, 1, 1)
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat("Special", 1, 1.0, 1, 1, 1)
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat("Attack", 1, 0.666, 1, 1, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_modern_stat("Attack", 1, 1.0, 1, 12345, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_modern_stat("Attack", 1, 1.0, 1, 1, 12345)

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
        #
        stat = pkmn.calculations.get_modern_stat("HP", 78, 1.0, 108, 74, 24)
        self.assertIntsAlmostEqual(stat, 289)
        stat = pkmn.calculations.get_modern_stat("Attack", 78, 1.1, 130, 195, 12)
        self.assertIntsAlmostEqual(stat, 280)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(calculations_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
