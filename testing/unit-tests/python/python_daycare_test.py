#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn
import pkmntest

import sys
import unittest

from nose_parameterized import parameterized

DAYCARE_PARAMS = [
    (pkmn.game.RED, False),
    (pkmn.game.BLUE, False),
    (pkmn.game.YELLOW, False),

    (pkmn.game.GOLD, True),
    (pkmn.game.SILVER, True),
    (pkmn.game.CRYSTAL, True),

    (pkmn.game.COLOSSEUM, False),
    (pkmn.game.XD, False)
]

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class daycare_test(pkmntest.base_test):

    def __test_empty_daycare(self, daycare):
        for pokemon in daycare.levelup_pokemon:
            self.assertEquals(pkmn.species.NONE, pokemon.species)

        # Test invalid indices.
        # What type of error is returned for the -1 case depends on SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pokemon = daycare.levelup_pokemon[-1]
        except:
            with self.assertRaises(TypeError):
                pokemon = daycare.levelup_pokemon[-1]

        with self.assertRaises(IndexError):
            pokemon = daycare.levelup_pokemon[len(daycare.levelup_pokemon)]

        if daycare.can_breed_pokemon:
            for pokemon in daycare.breeding_pokemon:
                self.assertEquals(pkmn.species.NONE, pokemon.species)

            # Test invalid indices.
            # What type of error is returned for the -1 case depends on SWIG version.
            try:
                with self.assertRaises(OverflowError):
                    pokemon = daycare.breeding_pokemon[-1]
            except:
                with self.assertRaises(TypeError):
                    pokemon = daycare.breeding_pokemon[-1]

            with self.assertRaises(IndexError):
                pokemon = daycare.breeding_pokemon[len(daycare.breeding_pokemon)]

    def __test_setting_pokemon(self, daycare):
        daycare.levelup_pokemon[0] = pkmn.pokemon(
                                         pkmn.species.VENUSAUR,
                                         daycare.game,
                                         "",
                                         50
                                     )
        self.assertEquals(
            pkmn.species.VENUSAUR,
            daycare.levelup_pokemon[0].species
        )

        if len(daycare.levelup_pokemon) == 2:
            daycare.levelup_pokemon[1] = pkmn.pokemon(
                                             pkmn.species.CHARIZARD,
                                             daycare.game,
                                             "",
                                             50
                                         )
            self.assertEquals(
                pkmn.species.CHARIZARD,
                daycare.levelup_pokemon[1].species
            )

        # TODO: validate genders
        if daycare.can_breed_pokemon:
            daycare.levelup_pokemon[0] = pkmn.pokemon(
                                             pkmn.species.BLASTOISE,
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.levelup_pokemon[0].gender = pkmn.gender.FEMALE

            self.assertEquals(
                pkmn.species.BLASTOISE,
                daycare.levelup_pokemon[0].species
            )
            self.assertEquals(
                pkmn.gender.FEMALE,
                daycare.levelup_pokemon[0].gender
            )

            daycare.levelup_pokemon[1] = pkmn.pokemon(
                                             pkmn.species.MAROWAK,
                                             daycare.game,
                                             "",
                                             50
                                         )
            daycare.levelup_pokemon[1].gender = pkmn.gender.MALE

            self.assertEquals(
                pkmn.species.MAROWAK,
                daycare.levelup_pokemon[1].species
            )
            self.assertEquals(
                pkmn.gender.MALE,
                daycare.levelup_pokemon[1].gender
            )

    @parameterized.expand(DAYCARE_PARAMS, testcase_func_name=test_name_func)
    def test_daycare(self, game, expected_can_breed):
        daycare = pkmn.daycare(game)
        self.assertEquals(game, daycare.game)
        self.assertEquals(expected_can_breed, daycare.can_breed_pokemon)

        self.__test_empty_daycare(daycare)
        self.__test_setting_pokemon(daycare)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(daycare_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
