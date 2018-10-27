#!/usr/bin/env python
#
# Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import sys
import unittest

class lists_test(unittest.TestCase):

    def test_ability_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_ability_list(0)
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_ability_name_list(0)

        ability_list = pkmn.database.lists.get_ability_list(6)
        self.assertEqual(191, len(ability_list))
        self.assertEqual(pkmn.ability.STENCH, ability_list[0])
        self.assertEqual(pkmn.ability.DELTA_STREAM, ability_list[-1])

        ability_name_list = pkmn.database.lists.get_ability_name_list(6)
        self.assertEqual(191, len(ability_name_list))
        self.assertEqual("Stench", ability_name_list[0])
        self.assertEqual("Delta Stream", ability_name_list[-1])

        for index, ability_name in enumerate(ability_name_list):
            self.assertEqual(ability_list[index], pkmn.string_to_ability(ability_name))

    def test_game_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_game_list(7, False)
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_game_name_list(7, False)

        game_list = pkmn.database.lists.get_game_list(6, True)
        self.assertEqual(26, len(game_list))
        self.assertEqual(pkmn.game.RED, game_list[0])
        self.assertEqual(pkmn.game.ALPHA_SAPPHIRE, game_list[-1])

        game_name_list = pkmn.database.lists.get_game_name_list(6, True)
        self.assertEqual(26, len(game_name_list))
        self.assertEqual("Red", game_name_list[0])
        self.assertEqual("Alpha Sapphire", game_name_list[-1])

        for index, game_name in enumerate(game_name_list):
            self.assertEqual(game_list[index], pkmn.string_to_game(game_name))

    def test_gamecube_shadow_pokemon_list(self):
        colosseum_shadow_pokemon_list = pkmn.database.lists.get_gamecube_shadow_pokemon_list(True)
        self.assertEqual(48, len(colosseum_shadow_pokemon_list))

        colosseum_shadow_pokemon_name_list = pkmn.database.lists.get_gamecube_shadow_pokemon_name_list(True)
        self.assertEqual(48, len(colosseum_shadow_pokemon_name_list))

        for index, species_name in enumerate(colosseum_shadow_pokemon_name_list):
            self.assertEqual(
                colosseum_shadow_pokemon_list[index],
                pkmn.string_to_species(species_name)
            )

        xd_shadow_pokemon_list = pkmn.database.lists.get_gamecube_shadow_pokemon_list(False)
        self.assertEqual(83, len(xd_shadow_pokemon_list))

        xd_shadow_pokemon_name_list = pkmn.database.lists.get_gamecube_shadow_pokemon_name_list(False)
        self.assertEqual(83, len(xd_shadow_pokemon_name_list))

        for index, species_name in enumerate(xd_shadow_pokemon_name_list):
            self.assertEqual(
                xd_shadow_pokemon_list[index],
                pkmn.string_to_species(species_name)
            )

    def test_hm_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_hm_move_list(pkmn.game.NONE)
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_hm_move_name_list(pkmn.game.NONE)

        hm_move_list = pkmn.database.lists.get_hm_move_list(pkmn.game.RED)
        self.assertEqual(5, len(hm_move_list))
        self.assertEqual(pkmn.move.CUT, hm_move_list[0])
        self.assertEqual(pkmn.move.FLASH, hm_move_list[4])

        hm_move_name_list = pkmn.database.lists.get_hm_move_name_list(pkmn.game.RED)
        self.assertEqual(5, len(hm_move_name_list))
        self.assertEqual("Cut", hm_move_name_list[0])
        self.assertEqual("Flash", hm_move_name_list[4])

        for index, hm_move_name in enumerate(hm_move_name_list):
            self.assertEqual(hm_move_list[index], pkmn.string_to_move(hm_move_name))

    def test_item_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_item_list(pkmn.game.NONE)
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_item_name_list(pkmn.game.NONE)

        item_list = pkmn.database.lists.get_item_list(pkmn.game.HEARTGOLD)
        self.assertEqual(513, len(item_list))
        self.assertEqual(pkmn.item.MASTER_BALL, item_list[0])
        self.assertEqual(pkmn.item.PHOTO_ALBUM, item_list[-1])

        item_name_list = pkmn.database.lists.get_item_name_list(pkmn.game.HEARTGOLD)
        self.assertEqual(513, len(item_name_list))
        self.assertEqual("Master Ball", item_name_list[0])
        self.assertEqual("Photo Album", item_name_list[-1])

        for index, item_name in enumerate(item_name_list):
            self.assertEqual(item_list[index], pkmn.string_to_item(item_name))

    def test_location_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_location_name_list(pkmn.game.NONE, False)

        location_name_list = pkmn.database.lists.get_location_name_list(pkmn.game.EMERALD, False)
        self.assertEqual(106, len(location_name_list))
        self.assertEqual("Abandoned Ship", location_name_list[0])
        self.assertEqual("Victory Road", location_name_list[-1])

    def test_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_move_list(pkmn.game.NONE)
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_move_name_list(pkmn.game.NONE)

        move_list = pkmn.database.lists.get_move_list(pkmn.game.RED)
        self.assertEqual(165, len(move_list))
        self.assertEqual(pkmn.move.POUND, move_list[0])
        self.assertEqual(pkmn.move.STRUGGLE, move_list[-1])

        move_name_list = pkmn.database.lists.get_move_name_list(pkmn.game.RED)
        self.assertEqual(165, len(move_name_list))
        self.assertEqual("Pound", move_name_list[0])
        self.assertEqual("Struggle", move_name_list[-1])

        for index, move_name in enumerate(move_name_list):
            self.assertEqual(move_list[index], pkmn.string_to_move(move_name))

    def test_nature_list(self):
        nature_list = pkmn.database.lists.get_nature_list()
        self.assertEqual(25, len(nature_list))
        self.assertEqual(pkmn.nature.HARDY, nature_list[0])
        self.assertEqual(pkmn.nature.QUIRKY, nature_list[-1])

        nature_name_list = pkmn.database.lists.get_nature_name_list()
        self.assertEqual(25, len(nature_name_list))
        self.assertEqual("Hardy", nature_name_list[0])
        self.assertEqual("Quirky", nature_name_list[-1])

        for index, nature_name in enumerate(nature_name_list):
            self.assertEqual(nature_list[index], pkmn.string_to_nature(nature_name))

    def test_pokemon_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_pokemon_list(0, True)
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_pokemon_name_list(0, True)

        pokemon_list = pkmn.database.lists.get_pokemon_list(1, True)
        self.assertEqual(151, len(pokemon_list))
        self.assertEqual(pkmn.species.BULBASAUR, pokemon_list[0])
        self.assertEqual(pkmn.species.MEW, pokemon_list[-1])

        pokemon_name_list = pkmn.database.lists.get_pokemon_name_list(1, True)
        self.assertEqual(151, len(pokemon_name_list))
        self.assertEqual("Bulbasaur", pokemon_name_list[0])
        self.assertEqual("Mew", pokemon_name_list[-1])

        for index, pokemon_name in enumerate(pokemon_name_list):
            self.assertEqual(pokemon_list[index], pkmn.string_to_species(pokemon_name))

    def test_region_list(self):
        region_name_list = pkmn.database.lists.get_region_name_list()
        self.assertEqual(7, len(region_name_list))
        self.assertEqual("Kanto", region_name_list[0])
        self.assertEqual("Kalos", region_name_list[-1])

    def test_ribbon_list(self):
        # Make sure invalid generations are caught.
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_ribbon_name_list(2)
        with self.assertRaises(IndexError):
            pkmn.database.lists.get_ribbon_name_list(7)

        ribbon_name_list = pkmn.database.lists.get_ribbon_name_list(3)
        self.assertEqual(32, len(ribbon_name_list))

    def test_super_training_medal_list(self):
        super_training_medal_name_list = pkmn.database.lists.get_super_training_medal_name_list()
        self.assertEqual(30, len(super_training_medal_name_list))
        self.assertEqual("Sp. Atk Level 1", super_training_medal_name_list[0])
        self.assertEqual("The Battle for the Best!", super_training_medal_name_list[-1])

    def test_tm_move_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_tm_move_list(pkmn.game.NONE)
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_tm_move_name_list(pkmn.game.NONE)

        tm_move_list = pkmn.database.lists.get_tm_move_list(pkmn.game.RED)
        self.assertEqual(50, len(tm_move_list))
        self.assertEqual(pkmn.move.MEGA_PUNCH, tm_move_list[0])
        self.assertEqual(pkmn.move.SUBSTITUTE, tm_move_list[49])

        tm_move_name_list = pkmn.database.lists.get_tm_move_name_list(pkmn.game.RED)
        self.assertEqual(50, len(tm_move_name_list))
        self.assertEqual("Mega Punch", tm_move_name_list[0])
        self.assertEqual("Substitute", tm_move_name_list[49])

        for index, tm_move_name in enumerate(tm_move_name_list):
            self.assertEqual(tm_move_list[index], pkmn.string_to_move(tm_move_name))

    def test_type_list(self):
        # Make sure trying to create an invalid list results in an error
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_type_list(pkmn.game.NONE)
        with self.assertRaises(ValueError):
            pkmn.database.lists.get_type_name_list(pkmn.game.NONE)

        type_list = pkmn.database.lists.get_type_list(pkmn.game.ALPHA_SAPPHIRE)
        self.assertEqual(18, len(type_list))
        self.assertEqual(pkmn.pokemon_type.NORMAL, type_list[0])
        self.assertEqual(pkmn.pokemon_type.FAIRY, type_list[-1])

        type_name_list = pkmn.database.lists.get_type_name_list(pkmn.game.ALPHA_SAPPHIRE)
        self.assertEqual(18, len(type_name_list))
        self.assertEqual("Normal", type_name_list[0])
        self.assertEqual("Fairy", type_name_list[-1])

        for index, type_name in enumerate(type_name_list):
            self.assertEqual(type_list[index], pkmn.string_to_type(type_name))

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(lists_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
