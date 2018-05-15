#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pkmntest.base_test

from nose_parameterized import parameterized
import unittest

import os
import random
import sys

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
PKSAV_TEST_SAVES = os.environ["PKSAV_TEST_SAVES"]
PKMN_TMP_DIR = pkmn.paths.get_tmp_dir()

TOO_LONG_OT_NAME = "LibPKMNLibPKMN"
DEFAULT_TRAINER_PID = 1351
DEFAULT_TRAINER_SID = 32123
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

class game_save_test(pkmntest.base_test):

    def __test_trainer_name(self):
        with self.assertRaises(ValueError):
            self.save.trainer_name = ""
        with self.assertRaises(ValueError):
            self.save.trainer_name = TOO_LONG_OT_NAME

        self.save.trainer_name = pkmn.pokemon.DEFAULT_TRAINER_NAME
        self.assertEqual(self.save.trainer_name, pkmn.pokemon.DEFAULT_TRAINER_NAME)

    def __test_rival_name(self):
        if self.save.game in RIVAL_NAME_SET_GAMES:
            with self.assertRaises(RuntimeError):
                self.save.rival_name = pkmn.pokemon.DEFAULT_TRAINER_NAME
        else:
            with self.assertRaises(ValueError):
                self.save.rival_name = ""
            with self.assertRaises(ValueError):
                self.save.rival_name = TOO_LONG_OT_NAME

            self.save.rival_name = pkmn.pokemon.DEFAULT_TRAINER_NAME
            self.assertEqual(self.save.rival_name, pkmn.pokemon.DEFAULT_TRAINER_NAME)

    def __test_trainer_id(self):
        is_gb_game = (self.save.game in GB_GAMES)
        self.assertEqual(
            self.save.trainer_id,
            (DEFAULT_TRAINER_PID if is_gb_game else pkmn.pokemon.DEFAULT_TRAINER_ID)
        )
        self.assertEqual(self.save.trainer_public_id, DEFAULT_TRAINER_PID)
        if is_gb_game:
            with self.assertRaises(RuntimeError):
                self.save.trainer_secret_id
        else:
            self.assertEqual(self.save.trainer_secret_id, DEFAULT_TRAINER_SID)

    def __test_common_fields(self):
        self.__test_trainer_name()

        is_gb_game = (self.save.game in GB_GAMES)
        self.save.trainer_id = \
            (DEFAULT_TRAINER_PID if is_gb_game else pkmn.pokemon.DEFAULT_TRAINER_ID)
        self.__test_trainer_id()

        self.save.trainer_public_id = DEFAULT_TRAINER_PID
        self.__test_trainer_id()

        # Make sure the SWIG wrapper keeps it within the proper bounds. Which error
        # applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                self.save.trainer_public_id = -1
        except:
            with self.assertRaises(TypeError):
                self.save.trainer_public_id = -1
        try:
            with self.assertRaises(OverflowError):
                self.save.trainer_public_id = 0xFFFF+1
        except:
            with self.assertRaises(TypeError):
                self.save.trainer_public_id = 0xFFFF+1
        try:
            with self.assertRaises(OverflowError):
                self.save.trainer_id = -1
        except:
            with self.assertRaises(TypeError):
                self.save.trainer_id = -1

        if is_gb_game:
            with self.assertRaises(IndexError):
                self.save.trainer_id = 0xFFFF+1

            with self.assertRaises(IndexError):
                self.save.trainer_id = pkmn.pokemon.DEFAULT_TRAINER_ID
            with self.assertRaises(RuntimeError):
                self.save.trainer_secret_id = DEFAULT_TRAINER_SID
        else:
            try:
                with self.assertRaises(OverflowError):
                    self.save.trainer_secret_id = 0xFFFF+1
            except:
                with self.assertRaises(TypeError):
                    self.save.trainer_secret_id = 0xFFFF+1
            try:
                with self.assertRaises(OverflowError):
                    self.save.trainer_public_id = 0xFFFFFFFF+1
            except:
                with self.assertRaises(TypeError):
                    self.save.trainer_public_id = 0xFFFFFFFF+1
            try:
                with self.assertRaises(OverflowError):
                    self.save.trainer_public_id = -1
            except:
                with self.assertRaises(TypeError):
                    self.save.trainer_public_id = -1

            self.save.trainer_secret_id = DEFAULT_TRAINER_SID
            self.__test_trainer_id()

        self.__test_rival_name()

        if self.save.game in MALE_ONLY_GAMES:
            self.assertEqual(self.save.trainer_gender, "Male")
            with self.assertRaises(RuntimeError):
                self.save.trainer_gender = "Male"
            with self.assertRaises(RuntimeError):
                self.save.trainer_gender = "Female"
        else:
            self.save.trainer_gender = "Male"
            self.assertEqual(self.save.trainer_gender, "Male")
            self.save.trainer_gender = "Female"
            self.assertEqual(self.save.trainer_gender, "Female")
            with self.assertRaises(ValueError):
                self.save.trainer_gender = "Genderless"

        with self.assertRaises(IndexError):
            self.save.money = -1
        with self.assertRaises(IndexError):
            self.save.money = MONEY_MAX_VALUE+1

        self.save.money = 123456
        self.assertEqual(self.save.money, 123456)

        # Pokemon Party
        party = self.save.pokemon_party
        self.assertEquals(len(party), 6)
        self.assertLessEqual(party.num_pokemon, 6)

        for party_index in range(6):
            if party_index < party.num_pokemon:
                self.assertNotEquals(party[party_index].species, "None")
            else:
                self.assertEquals(party[party_index].species, "None")

        # Pokemon PC
        pc = self.save.pokemon_pc

        for box in pc:
            self.assertLessEqual(box.num_pokemon, len(box))

            # Boxes are only contiguous in Game Boy games.
            if is_gb_game:
                for box_index in range(len(box)):
                    if box_index < box.num_pokemon:
                        self.assertNotEquals(box[box_index].species, "None")
                    else:
                        self.assertEquals(box[box_index].species, "None")

        # Pokedex
        if self.save.game not in ["Colosseum", "XD"]:
            pokedex = self.save.pokedex
            self.assertGreaterEqual(
                pokedex.num_seen,
                pokedex.num_caught
            )

            for pokemon in self.save.pokemon_party:
                species = pokemon.species
                if species != "None" and not pokemon.is_egg:
                    self.assertTrue(pokedex.seen_pokemon_map[species])
                    self.assertTrue(pokedex.caught_pokemon_map[species])

            for box in self.save.pokemon_pc:
                for pokemon in box:
                    species = pokemon.species
                    if species != "None" and not pokemon.is_egg:
                        self.assertTrue(pokedex.seen_pokemon_map[species])
                        self.assertTrue(pokedex.caught_pokemon_map[species])

    def __test_attributes(self):
        game = self.save.game
        generation = GAME_GENERATIONS[game]

        if generation == 1:
            self.assertTrue("Casino coins" in self.save.numeric_attributes.names)
            self.assertGreaterEqual(
                self.save.numeric_attributes["Casino coins"],
                0
            )
            self.assertLessEqual(
                self.save.numeric_attributes["Casino coins"],
                9999
            )

            # TODO: uncomment after fixing:
            #  * https://github.com/ncorgan/pksav/issues/3
            '''
            casino_coins = random.randint(0, 9999)
            self.save.numeric_attributes["Casino coins"] = casino_coins
            self.assertEqual(
                self.save.numeric_attributes["Casino coins"],
                casino_coins
            )
            '''

            if game == "Yellow":
                self.assertTrue("Pikachu friendship" in self.save.numeric_attributes.names)
                self.assertGreaterEqual(
                    self.save.numeric_attributes["Pikachu friendship"],
                    0
                )
                self.assertLessEqual(
                    self.save.numeric_attributes["Pikachu friendship"],
                    255
                )

                pikachu_friendship = random.randint(0, 255)
                self.save.numeric_attributes["Pikachu friendship"] = pikachu_friendship
                self.assertEqual(
                    self.save.numeric_attributes["Pikachu friendship"],
                    pikachu_friendship
                )
            else:
                self.assertFalse("Pikachu friendship" in self.save.numeric_attributes.names)
        elif generation == 3:
            if game != "Colosseum" and game != "XD":
                self.assertTrue("Casino coins" in self.save.numeric_attributes.names)
                self.assertGreaterEqual(
                    self.save.numeric_attributes["Casino coins"],
                    0
                )
                self.assertLessEqual(
                    self.save.numeric_attributes["Casino coins"],
                    9999
                )

                casino_coins = random.randint(0, 9999)
                self.save.numeric_attributes["Casino coins"] = casino_coins
                self.assertEqual(
                    self.save.numeric_attributes["Casino coins"],
                    casino_coins
                )

    def __randomize_pokemon(self, item_list):
        game = self.save.game
        generation = GAME_GENERATIONS[game]
        pokemon_list = pkmn.database.lists.get_pokemon_list(generation, True)
        move_list = pkmn.database.lists.get_move_list(game)

        party = self.save.pokemon_party
        for party_index in range(6):
            party[party_index] = self.get_random_pokemon(game, pokemon_list, move_list, item_list)

        pc = self.save.pokemon_pc
        for box in pc:
            for box_index in range(len(box)):
                box[box_index] = self.get_random_pokemon(game, pokemon_list, move_list, item_list)

    def __compare_item_lists(self, item_list1, item_list2):
        self.assertEquals(item_list1.game, item_list2.game)
        self.assertEquals(item_list1.name, item_list2.name)
        self.assertEquals(len(item_list1), len(item_list2))
        self.assertEquals(item_list1.num_items, item_list2.num_items)

        for item_index in range(len(item_list1)):
            self.assertEquals(item_list1[item_index].item, item_list2[item_index].item)
            self.assertEquals(item_list1[item_index].amount, item_list2[item_index].amount)

    def __compare_pokedexes(self, pokedex1, pokedex2):
        self.assertEquals(pokedex1.all_seen, pokedex2.all_seen)
        self.assertEquals(pokedex1.all_caught, pokedex2.all_caught)

    def __compare_game_saves(self):
        game = self.save.game
        generation = GAME_GENERATIONS[game]
        is_gb_game = (game in GB_GAMES)
        is_male_only = (game in MALE_ONLY_GAMES)
        is_rival_name_set = (game in RIVAL_NAME_SET_GAMES)

        self.assertEquals(self.save.game, self.save2.game)
        self.assertEquals(self.save.trainer_id, self.save2.trainer_id)
        self.assertEquals(self.save.trainer_public_id, self.save2.trainer_public_id)

        if not is_gb_game:
            self.assertEquals(self.save.trainer_secret_id, self.save2.trainer_secret_id)
        if not is_male_only:
            self.assertEquals(self.save.trainer_gender, self.save2.trainer_gender)
        if not is_rival_name_set:
            self.assertEquals(self.save.rival_name, self.save2.rival_name)

        self.assertEquals(self.save.money, self.save2.money)

        bag1 = self.save.item_bag
        bag2 = self.save2.item_bag
        self.assertEquals(len(bag1), len(bag2))
        self.assertEquals(len(bag1.pocket_names), len(bag2.pocket_names))
        for name in bag1.pocket_names:
            self.__compare_item_lists(bag1[name], bag2[name])

        if generation <= 3:
            self.__compare_item_lists(self.save.item_pc, self.save2.item_pc)

        party1 = self.save.pokemon_party
        party2 = self.save2.pokemon_party
        for party_index in range(6):
            self.compare_pokemon(party1[party_index], party2[party_index])

        pc1 = self.save.pokemon_pc
        pc2 = self.save2.pokemon_pc
        self.assertEquals(len(pc1), len(pc2))
        for pc_index in range(len(pc1)):
            box1 = pc1[pc_index]
            box2 = pc2[pc_index]
            self.assertEquals(len(box1), len(box2))
            for box_index in range(len(box1)):
                self.compare_pokemon(box1[box_index], box2[box_index])

        if game != "Colosseum" and game != "XD":
            pokedex1 = self.save.pokedex
            pokedex2 = self.save2.pokedex
            self.__compare_pokedexes(pokedex1, pokedex2)

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
        self.assertEquals(pkmn.game_save.detect_type(filepath), expected_type)

        self.save = pkmn.game_save(filepath)
        self.assertEquals(self.save.game, expected_game)

        item_list = pkmn.database.lists.get_item_list(expected_game)

        self.__test_common_fields()
        self.__test_attributes()
        # TODO: randomize_items
        self.__randomize_pokemon(item_list)

        temp_save_filepath = os.path.join(PKMN_TMP_DIR, "{0}_{1}.sav".format(expected_game, random.randint(0, 0xFFFF)))
        self.save.save_as(temp_save_filepath)

        self.save2 = pkmn.game_save(temp_save_filepath)
        self.__compare_game_saves()
        self.compare_attributes(self.save, self.save2)

        os.remove(temp_save_filepath)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(game_save_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
