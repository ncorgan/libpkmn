#!/usr/bin/env python
#
# Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
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
            ability_list = pkmn.database.lists.get_ability_list(0)

        ability_list = pkmn.database.lists.get_ability_list(6)
        self.assertEqual(191, len(ability_list))
        self.assertEqual("Adaptability", ability_list[0])
        self.assertEqual("Zen Mode", ability_list[-1])

    def test_game_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            game_list = pkmn.database.lists.get_game_list(7, False)

        game_list = pkmn.database.lists.get_game_list(6, True)
        self.assertEqual(26, len(game_list))
        self.assertEqual("Red", game_list[0])
        self.assertEqual("Alpha Sapphire", game_list[-1])

    def test_gamecube_shadow_pokemon_list(self):
        colosseum_shadow_pokemon_list = pkmn.database.lists.get_gamecube_shadow_pokemon_list(True)
        self.assertEqual(48, len(colosseum_shadow_pokemon_list))

        xd_shadow_pokemon_list = pkmn.database.lists.get_gamecube_shadow_pokemon_list(False)
        self.assertEqual(83, len(xd_shadow_pokemon_list))

    def test_hm_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            hm_move_list = pkmn.database.lists.get_hm_move_list("Not a game")

        hm_move_list = pkmn.database.lists.get_hm_move_list("Red")
        self.assertEqual(5, len(hm_move_list))
        self.assertEqual("Cut", hm_move_list[0])
        self.assertEqual("Flash", hm_move_list[4])

    def test_item_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            item_list = pkmn.database.lists.get_item_list("not a game")

        item_list = pkmn.database.lists.get_item_list("HeartGold")
        self.assertEqual(513, len(item_list))
        self.assertEqual("Adamant Orb", item_list[0])
        self.assertEqual("Zoom Lens", item_list[-1])

    def test_location_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            location_list = pkmn.database.lists.get_location_list("Not a game", False)

        location_list = pkmn.database.lists.get_location_list("Emerald", False)
        self.assertEqual(106, len(location_list))
        self.assertEqual("Abandoned Ship", location_list[0])
        self.assertEqual("Victory Road", location_list[-1])

    def test_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            move_list = pkmn.database.lists.get_move_list("Not a game")

        move_list = pkmn.database.lists.get_move_list("Red")
        self.assertEqual(165, len(move_list))
        self.assertEqual("Pound", move_list[0])
        self.assertEqual("Struggle", move_list[-1])

    def test_nature_list(self):
        nature_list = pkmn.database.lists.get_nature_list()
        self.assertEqual(25, len(nature_list))
        self.assertEqual("Hardy", nature_list[0])
        self.assertEqual("Quirky", nature_list[-1])

    def test_pokemon_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            pokemon_list = pkmn.database.lists.get_pokemon_list(0, True)

        pokemon_list = pkmn.database.lists.get_pokemon_list(1, True)
        self.assertEqual(151, len(pokemon_list))
        self.assertEqual("Bulbasaur", pokemon_list[0])
        self.assertEqual("Mew", pokemon_list[-1])

    def test_region_list(self):
        region_list = pkmn.database.lists.get_region_list()
        self.assertEqual(7, len(region_list))
        self.assertEqual("Kanto", region_list[0])
        self.assertEqual("Kalos", region_list[-1])

    @unittest.skip("Function not yet implemented")
    def test_ribbon_list(self):
        ribbon_list = pkmn.database.lists.get_ribbon_list(3)

    def test_super_training_medal_list(self):
        super_training_medal_list = pkmn.database.lists.get_super_training_medal_list()
        self.assertEqual(30, len(super_training_medal_list))
        self.assertEqual("Sp. Atk Level 1", super_training_medal_list[0])
        self.assertEqual("The Battle for the Best!", super_training_medal_list[-1])

    def test_tm_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            tm_move_list = pkmn.database.lists.get_tm_move_list("Not a game")

        tm_move_list = pkmn.database.lists.get_tm_move_list("Red")
        self.assertEqual(50, len(tm_move_list))
        self.assertEqual("Mega Punch", tm_move_list[0])
        self.assertEqual("Substitute", tm_move_list[49])

    def test_type_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            type_list = pkmn.database.lists.get_type_list("Not a game")

        type_list = pkmn.database.lists.get_type_list("Alpha Sapphire")
        self.assertEqual(18, len(type_list))
        self.assertEqual("Normal", type_list[0])
        self.assertEqual("Fairy", type_list[-1])

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(lists_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
