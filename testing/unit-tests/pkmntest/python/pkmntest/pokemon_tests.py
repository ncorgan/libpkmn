#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import string
import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

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

STARTERS = [
    "", "Bulbasaur", "Chikorita", "Treecko", "Turtwig", "Snivy", "Chespin"
]

GEN1_POKEMON_WITH_XY_MEGA_FORMS = [
    "Venusaur", "Blastoise", "Alakazam", "Gengar",
    "Kangaskhan", "Pinsir", "Gyarados", "Aerodactyl"
]

GEN1_POKEMON_WITH_ORAS_MEGA_FORMS = [
    "Beedrill", "Pidgeot", "Slowbro"
]

GEN2_POKEMON_WITH_XY_MEGA_FORMS = [
    "Ampharos", "Scizor", "Heracross", "Houndoom", "Tyranitar"
]

GEN3_POKEMON_WITH_XY_MEGA_FORMS = [
    "Blaziken", "Gardevoir", "Mawile", "Aggron", "Medicham",
    "Manectric", "Banette", "Absol", "Latias", "Latios", "Garchomp"
]

GEN3_POKEMON_WITH_ORAS_MEGA_FORMS = [
    "Sceptile", "Swampert", "Sableye", "Sharpedo", "Camerupt",
    "Altaria", "Glalie", "Salamence", "Metagross", "Rayquaza"
]

