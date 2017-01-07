#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import sys
import unittest

class lists_test(unittest.TestCase):

    def test_ability_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            ability_list = pkmn.database.get_ability_list(0)

        ability_list = pkmn.database.get_ability_list(6)
        self.assertEqual(len(ability_list), 191)
        self.assertEqual(ability_list[0], "Adaptability")
        self.assertEqual(ability_list[-1], "Zen Mode")

    def test_game_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            game_list = pkmn.database.get_game_list(7, False)

        game_list = pkmn.database.get_game_list(6, True)
        self.assertEqual(len(game_list), 26)
        self.assertEqual(game_list[0], "Red")
        self.assertEqual(game_list[-1], "Alpha Sapphire")

    def test_item_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            item_list = pkmn.database.get_item_list("not a game")

        item_list = pkmn.database.get_item_list("HeartGold")
        self.assertEqual(len(item_list), 513)
        self.assertEqual(item_list[0], "Adamant Orb")
        self.assertEqual(item_list[-1], "Zoom Lens")

    def test_location_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            location_list = pkmn.database.get_location_list("Not a game", False)

        location_list = pkmn.database.get_location_list("Emerald", False)
        self.assertEqual(len(location_list), 106)
        self.assertEqual(location_list[0], "Abandoned Ship")
        self.assertEqual(location_list[-1], "Victory Road")

    def test_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            move_list = pkmn.database.get_move_list("Not a game")

        move_list = pkmn.database.get_move_list("Red")
        self.assertEqual(len(move_list), 165)
        self.assertEqual(move_list[0], "Pound")
        self.assertEqual(move_list[-1], "Struggle")

    def test_nature_list(self):
        nature_list = pkmn.database.get_nature_list()
        self.assertEqual(len(nature_list), 25)
        self.assertEqual(nature_list[0], "Hardy")
        self.assertEqual(nature_list[-1], "Quirky")

    def test_pokemon_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            pokemon_list = pkmn.database.get_pokemon_list(0, True)

        pokemon_list = pkmn.database.get_pokemon_list(1, True)
        self.assertEqual(len(pokemon_list), 151)
        self.assertEqual(pokemon_list[0], "Bulbasaur")
        self.assertEqual(pokemon_list[-1], "Mew")

    def test_region_list(self):
        region_list = pkmn.database.get_region_list()
        self.assertEqual(len(region_list), 7)
        self.assertEqual(region_list[0], "Kanto")
        self.assertEqual(region_list[-1], "Kalos")

    @unittest.skip("Function not yet implemented")
    def test_ribbon_list(self):
        ribbon_list = pkmn.database.get_ribbon_list(3)

    def test_super_training_medal_list(self):
        super_training_medal_list = pkmn.database.get_super_training_medal_list()
        self.assertEqual(len(super_training_medal_list), 30)
        self.assertEqual(super_training_medal_list[0], "Sp. Atk Level 1")
        self.assertEqual(super_training_medal_list[-1], "The Battle for the Best!")

    def test_type_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            type_list = pkmn.database.get_type_list("Not a game")

        type_list = pkmn.database.get_type_list("Alpha Sapphire")
        self.assertEqual(len(type_list), 18)
        self.assertEqual(type_list[0], "Normal")
        self.assertEqual(type_list[-1], "Fairy")

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(lists_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
