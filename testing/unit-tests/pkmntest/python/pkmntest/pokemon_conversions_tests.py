#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http:#opensource.org/licenses/MIT)
#

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

class pokemon_conversions_test(unittest.TestCase):

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
        first_pokemon = pkmn.pokemon(species, origin_game, form, 50)

        origin_generation = GAME_TO_GENERATION[origin_game]
        dest_generation = GAME_TO_GENERATION[dest_game]
        min_generation = min(origin_generation, dest_generation)
        game_for_lists = origin_game if min_generation == origin_generation else dest_game

        items = pkmn.database.get_item_list(game_for_lists)
        moves = pkmn.database.get_move_list(game_for_lists)

        for i in range(4):
            # This will get rid of legitimate moves like Shadow Ball,
            # but not enough to cause an issue.
            move_name = "Shadow"
            while move_name.startswith("Shadow"):
               move_name = random.choice(moves)
            first_pokemon.set_move(move_name, i)

        if origin_generation >= 3:
            first_pokemon.set_original_trainer_secret_id(random.randint(0, 0xFFFF))

            abilities = first_pokemon.get_database_entry().get_abilities()
            if abilities[1] != "None":
                first_pokemon.set_ability(abilities[0] if random_bool() else abilities[1])

        first_pokemon.set_original_trainer_public_id(random.randint(0, 0xFFFF))

        if min_generation >= 2:
            item_name = "Scent"
            while item_name.endswith("Scent") or not pkmn.database.item_entry(item_name, game_for_lists).holdable():
                item_name = random.choice(items)

            first_pokemon.set_held_item(item_name)

        if origin_generation >= 2:
            first_pokemon.set_gender("Male" if random_bool() else "Female")
            first_pokemon.set_shininess(random_bool())
            first_pokemon.set_friendship(random.randint(0, 255))

            if origin_game != "Gold" and origin_game != "Silver":
                first_pokemon.set_original_trainer_gender("Male" if random_bool() else "Female")

            # The max level met in Generation II is 63.
            first_pokemon.set_level_met(random.randint(0, 100) if origin_generation >= 3 else random.randint(2, 63))

        if origin_generation >= 3:
            for marking in first_pokemon.get_markings():
                first_pokemon.set_marking(marking, random_bool())

            for ribbon in first_pokemon.get_ribbons():
                first_pokemon.set_ribbon(ribbon, random_bool())

            for contest_stat in first_pokemon.get_contest_stats():
                first_pokemon.set_contest_stat(contest_stat, random.randint(0, 255))

        first_pokemon.set_nickname(random_string(10))
        first_pokemon.set_original_trainer_name(random_string(7))

        # The max level met in Generation II is 63, which restricts this as well.
        first_pokemon.set_level(random.randint(1, 100) if origin_generation >= 3 else random.randint(2, 63))

        # Convert to the second game and compare.
        second_pokemon = first_pokemon.to_game(dest_game)

        self.assertEquals(first_pokemon.get_species(), second_pokemon.get_species())
        self.assertEquals(dest_game, second_pokemon.get_game())
        self.assertEquals(first_pokemon.get_form(), second_pokemon.get_form())
        self.assertEquals(first_pokemon.get_nickname(), second_pokemon.get_nickname())
        self.assertEquals(first_pokemon.get_original_trainer_name(), second_pokemon.get_original_trainer_name())
        self.assertEquals(first_pokemon.get_original_trainer_id(), second_pokemon.get_original_trainer_id())
        self.assertEquals(first_pokemon.get_original_trainer_public_id(), second_pokemon.get_original_trainer_public_id())
        self.assertEquals(first_pokemon.get_experience(), second_pokemon.get_experience())
        self.assertEquals(first_pokemon.get_level(), second_pokemon.get_level())

        for i in range(4):
            self.assertEquals(first_pokemon.get_moves()[i].move, second_pokemon.get_moves()[i].move)
            self.assertEquals(first_pokemon.get_moves()[i].pp, second_pokemon.get_moves()[i].pp)

        if min_generation >= 3:
            self.assertEquals(first_pokemon.get_original_trainer_secret_id(), second_pokemon.get_original_trainer_secret_id())
            self.assertEquals(first_pokemon.get_ability(), second_pokemon.get_ability())
            self.assertEquals(first_pokemon.get_ball(), second_pokemon.get_ball())
            self.assertEquals(first_pokemon.get_original_game(), second_pokemon.get_original_game())
            self.assertEquals(first_pokemon.get_personality(), second_pokemon.get_personality())

            if origin_generation == dest_generation:
                self.assertEquals(first_pokemon.get_markings(), second_pokemon.get_markings())
                self.assertEquals(first_pokemon.get_contest_stats(), second_pokemon.get_contest_stats())
                self.assertEquals(first_pokemon.get_ribbons(), second_pokemon.get_ribbons())
                self.assertEquals(first_pokemon.get_EVs(), second_pokemon.get_EVs())
                self.assertEquals(first_pokemon.get_IVs(), second_pokemon.get_IVs())

        if min_generation >= 2:
            self.assertEquals(first_pokemon.get_original_trainer_gender(), second_pokemon.get_original_trainer_gender())
            self.assertEquals(first_pokemon.get_gender(), second_pokemon.get_gender())
            self.assertEquals(first_pokemon.is_shiny(), second_pokemon.is_shiny())
            self.assertEquals(first_pokemon.get_held_item(), second_pokemon.get_held_item())
            self.assertEquals(first_pokemon.get_friendship(), second_pokemon.get_friendship())
            self.assertEquals(first_pokemon.get_level(), second_pokemon.get_level_met())
