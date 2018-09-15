#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
TMP_DIR = pkmn.paths.get_tmp_dir()

class pokemon_subclass(pkmn.pokemon):
    def __init__(self, species, game):
        pkmn.pokemon.__init__(self)

        self.__species = species
        self.__game = game
        self.__form = ""

    def clone(self):
        return pokemon_subclass(self.species, this.game)

    def to_game(self, game):
        return pokemon_subclass(self.species, game)

    def export_to_file(self):
        return

    def _get_species_internal(self):
        return self.__species

    def _get_game_internal(self):
        return self.__game

    def _get_form_internal(self):
        return self.__form

    def _set_form_internal(self, form):
        self.__form = form

    def _get_level_internal(self):
        return 5

    def _set_level_internal(self, level):
        return
