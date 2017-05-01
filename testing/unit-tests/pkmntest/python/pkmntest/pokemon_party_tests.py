#!/usr/bin/env python
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import unittest

class pokemon_party_tests(unittest.TestCase):

    def __test_empty_pokemon_party(self, party, game):
        self.assertEqual(party.get_game(), game)
        self.assertEqual(len(party), 6)

        # Make sure trying to get a Pokemon at an invalid index fails.
        with self.assertRaises(IndexError):
            party.get_pokemon(-1)
        with self.assertRaises(IndexError):
            party[-1]
        with self.assertRaises(IndexError):
            party.get_pokemon(len(party))
        with self.assertRaises(IndexError):
            party[len(party)]

        for pokemon in party:
            self.assertEqual(pokemon.get_species(), "None")
            self.assertEqual(pokemon.get_game(), game)

            for slot in pokemon.get_moves():
                self.assertEqual(slot.move.get_name(), "None")
                self.assertEqual(slot.pp, 0)

    def __test_setting_pokemon(self, party):
        game = party.get_game()

        original_first = party[0]
        original_second = party[1]

        # Make sure we can't set Pokemon at invalid indices.
        with self.assertRaises(IndexError):
            party[-1] = original_first
        with self.assertRaises(IndexError):
            party[len(party)] = original_second

        # Create Pokemon and place in party. The original variables should
        # have the same underlying Pokemon.
        bulbasaur = pkmn.pokemon("Bulbasaur", game, "", 5)
        charmander = pkmn.pokemon("Charmander", game, "", 5)
        squirtle = pkmn.pokemon("Squirtle", game, "", 5)
        
        party[0] = bulbasaur
        self.assertEqual(party.get_num_pokemon(), 1)
        self.assertEqual(party[0].get_species(), "Bulbasaur")
        party[1] = charmander
        self.assertEqual(party.get_num_pokemon(), 2)
        self.assertEqual(party[1].get_species(), "Charmander")

        # Replace one of the new ones.
        party[0] = squirtle
        self.assertEqual(party.get_num_pokemon(), 2)
        self.assertEqual(party[0].get_species(), "Squirtle")

        # Make sure we can't copy a Pokemon to itself.
        with self.assertRaises(ValueError):
            party[1] = party[1]

        # Copy a Pokemon whose memory is already part of the party.
        party[2] = party[1]
        self.assertEqual(party.get_num_pokemon(), 3)
        self.assertEqual(party[2].get_species(), "Charmander")

        # We should be able to clear the last contiguous Pokemon.
        party[2] = original_first
        self.assertEqual(party.get_num_pokemon(), 2)
        self.assertEqual(party[2].get_species(), "None")

        # Put it back.
        party[2] = party[1]
        self.assertEqual(party.get_num_pokemon(), 3)

        # Check that Pokemon cannot be placed non-contiguously.
        with self.assertRaises(ValueError):
            party[1] = original_first
        self.assertEqual(party.get_num_pokemon(), 3)
        self.assertEqual(party[1].get_species(), "Charmander")

        with self.assertRaises(IndexError):
            party[4] = bulbasaur
        self.assertEqual(party.get_num_pokemon(), 3)
        self.assertEqual(party[4].get_species(), "None")

        # Now check everything we've created. Each variable should have
        # the same underlying Pokemon.
        self.assertEqual(party[0].get_species(), "Squirtle")
        self.assertEqual(party[1].get_species(), "Charmander")
        self.assertEqual(party[2].get_species(), "Charmander")
        self.assertEqual(original_first.get_species(), "None")
        self.assertEqual(original_second.get_species(), "None")
        self.assertEqual(bulbasaur.get_species(), "Bulbasaur")
        self.assertEqual(charmander.get_species(), "Charmander")
        self.assertEqual(squirtle.get_species(), "Squirtle")

    def __test_pokemon_party(self, party, game):
        self.__test_empty_pokemon_party(party, game)
        self.__test_setting_pokemon(party)

    #
    # Red
    #

    def test_red_pokemon_party(self):
        party = pkmn.pokemon_party("Red")
        self.__test_pokemon_party(party, "Red")

    #
    # Blue
    #

    def test_blue_pokemon_party(self):
        party = pkmn.pokemon_party("Blue")
        self.__test_pokemon_party(party, "Blue")

    #
    # Yellow
    #

    def test_yellow_pokemon_party(self):
        party = pkmn.pokemon_party("Yellow")
        self.__test_pokemon_party(party, "Yellow")

    #
    # Gold
    #

    def test_gold_pokemon_party(self):
        party = pkmn.pokemon_party("Gold")
        self.__test_pokemon_party(party, "Gold")

    #
    # Silver
    #

    def test_silver_pokemon_party(self):
        party = pkmn.pokemon_party("Silver")
        self.__test_pokemon_party(party, "Silver")

    #
    # Crystal
    #

    def test_crystal_pokemon_party(self):
        party = pkmn.pokemon_party("Crystal")
        self.__test_pokemon_party(party, "Crystal")

    #
    # Ruby
    #

    def test_ruby_pokemon_party(self):
        party = pkmn.pokemon_party("Ruby")
        self.__test_pokemon_party(party, "Ruby")

    #
    # Sapphire
    #

    def test_sapphire_pokemon_party(self):
        party = pkmn.pokemon_party("Sapphire")
        self.__test_pokemon_party(party, "Sapphire")

    #
    # Emerald
    #

    def test_emerald_pokemon_party(self):
        party = pkmn.pokemon_party("Emerald")
        self.__test_pokemon_party(party, "Emerald")

    #
    # FireRed
    #

    def test_firered_pokemon_party(self):
        party = pkmn.pokemon_party("FireRed")
        self.__test_pokemon_party(party, "FireRed")

    #
    # LeafGreen
    #

    def test_leafgreen_pokemon_party(self):
        party = pkmn.pokemon_party("LeafGreen")
        self.__test_pokemon_party(party, "LeafGreen")

    #
    # Colosseum
    #

    def test_colosseum_pokemon_party(self):
        party = pkmn.pokemon_party("Colosseum")
        self.__test_pokemon_party(party, "Colosseum")

    #
    # XD
    #

    def test_xd_pokemon_party(self):
        party = pkmn.pokemon_party("XD")
        self.__test_pokemon_party(party, "XD")
