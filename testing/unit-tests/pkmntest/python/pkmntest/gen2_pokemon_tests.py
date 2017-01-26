#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .pokemon_tests import pokemon_tests

import string
import unittest

class gen2_pokemon_test(pokemon_tests):

    def unown_form_test(self, game):
        for letter in string.ascii_uppercase:
            unown = pkmn.pokemon("Unown", game, letter, 5)
            self.assertStringEqual(unown.get_form(), letter)

            # Make sure IVs are properly set.
            IVs = unown.get_IVs()
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                IVs["Attack"],
                                IVs["Defense"],
                                IVs["Speed"],
                                IVs["Special"]
                            )
            self.assertStringEqual(unown.get_form(), form_from_IVs)

        # Make sure setting IVs properly changes the form.
        unown = pkmn.pokemon("Unown", game, "A", 5)
        unown.set_IV("Attack", 10)
        unown.set_IV("Defense", 9)
        unown.set_IV("Speed", 1)
        unown.set_IV("Special", 14)
        self.assertStringEqual(unown.get_form(), "G")

        # Make sure setting the form properly changes IVs.
        for letter in string.ascii_uppercase:
            unown.set_form(letter)
            self.assertStringEqual(unown.get_form(), letter)

            # Make sure IVs are properly set.
            IVs = unown.get_IVs()
            form_from_IVs = pkmn.calculations.gen2_unown_form(
                                IVs["Attack"],
                                IVs["Defense"],
                                IVs["Speed"],
                                IVs["Special"]
                            )
            self.assertStringEqual(unown.get_form(), form_from_IVs)

    def check_stat_map(self, stat_map, is_stats):
        self.assertTrue(stat_map.has_key("HP"))
        self.assertTrue(stat_map.has_key("Attack"))
        self.assertTrue(stat_map.has_key("Defense"))
        self.assertTrue(stat_map.has_key("Speed"))
        if is_stats:
            self.assertFalse(stat_map.has_key("Special"))
            self.assertTrue(stat_map.has_key("Special Attack"))
            self.assertTrue(stat_map.has_key("Special Defense"))
        else:
            self.assertTrue(stat_map.has_key("Special"))
            self.assertFalse(stat_map.has_key("Special Attack"))
            self.assertFalse(stat_map.has_key("Special Defense"))

    def gen2_pokemon_test(self, pokemon, species, game):
        #
        # Check starting values, and confirm that we can't query values
        # that didn't exist in Generation II.
        #
        self.assertStringEqual(pokemon.get_species(), species)
        self.assertStringEqual(pokemon.get_form(), "Standard")
        self.assertStringEqual(pokemon.get_game(), game)
        self.assertStringEqual(pokemon.get_nickname(), species.upper())
        self.assertStringEqual(pokemon.get_held_item().get_name(), "None")
        self.assertStringEqual(pokemon.get_trainer_name(), pkmn.LIBPKMN_OT_NAME)
        self.assertEqual(pokemon.get_trainer_public_id(), (pkmn.LIBPKMN_OT_ID & 0xFFFF))

        with self.assertRaises(RuntimeError):
            pokemon.get_trainer_secret_id()

        self.assertEqual(pokemon.get_trainer_id(), (pkmn.LIBPKMN_OT_ID & 0xFFFF))
        self.assertStringEqual(pokemon.get_trainer_gender(), "Male")
        self.assertEqual(pokemon.get_friendship(), pokemon.get_database_entry().get_base_friendship())

        with self.assertRaises(RuntimeError):
            pokemon.get_ability()
        with self.assertRaises(RuntimeError):
            pokemon.get_ball()

        self.assertEqual(pokemon.get_level_met(), pokemon.get_level())

        with self.assertRaises(RuntimeError):
            pokemon.get_location_met(True)

        self.assertStringEqual(pokemon.get_location_met(False), "Special")

        with self.assertRaises(RuntimeError):
            pokemon.get_original_game()
        with self.assertRaises(RuntimeError):
            pokemon.get_personality()

        self.assertEqual(
            pokemon.get_experience(),
            pokemon.get_database_entry().get_experience_at_level(30)
        )
        self.assertEqual(pokemon.get_level(), 30)

        with self.assertRaises(RuntimeError):
            pokemon.get_markings()
        with self.assertRaises(RuntimeError):
            pokemon.get_ribbons()
        with self.assertRaises(RuntimeError):
            pokemon.get_contest_stats()

        for move_slot in pokemon.get_moves():
            self.assertStringEqual(move_slot.move.get_name(), "None")
            self.assertEqual(move_slot.pp, 0)

        self.check_stat_map(pokemon.get_EVs(), False)
        self.check_stat_map(pokemon.get_IVs(), False)
        self.check_stat_map(pokemon.get_stats(), True)

        #
        # Make sure the getters and setters agree. Also make sure it fails when
        # expected.
        #

        with self.assertRaises(ValueError):
            pokemon.set_nickname("")
        with self.assertRaises(ValueError):
            pokemon.set_nickname("Too long nickname")

        pokemon.set_nickname("foobarbaz")
        self.assertStringEqual(pokemon.get_nickname(), "foobarbaz")

        # Shininess affects IVs, so make sure the abstraction reflects that.
        pokemon.set_shininess(False)
        self.assertFalse(pokemon.is_shiny())
        self.assertEqual(pokemon.get_IVs()["Attack"], 13)

        pokemon.set_shininess(True)
        self.assertTrue(pokemon.is_shiny())

        IVs = pokemon.get_IVs()
        self.assertEqual(IVs["Attack"], 15)
        self.assertEqual(IVs["Defense"], 10)
        self.assertEqual(IVs["Speed"], 10)
        self.assertEqual(IVs["Special"], 10)

        with self.assertRaises(ValueError):
            pokemon.set_held_item("Not an item")
        self.assertStringEqual(pokemon.get_held_item().get_name(), "None")

        # Valid item, not holdable
        with self.assertRaises(ValueError):
            pokemon.set_held_item("Bicycle")
        self.assertStringEqual(pokemon.get_held_item().get_name(), "None")

        pokemon.set_held_item("Berry")
        self.assertStringEqual(pokemon.get_held_item().get_name(), "Berry")

        with self.assertRaises(ValueError):
            pokemon.set_trainer_name("")
        with self.assertRaises(ValueError):
            pokemon.set_trainer_name("Too long trainer name")

        pokemon.set_trainer_name("foobar")
        self.assertStringEqual(pokemon.get_trainer_name(), "foobar")

        pokemon.set_trainer_id(10001)
        self.assertEqual(pokemon.get_trainer_id(), 10001)
        self.assertEqual(pokemon.get_trainer_public_id(), 10001)

        with self.assertRaises(RuntimeError):
            pokemon.set_trainer_secret_id(54321)
        self.assertEqual(pokemon.get_trainer_id(), 10001)

        with self.assertRaises(RuntimeError):
            pokemon.set_ability("")
        with self.assertRaises(RuntimeError):
            pokemon.set_ball("Great Ball")
        with self.assertRaises(RuntimeError):
            pokemon.set_location_met("Pallet Town", True)

        pokemon.set_location_met("Pallet Town", False)
        self.assertStringEqual(pokemon.get_location_met(False), "Pallet Town")

        pokemon.set_trainer_gender("Female")
        self.assertStringEqual(pokemon.get_trainer_gender(), "Female")
        with self.assertRaises(ValueError):
            pokemon.set_trainer_gender("Genderless")

        pokemon.set_level_met(3)
        self.assertEqual(pokemon.get_level_met(), 3)

        with self.assertRaises(IndexError):
            pokemon.set_friendship(-1)
        with self.assertRaises(IndexError):
            pokemon.set_friendship(256)

        pokemon.set_friendship(123)
        self.assertEqual(pokemon.get_friendship(), 123)

        with self.assertRaises(RuntimeError):
            pokemon.set_original_game("Gold")
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
            pokemon.set_move("Bounce", 0)

        self.assertStringEqual(pokemon.get_moves()[0].move.get_name(), "None")

        move_names = ["Ember", "Flamethrower", "Slash", "Fire Blast"]
        for i, name in enumerate(move_names):
            pokemon.set_move(name, i)

        moves = pokemon.get_moves()
        for i, name in enumerate(move_names):
            self.assertStringEqual(moves[i].move.get_name(), name)
            self.assertEqual(moves[i].pp, moves[i].move.get_pp(0))

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
    # Gold
    #

    def test_gold_invalid_pokemon(self):
        self.invalid_pokemon_test("Gold")

    def test_gold_unown_forms(self):
        self.unown_form_test("Gold")

    def test_gold_pokemon(self):
        self.pokemon_test_common(
            "Cyndaquil",
            "Gold",
            self.gen2_pokemon_test
        )

    #
    # Silver
    #

    def test_silver_invalid_pokemon(self):
        self.invalid_pokemon_test("Silver")

    def test_silver_unown_forms(self):
        self.unown_form_test("Silver")

    def test_silver_pokemon(self):
        self.pokemon_test_common(
            "Cyndaquil",
            "Silver",
            self.gen2_pokemon_test
        )

    #
    # Crystal
    #

    def test_crystal_invalid_pokemon(self):
        self.invalid_pokemon_test("Crystal")

    def test_crystal_unown_forms(self):
        self.unown_form_test("Crystal")

    def test_crystal_pokemon(self):
        self.pokemon_test_common(
            "Cyndaquil",
            "Crystal",
            self.gen2_pokemon_test
        )
