#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import base_test

import pkmn

import sys
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

PYTHON_MAJOR_VERSION = sys.version_info[0]

class pokemon_pc_tests(base_test.base_test):

    def __test_empty_pokemon_box(self, box):
        self.assertEqual(box.name, "")

        # Make sure trying to get a Pokemon at an invalid index fails.
        with self.assertRaises(OverflowError):
            box[-1]
        with self.assertRaises(IndexError):
            box[len(box)]

        for pokemon in box:
            self.assertEqual(pokemon.species, "None")
            self.assertEqual(pokemon.game, box.game)

            for slot in pokemon.moves:
                self.assertEqual(slot.move, "None")
                self.assertEqual(slot.pp, 0)

    def __test_box_name(self, box):
        generation = base_test.GAME_TO_GENERATION[box.game]

        if generation == 1:
            strs = self.get_both_string_types("ABCDEFGH")
            with self.assertRaises(RuntimeError):
                box.name = strs[0]
            with self.assertRaises(RuntimeError):
                box.name = strs[1]
        else:
            too_long_strs = self.get_both_string_types("ABCDEFGHI")
            strs = self.get_both_string_types("ABCDEFGH")

            with self.assertRaises(ValueError):
                box.name = too_long_strs[0]
            with self.assertRaises(ValueError):
                box.name = too_long_strs[1]

            box.name = strs[0]
            self.assertStringEqual(box.name, strs[0])
            box.name = strs[1]
            self.assertStringEqual(box.name, strs[1])

    def __test_setting_pokemon(self, box):
        game = box.game
        generation = base_test.GAME_TO_GENERATION[game]

        original_first = box[0]
        original_second = box[1]

        # Make sure we can't set Pokemon at invalid indices.
        with self.assertRaises(OverflowError):
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
        self.assertEqual(box.num_pokemon, 2)
        self.assertStringEqual(box[2].species, "None")

        # Put it back.
        box[2] = box[1]
        self.assertEqual(box.num_pokemon, 3)

        # Check that Pokemon can be placed non-contiguously in the correct games.
        if generation <= 2:
            with self.assertRaises(ValueError):
                box[1] = original_first
            self.assertEqual(box.num_pokemon, 3)
            self.assertStringEqual(box[1].species, "Charmander")

            with self.assertRaises(IndexError):
                box[4] = bulbasaur
            self.assertEqual(box.num_pokemon, 3)
            self.assertStringEqual(box[4].species, "None")
        else:
            box[1] = original_first
            self.assertEqual(box.num_pokemon, 2)
            self.assertStringEqual(box[1].species, "None")

            box[4] = bulbasaur
            self.assertEqual(box.num_pokemon, 3)
            self.assertStringEqual(box[4].species, "Bulbasaur")

            # Restore it to how it was.
            box[1] = charmander
            box[4] = original_first
            self.assertStringEqual(box[1].species, "Charmander")
            self.assertStringEqual(box[4].species, "None")

        # Now check everything we've created. Each variable should have
        # the same underlying Pokemon.
        self.assertEqual(box[0].species, "Squirtle")
        self.assertEqual(box[1].species, "Charmander")
        self.assertEqual(box[2].species, "Charmander")
        self.assertEqual(original_first.species, "None")
        self.assertEqual(original_second.species, "None")
        self.assertEqual(bulbasaur.species, "Bulbasaur")
        self.assertEqual(charmander.species, "Charmander")
        self.assertEqual(squirtle.species, "Squirtle")

    def __test_empty_pokemon_pc(self, pc):
        for box in pc:
            self.__test_empty_pokemon_box(box)

    def __test_box_names(self, pc):
        generation = base_test.GAME_TO_GENERATION[pc.game]

        if generation == 1:
            strs = self.get_both_string_types("ABCDEFGH")

            with self.assertRaises(RuntimeError):
                pc[0].name = strs[0]
            with self.assertRaises(RuntimeError):
                pc[0].name = strs[1]
        else:
            for pc_index in range(len(pc)):
                name = "BOX{0}".format(pc_index)
                pc[pc_index].name = name
                self.assertEqual(pc[pc_index].name, name)

    def __test_setting_pokemon_in_boxes(self, pc):
        for box in pc:
            self.__test_setting_pokemon(box)
            self.assertEqual(box[0].species, "Squirtle")
            self.assertEqual(box[1].species, "Charmander")

    @parameterized.expand(base_test.ALL_GAMES, testcase_func_name=test_name_func)
    def test_pokemon_box(self, game):
        box = pkmn.pokemon_box(game)

        self.__test_empty_pokemon_box(box)
        self.__test_box_name(box)
        self.__test_setting_pokemon(box)

    @parameterized.expand(base_test.ALL_GAMES, testcase_func_name=test_name_func)
    def test_pokemon_pc(self, game):
        pc = pkmn.pokemon_pc(game)

        self.__test_empty_pokemon_pc(pc)
        self.__test_box_names(pc)
        self.__test_setting_pokemon_in_boxes(pc)
