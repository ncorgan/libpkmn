#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import base_test

import pkmn

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
TMP_DIR = pkmn.get_tmp_dir()

class pokemon_io_test(base_test.base_test):

    @parameterized.expand([
        "Red","Blue","Yellow"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_pk1(self, game):
        pk1_path = os.path.join(TMP_DIR, "{0}_{1}.pk1".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.get_item_list(game)
        move_list = pkmn.database.get_move_list(game)
        pokemon_list = pkmn.database.get_pokemon_list(1, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk1_path)

        imported_pokemon = pkmn.pokemon(pk1_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(pk1_path)

    @parameterized.expand([
        "Gold", "Silver", "Crystal"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_pk2(self, game):
        pk2_path = os.path.join(TMP_DIR, "{0}_{1}.pk2".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.get_item_list(game)
        move_list = pkmn.database.get_move_list(game)
        pokemon_list = pkmn.database.get_pokemon_list(2, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk2_path)

        imported_pokemon = pkmn.pokemon(pk2_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(pk2_path)

    @parameterized.expand([
        "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_3gpkm(self, game):
        _3gpkm_path = os.path.join(TMP_DIR, "{0}_{1}.3gpkm".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.get_item_list(game)
        move_list = pkmn.database.get_move_list(game)
        pokemon_list = pkmn.database.get_pokemon_list(3, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(_3gpkm_path)

        imported_pokemon = pkmn.pokemon(_3gpkm_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(_3gpkm_path)

    def test_outside_3gpkm(self):
        # Test files in repo and compare to known values.
        _3gpkm_dir = os.path.join(LIBPKMN_TEST_FILES, "3gpkm")

        mightyena = pkmn.pokemon(os.path.join(_3gpkm_dir, "MIGHTYENA.3gpkm"))
        self.assertEqual(mightyena.species, "Mightyena")
        self.assertEqual(mightyena.game, "Emerald")
        self.assertEqual(mightyena.form, "Standard")
        self.assertEqual(mightyena.nickname, "MIGHTYENA")
        self.assertFalse(mightyena.is_shiny)
        self.assertEqual(mightyena.held_item, "Heart Scale")
        self.assertEqual(mightyena.original_trainer_name, "A")
        self.assertEqual(mightyena.original_trainer_public_id, 61415)
        self.assertEqual(mightyena.original_trainer_secret_id, 3417)
        self.assertEqual(mightyena.original_trainer_id, 223997927)
        self.assertEqual(mightyena.original_trainer_gender, "Female")
        self.assertEqual(mightyena.current_trainer_friendship, 254)
        self.assertEqual(mightyena.ability, "Intimidate")
        self.assertEqual(mightyena.ball, "Great Ball")
        self.assertEqual(mightyena.level_met, 25)
        self.assertEqual(mightyena.location_met, "Route 120")
        self.assertEqual(mightyena.original_game, "Emerald")
        self.assertEqual(mightyena.personality, 3557601241)
        self.assertEqual(mightyena.experience, 128734)
        self.assertEqual(mightyena.level, 50)

        self.assertEqual(len(mightyena.markings), 4)
        for marking in mightyena.markings.keys:
            self.assertFalse(mightyena.markings[marking])

        self.assertEqual(len(mightyena.ribbons), 32)
        for ribbon in mightyena.ribbons.keys:
            if ribbon == "Champion":
                self.assertTrue(mightyena.ribbons[ribbon])
            else:
                self.assertFalse(mightyena.ribbons[ribbon])

        self.assertEqual(len(mightyena.contest_stats), 6)
        for contest_stat in mightyena.contest_stats.keys:
            self.assertEqual(mightyena.contest_stats[contest_stat], 0)

        expected_move_names = ["Crunch", "Strength", "Shadow Ball", "Double-Edge"]
        self.assertEqual(len(mightyena.moves), 4)
        for move_index in range(4):
            self.assertEqual(
                mightyena.moves[move_index].move,
                expected_move_names[move_index]
            )

        self.assertEqual(len(mightyena.EVs), 6)
        self.assertEqual(mightyena.EVs["HP"], 30)
        self.assertEqual(mightyena.EVs["Attack"], 110)
        self.assertEqual(mightyena.EVs["Defense"], 32)
        self.assertEqual(mightyena.EVs["Speed"], 48)
        self.assertEqual(mightyena.EVs["Special Attack"], 17)
        self.assertEqual(mightyena.EVs["Special Defense"], 83)

        self.assertEqual(len(mightyena.IVs), 6)
        self.assertEqual(mightyena.IVs["HP"], 26)
        self.assertEqual(mightyena.IVs["Attack"], 28)
        self.assertEqual(mightyena.IVs["Defense"], 4)
        self.assertEqual(mightyena.IVs["Speed"], 13)
        self.assertEqual(mightyena.IVs["Special Attack"], 25)
        self.assertEqual(mightyena.IVs["Special Defense"], 26)

        self.assertEqual(len(mightyena.stats), 6)
        self.assertEqual(mightyena.stats["HP"], 146)
        self.assertEqual(mightyena.stats["Attack"], 122)
        self.assertEqual(mightyena.stats["Defense"], 81)
        self.assertEqual(mightyena.stats["Speed"], 87)
        self.assertEqual(mightyena.stats["Special Attack"], 79)
        self.assertEqual(mightyena.stats["Special Defense"], 88)
