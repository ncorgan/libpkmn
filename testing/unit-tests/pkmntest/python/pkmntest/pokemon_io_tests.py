#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import unittest

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]

class pokemon_io_test(unittest.TestCase):

    def test_3gpkm(self):
        # Test files in repo and compare to known values.
        _3gpkm_dir = os.path.join(LIBPKMN_TEST_FILES, "3gpkm")

        mightyena = pkmn.pokemon(os.path.join(_3gpkm_dir, "MIGHTYENA.3gpkm"))
        self.assertEqual(mightyena.get_species(), "Mightyena")
        self.assertEqual(mightyena.get_game(), "Emerald")
        self.assertEqual(mightyena.get_form(), "Standard")
        self.assertEqual(mightyena.get_nickname(), "MIGHTYENA")
        self.assertFalse(mightyena.is_shiny())
        self.assertEqual(mightyena.get_held_item().get_name(), "Heart Scale")
        self.assertEqual(mightyena.get_trainer_name(), "A")
        self.assertEqual(mightyena.get_trainer_public_id(), 61415)
        self.assertEqual(mightyena.get_trainer_secret_id(), 3417)
        self.assertEqual(mightyena.get_trainer_id(), 223997927)
        self.assertEqual(mightyena.get_trainer_gender(), "Female")
        self.assertEqual(mightyena.get_friendship(), 254)
        self.assertEqual(mightyena.get_ability(), "Intimidate")
        self.assertEqual(mightyena.get_ball(), "Great Ball")
        self.assertEqual(mightyena.get_level_met(), 25)
        self.assertEqual(mightyena.get_location_met(False), "Route 120")
        self.assertEqual(mightyena.get_original_game(), "Emerald")
        self.assertEqual(mightyena.get_personality(), 3557601241)
        self.assertEqual(mightyena.get_experience(), 128734)
        self.assertEqual(mightyena.get_level(), 50)

        mightyena_markings = mightyena.get_markings()
        self.assertEqual(len(mightyena_markings), 4)
        for marking in mightyena_markings:
            self.assertFalse(mightyena_markings[marking])

        mightyena_ribbons = mightyena.get_ribbons()
        self.assertEqual(len(mightyena_ribbons), 32)
        for ribbon in mightyena_ribbons:
            if ribbon == "Champion":
                self.assertTrue(mightyena_ribbons[ribbon])
            else:
                self.assertFalse(mightyena_ribbons[ribbon])

        mightyena_contest_stats = mightyena.get_contest_stats()
        self.assertEqual(len(mightyena_contest_stats), 6)
        for contest_stat in mightyena_contest_stats:
            self.assertEqual(mightyena_contest_stats[contest_stat], 0)

        expected_move_names = ["Crunch", "Strength", "Shadow Ball", "Double-Edge"]
        mightyena_moves = mightyena.get_moves()
        self.assertEqual(len(mightyena_moves), 4)
        for i in range(4):
            self.assertEqual(mightyena_moves[i].move.get_name(), expected_move_names[i])

        mightyena_EVs = mightyena.get_EVs()
        self.assertEqual(len(mightyena_EVs), 6)
        self.assertEqual(mightyena_EVs["HP"], 30)
        self.assertEqual(mightyena_EVs["Attack"], 110)
        self.assertEqual(mightyena_EVs["Defense"], 32)
        self.assertEqual(mightyena_EVs["Speed"], 48)
        self.assertEqual(mightyena_EVs["Special Attack"], 17)
        self.assertEqual(mightyena_EVs["Special Defense"], 83)

        mightyena_IVs = mightyena.get_IVs()
        self.assertEqual(len(mightyena_IVs), 6)
        self.assertEqual(mightyena_IVs["HP"], 26)
        self.assertEqual(mightyena_IVs["Attack"], 28)
        self.assertEqual(mightyena_IVs["Defense"], 4)
        self.assertEqual(mightyena_IVs["Speed"], 13)
        self.assertEqual(mightyena_IVs["Special Attack"], 25)
        self.assertEqual(mightyena_IVs["Special Defense"], 26)

        mightyena_stats = mightyena.get_stats()
        self.assertEqual(len(mightyena_stats), 6)
        self.assertEqual(mightyena_stats["HP"], 146)
        self.assertEqual(mightyena_stats["Attack"], 122)
        self.assertEqual(mightyena_stats["Defense"], 81)
        self.assertEqual(mightyena_stats["Speed"], 87)
        self.assertEqual(mightyena_stats["Special Attack"], 79)
        self.assertEqual(mightyena_stats["Special Defense"], 88)
