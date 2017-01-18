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

class gen1_pokemon_test(pokemon_tests):
    #
    # Red
    #

    def test_red_invalid_pokemon(self):
        self.invalid_pokemon_test("Red")
        
    #
    # Blue
    #

    def test_blue_invalid_pokemon(self):
        self.invalid_pokemon_test("Blue")

    #
    # Yellow
    #

    def test_yellow_invalid_pokemon(self):
        self.invalid_pokemon_test("Yellow")
