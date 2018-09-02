#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from . import *

import pkmn

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
TMP_DIR = pkmn.paths.get_tmp_dir()

class pokemon_io_test(base_test):

    @parameterized.expand([
        "Red","Blue","Yellow"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_pk1(self, game_name):
        game = pkmn.string_to_game(game_name)

        pk1_path = os.path.join(TMP_DIR, "{0}_{1}.pk1".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.lists.get_item_list(game)
        move_list = pkmn.database.lists.get_move_list(game)
        pokemon_list = pkmn.database.lists.get_pokemon_list(1, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk1_path)

        imported_pokemon = pkmn.pokemon(pk1_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(pk1_path)

    @parameterized.expand([
        "Gold", "Silver", "Crystal"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_pk2(self, game_name):
        game = pkmn.string_to_game(game_name)

        pk2_path = os.path.join(TMP_DIR, "{0}_{1}.pk2".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.lists.get_item_list(game)
        move_list = pkmn.database.lists.get_move_list(game)
        pokemon_list = pkmn.database.lists.get_pokemon_list(2, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(pk2_path)

        imported_pokemon = pkmn.pokemon(pk2_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(pk2_path)

    @parameterized.expand([
        "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"
    ], testcase_func_name=test_name_func)
    def test_saving_and_loading_3gpkm(self, game_name):
        game = pkmn.string_to_game(game_name)

        _3gpkm_path = os.path.join(TMP_DIR, "{0}_{1}.3gpkm".format(game, random.randint(0, 1000)))

        item_list = pkmn.database.lists.get_item_list(game)
        move_list = pkmn.database.lists.get_move_list(game)
        pokemon_list = pkmn.database.lists.get_pokemon_list(3, True)

        random_pokemon = self.get_random_pokemon(game, pokemon_list, move_list, item_list)
        random_pokemon.export_to_file(_3gpkm_path)

        imported_pokemon = pkmn.pokemon(_3gpkm_path)
        self.compare_pokemon(random_pokemon, imported_pokemon)

        os.remove(_3gpkm_path)

    def test_outside_3gpkm(self):
        # Test files in repo and compare to known values.
        _3gpkm_dir = os.path.join(LIBPKMN_TEST_FILES, "3gpkm")

        mightyena = pkmn.pokemon(os.path.join(_3gpkm_dir, "MIGHTYENA.3gpkm"))
        self.assertEqual(mightyena.species, pkmn.species.MIGHTYENA)
        self.assertEqual(mightyena.game, pkmn.game.EMERALD)
        self.assertEqual(mightyena.form, "Standard")
        self.assertEqual(mightyena.nickname, "MIGHTYENA")
        self.assertFalse(mightyena.is_shiny)
        self.assertEqual(mightyena.condition, pkmn.condition.NONE)
        self.assertEqual(mightyena.held_item, pkmn.item.HEART_SCALE)
        self.assertEqual(mightyena.original_trainer_name, "A")
        self.assertEqual(mightyena.original_trainer_public_id, 61415)
        self.assertEqual(mightyena.original_trainer_secret_id, 3417)
        self.assertEqual(mightyena.original_trainer_id, 223997927)
        self.assertEqual(mightyena.original_trainer_gender, pkmn.gender.FEMALE)
        self.assertEqual(mightyena.current_trainer_friendship, 254)
        self.assertEqual(mightyena.ability, pkmn.ability.INTIMIDATE)
        self.assertEqual(mightyena.ball, pkmn.ball.GREAT_BALL)
        self.assertEqual(mightyena.level_met, 25)
        self.assertEqual(mightyena.location_met, "Route 120")
        self.assertEqual(mightyena.original_game, pkmn.game.EMERALD)
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

        expected_moves = [
            pkmn.move.CRUNCH,
            pkmn.move.STRENGTH,
            pkmn.move.SHADOW_BALL,
            pkmn.move.DOUBLE_EDGE
        ]
        self.assertEqual(len(mightyena.moves), 4)
        for move_index in range(4):
            self.assertEqual(
                mightyena.moves[move_index].move,
                expected_moves[move_index]
            )

        self.assertEqual(len(mightyena.EVs), 6)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.HP], 30)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.ATTACK], 110)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.DEFENSE], 32)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.SPEED], 48)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.SPECIAL_ATTACK], 17)
        self.assertEqual(mightyena.EVs[pkmn.pokemon_stat.SPECIAL_DEFENSE], 83)

        self.assertEqual(len(mightyena.IVs), 6)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.HP], 26)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.ATTACK], 28)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.DEFENSE], 4)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.SPEED], 13)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.SPECIAL_ATTACK], 25)
        self.assertEqual(mightyena.IVs[pkmn.pokemon_stat.SPECIAL_DEFENSE], 26)

        self.assertEqual(len(mightyena.stats), 6)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.HP], 146)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.ATTACK], 122)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.DEFENSE], 81)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.SPEED], 87)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.SPECIAL_ATTACK], 79)
        self.assertEqual(mightyena.stats[pkmn.pokemon_stat.SPECIAL_DEFENSE], 88)