class pokemon_tests(unittest.TestCase):

    def assertStringEqual(self, str1, str2):
        if PYTHON_MAJOR_VERSION == 2:
            if "str" in str(type(str1)):
                str1 = str1.decode("utf-8")
            if "str" in str(type(str2)):
                str2 = str2.decode("utf-8")

        self.assertEqual(str1, str2)

    def __get_both_string_types(self, input_str):
        if PYTHON_MAJOR_VERSION == 2:
            # Which call succeeds depends on SWIG version
            try:
                return (input_str, input_str.decode("utf-8"))
            except:
                return (input_str, unicode(input_str))
        else:
            return (input_str, input_str)

    def gen1_forms_test(self, game):
        generation = GAME_TO_GENERATION[game]

        # Check that Mega forms only work in their given games.
        for species in GEN1_POKEMON_WITH_XY_MEGA_FORMS:
            if generation >= 6:
                pkmn.pokemon(species, game, "Mega", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega", 100)

        for species in GEN1_POKEMON_WITH_ORAS_MEGA_FORMS:
            if game in ["Omega Ruby", "Alpha Sapphire"]:
                pkmn.pokemon(species, game, "Mega", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega", 100)

        # Cosplay Pikachu should only work in OR/AS.
        for form in pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "").get_forms()[1:]:
            if game in ["Omega Ruby", "Alpha Sapphire"]:
                pkmn.pokemon("Pikachu", game, form, 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon("Pikachu", game, form, 100)

        # Hardcode Mega X/Y cases.
        for species in ["Charizard", "Mewtwo"]:
            if generation >= 6:
                pkmn.pokemon(species, game, "Mega X", 100)
                pkmn.pokemon(species, game, "Mega Y", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega X", 100)
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega Y", 100)

    def gen2_forms_test(self, game):
        generation = GAME_TO_GENERATION[game]

        # Check that Mega forms only work in their given games.
        for species in GEN2_POKEMON_WITH_XY_MEGA_FORMS:
            if generation >= 6:
                pkmn.pokemon(species, game, "Mega", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega", 100)

        if game in ["Omega Ruby", "Alpha Sapphire"]:
            pkmn.pokemon("Steelix", game, "Mega", 100)
        else:
            with self.assertRaises(ValueError):
                pkmn.pokemon("Steelix", game, "Mega", 100)

        # Spiky-eared Pichu should only work in HG/SS.
        if game in ["HeartGold", "SoulSilver"]:
            pkmn.pokemon("Pichu", game, "Spiky-eared", 100)
        else:
            with self.assertRaises(ValueError):
                pkmn.pokemon("Pichu", game, "Spiky-eared", 100)

        # Unown's "!" and "?" forms aren't in Generation II.
        for letter in string.ascii_uppercase:
            unown = pkmn.pokemon("Unown", game, letter, 10)
            self.assertTrue(os.path.exists(unown.get_icon_filepath()))
            self.assertTrue(os.path.exists(unown.get_sprite_filepath()))

        if generation > 2:
            pkmn.pokemon("Unown", game, "!", 10)
            pkmn.pokemon("Unown", game, "?", 10)
        else:
            with self.assertRaises(ValueError):
                pkmn.pokemon("Unown", game, "!", 10)
            with self.assertRaises(ValueError):
                pkmn.pokemon("Unown", game, "?", 10)

    def gen3_forms_test(self, game):
        # TODO: Shadow forms
        gamecube = (game in ["Colosseum", "XD"])
        generation = GAME_TO_GENERATION[game]

        # Check that Mega forms only work in their given games.
        for species in GEN3_POKEMON_WITH_XY_MEGA_FORMS:
            if generation >= 6:
                pkmn.pokemon(species, game, "Mega", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega", 100)

        for species in GEN3_POKEMON_WITH_ORAS_MEGA_FORMS:
            if game in ["Omega Ruby", "Alpha Sapphire"]:
                pkmn.pokemon(species, game, "Mega", 100)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Mega", 100)

        # Castform should always work.
        for form in pkmn.database.pokemon_entry("Castform", "Omega Ruby", "").get_forms():
            castform = pkmn.pokemon("Castform", game, form, 30)
            self.assertTrue(os.path.exists(castform.get_icon_filepath()))
            self.assertTrue(os.path.exists(castform.get_sprite_filepath()))

        # Primal Reversion should only work in OR/AS.
        for species in ["Groudon", "Kyogre"]:
            pokemon = pkmn.pokemon(species, game, "", 70)
            self.assertTrue(os.path.exists(pokemon.get_icon_filepath()))
            self.assertTrue(os.path.exists(pokemon.get_sprite_filepath()))
            if game in ["Omega Ruby", "Alpha Sapphire"]:
                pkmn.pokemon(species, game, "Primal Reversion", 70)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Primal Reversion", 70)

        # In Generation III, Deoxys's form is game-specific.
        if generation == 3:
            if game in ["Ruby", "Sapphire", "Colosseum", "XD"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Normal", 70)
                self.assertTrue(os.path.exists(deoxys.get_icon_filepath()))
                self.assertTrue(os.path.exists(deoxys.get_sprite_filepath()))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Normal", 70)

            if game in ["FireRed"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)
                self.assertTrue(os.path.exists(deoxys.get_icon_filepath()))
                self.assertTrue(os.path.exists(deoxys.get_sprite_filepath()))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)

            if game in ["LeafGreen"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)
                self.assertTrue(os.path.exists(deoxys.get_icon_filepath()))
                self.assertTrue(os.path.exists(deoxys.get_sprite_filepath()))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)

            if game in ["Emerald"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
                self.assertTrue(os.path.exists(deoxys.get_icon_filepath()))
                self.assertTrue(os.path.exists(deoxys.get_sprite_filepath()))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
        else:
            for form in pkmn.database.pokemon_entry("Deoxys", "Omega Ruby", "").get_forms():
                deoxys = pkmn.pokemon("Deoxys", game, form, 70)
                self.assertTrue(os.path.exists(deoxys.get_icon_filepath()))
                self.assertTrue(os.path.exists(deoxys.get_sprite_filepath()))

    def forms_test(self, game):
        generation = GAME_TO_GENERATION[game]

        if generation >= 1:
            self.gen1_forms_test(game)
        if generation >= 2:
            self.gen2_forms_test(game)
        if generation >= 3:
            self.gen3_forms_test(game)

    def invalid_starters_test(self, game):
        generation = GAME_TO_GENERATION[game]

        for i in range(generation+1, 6):
            with self.assertRaises(ValueError):
                pkmn.pokemon(STARTERS[i], game, "", 5)

    def invalid_pokemon_test(self, game):
        self.forms_test(game)
        self.invalid_starters_test(game)

    def pokemon_test_common(self, species, game, test_fcn):
        test_fcn(
            pkmn.pokemon(species, game, "", 30),
            species,
            game
        )

    def gender_test(self, game):
        # Single-gender
        nidorina = pkmn.pokemon("Nidorina", game, "", 50)
        self.assertEqual(nidorina.get_gender(), "Female")
        nidorina.set_gender("Female")
        with self.assertRaises(ValueError):
            nidorina.set_gender("Male")
        with self.assertRaises(ValueError):
            nidorina.set_gender("Genderless")

        nidorino = pkmn.pokemon("Nidorino", game, "", 50)
        self.assertEqual(nidorino.get_gender(), "Male")
        nidorino.set_gender("Male")
        with self.assertRaises(ValueError):
            nidorino.set_gender("Female")
        with self.assertRaises(ValueError):
            nidorino.set_gender("Genderless")

        magnemite = pkmn.pokemon("Magnemite", game, "", 50)
        self.assertEqual(magnemite.get_gender(), "Genderless")
        magnemite.set_gender("Genderless")
        with self.assertRaises(ValueError):
            magnemite.set_gender("Male")
        with self.assertRaises(ValueError):
            magnemite.set_gender("Female")

        mixed_pokemon = [
            "Charmander", # 87.5% male
            "Growlithe",  # 75% male
            "Pidgey",     # 50% male
            "Vulpix"      # 25% male
        ]

        for species in mixed_pokemon:
            pokemon = pkmn.pokemon(species, game, "", 50)
            pokemon.set_gender("Female")
            self.assertEqual(pokemon.get_gender(), "Female")
            pokemon.set_gender("Male")
            self.assertEqual(pokemon.get_gender(), "Male")
            with self.assertRaises(ValueError):
                pokemon.set_gender("Genderless")
