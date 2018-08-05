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
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, pkmn.game_to_string(param.args[1]))

class gen1_pokemon_test(pokemon_test_base.pokemon_test_base):

    @parameterized.expand([
        (pkmn.species.CHARMANDER, pkmn.game.RED),
        (pkmn.species.SQUIRTLE,   pkmn.game.BLUE),
        (pkmn.species.BULBASAUR,  pkmn.game.YELLOW)
    ],
    testcase_func_name=test_name_func)
    def test_pokemon(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = pokemon_test_base.pokemon_test_params(
                          pkmn.ball.GREAT_BALL,
                          [pkmn.ball.GREAT_BALL],
                          pkmn.item.POTION,
                          [pkmn.item.POTION],
                          "Special",
                          ["Route 1"],
                          ["Route 1"],
                          [
                              pkmn.move.SLASH,
                              pkmn.move.FLAMETHROWER,
                              pkmn.move.TAIL_WHIP,
                              pkmn.move.FIRE_BLAST
                          ],
                          [pkmn.move.RETURN],
                          [pkmn.game.RED],
                          [pkmn.game.RED]
                      )
        self.common_test(pokemon, test_params)

        # Test attributes.
        self.assertEqual(pokemon.numeric_attributes["Catch rate"], 45)
