#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http:#opensource.org/licenses/MIT)
#

from . import *

import pkmn

import random
import sys
import string
import unittest

from nose_parameterized import parameterized

def random_bool():
    return random.choice([True, False])

# https://stackoverflow.com/a/2030081
def random_string(length):
       return ''.join(random.choice(string.ascii_lowercase if random_bool() else string.ascii_uppercase) for i in range(length))

def test_name_func(testcase_func, param_num, param):
    return "{0}_from_{1}_to_{2}".format(testcase_func.__name__, pkmn.game_to_string(param.args[2]), pkmn.game_to_string(param.args[3]))

class pokemon_conversions_test(base_test):

    @parameterized.expand([
        # Generation I -> Generation I
        (pkmn.species.BULBASAUR, "", pkmn.game.RED, pkmn.game.YELLOW),

        # Generation I -> Generation II
        (pkmn.species.SQUIRTLE, "", pkmn.game.BLUE, pkmn.game.GOLD),

        # Generation II -> Generation II
        (pkmn.species.CYNDAQUIL, "", pkmn.game.GOLD, pkmn.game.CRYSTAL),
        (pkmn.species.TOTODILE, "", pkmn.game.CRYSTAL, pkmn.game.GOLD),

        # Generation II -> Generation I
        (pkmn.species.CHARMANDER, "", pkmn.game.SILVER, pkmn.game.BLUE),

        # GBA -> GBA
        (pkmn.species.TORCHIC, "", pkmn.game.RUBY, pkmn.game.SAPPHIRE),
        (pkmn.species.MUDKIP, "", pkmn.game.RUBY, pkmn.game.EMERALD),
        (pkmn.species.TREECKO, "", pkmn.game.RUBY, pkmn.game.FIRERED),
        (pkmn.species.TORCHIC, "", pkmn.game.EMERALD, pkmn.game.SAPPHIRE),
        (pkmn.species.MUDKIP, "", pkmn.game.EMERALD, pkmn.game.EMERALD),
        (pkmn.species.TREECKO, "", pkmn.game.EMERALD, pkmn.game.FIRERED),
        (pkmn.species.CHARMANDER, "", pkmn.game.FIRERED, pkmn.game.SAPPHIRE),
        (pkmn.species.SQUIRTLE, "", pkmn.game.FIRERED, pkmn.game.EMERALD),
        (pkmn.species.BULBASAUR, "", pkmn.game.FIRERED, pkmn.game.FIRERED),

        # GBA -> GCN
        (pkmn.species.EEVEE, "", pkmn.game.RUBY, pkmn.game.COLOSSEUM),
        (pkmn.species.ESPEON, "", pkmn.game.EMERALD, pkmn.game.COLOSSEUM),
        (pkmn.species.UMBREON, "", pkmn.game.FIRERED, pkmn.game.COLOSSEUM),
        (pkmn.species.EEVEE, "", pkmn.game.RUBY, pkmn.game.XD),
        (pkmn.species.ESPEON, "", pkmn.game.EMERALD, pkmn.game.XD),
        (pkmn.species.UMBREON, "", pkmn.game.FIRERED, pkmn.game.XD),

        # GCN -> GBA
        (pkmn.species.EEVEE, "", pkmn.game.COLOSSEUM, pkmn.game.SAPPHIRE),
        (pkmn.species.ESPEON, "", pkmn.game.COLOSSEUM, pkmn.game.EMERALD),
        (pkmn.species.UMBREON, "", pkmn.game.COLOSSEUM, pkmn.game.LEAFGREEN),
        (pkmn.species.EEVEE, "", pkmn.game.XD, pkmn.game.SAPPHIRE),
        (pkmn.species.ESPEON, "", pkmn.game.XD, pkmn.game.EMERALD),
        (pkmn.species.UMBREON, "", pkmn.game.XD, pkmn.game.LEAFGREEN),

        # GCN -> GCN
        (pkmn.species.VAPOREON, "", pkmn.game.COLOSSEUM, pkmn.game.COLOSSEUM),
        (pkmn.species.JOLTEON, "", pkmn.game.COLOSSEUM, pkmn.game.XD),
        (pkmn.species.VAPOREON, "", pkmn.game.XD, pkmn.game.XD),
        (pkmn.species.JOLTEON, "", pkmn.game.XD, pkmn.game.COLOSSEUM)
    ], testcase_func_name=test_name_func)
    def test_conversion(self, species_name, form, origin_game_name, dest_game_name):
        species = pkmn.string_to_species(species_name)
        origin_game_name = pkmn.string_to_game(origin_game_name)
        dest_game_name = pkmn.string_to_game(dest_game_name)

        origin_generation = GAME_TO_GENERATION[origin_game]
        dest_generation = GAME_TO_GENERATION[dest_game]
        min_generation = min(origin_generation, dest_generation)
        game_for_lists = origin_game if min_generation == origin_generation else dest_game

        items = pkmn.database.lists.get_item_list(game_for_lists)
        moves = pkmn.database.lists.get_move_list(game_for_lists)

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
