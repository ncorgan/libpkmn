#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from . import pokemon_test_base

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func_pokemon(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[1])

def test_name_func_unown(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

MARKINGS = ["Circle", "Triangle", "Square", "Heart"]
CONTEST_TYPES = ["Cool", "Beauty", "Cute", "Smart", "Tough"]
CONTEST_LEVELS = ["", " Super", " Hyper", " Master"]
RIBBONS = ["Champion", "Winning", "Victory", "Artist",
           "Effort", "Marine", "Land", "Sky",
           "Country", "National", "Earth", "World"]
STATS = ["HP", "Attack", "Defense", "Speed", "Special Attack", "Special Defense"]

class gen3_pokemon_test(pokemon_test_base.pokemon_test_base):

    @parameterized.expand([
        ("Torchic", "Ruby"),
        ("Mudkip", "Sapphire"),
        ("Treecko", "Emerald"),
        ("Charmander", "FireRed"),
        ("Bulbasaur", "LeafGreen"),
        ("Espeon", "Colosseum"),
        ("Umbreon", "XD")
    ], testcase_func_name=test_name_func_pokemon)
    def test_pokemon(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = None

        if game in ["Colosseum", "XD"]:
            test_params = pokemon_test_base.pokemon_test_params(
                              "Great Ball",
                              ["Friend Ball", "Heal Ball"],
                              "Razz Berry",
                              ["Berry", "Mach Bike"],
                              "Distant land",
                              ["Phenac Story", "Orre Colosseum"],
                              ["New Bark Town", "Twinleaf Town"],
                              ["Swallow", "Flamethrower", "Return", "Fire Blast"],
                              ["Roost", "Flame Burst"],
                              ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"],
                              ["Gold", "HeartGold"]
                          )
        else:
            test_params = pokemon_test_base.pokemon_test_params(
                              "Great Ball",
                              ["Friend Ball", "Heal Ball"],
                              "Razz Berry",
                              ["Berry", "Mach Bike"],
                              "Fateful encounter",
                              ["Petalburg Woods", "Viridian Forest"],
                              ["New Bark Town", "Twinleaf Town"],
                              ["Swallow", "Flamethrower", "Return", "Fire Blast"],
                              ["Shadow Sky", "Roost"],
                              ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"],
                              ["Gold", "HeartGold"]
                          )

        self.common_test(pokemon, test_params)

    @parameterized.expand([
        "Ruby", "Sapphire", "Emerald",
        "FireRed", "LeafGreen",
        "Colosseum", "XD"
    ], testcase_func_name=test_name_func_unown)
    def test_unown(self, game):
        unown_entry = pkmn.database.pokemon_entry("Unown", game, "")

        unown = None

        for form in unown_entry.forms:
            unown = pkmn.pokemon("Unown", game, form, 5)
            self.assertEqual(unown.form, form)

            # Make sure personality is properly set.
            form_from_personality = pkmn.calculations.gen3_unown_form(
                                        unown.personality
                                    )
            self.assertEqual(unown.form, form_from_personality)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the form properly changes the IVs.
        for form in unown_entry.forms:
            unown.form = form
            self.assertEqual(unown.form, form)

            # Make sure personality is properly set.
            form_from_personality = pkmn.calculations.gen3_unown_form(
                                        unown.personality
                                    )
            self.assertEqual(unown.form, form_from_personality)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the personality properly sets the form.
        unown.personality = 0x4C07DE71;
        self.assertEqual(unown.form, "B");
