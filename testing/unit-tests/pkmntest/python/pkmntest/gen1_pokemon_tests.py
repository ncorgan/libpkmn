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

class gen1_pokemon_test(pokemon_tests.pokemon_tests):

    def gen1_pokemon_test(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = pokemon_tests.pokemon_test_params(
                          "Great Ball",
                          ["Great Ball"],
                          "Potion",
                          ["Potion"],
                          "Special",
                          ["Route 1"],
                          ["Route 1"],
                          ["Slash", "Flamethrower", "Tail Whip", "Fire Blast"],
                          ["Return"],
                          ["Red"],
                          ["Red"]
                      )
        self.common_test(pokemon, test_params)

        # Test attributes.
        self.assertEqual(pokemon.numeric_attributes["Catch rate"], 45)

    #
    # Red
    #

    def test_red_forms(self):
        self.forms_test("Red")

    def test_red_pokemon(self):
        self.gen1_pokemon_test("Charmander", "Red")

    #
    # Blue
    #

    def test_blue_forms(self):
        self.forms_test("Blue")

    def test_blue_pokemon(self):
        self.gen1_pokemon_test("Squirtle", "Blue")

    #
    # Yellow
    #

    def test_yellow_forms(self):
        self.forms_test("Yellow")

    def test_yellow_pokemon(self):
        self.gen1_pokemon_test("Bulbasaur", "Yellow")
