#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import sys
import unittest

class database_entry_test(unittest.TestCase):

    def test_item_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry("Not an item", "SoulSilver")
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry("Fast Ball", "Not a game")
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry("Fast Ball", "Red")

        entry = pkmn.database.item_entry("Fast Ball", "SoulSilver")

        self.assertEqual(entry.get_name(), "Fast Ball")
        self.assertEqual(entry.get_game(), "SoulSilver")
        self.assertEqual(entry.get_category(), "Apricorn balls")
        self.assertTrue(entry.get_pocket().startswith("Pok"))

        # Just make sure it works
        description = entry.get_description()

        self.assertEqual(entry.get_cost(), 300)
        self.assertTrue(entry.holdable())
        self.assertEqual(entry.get_fling_power(), 0)
        self.assertEqual(entry.get_fling_effect(), "None")

        entry_same = pkmn.database.item_entry("Fast Ball", "SoulSilver")
        entry_different_item = pkmn.database.item_entry("Potion", "SoulSilver")
        entry_different_game = pkmn.database.item_entry("Fast Ball", "HeartGold")

        self.assertEqual(entry, entry_same)
        self.assertNotEqual(entry, entry_different_item)
        self.assertNotEqual(entry, entry_different_game)

    def test_move_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry("Not a move", "Silver")
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry("Octazooka", "Not a game")
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry("Octazooka", "Red")

        entry = pkmn.database.move_entry("Octazooka", "Silver")

        self.assertEqual(entry.get_name(), "Octazooka")
        self.assertEqual(entry.get_game(), "Silver")
        self.assertEqual(entry.get_type(), "Water")

        # Just make sure it works
        description = entry.get_description()

        self.assertTrue(entry.get_target().startswith("Selected"))
        self.assertEqual(entry.get_damage_class(), "Special")
        self.assertEqual(entry.get_base_power(), 65)

        pps = [10,12,14,16]
        for i in range(4):
            pp = entry.get_pp(i)
            self.assertEqual(pp, pps[i])

        self.assertAlmostEqual(entry.get_accuracy(), 0.85)
        self.assertEqual(entry.get_priority(), 0)

        # Just make sure it works
        effect = entry.get_effect()

        self.assertEqual(entry.get_contest_type(), "None")
        self.assertEqual(entry.get_contest_effect(), "None")
        self.assertEqual(entry.get_super_contest_effect(), "None")

        entry_same = pkmn.database.move_entry("Octazooka", "Silver")
        entry_different_move = pkmn.database.move_entry("Hydro Pump", "Silver")
        entry_different_game = pkmn.database.move_entry("Octazooka", "Gold")

        self.assertEqual(entry, entry_same)
        self.assertNotEqual(entry, entry_different_move)
        self.assertNotEqual(entry, entry_different_game)

    def test_pokemon_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.pokemon_entry("Invalid", "Black 2", "Sunny")
        with self.assertRaises(ValueError):
            entry = pkmn.database.pokemon_entry("Castform", "Not a game", "Sunny")
        with self.assertRaises(ValueError):
            entry = pkmn.database.pokemon_entry("Castform", "Black 2", "Not a form")

        entry = pkmn.database.pokemon_entry("Stunfisk", "Black 2", "")

        self.assertEqual(entry.get_name(), "Stunfisk")
        self.assertEqual(entry.get_game(), "Black 2")
        self.assertEqual(entry.get_species(), "Trap")
        self.assertEqual(entry.get_form(), "Standard")
        self.assertGreater(len(entry.get_pokedex_entry()), 0)
        self.assertAlmostEqual(entry.get_height(), 0.7)
        self.assertAlmostEqual(entry.get_weight(), 11.0)
        self.assertAlmostEqual(entry.get_chance_male(), 0.5)
        self.assertAlmostEqual(entry.get_chance_female(), 0.5)
        self.assertFalse(entry.has_gender_differences())
        self.assertEqual(entry.get_base_friendship(), 70)

        types = entry.get_types()
        self.assertEqual(types[0], "Ground")
        self.assertEqual(types[1], "Electric")

        abilities = entry.get_abilities()
        self.assertEqual(abilities[0], "Static")
        self.assertEqual(abilities[1], "Limber")

        self.assertEqual(entry.get_hidden_ability(), "Sand Veil")

        egg_groups = entry.get_egg_groups()
        self.assertEqual(egg_groups[0], "Water 1")
        self.assertEqual(egg_groups[1], "Amorphous")

        base_stats = entry.get_base_stats()
        self.assertEqual(base_stats["HP"], 109)
        self.assertEqual(base_stats["Attack"], 66)
        self.assertEqual(base_stats["Defense"], 84)
        self.assertEqual(base_stats["Speed"], 32)
        self.assertEqual(base_stats["Special Attack"], 81)
        self.assertEqual(base_stats["Special Defense"], 99)

        EV_yields = entry.get_EV_yields()
        self.assertEqual(EV_yields["HP"], 2)
        self.assertEqual(EV_yields["Attack"], 0)
        self.assertEqual(EV_yields["Defense"], 0)
        self.assertEqual(EV_yields["Speed"], 0)
        self.assertEqual(EV_yields["Special Attack"], 0)
        self.assertEqual(EV_yields["Special Defense"], 0)

        self.assertEqual(entry.get_experience_yield(), 165)
        self.assertEqual(entry.get_experience_at_level(50), 125000)
        self.assertEqual(entry.get_level_at_experience(200000), 58)
        self.assertGreater(len(entry.get_levelup_moves()), 0)
        self.assertGreater(len(entry.get_tm_hm_moves()), 0)
        self.assertGreater(len(entry.get_egg_moves()), 0)
        self.assertGreater(len(entry.get_tutor_moves()), 0)
        self.assertEqual(len(entry.get_forms()), 1)
        self.assertEqual(len(entry.get_evolutions()), 0)

        self.assertTrue(os.path.exists(entry.get_icon_filepath(False)))
        self.assertTrue(os.path.exists(entry.get_icon_filepath(True)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(False,False)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(False,True)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(True,False)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(True,True)))

        # Use different Pokemon for testing (in)equality
        entry_first = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
        entry_same = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
        entry_different_pokemon = pkmn.database.pokemon_entry("Pichu", "Omega Ruby", "Standard")
        entry_different_game = pkmn.database.pokemon_entry("Pikachu", "Alpha Sapphire", "Standard")
        entry_different_form = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Belle")

        self.assertEqual(entry_first, entry_same)
        self.assertNotEqual(entry_first, entry_different_pokemon)
        self.assertNotEqual(entry_first, entry_different_game)
        self.assertNotEqual(entry_first, entry_different_form)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(database_entry_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
