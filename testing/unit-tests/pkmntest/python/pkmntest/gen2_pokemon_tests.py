#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .pokemon_tests import pokemon_tests

import unittest

class gen2_pokemon_test(pokemon_tests):
    #
    # Gold
    #

    def test_gold_invalid_pokemon(self):
        self.invalid_pokemon_test("Gold")

    #
    # Silver
    #

    def test_silver_invalid_pokemon(self):
        self.invalid_pokemon_test("Silver")

    #
    # Crystal
    #

    def test_crystal_invalid_pokemon(self):
        self.invalid_pokemon_test("Crystal")
