#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from . import *

import os
import random
import string
import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

def random_bool():
    return (random.random() < 0.5)

class base_test(unittest.TestCase):

    def assertStringEqual(self, str1, str2):
        if PYTHON_MAJOR_VERSION == 2:
            if "str" in str(type(str1)):
                str1 = str1.decode("utf-8")
            if "str" in str(type(str2)):
                str2 = str2.decode("utf-8")

        self.assertEqual(str1, str2)

    def get_both_string_types(self, input_str):
        if PYTHON_MAJOR_VERSION == 2:
            # Which call succeeds depends on SWIG version
            try:
                return (input_str, input_str.decode("utf-8"))
            except:
                return (input_str, unicode(input_str))
        else:
            return (input_str, input_str)

    def compare_attributes(self, var1, var2):
        self.assertEqual(
            var1.numeric_attributes.names,
            var2.numeric_attributes.names
        )
        for attribute_name in var1.numeric_attributes.names:
            self.assertEqual(
                var1.numeric_attributes[attribute_name],
                var2.numeric_attributes[attribute_name]
            )

        self.assertEqual(
            var1.string_attributes.names,
            var2.string_attributes.names
        )
        for attribute_name in var1.string_attributes.names:
            self.assertEqual(
                var1.string_attributes[attribute_name],
                var2.string_attributes[attribute_name]
            )

        self.assertEqual(
            var1.boolean_attributes.names,
            var2.boolean_attributes.names
        )
        for attribute_name in var1.boolean_attributes.names:
            self.assertEqual(
                var1.boolean_attributes[attribute_name],
                var2.boolean_attributes[attribute_name]
            )

    def get_specific_random_pokemon(
        self,
        game,
        species,
        form,
        move_list,
        item_list
    ):
        generation = GAME_TO_GENERATION[game]

        # Level bound accounts for Gen II level met limitation
        pokemon = pkmn.pokemon(species, game, form, random.randint(2, 63))

        for move_index in range(4):
            move = ""
            while True:
                move = random.choice(move_list)
                if not move.startswith("Shadow"):
                    break
            pokemon.moves[move_index].move = move

        for EV in pokemon.EVs.keys:
            pokemon.EVs[EV] = random.randint(0, 255)
        for IV in pokemon.IVs.keys:
            pokemon.IVs[IV] = random.randint(0, 15)

        if generation >= 2:
            # Keep going until one is holdable.
            while pokemon.held_item == "None":
                try:
                    pokemon.held_item = random.choice(item_list)
                except:
                    continue

            pokemon.pokerus_duration = random.randint(0, 15)

        if generation >= 3:
            for marking in pokemon.markings.keys:
                pokemon.markings[marking] = random_bool()
            for ribbon in pokemon.ribbons.keys:
                pokemon.ribbons[ribbon] = random_bool()
            for contest_stat in pokemon.contest_stats.keys:
                pokemon.contest_stats[contest_stat] = random.randint(0, 255)

        return pokemon

    def get_random_pokemon(self, game, pokemon_list, move_list, item_list):
        generation = GAME_TO_GENERATION[game]
        species = ""
        if generation == 3:
            while True:
                species = random.choice(pokemon_list)
                if species != "Deoxys":
                    break
        else:
            species = random.choice(pokemon_list)

        return self.get_specific_random_pokemon(game, species, "", move_list, item_list)

    def compare_pokemon(self, pokemon1, pokemon2):
        generation = GAME_TO_GENERATION[pokemon1.game]

        # There is no way to tell what game an imported Generation I-II
        # Pokemon comes from, so LibPKMN defaults to a default valid game.
        if generation >= 3:
            self.assertEqual(
                pokemon1.game,
                pokemon2.game
            )

        self.assertEqual(pokemon1.species, pokemon2.species)
        self.assertEqual(pokemon1.form, pokemon2.form)
        self.assertEqual(pokemon1.original_trainer_id, pokemon2.original_trainer_id)
        self.assertEqual(pokemon1.experience, pokemon2.experience)
        self.assertEqual(pokemon1.level, pokemon2.level)
        self.assertEqual(pokemon1.nickname, pokemon2.nickname)
        self.assertEqual(pokemon1.original_trainer_name, pokemon2.original_trainer_name)

        self.assertEqual(pokemon1.EVs.keys, pokemon2.EVs.keys)
        for EV in pokemon1.EVs.keys:
            self.assertEqual(pokemon1.EVs[EV], pokemon2.EVs[EV])

        self.assertEqual(pokemon1.IVs.keys, pokemon2.IVs.keys)
        for IV in pokemon1.IVs.keys:
            self.assertEqual(pokemon1.IVs[IV], pokemon2.IVs[IV])

        self.assertEqual(pokemon1.stats.keys(), pokemon2.stats.keys())
        for stat in pokemon1.stats.keys():
            self.assertEqual(pokemon1.stats[stat], pokemon2.stats[stat])

        if pokemon1.game == pokemon2.game:
            self.assertEqual(pokemon1.icon_filepath, pokemon2.icon_filepath)
            self.assertEqual(pokemon1.sprite_filepath, pokemon2.sprite_filepath)

        self.compare_attributes(pokemon1, pokemon2)

        if generation >= 2:
            self.assertEqual(pokemon1.original_trainer_gender, pokemon2.original_trainer_gender)
            self.assertEqual(pokemon1.current_trainer_friendship, pokemon2.current_trainer_friendship)
            self.assertEqual(pokemon1.gender, pokemon2.gender)
            self.assertEqual(pokemon1.is_shiny, pokemon2.is_shiny)
            self.assertEqual(pokemon1.held_item, pokemon2.held_item)
            self.assertEqual(pokemon1.level_met, pokemon2.level_met)
            self.assertEqual(pokemon1.location_met, pokemon2.location_met)
            self.assertEqual(pokemon1.pokerus_duration, pokemon2.pokerus_duration)

        if generation >= 3:
            self.assertEqual(pokemon1.ability, pokemon2.ability)
            self.assertEqual(pokemon1.ball, pokemon2.ball)
            self.assertEqual(pokemon1.personality, pokemon2.personality)

            self.assertEqual(pokemon1.markings.keys, pokemon2.markings.keys)
            for marking in pokemon1.markings.keys:
                self.assertEqual(
                    pokemon1.markings[marking],
                    pokemon2.markings[marking]
                )
            self.assertEqual(pokemon1.ribbons.keys, pokemon2.ribbons.keys)
            for ribbon in pokemon1.ribbons.keys:
                self.assertEqual(
                    pokemon1.ribbons[ribbon],
                    pokemon2.ribbons[ribbon]
                )
            self.assertEqual(
                pokemon1.contest_stats.keys,
                pokemon2.contest_stats.keys
            )
            for contest_stat in pokemon1.contest_stats.keys:
                self.assertEqual(
                    pokemon1.contest_stats[contest_stat],
                    pokemon2.contest_stats[contest_stat]
                )

        if generation >= 4:
            self.assertEqual(
                pokemon1.location_met_as_egg,
                pokemon2.location_met_as_egg
            )
