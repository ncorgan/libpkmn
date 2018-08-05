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
import random
import unittest

from nose_parameterized import parameterized

def test_name_func_pokemon(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[1])

def test_name_func_unown(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

class gen3_pokemon_test(pokemon_test_base.pokemon_test_base):

    @parameterized.expand([
        ("Torchic", "Ruby"),
        ("Mudkip", "Sapphire"),
        ("Treecko", "Emerald"),
        ("Charmander", "FireRed"),
        ("Bulbasaur", "LeafGreen"),
        ("Espeon", "Colosseum"),
        ("Umbreon", "XD")
    ], testcase_func_name=test_name_func_pokemon)
    def test_pokemon(self, species_name, game_name):
        species = pkmn.string_to_species(species_name)
        game = pkmn.string_to_game(game_name)

        pokemon = pkmn.pokemon(species, game, "", 30)
        test_params = None

        valid_moves = [
            pkmn.move.SWALLOW,
            pkmn.move.FLAMETHROWER,
            pkmn.move.RETURN,
            pkmn.move.FIRE_BLAST
        ]
        valid_games = [
            pkmn.game.RUBY,
            pkmn.game.SAPPHIRE,
            pkmn.game.EMERALD,
            pkmn.game.FIRERED,
            pkmn.game.LEAFGREEN,
            pkmn.game.COLOSSEUM,
            pkmn.game.XD
        ]

        if game in [pkmn.game.COLOSSEUM, pkmn.game.XD]:
            test_params = pokemon_test_base.pokemon_test_params(
                              pkmn.ball.GREAT_BALL,
                              [pkmn.ball.FRIEND_BALL, pkmn.ball.HEAL_BALL],
                              pkmn.item.RAZZ_BERRY,
                              [pkmn.item.BERRY, pkmn.item.MACH_BIKE],
                              "Distant land",
                              ["Phenac Story", "Orre Colosseum"],
                              ["New Bark Town", "Twinleaf Town"],
                              valid_moves,
                              [pkmn.move.ROOST, pkmn.move.FLAME_BURST],
                              valid_games,
                              [pkmn.game.GOLD, pkmn.game.HEARTGOLD]
                          )
        else:
            test_params = pokemon_test_base.pokemon_test_params(
                              pkmn.ball.GREAT_BALL,
                              [pkmn.ball.FRIEND_BALL, pkmn.ball.HEAL_BALL],
                              pkmn.item.RAZZ_BERRY,
                              [pkmn.item.BERRY, pkmn.item.MACH_BIKE],
                              "Fateful encounter",
                              ["Petalburg Woods", "Viridian Forest"],
                              ["New Bark Town", "Twinleaf Town"],
                              valid_moves,
                              [pkmn.move.SHADOW_SKY, pkmn.move.ROOST],
                              valid_games,
                              [pkmn.game.GOLD, pkmn.game.HEARTGOLD]
                          )

        self.common_test(pokemon, test_params)

    @parameterized.expand([
        "Ruby", "Sapphire", "Emerald",
        "FireRed", "LeafGreen",
        "Colosseum", "XD"
    ], testcase_func_name=test_name_func_unown)
    def test_unown(self, game_name):
        game = pkmn.string_to_game(game_name)

        unown_entry = pkmn.database.pokemon_entry(pkmn.species.UNOWN, game, "")

        unown = None

        for form in unown_entry.forms:
            unown = pkmn.pokemon(pkmn.species.UNOWN, game, form, 5)
            self.assertEqual(unown.form, form)

            # Make sure personality is properly set.
            form_from_personality = pkmn.calculations.gen3_unown_form(
                                        unown.personality
                                    )
            self.assertEqual(unown.form, form_from_personality)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the form properly changes the IVs.
        for form in unown_entry.forms:
            unown.form = form
            self.assertEqual(unown.form, form)

            # Make sure personality is properly set.
            form_from_personality = pkmn.calculations.gen3_unown_form(
                                        unown.personality
                                    )
            self.assertEqual(unown.form, form_from_personality)

            self.assertTrue(os.path.exists(unown.icon_filepath))
            self.assertTrue(os.path.exists(unown.sprite_filepath))

        # Make sure setting the personality properly sets the form.
        unown.personality = 0x4C07DE71;
        self.assertEqual(unown.form, "B");
