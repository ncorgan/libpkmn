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

class gba_pokemon_test(pokemon_tests):
    #
    # Ruby
    #

    def test_ruby_invalid_pokemon(self):
        self.invalid_pokemon_test("Ruby")

    #
    # Sapphire
    #

    def test_sapphire_invalid_pokemon(self):
        self.invalid_pokemon_test("Sapphire")

    #
    # Emerald
    #

    def test_emerald_invalid_pokemon(self):
        self.invalid_pokemon_test("Emerald")

    #
    # FireRed
    #

    def test_firered_invalid_pokemon(self):
        self.invalid_pokemon_test("FireRed")

    #
    # LeafGreen
    #

    def test_leafgreen_invalid_pokemon(self):
        self.invalid_pokemon_test("LeafGreen")
