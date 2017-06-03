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
import random
import unittest

MARKINGS = ["Circle", "Triangle", "Square", "Heart"]
CONTEST_TYPES = ["Cool", "Beauty", "Cute", "Smart", "Tough"]
CONTEST_LEVELS = ["", " Super", " Hyper", " Master"]
RIBBONS = ["Champion", "Winning", "Victory", "Artist",
           "Effort", "Marine", "Land", "Sky",
           "Country", "National", "Earth", "World"]
STATS = ["HP", "Attack", "Defense", "Speed", "Special Attack", "Special Defense"]

class gen3_pokemon_test(pokemon_tests):

    def unown_form_test(self, game):
        unown_entry = pkmn.database.pokemon_entry("Unown", "Omega Ruby", "")
        for form in unown_entry.get_forms():
            unown = pkmn.pokemon("Unown", game, form, 5)
            self.assertStringEqual(unown.get_form(), form)

        # Make sure setting the personality properly sets the form.
        unown = pkmn.pokemon("Unown", game, "A", 5)
        unown.set_personality(0x4C07DE71);
        self.assertStringEqual(unown.get_form(), "B")

        for form in unown_entry.get_forms():
            unown.set_form(form)
            self.assertStringEqual(unown.get_form(), form)
            form_from_personality = pkmn.calculations.gen3_unown_form(
                                        unown.get_personality()
                                    )
            self.assertStringEqual(form_from_personality, form)

    def check_markings_map(self, markings_map):
        self.assertEqual(len(markings_map), 4)

        for key in MARKINGS:
            self.assertTrue(markings_map.has_key(key))
            self.assertFalse(markings_map[key])

        self.assertFalse(markings_map.has_key("Star"))
        self.assertFalse(markings_map.has_key("Diamond"))

    def check_ribbons_map(self, ribbons_map):
        self.assertEqual(len(ribbons_map), 32)

        # Check contest ribbons.
        for contest_type in CONTEST_TYPES:
            for contest_level in CONTEST_LEVELS:
                ribbon_name = "{0}{1}".format(contest_type, contest_level)
                self.assertTrue(ribbons_map.has_key(ribbon_name))
                self.assertFalse(ribbons_map[ribbon_name])

        # Check other ribbons.
        for ribbon in RIBBONS:
            self.assertTrue(ribbons_map.has_key(ribbon))
            self.assertFalse(ribbons_map[ribbon])

    def check_contest_stats_map(self, contest_stats_map):
        self.assertEqual(len(contest_stats_map), 6)

        for stat in CONTEST_TYPES+["Feel"]:
            self.assertTrue(contest_stats_map.has_key(stat))
            self.assertEqual(contest_stats_map[stat], 0)

        self.assertFalse(contest_stats_map.has_key("Sheen"))

    def check_stats_map(self, stats_map):
        self.assertEqual(len(stats_map), 6)

        for stat in STATS:
            self.assertTrue(stats_map.has_key(stat))

        self.assertFalse(stats_map.has_key("Special"))

    def markings_test(self, pokemon):
        for key in MARKINGS:
            self.assertFalse(pokemon.get_markings()[key])
            pokemon.set_marking(key, True)
            self.assertTrue(pokemon.get_markings()[key])

    def ribbons_test(self, pokemon):
        # Check contest ribbons.
        for contest_type in CONTEST_TYPES:
            for contest_level in CONTEST_LEVELS:
                ribbon_name = "{0}{1}".format(contest_type, contest_level)
                self.assertFalse(pokemon.get_ribbons()[ribbon_name])
                pokemon.set_ribbon(ribbon_name, True)
                self.assertTrue(pokemon.get_ribbons()[ribbon_name])

        # Check other ribbons.
        for ribbon in RIBBONS:
            self.assertFalse(pokemon.get_ribbons()[ribbon])
            pokemon.set_ribbon(ribbon, True)
            self.assertTrue(pokemon.get_ribbons()[ribbon])

    def contest_stats_test(self, pokemon):
        for contest_type in CONTEST_TYPES:
            self.assertEqual(pokemon.get_contest_stats()[contest_type], 0)
            val = random.randint(1,255)
            pokemon.set_contest_stat(contest_type, val)
            self.assertEqual(pokemon.get_contest_stats()[contest_type], val)

    def EVs_test(self, pokemon):
        with self.assertRaises(ValueError):
            pokemon.set_EV("Not a stat", 0)
        with self.assertRaises(IndexError):
            pokemon.set_EV("HP", -1)
        with self.assertRaises(IndexError):
            pokemon.set_EV("HP", 256)

        for stat in STATS:
            val = random.randint(1,255)
            pokemon.set_EV(stat, val)
            self.assertEqual(pokemon.get_EVs()[stat], val)

    def IVs_test(self, pokemon):
        with self.assertRaises(ValueError):
            pokemon.set_IV("Not a stat", 0)
        with self.assertRaises(IndexError):
            pokemon.set_IV("HP", -1)
        with self.assertRaises(IndexError):
            pokemon.set_IV("HP", 32)

        for stat in STATS:
            val = random.randint(1,31)
            pokemon.set_IV(stat, val)
            self.assertEqual(pokemon.get_IVs()[stat], val)

    def gen3_pokemon_test(self, pokemon, species, game):
        #
        # Check known starting values, and confirm that we can't query values
        # that didn't exist in Generation III.
        #
        self.assertStringEqual(pokemon.get_species(), species)
        self.assertStringEqual(pokemon.get_form(), "Standard")
        self.assertStringEqual(pokemon.get_game(), game)
        self.assertStringEqual(pokemon.get_nickname(), species.upper())
        self.assertStringEqual(pokemon.get_held_item().get_name(), "None")
        self.assertStringEqual(pokemon.get_trainer_name(), pkmn.LIBPKMN_OT_NAME)
        self.assertEqual(pokemon.get_trainer_public_id(), (pkmn.LIBPKMN_OT_ID & 0xFFFF))

        self.assertEqual(
            pokemon.get_trainer_secret_id(),
            ((pkmn.LIBPKMN_OT_ID & 0xFFFF0000) >> 16)
        )

        self.assertEqual(pokemon.get_trainer_id(), pkmn.LIBPKMN_OT_ID)
        self.assertStringEqual(pokemon.get_trainer_gender(), "Male")

        self.assertEqual(
            pokemon.get_friendship(),
            pokemon.get_database_entry().get_base_friendship()
        )

        abilities = pokemon.get_database_entry().get_abilities()
        if abilities[1] == "None":
            self.assertStringEqual(pokemon.get_ability(), abilities[0])
        else:
            self.assertStringEqual(pokemon.get_ability(), abilities[pokemon.get_personality() % 2])

        self.assertStringEqual(pokemon.get_ball(), "Premier Ball")
        self.assertEqual(pokemon.get_level_met(), pokemon.get_level())

        with self.assertRaises(RuntimeError):
            pokemon.get_location_met(True)

        if game in ["Colosseum", "XD"]:
            self.assertStringEqual(pokemon.get_location_met(False), "Distant land")
            self.assertStringEqual(pokemon.get_original_game(), "Colosseum/XD")
        else:
            self.assertStringEqual(pokemon.get_location_met(False), "Fateful encounter")
            self.assertStringEqual(pokemon.get_original_game(), pokemon.get_game())

        self.assertEqual(
            pokemon.get_experience(),
            pokemon.get_database_entry().get_experience_at_level(30)
        )
        self.assertEqual(pokemon.get_level(), 30)

        self.check_markings_map(pokemon.get_markings())
        self.check_ribbons_map(pokemon.get_ribbons())
        self.check_contest_stats_map(pokemon.get_contest_stats())

        for move_slot in pokemon.get_moves():
            self.assertStringEqual(move_slot.move.get_name(), "None")
            self.assertEqual(move_slot.pp, 0)

        self.check_stats_map(pokemon.get_EVs())
        self.check_stats_map(pokemon.get_IVs())
        self.check_stats_map(pokemon.get_stats())

        if game not in ["Colosseum", "XD"]:
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
        self.assertStringEqual(pokemon.get_nickname(), "foobarbaz")

        # Gender is tied to personality, so make sure they affect each other.
        pokemon.set_gender("Female")
        self.assertLess(pokemon.get_personality() & 0xFF, 0xFF)
        pokemon.set_gender("Male")
        self.assertEqual(pokemon.get_personality() & 0xFF, 0xFF)

        pokemon.set_personality(0x1234AB00)
        self.assertEqual(pokemon.get_gender(), "Female")
        pokemon.set_personality(0xCD5678FF)
        self.assertEqual(pokemon.get_gender(), "Male")

        # Setting shininess should affect personality. Also check filepaths.
        pokemon.set_shininess(False)
        self.assertFalse(pokemon.is_shiny())
        personality = pokemon.get_personality()

        if game not in ["Colosseum", "XD"]:
            self.assertTrue(os.path.exists(pokemon.get_sprite_filepath()))

            # This will fail if "shiny" is anywhere in the filepath.
            self.assertFalse("shiny" in pokemon.get_sprite_filepath())

        pokemon.set_shininess(True)
        self.assertTrue(pokemon.is_shiny())
        self.assertNotEqual(pokemon.get_personality(), personality)
        if game not in ["Colosseum", "XD"]:
            self.assertTrue(os.path.exists(pokemon.get_sprite_filepath()))
            self.assertTrue("shiny" in pokemon.get_sprite_filepath())

        with self.assertRaises(ValueError):
            pokemon.set_held_item("Not an item")

        # Not in this game
        with self.assertRaises(ValueError):
            pokemon.set_held_item("Berry")

        # Not holdable
        with self.assertRaises(ValueError):
            pokemon.set_held_item("Mach Bike")

        self.assertStringEqual(pokemon.get_held_item().get_name(), "None")

        pokemon.set_held_item("Razz Berry")
        self.assertStringEqual(pokemon.get_held_item().get_name(), "Razz Berry")

        with self.assertRaises(ValueError):
            pokemon.set_trainer_name("")
        with self.assertRaises(ValueError):
            pokemon.set_trainer_name("Too long trainer name")

        pokemon.set_trainer_name("foobar")
        self.assertStringEqual(pokemon.get_trainer_name(), "foobar")

        # Make sure the SWIG wrapper keeps it within the proper bounds. Which error
        # applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_id(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_id(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_id(0xFFFFFFFFF)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_id(0xFFFFFFFFF)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_public_id(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_public_id(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_public_id(0xFFFFF)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_public_id(0xFFFFF)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_secret_id(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_secret_id(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_trainer_secret_id(0xFFFFF)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_trainer_secret_id(0xFFFFF)
        self.assertEqual(pokemon.get_trainer_id(), pkmn.LIBPKMN_OT_ID)

        pokemon.set_trainer_id(0x1234ABCD)
        self.assertEqual(pokemon.get_trainer_id(), 0x1234ABCD)
        self.assertEqual(pokemon.get_trainer_public_id(), 0xABCD)
        self.assertEqual(pokemon.get_trainer_secret_id(), 0x1234)

        pokemon.set_trainer_public_id(0x1A2B)
        self.assertEqual(pokemon.get_trainer_id(), 0x12341A2B)
        self.assertEqual(pokemon.get_trainer_public_id(), 0x1A2B)
        self.assertEqual(pokemon.get_trainer_secret_id(), 0x1234)

        pokemon.set_trainer_secret_id(0x3C4D)
        self.assertEqual(pokemon.get_trainer_id(), 0x3C4D1A2B)
        self.assertEqual(pokemon.get_trainer_public_id(), 0x1A2B)
        self.assertEqual(pokemon.get_trainer_secret_id(), 0x3C4D)

        pokemon.set_trainer_gender("Male")
        self.assertStringEqual(pokemon.get_trainer_gender(), "Male")
        pokemon.set_trainer_gender("Female")
        self.assertStringEqual(pokemon.get_trainer_gender(), "Female")

        with self.assertRaises(ValueError):
            pokemon.set_trainer_gender("Genderless")

        pokemon.set_friendship(123)
        self.assertEqual(pokemon.get_friendship(), 123)

        with self.assertRaises(IndexError):
            pokemon.set_friendship(-1)
        with self.assertRaises(IndexError):
            pokemon.set_friendship(256)

        pokemon.set_ability(abilities[0])
        self.assertStringEqual(pokemon.get_ability(), abilities[0])

        with self.assertRaises(ValueError):
            pokemon.set_ability("None")

        # Invalid
        with self.assertRaises(ValueError):
            pokemon.set_ability("Torrent")

        # Hidden ability
        with self.assertRaises(ValueError):
            pokemon.set_ability(pkmn.database.pokemon_entry(pokemon.get_species(), "Omega Ruby", "").get_hidden_ability())

        self.assertStringEqual(pokemon.get_ability(), abilities[0])

        pokemon.set_ball("Great Ball")
        self.assertStringEqual(pokemon.get_ball(), "Great Ball")

        # Not in Generation III
        with self.assertRaises(ValueError):
            pokemon.set_ball("Friend Ball")
        self.assertStringEqual(pokemon.get_ball(), "Great Ball")

        pokemon.set_level_met(67)
        self.assertEqual(pokemon.get_level_met(), 67)

        with self.assertRaises(IndexError):
            pokemon.set_level_met(-1)
        with self.assertRaises(IndexError):
            pokemon.set_level_met(101)

        if game in ["FireRed", "LeafGreen"]:
            location = "Viridian Forest"
        elif game in ["Colosseum", "XD"]:
            location = "Phenac City"
        else:
            location = "Petalburg Woods"
        pokemon.set_location_met(location, False)
        self.assertStringEqual(pokemon.get_location_met(False), location)

        with self.assertRaises(RuntimeError):
            pokemon.set_location_met(location, True)

        pokemon.set_original_game("Ruby")
        self.assertStringEqual(pokemon.get_original_game(), "Ruby")
        with self.assertRaises(ValueError):
            pokemon.set_original_game("Not a game")
        with self.assertRaises(ValueError):
            pokemon.set_original_game("Red") # Impossible
        with self.assertRaises(ValueError):
            pokemon.set_original_game("HeartGold") # From a later game
        self.assertStringEqual(pokemon.get_original_game(), "Ruby")

        pokemon.set_personality(0x7F3AB3A8)
        self.assertEqual(pokemon.get_personality(), 0x7F3AB3A8)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_personality(-1)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_personality(-1)
        try:
            with self.assertRaises(OverflowError):
                pokemon.set_personality(0xFFFFFFFFF)
        except:
            with self.assertRaises(TypeError):
                pokemon.set_personality(0xFFFFFFFFF)

        with self.assertRaises(IndexError):
            pokemon.set_contest_stat("Cool", -1)
        with self.assertRaises(IndexError):
            pokemon.set_contest_stat("Cool", 256)
        with self.assertRaises(ValueError):
            pokemon.set_contest_stat("Not a stat", 1)

        self.markings_test(pokemon)
        self.ribbons_test(pokemon)
        self.contest_stats_test(pokemon)
        self.EVs_test(pokemon)
        self.IVs_test(pokemon)

    #
    # Ruby
    #

    def test_ruby_invalid_pokemon(self):
        self.invalid_pokemon_test("Ruby")

    def test_ruby_gender(self):
        self.gender_test("Ruby")

    def test_ruby_unown_forms(self):
        self.unown_form_test("Ruby")

    def test_ruby_pokemon(self):
        self.pokemon_test_common(
            "Torchic",
            "Ruby",
            self.gen3_pokemon_test
        )

    #
    # Sapphire
    #

    def test_sapphire_invalid_pokemon(self):
        self.invalid_pokemon_test("Sapphire")

    def test_sapphire_gender(self):
        self.gender_test("Sapphire")

    def test_sapphire_unown_forms(self):
        self.unown_form_test("Sapphire")

    def test_sapphire_pokemon(self):
        self.pokemon_test_common(
            "Torchic",
            "Sapphire",
            self.gen3_pokemon_test
        )

    #
    # Emerald
    #

    def test_emerald_invalid_pokemon(self):
        self.invalid_pokemon_test("Emerald")

    def test_emerald_gender(self):
        self.gender_test("Emerald")

    def test_emerald_unown_forms(self):
        self.unown_form_test("Emerald")

    def test_emerald_pokemon(self):
        self.pokemon_test_common(
            "Torchic",
            "Emerald",
            self.gen3_pokemon_test
        )

    #
    # FireRed
    #

    def test_firered_invalid_pokemon(self):
        self.invalid_pokemon_test("FireRed")

    def test_firered_gender(self):
        self.gender_test("FireRed")

    def test_firered_unown_forms(self):
        self.unown_form_test("FireRed")

    def test_firered_pokemon(self):
        self.pokemon_test_common(
            "Torchic",
            "FireRed",
            self.gen3_pokemon_test
        )

    #
    # LeafGreen
    #

    def test_leafgreen_invalid_pokemon(self):
        self.invalid_pokemon_test("LeafGreen")

    def test_leafgreen_gender(self):
        self.gender_test("LeafGreen")

    def test_leafgreen_unown_forms(self):
        self.unown_form_test("LeafGreen")

    def test_ruby_pokemon(self):
        self.pokemon_test_common(
            "Torchic",
            "LeafGreen",
            self.gen3_pokemon_test
        )

    #
    # Colosseum
    #

    def test_colosseum_invalid_pokemon(self):
        self.invalid_pokemon_test("Colosseum")

    def test_colosseum_gender(self):
        self.gender_test("Colosseum")

    def test_colosseum_unown_forms(self):
        self.unown_form_test("Colosseum")

    def test_colosseum_pokemon(self):
        self.pokemon_test_common(
            "Espeon",
            "Colosseum",
            self.gen3_pokemon_test
        )

    #
    # XD
    #

    def test_xd_invalid_pokemon(self):
        self.invalid_pokemon_test("XD")

    def test_xd_gender(self):
        self.gender_test("XD")

    def test_xd_unown_forms(self):
        self.unown_form_test("XD")

    def test_xd_pokemon(self):
        self.pokemon_test_common(
            "Umbreon",
            "XD",
            self.gen3_pokemon_test
        )
