#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pokemon_test_base

import os
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[1])

class gen1_pokemon_test(pokemon_test_base.pokemon_test_base):

    @parameterized.expand([
        ("Charmander", "Red"),
        ("Squirtle", "Blue"),
        ("Bulbasaur", "Yellow")
    ],
    testcase_func_name=test_name_func)
    def test_pokemon(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = pokemon_test_base.pokemon_test_params(
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
