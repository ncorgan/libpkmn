#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import base_test

import pkmn

import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class pokemon_party_tests(base_test.base_test):

    def __test_empty_pokemon_party(self, party):
        self.assertEqual(len(party), 6)

        # Make sure trying to get a Pokemon at an invalid index fails.
        with self.assertRaises(IndexError):
            party[-1]
        with self.assertRaises(IndexError):
            party[len(party)]

        for pokemon in party:
            self.assertEqual(pokemon.species, "None")
            self.assertEqual(pokemon.game, party.game)

            for slot in pokemon.moves:
                self.assertEqual(slot.move, "None")
                self.assertEqual(slot.pp, 0)

    def __test_setting_pokemon(self, party):
        game = party.game

        original_first = party[0]
        original_second = party[1]

        # Make sure we can't set Pokemon at invalid indices.
        with self.assertRaises(IndexError):
            party[-1] = original_first
        with self.assertRaises(IndexError):
            party[len(party)] = original_second

        # Create Pokemon and place in party. The original variables should
        # have the same underlying Pokemon.
        bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
        charmander = pkmn.pokemon("Charmander", game, "", 5)
        squirtle = pkmn.pokemon("Squirtle", game, "", 5)

        party[0] = bulbasaur
        self.assertEqual(party.num_pokemon, 1)
        self.assertEqual(party[0].species, "Bulbasaur")
        party[1] = charmander
        self.assertEqual(party.num_pokemon, 2)
        self.assertEqual(party[1].species, "Charmander")

        # Replace one of the new ones.
        party[0] = squirtle
        self.assertEqual(party.num_pokemon, 2)
        self.assertEqual(party[0].species, "Squirtle")

        # Make sure we can't copy a Pokemon to itself.
        with self.assertRaises(ValueError):
            party[1] = party[1]

        # Copy a Pokemon whose memory is already part of the party.
        party[2] = party[1]
        self.assertEqual(party.num_pokemon, 3)
        self.assertEqual(party[2].species, "Charmander")

        # We should be able to clear the last contiguous Pokemon.
        party[2] = original_first
        self.assertEqual(party.num_pokemon, 2)
        self.assertEqual(party[2].species, "None")

        # Put it back.
        party[2] = party[1]
        self.assertEqual(party.num_pokemon, 3)

        # Check that Pokemon cannot be placed non-contiguously.
        with self.assertRaises(ValueError):
            party[1] = original_first
        self.assertEqual(party.num_pokemon, 3)
        self.assertEqual(party[1].species, "Charmander")

        with self.assertRaises(IndexError):
            party[4] = bulbasaur
        self.assertEqual(party.num_pokemon, 3)
        self.assertEqual(party[4].species, "None")

        # Now check everything we've created. Each variable should have
        # the same underlying Pokemon.
        self.assertEqual(party[0].species, "Squirtle")
        self.assertEqual(party[1].species, "Charmander")
        self.assertEqual(party[2].species, "Charmander")
        self.assertEqual(original_first.species, "None")
        self.assertEqual(original_second.species, "None")
        self.assertEqual(bulbasaur.species, "Bulbasaur")
        self.assertEqual(charmander.species, "Charmander")
        self.assertEqual(squirtle.species, "Squirtle")

    @parameterized.expand(base_test.ALL_GAMES, testcase_func_name=test_name_func)
    def test_pokemon_party(self, game):
        party = pkmn.pokemon_party(game)

        self.__test_empty_pokemon_party(party)
        self.__test_setting_pokemon(party)
