#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from . import *

import pkmn

import os
import random
import string
import sys
import unittest

from nose_parameterized import parameterized

PYTHON_MAJOR_VERSION = sys.version_info[0]

class pokemon_test_params:
    def __init__(
        self,
        valid_ball, invalid_balls,
        valid_item, invalid_items,
        expected_original_location, valid_locations, invalid_locations,
        valid_moves, invalid_moves,
        valid_original_games, invalid_original_games
    ):
        self.valid_ball = valid_ball
        self.invalid_balls = invalid_balls
        self.valid_item = valid_item
        self.invalid_items = invalid_items
        self.expected_original_location = expected_original_location
        self.valid_locations = valid_locations
        self.invalid_locations = invalid_locations
        self.valid_moves = valid_moves
        self.invalid_moves = invalid_moves
        self.valid_original_games = valid_original_games
        self.invalid_original_games = invalid_original_games

class pokemon_test_base(base_test):

    def initial_values_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        self.assertEqual(pokemon.condition, "None")

        if generation >= 5:
            self.assertEqual(pokemon.nickname, pokemon.database_entry.species_name)
        else:
            self.assertEqual(pokemon.nickname, pokemon.database_entry.species_name.upper())

        self.assertEqual(pokemon.original_trainer_name, pkmn.pokemon.DEFAULT_TRAINER_NAME)

        self.assertEqual(pokemon.held_item, pkmn.item.NONE)
        if generation >= 2:
            self.assertEqual(pokemon.original_trainer_gender, pkmn.gender.MALE)
        else:
            # Default value since a getter shouldn't throw an exception.
            self.assertEqual(pokemon.original_trainer_gender, pkmn.gender.NONE)

        self.assertEqual(
            pokemon.original_trainer_public_id,
            (pkmn.pokemon.DEFAULT_TRAINER_ID & 0xFFFF)
        )

        if generation >= 3:
            self.assertEqual(
                pokemon.original_trainer_secret_id,
                ((pkmn.pokemon.DEFAULT_TRAINER_ID & 0xFFFF0000) >> 16)
            )
            self.assertEqual(
                pokemon.original_trainer_id,
                pkmn.pokemon.DEFAULT_TRAINER_ID
            )
        else:
            self.assertEqual(
                pokemon.original_trainer_id,
                (pkmn.pokemon.DEFAULT_TRAINER_ID & 0xFFFF)
            )

            # Default value since a getter shouldn't throw an exception.
            self.assertEqual(pokemon.original_trainer_secret_id, 0)

        if generation >= 2:
            self.assertEqual(
                pokemon.current_trainer_friendship,
                pokemon.database_entry.base_friendship
            )
        else:
            # Default value since a getter shouldn't throw an exception.
            self.assertEqual(pokemon.current_trainer_friendship, 0)

        if generation >= 3:
            self.assertEqual(pokemon.ball, pkmn.ball.POKE_BALL)

            # There is no distinction between Colosseum and XD in the game
            # storage.
            if pokemon.game in [pkmn.game.COLOSSEUM, pkmn.game.XD]:
                self.assertEqual(pokemon.original_game, pkmn.game.COLOSSEUM)
            else:
                self.assertEqual(pokemon.original_game, pokemon.game)
        else:
            # Default value since a getter shouldn't throw an exception.
            self.assertEqual(pokemon.ball, pkmn.ball.NONE)
            self.assertEqual(pokemon.original_game, pkmn.game.NONE)

        self.assertEqual(
            pokemon.experience,
            pokemon.database_entry.get_experience_at_level(pokemon.level)
        )

        if generation >= 2:
            self.assertEqual(pokemon.level_met, pokemon.level)

        self.assertEqual(len(pokemon.moves), 4)

        self.assertTrue(os.path.exists(pokemon.icon_filepath))
        self.assertTrue(os.path.exists(pokemon.sprite_filepath))

        self.assertEqual(pokemon.current_hp, pokemon.stats[pkmn.pokemon_stat.HP])

    def initial_maps_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        # EVs

        self.assertTrue(pkmn.pokemon_stat.HP in pokemon.EVs.keys)
        self.assertTrue(pkmn.pokemon_stat.ATTACK in pokemon.EVs.keys)
        self.assertTrue(pkmn.pokemon_stat.DEFENSE in pokemon.EVs.keys)
        self.assertTrue(pkmn.pokemon_stat.SPEED in pokemon.EVs.keys)
        if generation >= 3:
            self.assertFalse(pkmn.pokemon_stat.SPECIAL in pokemon.EVs.keys)
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.EVs.keys)
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.EVs.keys)
        else:
            self.assertTrue(pkmn.pokemon_stat.SPECIAL in pokemon.EVs.keys)
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.EVs.keys)
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.EVs.keys)

        for EV in pokemon.EVs.keys:
            self.assertEqual(pokemon.EVs[EV], 0)

        # IVs

        self.assertTrue(pkmn.pokemon_stat.HP in pokemon.IVs.keys)
        self.assertTrue(pkmn.pokemon_stat.ATTACK in pokemon.IVs.keys)
        self.assertTrue(pkmn.pokemon_stat.DEFENSE in pokemon.IVs.keys)
        self.assertTrue(pkmn.pokemon_stat.SPEED in pokemon.IVs.keys)
        if generation >= 3:
            self.assertFalse(pkmn.pokemon_stat.SPECIAL in pokemon.IVs.keys)
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.IVs.keys)
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.IVs.keys)
        else:
            self.assertTrue(pkmn.pokemon_stat.SPECIAL in pokemon.IVs.keys)
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.IVs.keys)
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.IVs.keys)

        for IV in pokemon.IVs.keys:
            self.assertGreaterEqual(pokemon.IVs[IV], 0)
            self.assertLessEqual(
                pokemon.IVs[IV],
                (31 if generation >= 3 else 15)
            )

        # Stats

        self.assertTrue(pkmn.pokemon_stat.HP in pokemon.stats.keys())
        self.assertTrue(pkmn.pokemon_stat.ATTACK in pokemon.stats.keys())
        self.assertTrue(pkmn.pokemon_stat.DEFENSE in pokemon.stats.keys())
        self.assertTrue(pkmn.pokemon_stat.SPEED in pokemon.stats.keys())
        if generation >= 2:
            self.assertFalse(pkmn.pokemon_stat.SPECIAL in pokemon.stats.keys())
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.stats.keys())
            self.assertTrue(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.stats.keys())
        else:
            self.assertTrue(pkmn.pokemon_stat.SPECIAL in pokemon.stats.keys())
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_ATTACK in pokemon.stats.keys())
            self.assertFalse(pkmn.pokemon_stat.SPECIAL_DEFENSE in pokemon.stats.keys())

        for stat in pokemon.stats.keys():
            self.assertGreaterEqual(pokemon.stats[stat], 0)
            self.assertLessEqual(pokemon.stats[stat], 65535)

        if generation >= 3:
            # Contest stats

            self.assertTrue(pkmn.contest_stat.COOL in pokemon.contest_stats.keys)
            self.assertTrue(pkmn.contest_stat.BEAUTY in pokemon.contest_stats.keys)
            self.assertTrue(pkmn.contest_stat.CUTE in pokemon.contest_stats.keys)
            self.assertTrue(pkmn.contest_stat.SMART in pokemon.contest_stats.keys)
            self.assertTrue(pkmn.contest_stat.TOUGH in pokemon.contest_stats.keys)
            if generation == 3:
                self.assertTrue(pkmn.contest_stat.FEEL in pokemon.contest_stats.keys)
                self.assertFalse(pkmn.contest_stat.SHEEN in pokemon.contest_stats.keys)
            else:
                self.assertFalse(pkmn.contest_stat.FEEL in pokemon.contest_stats.keys)
                self.assertTrue(pkmn.contest_stat.SHEEN in pokemon.contest_stats.keys)

            for contest_stat in pokemon.contest_stats.keys:
                self.assertEqual(pokemon.contest_stats[contest_stat], 0)

            # Markings

            self.assertTrue(pkmn.marking.CIRCLE in pokemon.markings.keys)
            self.assertTrue(pkmn.marking.TRIANGLE in pokemon.markings.keys)
            self.assertTrue(pkmn.marking.SQUARE in pokemon.markings.keys)
            self.assertTrue(pkmn.marking.HEART in pokemon.markings.keys)
            if generation >= 4:
                self.assertTrue(pkmn.marking.STAR in pokemon.markings.keys)
                self.assertTrue(pkmn.marking.DIAMOND in pokemon.markings.keys)
            else:
                self.assertFalse(pkmn.marking.STAR in pokemon.markings.keys)
                self.assertFalse(pkmn.marking.DIAMOND in pokemon.markings.keys)

            for marking in pokemon.markings.keys:
                self.assertFalse(pokemon.markings[marking])

    def image_filepaths_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        self.assertTrue(os.path.exists(pokemon.icon_filepath))

        if generation >= 2:
            for gender in [pkmn.gender.MALE, pkmn.gender.FEMALE]:
                for is_shiny in [True, False]:
                    pokemon.gender = gender
                    pokemon.is_shiny = is_shiny
                    self.assertTrue(os.path.exists(pokemon.sprite_filepath))
        else:
            self.assertTrue(os.path.exists(pokemon.sprite_filepath))

    def ability_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 3:
            self.assertNotEquals(
                pokemon.database_entry.abilities.first,
                pkmn.ability.NONE
            )

            pokemon.ability = pokemon.database_entry.abilities.first
            self.assertEqual(
                pokemon.ability,
                pokemon.database_entry.abilities.first
            )

            if pokemon.database_entry.abilities.second != pkmn.ability.NONE:
                pokemon.ability = pokemon.database_entry.abilities.second
                self.assertEqual(
                    pokemon.ability,
                    pokemon.database_entry.abilities.second
                )

            if generation >= 5:
                if pokemon.database_entry.hidden_ability != pkmn.ability.NONE:
                    pokemon.ability = pokemon.database_entry.ability
                    self.assertEqual(
                        pokemon.ability,
                        pokemon.database_entry.hidden_ability
                    )

            with self.assertRaises(ValueError):
                pokemon.ability = pkmn.ability.NONE
            with self.assertRaises(ValueError):
                pokemon.ability = pkmn.ability.WONDER_GUARD
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.ability, pkmn.ability.NONE)

            with self.assertRaises(RuntimeError):
                pokemon.ability = pkmn.ability.WONDER_GUARD

    def ball_test(self, pokemon, valid_ball_name, invalid_ball_names):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 3:
            pokemon.ball = valid_ball_name
            self.assertEqual(pokemon.ball, valid_ball_name)

            for invalid_ball_name in invalid_ball_names:
                with self.assertRaises(ValueError):
                    pokemon.ball = invalid_ball_name
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.ball, pkmn.ball.NONE)

            with self.assertRaises(RuntimeError):
                pokemon.ball = pkmn.ball.GREAT_BALL

    def condition_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        conditions = ["None", "Asleep", "Poison", "Burn", "Frozen", "Paralysis"]
        if generation >= 3:
            conditions += ["Bad Poison"]

        for condition in conditions:
            pokemon.condition = condition
            self.assertEqual(pokemon.condition, condition)

    def friendship_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 2:
            friendship = random.randint(0, 255)

            pokemon.current_trainer_friendship = friendship
            self.assertEqual(pokemon.current_trainer_friendship, friendship)

            with self.assertRaises(IndexError):
                pokemon.current_trainer_friendship = -1
            with self.assertRaises(IndexError):
                pokemon.current_trainer_friendship = 256
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.current_trainer_friendship, 0)

            with self.assertRaises(RuntimeError):
                pokemon.current_trainer_friendship = 0

    def item_test(self, pokemon, valid_item_name, invalid_item_names):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 2:
            pokemon.held_item = valid_item_name
            self.assertEqual(pokemon.held_item, valid_item_name)

            for invalid_item_name in invalid_item_names:
                with self.assertRaises(ValueError):
                    pokemon.held_item = invalid_item_name
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.held_item, pkmn.item.NONE)

            with self.assertRaises(RuntimeError):
                pokemon.held_item = pkmn.item.POTION

    def level_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        with self.assertRaises(IndexError):
            pokemon.level = -1
        with self.assertRaises(IndexError):
            pokemon.level = 101

        level = 50
        pokemon.level = level
        self.assertEqual(pokemon.level, level)
        self.assertEqual(
            pokemon.database_entry.get_level_at_experience(pokemon.experience),
            level
        )

        experience = 123456
        pokemon.experience = experience
        self.assertEqual(pokemon.experience, experience)
        self.assertLess(
            pokemon.database_entry.get_experience_at_level(pokemon.level-1),
            experience
        )
        self.assertLessEqual(
            pokemon.database_entry.get_experience_at_level(pokemon.level),
            experience
        )

        if generation == 1:
            with self.assertRaises(IndexError):
                pokemon.level = 1

            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.level_met, 0)

            with self.assertRaises(RuntimeError):
                pokemon.level_met = 10
        elif generation == 2:
            level_met = 5
            pokemon.level_met = level_met
            self.assertEqual(pokemon.level_met, level_met)

            with self.assertRaises(IndexError):
                pokemon.level = 1
            with self.assertRaises(IndexError):
                pokemon.level_met = 101
        else:
            level_met = 5
            pokemon.level_met = level_met
            self.assertEqual(pokemon.level_met, level_met)

            with self.assertRaises(IndexError):
                pokemon.level = 0
            with self.assertRaises(IndexError):
                pokemon.level_met = 101

    def location_met_test(
        self,
        pokemon,
        expected_original_location,
        valid_locations,
        invalid_locations
    ):
        generation = GAME_TO_GENERATION(pokemon.game)

        if generation == 1:
            # The getters shouldn't throw by convention, but the setters will.
            self.assertEqual(pokemon.location_met, "")
            self.assertEqual(pokemon.location_met_as_egg, "")

            with self.assertRaises(RuntimeError):
                pokemon.location_met = valid_locations[0]
            with self.assertRaises(RuntimeError):
                pokemon.location_met_as_egg = valid_locations[0]
        elif generation in [2,3]:
            self.assertEqual(pokemon.location_met, expected_original_location)

            for valid_location in valid_locations:
                pokemon.location_met = valid_location
                self.assertEqual(pokemon.location_met, valid_location)

            for invalid_location in invalid_locations:
                with self.assertRaises(ValueError):
                    pokemon.location_met = invalid_location

            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.location_met, "")

            with self.assertRaises(RuntimeError):
                pokemon.location_met_as_egg = valid_locations[0]
        else:
            self.assertEqual(pokemon.location_met, expected_original_location)
            self.assertEqual(pokemon.location_met_as_egg, expected_original_location)

            for valid_location in valid_locations:
                pokemon.location_met = valid_location
                self.assertEqual(pokemon.location_met, valid_location)

                pokemon.location_met_as_egg = valid_location
                self.assertEqual(pokemon.location_met_as_egg, valid_location)

            for invalid_location in invalid_locations:
                with self.assertRaises(ValueError):
                    pokemon.location_met = invalid_location

                with self.assertRaises(ValueError):
                    pokemon.location_met_as_egg = invalid_location

    def markings_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 3:
            for marking in pokemon.markings.keys:
                self.assertFalse(pokemon.markings[marking])
                pokemon.markings[marking] = True
                self.assertTrue(pokemon.markings[marking])
        else:
            self.assertEqual(len(pokemon.markings), 0)
            with self.assertRaises(RuntimeError):
                pokemon.markings[pkmn.marking.CIRCLE] = True

    def move_test(self, pokemon, valid_move_names, invalid_move_names):
        self.assertEqual(len(valid_move_names), 4)

        for move_index in range(len(valid_move_names)):
            pokemon.moves[move_index].move = valid_move_names[move_index]
            self.assertEqual(
                pokemon.moves[move_index].move,
                valid_move_names[move_index]
            )

            move_entry = pkmn.database.move_entry(
                             valid_move_names[move_index],
                             pokemon.game
                         )
            pokemon.moves[move_index].pp = 0
            self.assertEqual(pokemon.moves[move_index].pp, 0)

            max_pp = move_entry.get_pp(3)
            pokemon.moves[move_index].pp = max_pp
            self.assertEqual(pokemon.moves[move_index].pp, max_pp)

            with self.assertRaises(IndexError):
                pokemon.moves[move_index].pp = -1
            with self.assertRaises(IndexError):
                pokemon.moves[move_index].pp = max_pp+1

        for invalid_move_name in invalid_move_names:
            with self.assertRaises(ValueError):
                pokemon.moves[0].move = invalid_move_name

        with self.assertRaises(IndexError):
            pokemon.moves[-1].move = valid_move_names[0]
        with self.assertRaises(IndexError):
            pokemon.moves[4].move = valid_move_names[0]

        with self.assertRaises(IndexError):
            pokemon.moves[-1].pp = valid_move_names[0]
        with self.assertRaises(IndexError):
            pokemon.moves[4].pp = valid_move_names[0]

    def original_game_test(self, pokemon, valid_games, invalid_games):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 3:
            for valid_game in valid_games:
                pokemon.original_game = valid_game
                if valid_game in [pkmn.game.COLOSSEUM, pkmn.game.XD]:
                    self.assertEqual(
                        pokemon.original_game,
                        pkmn.game.COLOSSEUM
                    )
                else:
                    self.assertEqual(
                        pokemon.original_game,
                        valid_game
                    )

            for invalid_game in invalid_games:
                with self.assertRaises(ValueError):
                    pokemon.original_game = invalid_game
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.original_game, pkmn.game.NONE)

            with self.assertRaises(RuntimeError):
                pokemon.original_game = valid_games[0]

    def personality_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 3:
            personality = random.randint(0, 0xFFFFFFFF)

            pokemon.personality = personality
            self.assertEqual(pokemon.personality, personality)

            # Make sure SWIG+Python catches values outside the uint32_t
            # range. What type of error is thrown depends on SWIG version.
            try:
                with self.assertRaises(OverflowError):
                    pokemon.personality = -1
            except:
                with self.assertRaises(TypeError):
                    pokemon.personality = -1

            try:
                with self.assertRaises(OverflowError):
                    pokemon.personality = 0xFFFFFFFF+1
            except:
                with self.assertRaises(TypeError):
                    pokemon.personality = 0xFFFFFFFF+1
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEqual(pokemon.personality, 0)

            with self.assertRaises(RuntimeError):
                pokemon.personality = 0

    def pokerus_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        if generation >= 2:
            pokerus_duration = random.randint(0, 15)
            pokemon.pokerus_duration = pokerus_duration
            self.assertEqual(pokemon.pokerus_duration, pokerus_duration)

            with self.assertRaises(IndexError):
                pokemon.pokerus_duration = -1
            with self.assertRaises(IndexError):
                pokemon.pokerus_duration = 16
        else:
            # The getter shouldn't throw by convention, but the setter will.
            self.assertEquals(pokemon.pokerus_duration, 0)

            with self.assertRaises(RuntimeError):
                pokemon.pokerus_duration = 1

    def stats_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        # Check bounds for setting the current HP.
        with self.assertRaises(IndexError):
            pokemon.current_hp = -1
        with self.assertRaises(IndexError):
            pokemon.current_hp = pokemon.stats[pkmn.pokemon_stat.HP] + 1

        pokemon.current_hp = 0
        self.assertEqual(pokemon.current_hp, 0)

        pokemon.current_hp = pokemon.stats[pkmn.pokemon_stat.HP] - 1
        self.assertEqual(pokemon.current_hp, pokemon.stats[pkmn.pokemon_stat.HP] - 1)

        pokemon.current_hp = pokemon.stats[pkmn.pokemon_stat.HP]
        self.assertEqual(pokemon.current_hp, pokemon.stats[pkmn.pokemon_stat.HP])

        # Set the HP stat to lower than the current HP, and make sure
        # it's updated.

        current_hp = pokemon.current_hp
        pokemon.EVs[pkmn.pokemon_stat.HP] = 0
        pokemon.IVs[pkmn.pokemon_stat.HP] = 0
        self.assertLessEqual(pokemon.current_hp, current_hp)

        if generation >= 3:
            for contest_stat in pokemon.contest_stats.keys:
                stat_value = random.randint(0, 255)
                pokemon.contest_stats[contest_stat] = stat_value
                self.assertEqual(
                    pokemon.contest_stats[contest_stat],
                    stat_value
                )

    def trainer_info_test(self, pokemon):
        generation = GAME_TO_GENERATION[pokemon.game]

        with self.assertRaises(ValueError):
            pokemon.nickname = ""
        with self.assertRaises(ValueError):
            pokemon.nickname = "Too long nickname"

        pokemon.nickname = "foobarbaz"
        self.assertEqual(pokemon.nickname, "foobarbaz")

        with self.assertRaises(ValueError):
            pokemon.original_trainer_name = ""
        with self.assertRaises(ValueError):
            pokemon.original_trainer_name = "Too long trainer name"

        pokemon.original_trainer_name = "foobar"
        self.assertEqual(pokemon.original_trainer_name, "foobar")

        if generation >= 2:
            for gender in [pkmn.gender.MALE, pkmn.gender.FEMALE]:
                pokemon.original_trainer_gender = gender
                self.assertEqual(
                    pokemon.original_trainer_gender,
                    gender
                )

                with self.assertRaises(ValueError):
                    pokemon.original_trainer_gender = pkmn.gender.GENDERLESS
        else:
            for gender in [pkmn.gender.MALE, pkmn.gender.FEMALE]:
                with self.assertRaises(RuntimeError):
                    pokemon.original_trainer_gender = gender

        if generation >= 3:
            pokemon.original_trainer_id = 0x1234ABCD
            self.assertEqual(pokemon.original_trainer_id, 0x1234ABCD)
            self.assertEqual(pokemon.original_trainer_public_id, 0xABCD)
            self.assertEqual(pokemon.original_trainer_secret_id, 0x1234)

            pokemon.original_trainer_public_id = 0x1A2B
            self.assertEqual(pokemon.original_trainer_id, 0x12341A2B)
            self.assertEqual(pokemon.original_trainer_public_id, 0x1A2B)
            self.assertEqual(pokemon.original_trainer_secret_id, 0x1234)

            pokemon.original_trainer_secret_id = 0x3C4D
            self.assertEqual(pokemon.original_trainer_id, 0x3C4D1A2B)
            self.assertEqual(pokemon.original_trainer_public_id, 0x1A2B)
            self.assertEqual(pokemon.original_trainer_secret_id, 0x3C4D)

            # Make sure SWIG+Python catches values outside the uint(16|32)_t
            # ranges. What type of error is thrown depends on SWIG version.

            try:
                with self.assertRaises(OverflowError):
                    pokemon.original_trainer_id = -1
            except:
                with self.assertRaises(TypeError):
                    pokemon.original_trainer_id = -1

            try:
                with self.assertRaises(OverflowError):
                    pokemon.original_trainer_id = 0xFFFFFFFF+1
            except:
                with self.assertRaises(TypeError):
                    pokemon.original_trainer_id = 0xFFFFFFFF+1

            try:
                with self.assertRaises(OverflowError):
                    pokemon.original_trainer_secret_id = -1
            except:
                with self.assertRaises(TypeError):
                    pokemon.original_trainer_secret_id = -1

            try:
                with self.assertRaises(OverflowError):
                    pokemon.original_trainer_secret_id = 0xFFFF+1
            except:
                with self.assertRaises(TypeError):
                    pokemon.original_trainer_secret_id = 0xFFFF+1
        else:
            pokemon.original_trainer_id = 0xABCD
            self.assertEqual(pokemon.original_trainer_id, 0xABCD)
            self.assertEqual(pokemon.original_trainer_public_id, 0xABCD)

            pokemon.original_trainer_public_id = 0x9876
            self.assertEqual(pokemon.original_trainer_id, 0x9876)
            self.assertEqual(pokemon.original_trainer_public_id, 0x9876)

            with self.assertRaises(IndexError):
                pokemon.original_trainer_id = 0xFFFF+1
            with self.assertRaises(RuntimeError):
                pokemon.original_trainer_secret_id = 0

        # Make sure SWIG+Python catches values outside the uint16_t
        # ranges. What type of error is thrown depends on SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pokemon.original_trainer_public_id = -1
        except:
            with self.assertRaises(TypeError):
                pokemon.original_trainer_public_id = -1

        try:
            with self.assertRaises(OverflowError):
                pokemon.original_trainer_public_id = 0xFFFF+1
        except:
            with self.assertRaises(TypeError):
                pokemon.original_trainer_public_id = 0xFFFF+1

    def common_test(self, pokemon, test_params):
        self.initial_values_test(pokemon)
        self.initial_maps_test(pokemon)
        self.ability_test(pokemon)
        self.ball_test(
            pokemon,
            test_params.valid_ball,
            test_params.invalid_balls
        )
        self.condition_test(pokemon)
        self.image_filepaths_test(pokemon)
        self.item_test(
            pokemon,
            test_params.valid_item,
            test_params.invalid_items
        )
        self.level_test(pokemon)
        self.move_test(
            pokemon,
            test_params.valid_moves,
            test_params.invalid_moves
        )
        self.original_game_test(
            pokemon,
            test_params.valid_original_games,
            test_params.invalid_original_games
        )
        self.personality_test(pokemon)
        self.pokerus_test(pokemon)
        self.stats_test(pokemon)
        self.trainer_info_test(pokemon)
