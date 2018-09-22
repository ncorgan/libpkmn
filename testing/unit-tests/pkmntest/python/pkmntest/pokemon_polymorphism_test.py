#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from . import *

from .pokemon_subclass import pokemon_subclass

import pkmn

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class pokemon_polymorphism_test(base_test):

    SPECIES = pkmn.species.BULBASAUR

    def _test_setting_pokemon_in_container(self, container):
        container[0] = pokemon_subclass(self.SPECIES, container.game)

    @parameterized.expand(
        ALL_MAIN_SERIES_GAME_NAMES,
        testcase_func_name=test_name_func
    )
    def test_setting_pokemon_in_pokemon_box(self, game_name):
        box = pkmn.pokemon_box(pkmn.string_to_game(game_name))
        self._test_setting_pokemon_in_container(box)

    @parameterized.expand(
        ALL_MAIN_SERIES_GAME_NAMES,
        testcase_func_name=test_name_func
    )
    def test_setting_pokemon_in_pokemon_party(self, game_name):
        party = pkmn.pokemon_party(pkmn.string_to_game(game_name))
        self._test_setting_pokemon_in_container(party)

    @parameterized.expand(
        DAYCARE_SUPPORTED_GAME_NAMES,
        testcase_func_name=test_name_func
    )
    def test_setting_pokemon_in_daycare(self, game_name):
        game = pkmn.string_to_game(game_name)
        daycare = pkmn.daycare(game)

        daycare.levelup_pokemon[0] = pokemon_subclass(self.SPECIES, game)
        if daycare.can_breed_pokemon:
            daycare.breeding_pokemon[0] = pokemon_subclass(self.SPECIES, game)
