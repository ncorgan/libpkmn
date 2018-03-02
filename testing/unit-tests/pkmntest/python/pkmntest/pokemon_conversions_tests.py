#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http:#opensource.org/licenses/MIT)
#

import base_test

import pkmn

import random
import sys
import string
import unittest

from nose_parameterized import parameterized

GAME_TO_GENERATION = {
    "Red": 1,
    "Blue": 1,
    "Yellow": 1,
    "Gold": 2,
    "Silver": 2,
    "Crystal": 2,
    "Ruby": 3,
    "Sapphire": 3,
    "Emerald": 3,
    "FireRed": 3,
    "LeafGreen": 3,
    "Colosseum": 3,
    "XD": 3
}

def random_bool():
    return random.choice([True, False])

# https://stackoverflow.com/a/2030081
def random_string(length):
       return ''.join(random.choice(string.ascii_lowercase if random_bool() else string.ascii_uppercase) for i in range(length))

def test_name_func(testcase_func, param_num, param):
    return "{0}_from_{1}_to_{2}".format(testcase_func.__name__, param.args[2], param.args[3])

class pokemon_conversions_test(base_test.base_test):

    @parameterized.expand([
        # Generation I -> Generation I
        ("Bulbasaur", "", "Red", "Yellow"),

        # Generation I -> Generation II
        ("Squirtle", "", "Blue", "Gold"),

        # Generation II -> Generation II
        ("Cyndaquil", "", "Gold", "Crystal"),
        ("Totodile", "", "Crystal", "Gold"),

        # Generation II -> Generation I
        ("Charmander", "", "Silver", "Blue"),

        # GBA -> GBA
        ("Torchic", "", "Ruby", "Sapphire"),
        ("Mudkip", "", "Ruby", "Emerald"),
        ("Treecko", "", "Ruby", "FireRed"),
        ("Torchic", "", "Emerald", "Sapphire"),
        ("Mudkip", "", "Emerald", "Emerald"),
        ("Treecko", "", "Emerald", "FireRed"),
        ("Charmander", "", "FireRed", "Sapphire"),
        ("Squirtle", "", "FireRed", "Emerald"),
        ("Bulbasaur", "", "FireRed", "FireRed"),

        # GBA -> GCN
        ("Eevee", "", "Ruby", "Colosseum"),
        ("Espeon", "", "Emerald", "Colosseum"),
        ("Umbreon", "", "FireRed", "Colosseum"),
        ("Eevee", "", "Ruby", "XD"),
        ("Espeon", "", "Emerald", "XD"),
        ("Umbreon", "", "FireRed", "XD"),

        # GCN -> GBA
        ("Eevee", "", "Colosseum", "Sapphire"),
        ("Espeon", "", "Colosseum", "Emerald"),
        ("Umbreon", "", "Colosseum", "LeafGreen"),
        ("Eevee", "", "XD", "Sapphire"),
        ("Espeon", "", "XD", "Emerald"),
        ("Umbreon", "", "XD", "LeafGreen"),

        # GCN -> GCN
        ("Vaporeon", "", "Colosseum", "Colosseum"),
        ("Jolteon", "", "Colosseum", "XD"),
        ("Vaporeon", "", "XD", "XD"),
        ("Jolteon", "", "XD", "Colosseum")
    ], testcase_func_name=test_name_func)
    def test_conversion(self, species, form, origin_game, dest_game):
        origin_generation = GAME_TO_GENERATION[origin_game]
        dest_generation = GAME_TO_GENERATION[dest_game]
        min_generation = min(origin_generation, dest_generation)
        game_for_lists = origin_game if min_generation == origin_generation else dest_game

        items = pkmn.database.get_item_list(game_for_lists)
        moves = pkmn.database.get_move_list(game_for_lists)

        first_pokemon = self.get_specific_random_pokemon(
                            origin_game,
                            species,
                            form,
                            moves,
                            items
                        )

        # Convert to the second game and compare.
        second_pokemon = first_pokemon.to_game(dest_game)

        self.assertEqual(first_pokemon.species, second_pokemon.species)
        self.assertEqual(dest_game, second_pokemon.game)
        self.assertEqual(first_pokemon.form, second_pokemon.form)
        self.assertEqual(first_pokemon.nickname, second_pokemon.nickname)
        self.assertEqual(first_pokemon.original_trainer_name, second_pokemon.original_trainer_name)
        self.assertEqual(first_pokemon.original_trainer_id, second_pokemon.original_trainer_id)
        self.assertEqual(first_pokemon.original_trainer_public_id, second_pokemon.original_trainer_public_id)
        self.assertEqual(first_pokemon.experience, second_pokemon.experience)
        self.assertEqual(first_pokemon.level, second_pokemon.level)

        if origin_generation == dest_generation:
            self.assertEqual(first_pokemon.EVs.keys, second_pokemon.EVs.keys)
            for EV in first_pokemon.EVs.keys:
                self.assertEqual(first_pokemon.EVs[EV], second_pokemon.EVs[EV])

            self.assertEqual(first_pokemon.IVs.keys, second_pokemon.IVs.keys)
            for IV in first_pokemon.IVs.keys:
                self.assertEqual(first_pokemon.IVs[IV], second_pokemon.IVs[IV])

        for move_index in range(4):
            self.assertEqual(
                first_pokemon.moves[move_index].move,
                second_pokemon.moves[move_index].move
            )
            self.assertEqual(
                first_pokemon.moves[move_index].pp,
                second_pokemon.moves[move_index].pp
            )

        if min_generation >= 2:
            self.assertEqual(first_pokemon.original_trainer_gender, second_pokemon.original_trainer_gender)
            self.assertEqual(first_pokemon.gender, second_pokemon.gender)
            self.assertEqual(first_pokemon.is_shiny, second_pokemon.is_shiny)
            self.assertEqual(first_pokemon.held_item, second_pokemon.held_item)
            self.assertEqual(first_pokemon.current_trainer_friendship, second_pokemon.current_trainer_friendship)
            self.assertEqual(first_pokemon.level, second_pokemon.level_met)

        if min_generation >= 3:
            self.assertEqual(first_pokemon.original_trainer_secret_id, second_pokemon.original_trainer_secret_id)
            self.assertEqual(first_pokemon.ability, second_pokemon.ability)
            self.assertEqual(first_pokemon.ball, second_pokemon.ball)
            self.assertEqual(first_pokemon.original_game, second_pokemon.original_game)
            self.assertEqual(first_pokemon.personality, second_pokemon.personality)

            if origin_generation == dest_generation:
                self.assertEqual(
                    first_pokemon.markings.keys,
                    second_pokemon.markings.keys
                )
                for marking in first_pokemon.markings.keys:
                    self.assertEqual(
                        first_pokemon.markings[marking],
                        second_pokemon.markings[marking]
                    )

                self.assertEqual(
                    first_pokemon.ribbons.keys,
                    second_pokemon.ribbons.keys
                )
                for ribbon in first_pokemon.ribbons.keys:
                    self.assertEqual(
                        first_pokemon.ribbons[ribbon],
                        second_pokemon.ribbons[ribbon]
                    )

                self.assertEqual(
                    first_pokemon.contest_stats.keys,
                    second_pokemon.contest_stats.keys
                )
                for contest_stat in first_pokemon.contest_stats.keys:
                    self.assertEqual(
                        first_pokemon.contest_stats[contest_stat],
                        second_pokemon.contest_stats[contest_stat]
                    )
