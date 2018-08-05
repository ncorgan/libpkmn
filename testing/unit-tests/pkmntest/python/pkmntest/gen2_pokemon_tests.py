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

def test_name_func_pokemon(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, pkmn.game_to_string(param.args[1]))

# Why does only this one need the string and not an enum passed through game_to_string?
def test_name_func_unown(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class gen2_pokemon_test(pokemon_test_base.pokemon_test_base):

    @parameterized.expand([
        (pkmn.species.CYNDAQUIL, pkmn.game.GOLD),
        (pkmn.species.TOTODILE, pkmn.game.SILVER),
        (pkmn.species.CHIKORITA, pkmn.game.CRYSTAL)
    ], testcase_func_name=test_name_func_pokemon)
    def test_pokemon(self, species, game):
        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = pokemon_test_base.pokemon_test_params(
                          pkmn.ball.GREAT_BALL,
                          [pkmn.ball.GREAT_BALL],
                          pkmn.item.BERRY,
                          [pkmn.item.RAZZ_BERRY, pkmn.item.BICYCLE],
                          pkmn.pokemon_stat.SPECIAL,
                          ["Sprout Tower", "Tohjo Falls"],
                          ["Littleroot Town", "Petalburg Woods"],
                          [
                              pkmn.move.SLASH,
                              pkmn.move.FLAMETHROWER,
                              pkmn.move.RETURN,
                              pkmn.move.FIRE_BLAST
                          ],
                          [
                              pkmn.move.FRENZY_PLANT,
                              pkmn.move.ROOST
                          ],
                          [pkmn.game.GOLD],
                          [pkmn.game.GOLD]
                      )
        self.common_test(pokemon, test_params)

        # Gender is tied to IVs, so make sure the abstraction reflects that.

        pokemon.gender = pkmn.gender.MALE
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.ATTACK], 15)
        pokemon.gender = pkmn.gender.FEMALE
        self.assertLess(pokemon.IVs[pkmn.pokemon_stat.ATTACK], 15)

        pokemon.IVs[pkmn.pokemon_stat.ATTACK] = 0
        self.assertEqual(pokemon.gender, pkmn.gender.FEMALE)
        pokemon.IVs[pkmn.pokemon_stat.ATTACK] = 15
        self.assertEqual(pokemon.gender, pkmn.gender.MALE)

        # Shininess is tied to IVs, so make sure the abstraction reflects that.

        pokemon.is_shiny = False
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.ATTACK], 13)

        pokemon.is_shiny = True
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.ATTACK], 15)
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.DEFENSE], 10)
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.SPEED], 10)
        self.assertEqual(pokemon.IVs[pkmn.pokemon_stat.SPECIAL], 10)

    @parameterized.expand([
        "Gold", "Silver", "Crystal"
    ], testcase_func_name=test_name_func_unown)
    def test_unown(self, game_name):
        game = pkmn.string_to_game(game_name)

        unown_entry = pkmn.database.pokemon_entry(pkmn.species.UNOWN, game, "")

        unown = None

        for form in unown_entry.forms:
            unown = pkmn.pokemon(pkmn.species.UNOWN, game, form, 5)
            self.assertEqual(unown.form, form)

            # Make sure IVs are properly set.
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                unown.IVs[pkmn.pokemon_stat.ATTACK],
                                unown.IVs[pkmn.pokemon_stat.DEFENSE],
                                unown.IVs[pkmn.pokemon_stat.SPEED],
                                unown.IVs[pkmn.pokemon_stat.SPECIAL]
                            )
            self.assertEqual(unown.form, form_from_IVs)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the form properly changes the IVs.
        for form in unown_entry.forms:
            unown.form = form
            self.assertEqual(unown.form, form)

            # Make sure IVs are properly set.
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                unown.IVs[pkmn.pokemon_stat.ATTACK],
                                unown.IVs[pkmn.pokemon_stat.DEFENSE],
                                unown.IVs[pkmn.pokemon_stat.SPEED],
                                unown.IVs[pkmn.pokemon_stat.SPECIAL]
                            )
            self.assertEqual(unown.form, form_from_IVs)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting IVs properly changes the form.
        unown.IVs[pkmn.pokemon_stat.ATTACK] = 10;
        unown.IVs[pkmn.pokemon_stat.DEFENSE] = 9;
        unown.IVs[pkmn.pokemon_stat.SPEED] = 1;
        unown.IVs[pkmn.pokemon_stat.SPECIAL] = 14;
        self.assertEquals(unown.form, "G");
