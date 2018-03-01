#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pokemon_tests

import os
import unittest

class gen2_pokemon_test(pokemon_tests.pokemon_tests):

    def gen2_pokemon_test(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = pokemon_tests.pokemon_test_params(
                          "Great Ball",
                          ["Great Ball"],
                          "Berry",
                          ["Razz Berry", "Bicycle"],
                          "Special",
                          ["Sprout Tower", "Tohjo Falls"],
                          ["Littleroot Town", "Petalburg Woods"],
                          ["Slash", "Flamethrower", "Return", "Fire Blast"],
                          ["Frenzy Plant", "Roost"],
                          ["Gold"],
                          ["Gold"]
                      )
        self.common_test(pokemon, test_params)

        # Gender is tied to IVs, so make sure the abstraction reflects that.

        pokemon.gender = "Male"
        self.assertEqual(pokemon.IVs["Attack"], 15)
        pokemon.gender = "Female"
        self.assertLess(pokemon.IVs["Attack"], 15)

        pokemon.IVs["Attack"] = 0
        self.assertEqual(pokemon.gender, "Female")
        pokemon.IVs["Attack"] = 15
        self.assertEqual(pokemon.gender, "Male")

        # Shininess is tied to IVs, so make sure the abstraction reflects that.

        pokemon.is_shiny = False
        self.assertEqual(pokemon.IVs["Attack"], 13)

        pokemon.is_shiny = True
        self.assertEqual(pokemon.IVs["Attack"], 15)
        self.assertEqual(pokemon.IVs["Defense"], 10)
        self.assertEqual(pokemon.IVs["Speed"], 10)
        self.assertEqual(pokemon.IVs["Special"], 10)

    def unown_test(self, game):
        unown_entry = pkmn.database.pokemon_entry("Unown", game, "")

        unown = None

        for form in unown_entry.forms:
            unown = pkmn.pokemon("Unown", game, form, 5)
            self.assertEqual(unown.form, form)

            # Make sure IVs are properly set.
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                unown.IVs["Attack"],
                                unown.IVs["Defense"],
                                unown.IVs["Speed"],
                                unown.IVs["Special"]
                            )
            self.assertEqual(unown.form, form_from_IVs)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the form properly changes the IVs.
        for form in unown_entry.forms:
            unown.form = form
            self.assertEqual(unown.form, form)

            # Make sure IVs are properly set.
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                unown.IVs["Attack"],
                                unown.IVs["Defense"],
                                unown.IVs["Speed"],
                                unown.IVs["Special"]
                            )
            self.assertEqual(unown.form, form_from_IVs)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting IVs properly changes the form.
        unown.IVs["Attack"] = 10;
        unown.IVs["Defense"] = 9;
        unown.IVs["Speed"] = 1;
        unown.IVs["Special"] = 14;
        self.assertEquals(unown.form, "G");

    #
    # Gold
    #

    def test_gold_forms(self):
        self.forms_test("Gold")

    def test_gold_gender(self):
        self.gender_test("Gold")

    def test_gold_unown(self):
        self.unown_test("Gold")

    def test_gold_pokemon(self):
        self.gen2_pokemon_test("Cyndaquil", "Gold")

    #
    # Silver
    #

    def test_silver_forms(self):
        self.forms_test("Silver")

    def test_silver_gender(self):
        self.gender_test("Silver")

    def test_silver_unown(self):
        self.unown_test("Silver")

    def test_silver_pokemon(self):
        self.gen2_pokemon_test("Totodile", "Silver")

    #
    # Crystal
    #

    def test_crystal_forms(self):
        self.forms_test("Crystal")

    def test_crystal_gender(self):
        self.gender_test("Crystal")

    def test_crystal_unown(self):
        self.unown_test("Crystal")

    def test_crystal_pokemon(self):
        self.gen2_pokemon_test("Chikorita", "Crystal")
