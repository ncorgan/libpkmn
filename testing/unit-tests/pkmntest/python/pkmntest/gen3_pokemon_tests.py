#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pokemon_tests

import os
import random
import unittest

MARKINGS = ["Circle", "Triangle", "Square", "Heart"]
CONTEST_TYPES = ["Cool", "Beauty", "Cute", "Smart", "Tough"]
CONTEST_LEVELS = ["", " Super", " Hyper", " Master"]
RIBBONS = ["Champion", "Winning", "Victory", "Artist",
           "Effort", "Marine", "Land", "Sky",
           "Country", "National", "Earth", "World"]
STATS = ["HP", "Attack", "Defense", "Speed", "Special Attack", "Special Defense"]

class gen3_pokemon_test(pokemon_tests.pokemon_tests):

    def gen3_pokemon_test(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = None

        if game in ["Colosseum", "XD"]:
            test_params = pokemon_tests.pokemon_test_params(
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
            test_params = pokemon_tests.pokemon_test_params(
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

    def unown_test(self, game):
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

    #
    # Ruby
    #

    def test_ruby_forms(self):
        self.forms_test("Ruby")

    def test_ruby_gender(self):
        self.gender_test("Ruby")

    def test_ruby_unown(self):
        self.unown_test("Ruby")

    def test_ruby_pokemon(self):
        self.gen3_pokemon_test("Torchic", "Ruby")

    #
    # Sapphire
    #

    def test_sapphire_forms(self):
        self.forms_test("Sapphire")

    def test_sapphire_gender(self):
        self.gender_test("Sapphire")

    def test_sapphire_unown(self):
        self.unown_test("Sapphire")

    def test_sapphire_pokemon(self):
        self.gen3_pokemon_test("Mudkip", "Sapphire")

    #
    # Emerald
    #

    def test_emerald_forms(self):
        self.forms_test("Emerald")

    def test_emerald_gender(self):
        self.gender_test("Emerald")

    def test_emerald_unown(self):
        self.unown_test("Emerald")

    def test_emerald_pokemon(self):
        self.gen3_pokemon_test("Treecko", "Emerald")

    #
    # FireRed
    #

    def test_firered_forms(self):
        self.forms_test("FireRed")

    def test_firered_gender(self):
        self.gender_test("FireRed")

    def test_firered_unown(self):
        self.unown_test("FireRed")

    def test_firered_pokemon(self):
        self.gen3_pokemon_test("Charmander", "FireRed")

    #
    # LeafGreen
    #

    def test_leafgreen_forms(self):
        self.forms_test("LeafGreen")

    def test_leafgreen_gender(self):
        self.gender_test("LeafGreen")

    def test_leafgreen_unown(self):
        self.unown_test("LeafGreen")

    def test_leafgreen_pokemon(self):
        self.gen3_pokemon_test("Bulbasaur", "LeafGreen")

    #
    # Colosseum
    #

    def test_colosseum_forms(self):
        self.forms_test("Colosseum")

    def test_colosseum_gender(self):
        self.gender_test("Colosseum")

    def test_colosseum_unown(self):
        self.unown_test("Colosseum")

    def test_colosseum_pokemon(self):
        self.gen3_pokemon_test("Espeon", "Colosseum")

    #
    # XD
    #

    def test_xd_forms(self):
        self.forms_test("XD")

    def test_xd_gender(self):
        self.gender_test("XD")

    def test_xd_unown(self):
        self.unown_test("XD")

    def test_xd_pokemon(self):
        self.gen3_pokemon_test("Umbreon", "XD")
