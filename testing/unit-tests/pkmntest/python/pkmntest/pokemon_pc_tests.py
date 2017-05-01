#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

GAME_TO_GENERATION = {
    "Red": 1,
    "Blue": 1,
    "Yellow": 1,
    "Gold": 2,
    "Silver": 2,
    "Crystal": 2,
    "Ruby": 3,
    "Sapphire": 3,
    "Emerald": 3,
    "FireRed": 3,
    "LeafGreen": 3,
    "Colosseum": 3,
    "XD": 3
}

class pokemon_pc_tests(unittest.TestCase):

    def assertStringEqual(self, str1, str2):
        if PYTHON_MAJOR_VERSION == 2:
            if "str" in str(type(str1)):
                str1 = str1.decode("utf-8")
            if "str" in str(type(str2)):
                str2 = str2.decode("utf-8")

        self.assertEqual(str1, str2)

    def __get_both_string_types(self, input_str):
        if PYTHON_MAJOR_VERSION == 2:
            # Which call succeeds depends on SWIG version
            try:
                return (input_str, input_str.decode("utf-8"))
            except:
                return (input_str, unicode(input_str))
        else:
            return (input_str, input_str)

    def __test_empty_pokemon_box(self, box, game):
        generation = GAME_TO_GENERATION[game]

        self.assertEqual(box.get_game(), game)
        if generation == 1:
            with self.assertRaises(RuntimeError):
                box.get_name()
        else:
            self.assertEqual(box.get_name(), "")

        self.assertEqual(box.get_capacity(), len(box))

        # Make sure trying to get a Pokemon at an invalid index fails.
        with self.assertRaises(IndexError):
            box.get_pokemon(-1)
        with self.assertRaises(IndexError):
            box[-1]
        with self.assertRaises(IndexError):
            box.get_pokemon(len(box))
        with self.assertRaises(IndexError):
            box[len(box)]

        for pokemon in box:
            self.assertEqual(pokemon.get_species(), "None")
            self.assertEqual(pokemon.get_game(), game)

            for slot in pokemon.get_moves():
                self.assertEqual(slot.move.get_name(), "None")
                self.assertEqual(slot.pp, 0)

    def __test_box_name(self, box):
        generation = GAME_TO_GENERATION[box.get_game()]

        if generation == 1:
            strs = self.__get_both_string_types("ABCDEFGH")
            with self.assertRaises(RuntimeError):
                box.set_name(strs[0])
            with self.assertRaises(RuntimeError):
                box.set_name(strs[1])
            with self.assertRaises(RuntimeError):
                box.get_name()
        else:
            too_long_strs = self.__get_both_string_types("ABCDEFGHI")
            strs = self.__get_both_string_types("ABCDEFGH")

            with self.assertRaises(ValueError):
                box.set_name(too_long_strs[0])
            with self.assertRaises(ValueError):
                box.set_name(too_long_strs[1])

            box.set_name(strs[0])
            self.assertStringEqual(box.get_name(), strs[0])
            box.set_name(strs[1])
            self.assertStringEqual(box.get_name(), strs[1])

    def __test_setting_pokemon(self, box):
        game = box.get_game()
        generation = GAME_TO_GENERATION[game]

        original_first = box[0]
        original_second = box[1]

        # Make sure we can't set Pokemon at invalid indices.
        with self.assertRaises(IndexError):
            box[-1] = original_first
        with self.assertRaises(IndexError):
            box[len(box)] = original_second

        # Create Pokemon and place in box. The original variables should
        # have the same underlying Pokemon.
        bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
        charmander = pkmn.pokemon("Charmander", game, "", 5)
        squirtle = pkmn.pokemon("Squirtle", game, "", 5)
        
        box[0] = bulbasaur
        box[1] = charmander

        # Replace one of the new ones.
        box[0] = squirtle

        # Make sure we can't copy a Pokemon to itself.
        with self.assertRaises(ValueError):
            box[1] = box[1]

        # Copy a Pokemon whose memory is already part of the box.
        box[2] = box[1]

        # We should always be able to clear the last contiguous Pokemon.
        box[2] = original_first
        self.assertEqual(box.get_num_pokemon(), 2)
        self.assertStringEqual(box[2].get_species(), "None")

        # Put it back.
        box[2] = box[1]
        self.assertEqual(box.get_num_pokemon(), 3)

        # Check that Pokemon can be placed non-contiguously in the correct games.
        if generation <= 2:
            with self.assertRaises(ValueError):
                box[1] = original_first
            self.assertEqual(box.get_num_pokemon(), 3)
            self.assertStringEqual(box[1].get_species(), "Charmander")

            with self.assertRaises(IndexError):
                box[4] = bulbasaur
            self.assertEqual(box.get_num_pokemon(), 3)
            self.assertStringEqual(box[4].get_species(), "None")
        else:
            box[1] = original_first
            self.assertEqual(box.get_num_pokemon(), 2)
            self.assertStringEqual(box[1].get_species(), "None")

            box[4] = bulbasaur
            self.assertEqual(box.get_num_pokemon(), 3)
            self.assertStringEqual(box[4].get_species(), "Bulbasaur")

            # Restore it to how it was.
            box[1] = charmander
            box[4] = original_first
            self.assertStringEqual(box[1].get_species(), "Charmander")
            self.assertStringEqual(box[4].get_species(), "None")

        # Now check everything we've created. Each variable should have
        # the same underlying Pokemon.
        self.assertEqual(box[0].get_species(), "Squirtle")
        self.assertEqual(box[1].get_species(), "Charmander")
        self.assertEqual(box[2].get_species(), "Charmander")
        self.assertEqual(original_first.get_species(), "None")
        self.assertEqual(original_second.get_species(), "None")
        self.assertEqual(bulbasaur.get_species(), "Bulbasaur")
        self.assertEqual(charmander.get_species(), "Charmander")
        self.assertEqual(squirtle.get_species(), "Squirtle")

    def __test_pokemon_box(self, box, game):
        self.__test_empty_pokemon_box(box, game)
        self.__test_box_name(box)
        self.__test_setting_pokemon(box)

    def __test_empty_pokemon_pc(self, pc, game):
        self.assertEqual(pc.get_game(), game)
        self.assertEqual(pc.get_num_boxes(), len(pc))

        for box in pc:
            self.__test_empty_pokemon_box(box, game)

    def __test_box_names(self, pc):
        generation = GAME_TO_GENERATION[pc.get_game()]

        if generation == 1:
            strs = self.__get_both_string_types("ABCDEFGH")

            with self.assertRaises(RuntimeError):
                pc[0].set_name(strs[0])
            with self.assertRaises(RuntimeError):
                pc[0].set_name(strs[1])
        else:
            for i in range(len(pc)):
                name = "BOX{0}".format(i)
                pc[i].set_name(name)

            for i in range(len(pc)):
                name = "BOX{0}".format(i)
                self.assertEqual(pc[i].get_name(), name)

    def __test_setting_pokemon_in_boxes(self, pc):
        for box in pc:
            self.__test_setting_pokemon(box)
            self.assertEqual(box[0].get_species(), "Squirtle")
            self.assertEqual(box[1].get_species(), "Charmander")

    def __test_pokemon_pc(self, pc, game):
        self.__test_empty_pokemon_pc(pc, game)
        self.__test_box_names(pc)
        self.__test_setting_pokemon_in_boxes(pc)

    #
    # Red
    #

    def test_red_pokemon_box(self):
        box = pkmn.pokemon_box("Red")
        self.__test_pokemon_box(box, "Red")

    def test_red_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Red")
        self.__test_pokemon_pc(pc, "Red")

    #
    # Blue
    #

    def test_blue_pokemon_box(self):
        box = pkmn.pokemon_box("Blue")
        self.__test_pokemon_box(box, "Blue")

    def test_blue_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Blue")
        self.__test_pokemon_pc(pc, "Blue")

    #
    # Yellow
    #

    def test_yellow_pokemon_box(self):
        box = pkmn.pokemon_box("Yellow")
        self.__test_pokemon_box(box, "Yellow")

    def test_yellow_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Yellow")
        self.__test_pokemon_pc(pc, "Yellow")

    #
    # Gold
    #

    def test_gold_pokemon_box(self):
        box = pkmn.pokemon_box("Gold")
        self.__test_pokemon_box(box, "Gold")

    def test_gold_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Gold")
        self.__test_pokemon_pc(pc, "Gold")

    #
    # Silver
    #

    def test_silver_pokemon_box(self):
        box = pkmn.pokemon_box("Silver")
        self.__test_pokemon_box(box, "Silver")

    def test_silver_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Silver")
        self.__test_pokemon_pc(pc, "Silver")

    #
    # Crystal
    #

    def test_crystal_pokemon_box(self):
        box = pkmn.pokemon_box("Crystal")
        self.__test_pokemon_box(box, "Crystal")

    def test_crystal_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Crystal")
        self.__test_pokemon_pc(pc, "Crystal")

    #
    # Ruby
    #

    def test_ruby_pokemon_box(self):
        box = pkmn.pokemon_box("Ruby")
        self.__test_pokemon_box(box, "Ruby")

    def test_ruby_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Ruby")
        self.__test_pokemon_pc(pc, "Ruby")

    #
    # Sapphire
    #

    def test_sapphire_pokemon_box(self):
        box = pkmn.pokemon_box("Sapphire")
        self.__test_pokemon_box(box, "Sapphire")

    def test_sapphire_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Sapphire")
        self.__test_pokemon_pc(pc, "Sapphire")

    #
    # Emerald
    #

    def test_emerald_pokemon_box(self):
        box = pkmn.pokemon_box("Emerald")
        self.__test_pokemon_box(box, "Emerald")

    def test_emerald_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Emerald")
        self.__test_pokemon_pc(pc, "Emerald")

    #
    # FireRed
    #

    def test_firered_pokemon_box(self):
        box = pkmn.pokemon_box("FireRed")
        self.__test_pokemon_box(box, "FireRed")

    def test_firered_pokemon_pc(self):
        pc = pkmn.pokemon_pc("FireRed")
        self.__test_pokemon_pc(pc, "FireRed")

    #
    # LeafGreen
    #

    def test_leafgreen_pokemon_box(self):
        box = pkmn.pokemon_box("LeafGreen")
        self.__test_pokemon_box(box, "LeafGreen")

    def test_leafgreen_pokemon_pc(self):
        pc = pkmn.pokemon_pc("LeafGreen")
        self.__test_pokemon_pc(pc, "LeafGreen")

    #
    # Colosseum
    #

    def test_colosseum_pokemon_box(self):
        box = pkmn.pokemon_box("Colosseum")
        self.__test_pokemon_box(box, "Colosseum")

    def test_colosseum_pokemon_pc(self):
        pc = pkmn.pokemon_pc("Colosseum")
        self.__test_pokemon_pc(pc, "Colosseum")

    #
    # XD
    #

    def test_xd_pokemon_box(self):
        box = pkmn.pokemon_box("XD")
        self.__test_pokemon_box(box, "XD")

    def test_xd_pokemon_pc(self):
        pc = pkmn.pokemon_pc("XD")
        self.__test_pokemon_pc(pc, "XD")
