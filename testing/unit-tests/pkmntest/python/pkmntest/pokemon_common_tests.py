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

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

PYTHON_MAJOR_VERSION = sys.version_info[0]

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

class pokemon_common_tests(base_test):

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
        for form in pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "").forms[1:]:
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
            if game not in ["Colosseum", "XD"]:
                self.assertTrue(os.path.exists(unown.icon_filepath))
                self.assertTrue(os.path.exists(unown.sprite_filepath))

        if generation > 2:
            pkmn.pokemon("Unown", game, "!", 10)
            pkmn.pokemon("Unown", game, "?", 10)
        else:
            with self.assertRaises(ValueError):
                pkmn.pokemon("Unown", game, "!", 10)
            with self.assertRaises(ValueError):
                pkmn.pokemon("Unown", game, "?", 10)

    def gen3_forms_test(self, game):
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
        for form in pkmn.database.pokemon_entry("Castform", "Omega Ruby", "").forms:
            castform = pkmn.pokemon("Castform", game, form, 30)
            if game not in ["Colosseum", "XD"]:
                self.assertTrue(os.path.exists(castform.icon_filepath))
                self.assertTrue(os.path.exists(castform.sprite_filepath))

        # Primal Reversion should only work in OR/AS.
        for species in ["Groudon", "Kyogre"]:
            pokemon = pkmn.pokemon(species, game, "", 70)
            if game not in ["Colosseum", "XD"]:
                self.assertTrue(os.path.exists(pokemon.icon_filepath))
                self.assertTrue(os.path.exists(pokemon.sprite_filepath))
            if game in ["Omega Ruby", "Alpha Sapphire"]:
                pkmn.pokemon(species, game, "Primal Reversion", 70)
            else:
                with self.assertRaises(ValueError):
                    pkmn.pokemon(species, game, "Primal Reversion", 70)

        # In Generation III, Deoxys's form is game-specific.
        if generation == 3:
            if game in ["Ruby", "Sapphire", "Colosseum", "XD"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Normal", 70)
                if game not in ["Colosseum", "XD"]:
                    self.assertTrue(os.path.exists(deoxys.icon_filepath))
                    self.assertTrue(os.path.exists(deoxys.sprite_filepath))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Normal", 70)

            if game in ["FireRed"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)
                self.assertTrue(os.path.exists(deoxys.icon_filepath))
                self.assertTrue(os.path.exists(deoxys.sprite_filepath))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Attack", 70)

            if game in ["LeafGreen"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)
                self.assertTrue(os.path.exists(deoxys.icon_filepath))
                self.assertTrue(os.path.exists(deoxys.sprite_filepath))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Defense", 70)

            if game in ["Emerald"]:
                deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
                self.assertTrue(os.path.exists(deoxys.icon_filepath))
                self.assertTrue(os.path.exists(deoxys.sprite_filepath))
            else:
                with self.assertRaises(ValueError):
                    deoxys = pkmn.pokemon("Deoxys", game, "Speed", 70)
        else:
            for form in pkmn.database.pokemon_entry("Deoxys", "Omega Ruby", "").forms:
                deoxys = pkmn.pokemon("Deoxys", game, form, 70)
                self.assertTrue(os.path.exists(deoxys.icon_filepath))
                self.assertTrue(os.path.exists(deoxys.sprite_filepath))

    @parameterized.expand(ALL_GAME_NAMES, testcase_func_name=test_name_func)
    def test_forms(self, game_name):
        game = pkmn.string_to_game(game_name)

        generation = GAME_TO_GENERATION[game]

        if generation >= 1:
            self.gen1_forms_test(game)
        if generation >= 2:
            self.gen2_forms_test(game)
        if generation >= 3:
            self.gen3_forms_test(game)

    @parameterized.expand(ALL_GAME_NAMES[3:], testcase_func_name=test_name_func)
    def test_gender(self, game_name):
        game = pkmn.string_to_game(game_name)

        # Single-gender
        nidorina = pkmn.pokemon(pkmn.species.NIDORINA, game, "", 50)
        self.assertEqual(nidorina.gender, pkmn.gender.FEMALE)
        nidorina.gender = pkmn.gender.FEMALE
        with self.assertRaises(ValueError):
            nidorina.gender = pkmn.gender.MALE
        with self.assertRaises(ValueError):
            nidorina.gender = pkmn.gender.GENDERLESS

        nidorino = pkmn.pokemon(pkmn.species.NIDORINO, game, "", 50)
        self.assertEqual(nidorino.gender, pkmn.gender.MALE)
        nidorino.gender = pkmn.gender.MALE
        with self.assertRaises(ValueError):
            nidorino.gender = pkmn.gender.FEMALE
        with self.assertRaises(ValueError):
            nidorino.gender = pkmn.gender.GENDERLESS

        magnemite = pkmn.pokemon(pkmn.species.MAGNEMITE, game, "", 50)
        self.assertEqual(magnemite.gender, pkmn.gender.GENDERLESS)
        magnemite.gender = pkmn.gender.GENDERLESS
        with self.assertRaises(ValueError):
            magnemite.gender = pkmn.gender.MALE
        with self.assertRaises(ValueError):
            magnemite.gender = pkmn.gender.FEMALE

        mixed_pokemon = [
            pkmn.species.CHARMANDER, # 87.5% male
            pkmn.species.GROWLITHE,  # 75% male
            pkmn.species.PIDGEY,     # 50% male
            pkmn.species.VULPIX      # 25% male
        ]

        for species in mixed_pokemon:
            pokemon = pkmn.pokemon(species, game, "", 50)
            pokemon.gender = pkmn.gender.FEMALE
            self.assertEqual(pokemon.gender, pkmn.gender.FEMALE)
            pokemon.gender = pkmn.gender.MALE
            self.assertEqual(pokemon.gender, pkmn.gender.MALE)
            with self.assertRaises(ValueError):
                pokemon.gender = pkmn.gender.GENDERLESS
