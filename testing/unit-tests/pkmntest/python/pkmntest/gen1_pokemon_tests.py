#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .pokemon_tests import pokemon_tests

import os
import unittest

class gen1_pokemon_test(pokemon_tests):

    def gen1_friendship_test(self, game):
        pikachu = pkmn.pokemon("Pikachu", game, "", 5)

        if game == "Yellow":
            pikachu.set_friendship(123)
            self.assertEqual(pikachu.get_friendship(), 123)

            # Also check a non-Pikachu.
            mewtwo = pkmn.pokemon("Mewtwo", game, "", 70)
            with self.assertRaises(RuntimeError):
                mewtwo.set_friendship(123)
            with self.assertRaises(RuntimeError):
                mewtwo.get_friendship()
        else:
            with self.assertRaises(RuntimeError):
                pikachu.set_friendship(123)
            with self.assertRaises(RuntimeError):
                pikachu.get_friendship()

    def gen1_check_stat_map(self, stat_map):
        self.assertTrue(stat_map.has_key("HP"))
        self.assertTrue(stat_map.has_key("Attack"))
        self.assertTrue(stat_map.has_key("Defense"))
        self.assertTrue(stat_map.has_key("Speed"))
        self.assertTrue(stat_map.has_key("Special"))
        self.assertFalse(stat_map.has_key("Special Attack"))
        self.assertFalse(stat_map.has_key("Special Defense"))

    def gen1_pokemon_test(self, pokemon, species, game):
        #
        # Check starting values, and confirm that we can't query values
        # that didn't exist in Generation I.
        #
        self.assertStringEqual(pokemon.get_species(), species)
        self.assertStringEqual(pokemon.get_form(), "Standard")
        self.assertStringEqual(pokemon.get_game(), game)
        self.assertStringEqual(pokemon.get_nickname(), species.upper())

        with self.assertRaises(RuntimeError):
            pokemon.get_gender()
        with self.assertRaises(RuntimeError):
            pokemon.is_shiny()
        with self.assertRaises(RuntimeError):
            pokemon.get_held_item()

        self.assertStringEqual(pokemon.get_original_trainer_name(), pkmn.DEFAULT_TRAINER_NAME)
        self.assertEqual(pokemon.get_original_trainer_public_id(), (pkmn.DEFAULT_TRAINER_ID & 0xFFFF))

        with self.assertRaises(RuntimeError):
            pokemon.get_original_trainer_secret_id()

        self.assertEqual(pokemon.get_original_trainer_id(), (pkmn.DEFAULT_TRAINER_ID & 0xFFFF))
        self.assertStringEqual(pokemon.get_original_trainer_gender(), "Male")

        with self.assertRaises(RuntimeError):
            pokemon.get_friendship()
        with self.assertRaises(RuntimeError):
            pokemon.get_ability()
        with self.assertRaises(RuntimeError):
            pokemon.get_ball()
        with self.assertRaises(RuntimeError):
            pokemon.get_level_met()
        with self.assertRaises(RuntimeError):
            pokemon.get_location_met(True)
        with self.assertRaises(RuntimeError):
            pokemon.get_location_met(False)
        with self.assertRaises(RuntimeError):
            pokemon.get_original_game()
        with self.assertRaises(RuntimeError):
            pokemon.get_personality()

        self.assertEqual(pokemon.get_experience(), pokemon.get_database_entry().get_experience_at_level(30))
        self.assertEqual(pokemon.get_level(), 30)

        with self.assertRaises(RuntimeError):
            pokemon.get_markings()
        with self.assertRaises(RuntimeError):
            pokemon.get_ribbons()
        with self.assertRaises(RuntimeError):
            pokemon.get_contest_stats()

        move_slots = pokemon.get_moves()
        self.assertEqual(len(move_slots), 4)
        for move_slot in move_slots:
            self.assertEqual(move_slot.move, "None")
            self.assertEqual(move_slot.pp, 0)

        self.gen1_check_stat_map(pokemon.get_EVs())
        self.gen1_check_stat_map(pokemon.get_IVs())
        self.gen1_check_stat_map(pokemon.get_stats())

        self.assertTrue(os.path.exists(pokemon.get_icon_filepath()))
        self.assertTrue(os.path.exists(pokemon.get_sprite_filepath()))

        #
        # Make sure the getters and setters agree. Also make sure it fails when
        # expected.
        #

        with self.assertRaises(ValueError):
            pokemon.set_nickname("")
        with self.assertRaises(ValueError):
            pokemon.set_nickname("Too long nickname")

        pokemon.set_nickname("foobarbaz")
        self.assertEqual(pokemon.get_nickname(), "foobarbaz")

        with self.assertRaises(RuntimeError):
            pokemon.set_gender("Male")
        with self.assertRaises(RuntimeError):
            pokemon.set_shininess(True)
        with self.assertRaises(RuntimeError):
            pokemon.set_shininess(False)
        with self.assertRaises(RuntimeError):
            pokemon.set_held_item("Potion")

        with self.assertRaises(ValueError):
            pokemon.set_original_trainer_name("")
        with self.assertRaises(ValueError):
            pokemon.set_original_trainer_name("Too long trainer name")

        pokemon.set_original_trainer_name("foobar")
        self.assertEqual(pokemon.get_original_trainer_name(), "foobar")

        pokemon.set_original_trainer_id(10001)
        self.assertEqual(pokemon.get_original_trainer_id(), 10001)
        self.assertEqual(pokemon.get_original_trainer_public_id(), 10001)
        with self.assertRaises(RuntimeError):
            pokemon.get_original_trainer_secret_id()

        with self.assertRaises(RuntimeError):
            pokemon.set_original_trainer_secret_id(54321)
        self.assertEqual(pokemon.get_original_trainer_id(), 10001)

        # Make sure the SWIG wrapper keeps it within the proper bounds. Which error
        # applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_original_trainer_id(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_original_trainer_id(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_original_trainer_public_id(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_original_trainer_public_id(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_original_trainer_public_id(0xFFFF+1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_original_trainer_public_id(0xFFFF+1)

        with self.assertRaises(RuntimeError):
            pokemon.set_original_trainer_gender("Male")
        with self.assertRaises(RuntimeError):
            pokemon.set_friendship(123)
        with self.assertRaises(RuntimeError):
            pokemon.set_ability("")
        with self.assertRaises(RuntimeError):
            pokemon.set_ball("Great Ball")
        with self.assertRaises(RuntimeError):
            pokemon.set_level_met(78)
        with self.assertRaises(RuntimeError):
            pokemon.set_location_met("Victory Road", True)
        with self.assertRaises(RuntimeError):
            pokemon.set_location_met("Victory Road", False)
        with self.assertRaises(RuntimeError):
            pokemon.set_original_game("Blue")
        with self.assertRaises(RuntimeError):
            pokemon.set_marking("Circle", True)
        with self.assertRaises(RuntimeError):
            pokemon.set_ribbon("Cool", False)

        pokemon.set_experience(12345)
        self.assertEqual(pokemon.get_experience(), 12345)

        pokemon.set_level(45)
        self.assertEqual(pokemon.get_level(), 45)

        with self.assertRaises(IndexError):
            pokemon.set_move("Ember", -1)
        with self.assertRaises(ValueError):
            pokemon.set_move("Synthesis", 0)

        self.assertEqual(pokemon.get_moves()[0].move, "None")

        move_names = ["Ember", "Flamethrower", "Slash", "Fire Blast"]
        for i, move_name in enumerate(move_names):
            pokemon.set_move(move_name, i)

        for i, move in enumerate(pokemon.get_moves()):
            self.assertEqual(move.move, move_names[i])
            self.assertEqual(
                move.pp,
                pkmn.database.move_entry(move.move, game).get_pp(0)
            )

        with self.assertRaises(ValueError):
            pokemon.set_EV("Not a stat", 1)
        with self.assertRaises(IndexError):
            pokemon.set_EV("Attack", -1)
        with self.assertRaises(IndexError):
            pokemon.set_EV("Attack", 65536)

        pokemon.set_EV("Attack", 12345)
        self.assertEqual(pokemon.get_EVs()["Attack"], 12345)

        with self.assertRaises(ValueError):
            pokemon.set_IV("Not a stat", 1)
        with self.assertRaises(IndexError):
            pokemon.set_IV("Attack", -1)
        with self.assertRaises(IndexError):
            pokemon.set_IV("Attack", 16)

        pokemon.set_IV("Attack", 12)
        self.assertEqual(pokemon.get_IVs()["Attack"], 12)

    #
    # Red
    #

    def test_red_invalid_pokemon(self):
        self.invalid_pokemon_test("Red")

    def test_red_friendship(self):
        self.gen1_friendship_test("Red")

    def test_red_pokemon(self):
        self.pokemon_test_common(
            "Charmander",
            "Red",
            self.gen1_pokemon_test
        )

    #
    # Blue
    #

    def test_blue_invalid_pokemon(self):
        self.invalid_pokemon_test("Blue")

    def test_blue_friendship(self):
        self.gen1_friendship_test("Blue")

    def test_blue_pokemon(self):
        self.pokemon_test_common(
            "Charmander",
            "Blue",
            self.gen1_pokemon_test
        )

    #
    # Yellow
    #

    def test_yellow_invalid_pokemon(self):
        self.invalid_pokemon_test("Yellow")

    def test_yellow_friendship(self):
        self.gen1_friendship_test("Yellow")

    def test_yellow_pokemon(self):
        self.pokemon_test_common(
            "Charmander",
            "Yellow",
            self.gen1_pokemon_test
        )
