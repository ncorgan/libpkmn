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

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
PKSAV_TEST_SAVES = os.environ["PKSAV_TEST_SAVES"]
PKMN_TMP_DIR = pkmn.get_tmp_dir()

TOO_LONG_OT_NAME = "LibPKMNLibPKMN"
LIBPKMN_OT_PID = 1351
LIBPKMN_OT_SID = 32123
MONEY_MAX_VALUE = 999999

GB_GAMES = ["Red", "Blue", "Yellow",
            "Gold", "Silver", "Crystal"]
RIVAL_NAME_SET_GAMES = ["Ruby", "Sapphire", "Emerald",
                        "Colosseum", "XD",
                        "Black", "White",
                        "X", "Y"]
MALE_ONLY_GAMES = ["Red", "Blue", "Yellow",
                   "Gold", "Silver",
                   "Colosseum", "XD"]

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

    def __get_random_pokemon(self, pokemon_list, move_list, item_list):
        game = self.save.get_game()
        generation = GAME_GENERATIONS[game]

        species = ""
        while True:
            species = pokemon_list[random.randint(0, len(pokemon_list)-1)]
            if generation > 3 or species != "Deoxys":
                break

        ret = pkmn.pokemon(species, game, "", random.randint(2, 100))

        for i in range(4):
            while True:
                move = move_list[random.randint(0, len(move_list)-1)]
                if not move.startswith("Shadow"):
                    break
            ret.set_move(move, i)

        if generation >= 2:
            while True:
                try:
                    ret.set_held_item(item_list[random.randint(0, len(item_list)-1)])
                    break
                except ValueError:
                    continue

        return ret

    def __randomize_pokemon(self, item_list):
        game = self.save.get_game()
        generation = GAME_GENERATIONS[game]
        pokemon_list = pkmn.database.get_pokemon_list(generation, True)
        move_list = pkmn.database.get_move_list(game)

        party = self.save.get_pokemon_party()
        for i in range(6):
            party[i] = self.__get_random_pokemon(pokemon_list, move_list, item_list)

        pc = self.save.get_pokemon_pc()
        for i in range(len(pc)):
            box = pc[i]
            for j in range(len(box)):
                box[j] = self.__get_random_pokemon(pokemon_list, move_list, item_list)

    def __compare_item_lists(self, item_list1, item_list2):
        self.assertEquals(item_list1.get_game(), item_list2.get_game())
        self.assertEquals(item_list1.get_name(), item_list2.get_name())
        self.assertEquals(len(item_list1), len(item_list2))
        self.assertEquals(item_list1.get_num_items(), item_list2.get_num_items())

        for i in range(len(item_list1)):
            self.assertEquals(item_list1[i].item.get_name(), item_list2[i].item.get_name())
            self.assertEquals(item_list1[i].amount, item_list2[i].amount)
        return

    def __compare_pokemon(self, pokemon1, pokemon2):
        self.assertEquals(pokemon1.get_game(), pokemon2.get_game())
        self.assertEquals(pokemon1.get_species(), pokemon2.get_species())
        self.assertEquals(pokemon1.get_form(), pokemon2.get_form())
        self.assertEquals(pokemon1.get_nickname(), pokemon2.get_nickname())
        self.assertEquals(pokemon1.get_trainer_name(), pokemon2.get_trainer_name())

        moves1 = pokemon1.get_moves()
        moves2 = pokemon2.get_moves()
        for i in range(4):
            self.assertEquals(moves1[i].move.get_name(), moves2[i].move.get_name())
            self.assertEquals(moves1[i].pp, moves2[i].pp)

        if GAME_GENERATIONS[pokemon1.get_game()] >= 2:
            self.assertEquals(pokemon1.get_held_item(), pokemon2.get_held_item())

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
            self.__compare_item_lists(bag1[name], bag2[name])

        if generation <= 3:
            self.__compare_item_lists(self.save.get_item_pc(), self.save2.get_item_pc())

        party1 = self.save.get_pokemon_party()
        party2 = self.save2.get_pokemon_party()
        for i in range(6):
            self.__compare_pokemon(party1[i], party2[i])

        pc1 = self.save.get_pokemon_pc()
        pc2 = self.save2.get_pokemon_pc()
        self.assertEquals(len(pc1), len(pc2))
        for i in range(len(pc1)):
            box1 = pc1[i]
            box2 = pc2[i]
            self.assertEquals(len(box1), len(box2))
            for j in range(len(box1)):
                self.__compare_pokemon(box1[j], box2[j])

    @parameterized.expand([
        ("Red/Blue/Yellow", "Red", "red_blue", "pokemon_red.sav"),
        ("Red/Blue/Yellow", "Yellow", "yellow", "pokemon_yellow.sav"),
        ("Gold/Silver", "Gold", "gold_silver", "pokemon_gold.sav"),
        ("Crystal", "Crystal", "crystal", "pokemon_crystal.sav"),
        ("Ruby/Sapphire", "Ruby", "ruby_sapphire", "pokemon_ruby.sav"),
        ("Emerald", "Emerald", "emerald", "pokemon_emerald.sav"),
        ("FireRed/LeafGreen", "FireRed", "firered_leafgreen", "pokemon_firered.sav"),
        ("Colosseum/XD", "Colosseum", "gamecube_saves", "pokemon_colosseum.gci"),
        ("Colosseum/XD", "XD", "gamecube_saves", "pokemon_xd.gci")
    ], testcase_func_name=test_name_func)
    def test_game_save(self, expected_type, expected_game, subdir, filename):
        if expected_game == "Colosseum" or expected_game == "XD":
            filepath = os.path.join(LIBPKMN_TEST_FILES, subdir, filename)
        else:
            filepath = os.path.join(PKSAV_TEST_SAVES, subdir, filename)
        self.assertEquals(pkmn.detect_game_save_type(filepath), expected_type)

        self.save = pkmn.game_save(filepath)
        self.assertEquals(self.save.get_game(), expected_game)

        item_list = pkmn.database.get_item_list(expected_game)

        self.__test_common_fields()
        # TODO: randomize_items
        self.__randomize_pokemon(item_list)

        temp_save_filepath = os.path.join(PKMN_TMP_DIR, "{0}_{1}.sav".format(expected_game, random.randint(0, 0xFFFF)))
        self.save.save_as(temp_save_filepath)

        self.save2 = pkmn.game_save(temp_save_filepath)
        self.__compare_game_saves()

        os.remove(temp_save_filepath)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(game_save_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
