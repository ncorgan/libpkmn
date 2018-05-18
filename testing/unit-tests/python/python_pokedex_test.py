#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import pkmntest.base_test

from nose_parameterized import parameterized
import unittest

import random
import sys

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class pokedex_test(pkmntest.base_test):

    @parameterized.expand(
        pkmntest.ALL_MAIN_SERIES_GAMES,
        testcase_func_name=test_name_func
    )
    def test_pokedex(self, game):
        generation = pkmntest.GAME_TO_GENERATION[game]

        pokedex = pkmn.pokedex(game)

        # Test initial values.
        self.assertEqual(pokedex.num_seen, 0)
        self.assertEqual(len(pokedex.all_seen), 0)
        self.assertEqual(pokedex.num_caught, 0)
        self.assertEqual(len(pokedex.all_caught), 0)

        # Set random Pokemon to be seen and caught.
        all_pokemon = pkmn.database.lists.get_pokemon_list(generation, True)
        num_seen = random.randint(0, len(all_pokemon))
        num_caught = random.randint(0, len(all_pokemon))

        all_seen_nums = []
        while len(all_seen_nums) < num_seen:
            index = random.randint(0, len(all_pokemon)-1)
            if index not in all_seen_nums:
                all_seen_nums += [index]

                species = all_pokemon[index]
                pokedex.seen_pokemon_map[species] = True
                self.assertTrue(pokedex.seen_pokemon_map[species])
                self.assertTrue(species in pokedex.all_seen)
        self.assertEqual(pokedex.num_seen, len(all_seen_nums))
        self.assertEqual(len(pokedex.all_seen), len(all_seen_nums))

        all_caught_nums = []
        while len(all_caught_nums) < num_caught:
            index = random.randint(0, len(all_pokemon)-1)
            if index not in all_caught_nums:
                all_caught_nums += [index]

                species = all_pokemon[index]
                pokedex.caught_pokemon_map[species] = True
                self.assertTrue(pokedex.caught_pokemon_map[species])
                self.assertTrue(species in pokedex.all_caught)
        self.assertEqual(pokedex.num_caught, len(all_caught_nums))
        self.assertEqual(len(pokedex.all_caught), len(all_caught_nums))

        # Remove all entries.

        for species in pokedex.all_seen:
            pokedex.seen_pokemon_map[species] = False
            self.assertFalse(pokedex.seen_pokemon_map[species])
        self.assertEqual(pokedex.num_seen, 0)
        self.assertEqual(len(pokedex.all_seen), 0)

        for species in pokedex.all_caught:
            pokedex.caught_pokemon_map[species] = False
            self.assertFalse(pokedex.caught_pokemon_map[species])
        self.assertEqual(pokedex.num_caught, 0)
        self.assertEqual(len(pokedex.all_caught), 0)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(pokedex_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
