#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from nose_parameterized import parameterized
import unittest

import os
import random
import sys

PKSAV_TEST_SAVES = os.environ["PKSAV_TEST_SAVES"]
PKMN_TMP_DIR = pkmn.get_tmp_dir()

TOO_LONG_OT_NAME = "LibPKMNLibPKMN"
LIBPKMN_OT_PID = 1351
LIBPKMN_OT_SID = 32123
MONEY_MAX_VALUE = 999999

GB_GAMES = ["Red", "Blue", "Yellow",
            "Gold", "Silver", "Crystal"]
RIVAL_NAME_SET_GAMES = ["Ruby", "Sapphire", "Emerald",
                        "Black", "White",
                        "X", "Y"]
MALE_ONLY_GAMES = ["Red", "Blue", "Yellow",
                   "Gold", "Silver"]

GAME_GENERATIONS = dict(
    Red = 1,
    Blue = 1,
    Yellow = 1,
    Gold = 2,
    Silver = 2,
    Crystal = 2,
    Ruby = 3,
    Sapphire = 3,
    Emerald = 3,
    FireRed = 3,
    LeafGreen = 3,
    Colosseum = 3,
    XD = 3
)

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[1])

class game_save_test(unittest.TestCase):

    def __test_trainer_name(self):
        with self.assertRaises(ValueError):
            self.save.set_trainer_name("")
        with self.assertRaises(ValueError):
            self.save.set_trainer_name(TOO_LONG_OT_NAME)

        self.save.set_trainer_name(pkmn.LIBPKMN_OT_NAME)
        self.assertEqual(self.save.get_trainer_name(), pkmn.LIBPKMN_OT_NAME)

    def __test_rival_name(self):
        if self.save.get_game() in RIVAL_NAME_SET_GAMES:
            with self.assertRaises(RuntimeError):
                self.save.set_rival_name(pkmn.LIBPKMN_OT_NAME)
        else:
            with self.assertRaises(ValueError):
                self.save.set_rival_name("")
            with self.assertRaises(ValueError):
                self.save.set_rival_name(TOO_LONG_OT_NAME)

            self.save.set_rival_name(pkmn.LIBPKMN_OT_NAME)
            self.assertEqual(self.save.get_rival_name(), pkmn.LIBPKMN_OT_NAME)

    def __test_trainer_id(self):
        is_gb_game = (self.save.get_game() in GB_GAMES)
        self.assertEqual(
            self.save.get_trainer_id(),
            (LIBPKMN_OT_PID if is_gb_game else pkmn.LIBPKMN_OT_ID)
        )
        self.assertEqual(self.save.get_trainer_public_id(), LIBPKMN_OT_PID)
        if is_gb_game:
            with self.assertRaises(RuntimeError):
                self.save.get_trainer_secret_id()
        else:
            self.assertEqual(self.save.get_trainer_secret_id(), LIBPKMN_OT_SID)

    def __test_common_fields(self):
        self.__test_trainer_name()

        is_gb_game = (self.save.get_game() in GB_GAMES)
        self.save.set_trainer_id(
            (LIBPKMN_OT_PID if is_gb_game else pkmn.LIBPKMN_OT_ID)
        )
        self.__test_trainer_id()

        self.save.set_trainer_public_id(LIBPKMN_OT_PID)
        self.__test_trainer_id()

        # Make sure the SWIG wrapper keeps it within the proper bounds. Which error
        # applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                self.save.set_trainer_public_id(-1)
        except:
            with self.assertRaises(TypeError):
                self.save.set_trainer_public_id(-1)
        try:
            with self.assertRaises(OverflowError):
                self.save.set_trainer_public_id(0xFFFF+1)
        except:
            with self.assertRaises(TypeError):
                self.save.set_trainer_public_id(0xFFFF+1)
        try:
            with self.assertRaises(OverflowError):
                self.save.set_trainer_id(-1)
        except:
            with self.assertRaises(TypeError):
                self.save.set_trainer_id(-1)

        if is_gb_game:
            with self.assertRaises(IndexError):
                self.save.set_trainer_id(0xFFFF+1)

            with self.assertRaises(IndexError):
                self.save.set_trainer_id(pkmn.LIBPKMN_OT_ID)
            with self.assertRaises(RuntimeError):
                self.save.set_trainer_secret_id(LIBPKMN_OT_SID)
        else:
            try:
                with self.assertRaises(OverflowError):
                    self.save.set_trainer_secret_id(0xFFFF+1)
            except:
                with self.assertRaises(TypeError):
                    self.save.set_trainer_secret_id(0xFFFF+1)
            try:
                with self.assertRaises(OverflowError):
                    self.save.set_trainer_public_id(0xFFFFFFFF+1)
            except:
                with self.assertRaises(TypeError):
                    self.save.set_trainer_public_id(0xFFFFFFFF+1)
            try:
                with self.assertRaises(OverflowError):
                    self.save.set_trainer_public_id(-1)
            except:
                with self.assertRaises(TypeError):
                    self.save.set_trainer_public_id(-1)

            self.save.set_trainer_secret_id(LIBPKMN_OT_SID)
            self.__test_trainer_id()

        self.__test_rival_name()

        if self.save.get_game() in MALE_ONLY_GAMES:
            self.assertEqual(self.save.get_trainer_gender(), "Male")
            with self.assertRaises(RuntimeError):
                self.save.set_trainer_gender("Male")
            with self.assertRaises(RuntimeError):
                self.save.set_trainer_gender("Female")
        else:
            self.save.set_trainer_gender("Male")
            self.assertEqual(self.save.get_trainer_gender(), "Male")
            self.save.set_trainer_gender("Female")
            self.assertEqual(self.save.get_trainer_gender(), "Female")
            with self.assertRaises(ValueError):
                self.save.set_trainer_gender("Genderless")

        with self.assertRaises(IndexError):
            self.save.set_money(-1)
        with self.assertRaises(IndexError):
            self.save.set_money(MONEY_MAX_VALUE+1)

        self.save.set_money(123456)
        self.assertEqual(self.save.get_money(), 123456)

        # Pokemon Party
        party = self.save.get_pokemon_party()
        num_pokemon = party.get_num_pokemon()
        self.assertEquals(len(party), 6)
        self.assertLessEqual(num_pokemon, 6)

        for i in range(6):
            if i < num_pokemon:
                self.assertNotEquals(party[i].get_species(), "None")
            else:
                self.assertEquals(party[i].get_species(), "None")

        # Pokemon PC
        pc = self.save.get_pokemon_pc()
        self.assertEquals(len(pc), pc.get_num_boxes())

        for i in range(len(pc)):
            box = pc[i]
            num_pokemon = box.get_num_pokemon()

            self.assertEquals(len(box), box.get_capacity())
            self.assertLessEqual(num_pokemon, len(box))

            # Boxes are only contiguous in Game Boy games.
            if is_gb_game:
                for i in range(len(box)):
                    if i < num_pokemon:
                        self.assertNotEquals(box[i].get_species(), "None")
                    else:
                        self.assertEquals(box[i].get_species(), "None")

    def __compare_game_saves(self):
        game = self.save.get_game()
        generation = GAME_GENERATIONS[game]
        is_gb_game = (game in GB_GAMES)
        is_male_only = (game in MALE_ONLY_GAMES)
        is_rival_name_set = (game in RIVAL_NAME_SET_GAMES)

        self.assertEquals(self.save.get_game(), self.save2.get_game())
        self.assertEquals(self.save.get_trainer_id(), self.save2.get_trainer_id())
        self.assertEquals(self.save.get_trainer_public_id(), self.save2.get_trainer_public_id())

        if not is_gb_game:
            self.assertEquals(self.save.get_trainer_secret_id(), self.save2.get_trainer_secret_id())
        if not is_male_only:
            self.assertEquals(self.save.get_trainer_gender(), self.save2.get_trainer_gender())
        if not is_rival_name_set:
            self.assertEquals(self.save.get_rival_name(), self.save2.get_rival_name())

        self.assertEquals(self.save.get_money(), self.save2.get_money())

        bag1 = self.save.get_item_bag()
        bag2 = self.save2.get_item_bag()
        self.assertEquals(len(bag1), len(bag2))
        self.assertEquals(len(bag1.get_pocket_names()), len(bag2.get_pocket_names()))
        for name in bag1.get_pocket_names():
            foo = bag1.get_pocket(name)
            print(name)

    @parameterized.expand([
        ("Red/Blue/Yellow", "Red", "red_blue", "pokemon_red.sav"),
        ("Red/Blue/Yellow", "Yellow", "yellow", "pokemon_yellow.sav"),
        ("Gold/Silver", "Gold", "gold_silver", "pokemon_gold.sav"),
        ("Crystal", "Crystal", "crystal", "pokemon_crystal.sav"),
        ("Ruby/Sapphire", "Ruby", "ruby_sapphire", "pokemon_ruby.sav"),
        ("Emerald", "Emerald", "emerald", "pokemon_emerald.sav"),
        ("FireRed/LeafGreen", "FireRed", "firered_leafgreen", "pokemon_firered.sav")
    ], testcase_func_name=test_name_func)
    def test_game_save(self, expected_type, expected_game, subdir, filename):
        filepath = os.path.join(PKSAV_TEST_SAVES, subdir, filename)
        self.assertEquals(pkmn.detect_game_save_type(filepath), expected_type)

        self.save = pkmn.game_save(filepath)
        self.assertEquals(self.save.get_game(), expected_game)

        self.__test_common_fields()

        temp_save_filepath = os.path.join(PKMN_TMP_DIR, "{0}_{1}.sav".format(expected_game, random.randint(0, 0xFFFF)))
        self.save.save_as(temp_save_filepath)

        self.save2 = pkmn.game_save(temp_save_filepath)
        self.__compare_game_saves()

        os.remove(temp_save_filepath)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(game_save_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
