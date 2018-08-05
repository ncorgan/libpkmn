#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .items_tests import items_tests

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

class gba_items_test(items_tests):

    def setUp(self):
        self.__all_pocket_items = [
            pkmn.item.POTION,
            pkmn.item.MACH_BIKE,
            pkmn.item.GREAT_BALL,
            pkmn.item.TM01,
            pkmn.item.ASPEAR_BERRY,
            pkmn.item.WAILMER_PAIL,
            pkmn.item.MASTER_BALL,
            pkmn.item.HM04
        ]
        self.__wrong_game_all_pocket_items = [
            pkmn.item.PINK_BOW,
            pkmn.item.BLACK_SLUDGE,
            pkmn.item.EIN_FILE_S,
            pkmn.item.GONZAPS_KEY,
            pkmn.item.GS_BALL,
            pkmn.item.POFFIN_CASE,
            pkmn.item.TM51,
            pkmn.item.BERRY,
            pkmn.item.OCCA_BERRY
        ]

    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        if game in [pkmn.game.RUBY, pkmn.game.SAPPHIRE]:
            expected_capacity = 20
        elif game == pkmn.game.EMERALD:
            expected_capacity = 30
        else:
            expected_capacity = 42

        # Check unchanging and initial values.
        self.assertEqual(item_pocket.name, "Items")
        self.assertEqual(item_pocket.game, game)
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, pkmn.item.POTION)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.BICYCLE, pkmn.item.MASTER_BALL,
             pkmn.item.HM01, pkmn.item.RAZZ_BERRY]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
             pkmn.item.BINDING_BAND, pkmn.item.BEEDRILLITE]
        )

        # Make sure we can't add items from Gamecube games.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.TIME_FLUTE, pkmn.item.POKE_SNACK]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            item_pocket,
            [pkmn.item.POTION, pkmn.item.ORANGE_MAIL,
             pkmn.item.LAVA_COOKIE, pkmn.item.STARDUST,
             pkmn.item.SHADOW_MAIL, pkmn.item.PINK_SCARF,
             pkmn.item.ANTIDOTE, pkmn.item.GREEN_SHARD]
        )

        self.assertGreater(len(item_pocket.valid_items), 0)
        self.assertEqual(len(item_pocket.valid_items), len(item_pocket.valid_item_names))

    def __test_key_item_pocket(self, key_item_pocket, game):
        if game in [pkmn.game.RUBY, pkmn.game.SAPPHIRE]:
            expected_capacity = 20
        else:
            expected_capacity = 30

        # Check unchanging and initial values.
        self.assertEqual(key_item_pocket.name, "Key Items")
        self.assertEqual(key_item_pocket.game, game)
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(key_item_pocket, pkmn.item.BASEMENT_KEY)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.POTION, pkmn.item.MASTER_BALL,
             pkmn.item.HM01, pkmn.item.RAZZ_BERRY]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.GS_BALL, pkmn.item.POFFIN_CASE,
             pkmn.item.DNA_SPLICERS, pkmn.item.AQUA_SUIT]
        )

        # Make sure we can't add items from Gamecube games.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.EIN_FILE_S, pkmn.item.POWERUP_PART,
             pkmn.item.GONZAPS_KEY, pkmn.item.KRANE_MEMO_1]
        )
        if game in [pkmn.game.RUBY, pkmn.game.SAPPHIRE]:
            self.item_class_test_invalid_items(
                key_item_pocket,
                [pkmn.item.HELIX_FOSSIL, pkmn.item.TEA, pkmn.item.RUBY]
            )
        if game != pkmn.game.EMERALD:
            self.item_class_test_invalid_items(
                key_item_pocket,
                [pkmn.item.MAGMA_EMBLEM, pkmn.item.OLD_SEA_MAP]
            )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            key_item_pocket,
            [pkmn.item.WAILMER_PAIL, pkmn.item.BASEMENT_KEY,
             pkmn.item.METEORITE, pkmn.item.OLD_ROD,
             pkmn.item.RED_ORB, pkmn.item.ROOT_FOSSIL,
             pkmn.item.CONTEST_PASS, pkmn.item.EON_TICKET]
        )

        self.assertGreater(len(key_item_pocket.valid_items), 0)
        self.assertEqual(len(key_item_pocket.valid_items), len(key_item_pocket.valid_item_names))

    def __test_ball_pocket(self, ball_pocket, game):
        if game in [pkmn.game.FIRERED, pkmn.game.LEAFGREEN]:
            expected_capacity = 13
        else:
            expected_capacity = 16

        # Check unchanging and initial values.
        self.assertStringEqual(ball_pocket.name, u"Pok\u00e9 Balls")
        self.assertEqual(ball_pocket.game, game)
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertEqual(ball_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, pkmn.item.MASTER_BALL)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            ball_pocket,
            [pkmn.item.POTION, pkmn.item.BICYCLE,
             pkmn.item.HM01, pkmn.item.RAZZ_BERRY]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            ball_pocket,
            [pkmn.item.MOON_BALL, pkmn.item.HEAL_BALL, pkmn.item.DREAM_BALL]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items = [
            pkmn.item.MASTER_BALL, pkmn.item.ULTRA_BALL,
            pkmn.item.GREAT_BALL, pkmn.item.POKE_BALL,
            pkmn.item.SAFARI_BALL, pkmn.item.NET_BALL,
            pkmn.item.DIVE_BALL, pkmn.item.NEST_BALL
        ]
        self.item_list_test_add_remove(
            ball_pocket,
            items
        )

        self.assertGreater(len(ball_pocket.valid_items), 0)
        self.assertEqual(len(ball_pocket.valid_items), len(ball_pocket.valid_item_names))

    def __test_tmhm_pocket(self, tmhm_pocket, game):
        if game in [pkmn.game.FIRERED, pkmn.game.LEAFGREEN]:
            expected_capacity = 58
            pocket_name = "TM Case"
        else:
            expected_capacity = 64
            pocket_name = "TMs & HMs"

        # Check unchanging and initial values.
        self.assertEqual(tmhm_pocket.name, pocket_name)
        self.assertEqual(tmhm_pocket.game, game)
        self.assertEqual(len(tmhm_pocket), expected_capacity)
        self.assertEqual(tmhm_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(tmhm_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(tmhm_pocket, pkmn.item.TM01)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            [pkmn.item.POTION, pkmn.item.MASTER_BALL, pkmn.item.GREAT_BALL, pkmn.item.RAZZ_BERRY]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            tmhm_pocket,
            [pkmn.item.TM51]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            tmhm_pocket,
            [pkmn.item.TM01, pkmn.item.HM01,
             pkmn.item.TM02, pkmn.item.HM02,
             pkmn.item.TM03, pkmn.item.HM03,
             pkmn.item.TM04, pkmn.item.HM04]
        )

        self.assertGreater(len(tmhm_pocket.valid_items), 0)
        self.assertEqual(len(tmhm_pocket.valid_items), len(tmhm_pocket.valid_item_names))

    def __test_berry_pocket(self, berry_pocket, game):
        if game in [pkmn.game.FIRERED, pkmn.game.LEAFGREEN]:
            expected_capacity = 43
            pocket_name = "Berry Pouch"
        else:
            expected_capacity = 46
            pocket_name = "Berries"

        # Check unchanging and initial values.
        self.assertEqual(berry_pocket.name, pocket_name)
        self.assertEqual(berry_pocket.game, game)
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(berry_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(berry_pocket, pkmn.item.RAZZ_BERRY)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            berry_pocket,
            [pkmn.item.POTION, pkmn.item.BICYCLE,
             pkmn.item.GREAT_BALL, pkmn.item.HM02]
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            berry_pocket,
            [pkmn.item.BERRY, pkmn.item.OCCA_BERRY, pkmn.item.ROSELI_BERRY]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            berry_pocket,
            [pkmn.item.CHERI_BERRY, pkmn.item.RAZZ_BERRY,
             pkmn.item.LUM_BERRY, pkmn.item.PINAP_BERRY,
             pkmn.item.ASPEAR_BERRY, pkmn.item.IAPAPA_BERRY,
             pkmn.item.WIKI_BERRY, pkmn.item.APICOT_BERRY]
        )

        self.assertGreater(len(berry_pocket.valid_items), 0)
        self.assertEqual(len(berry_pocket.valid_items), len(berry_pocket.valid_item_names))

    def __test_pc(self, pc, game):
        # Check unchanging and initial values.
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), 50)
        self.assertEqual(pc.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            pc,
            self.__wrong_game_all_pocket_items
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            [pkmn.item.POTION, pkmn.item.MACH_BIKE,
             pkmn.item.GREAT_BALL, pkmn.item.TM01,
             pkmn.item.ASPEAR_BERRY, pkmn.item.WAILMER_PAIL,
             pkmn.item.MASTER_BALL, pkmn.item.HM04]
        )

        self.assertGreater(len(pc.valid_items), 0)
        self.assertEqual(len(pc.valid_items), len(pc.valid_item_names))

    def __test_item_bag(self, bag, game):
        if game in [pkmn.game.FIRERED, pkmn.game.LEAFGREEN]:
            tmhm_pocket_name = "TM Case"
            berry_pocket_name = "Berry Pouch"
        else:
            tmhm_pocket_name = "TMs & HMs"
            berry_pocket_name = "Berries"
        ball_pocket_name = u"Pok\u00e9 Balls"

        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        self.assertEqual(len(bag), 5)
        self.assertEqual(len(bag.pocket_names), 5)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        unicode_pocket_names = []
        if PYTHON_MAJOR_VERSION == 2:
            unicode_pocket_names = [pocket_name.decode("utf-8") for pocket_name in bag.pocket_names]
        else:
            unicode_pocket_names = bag.pocket_names

        self.assertTrue("Items" in unicode_pocket_names)
        self.assertTrue("Key Items" in unicode_pocket_names)
        self.assertTrue(ball_pocket_name in unicode_pocket_names)
        self.assertTrue(tmhm_pocket_name in unicode_pocket_names)
        self.assertTrue(berry_pocket_name in unicode_pocket_names)

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["Key Items"], game)
        self.__test_ball_pocket(bag[ball_pocket_name], game)
        self.__test_tmhm_pocket(bag[tmhm_pocket_name], game)
        self.__test_berry_pocket(bag[berry_pocket_name], game)

        items = [
            pkmn.item.POTION, pkmn.item.MACH_BIKE,
            pkmn.item.GREAT_BALL, pkmn.item.TM01,
            pkmn.item.ASPEAR_BERRY, pkmn.item.WAILMER_PAIL,
            pkmn.item.MASTER_BALL, pkmn.item.HM04
        ]

        # Make sure adding items through the bag adds to the proper pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        self.assertEqual(bag["Key Items"].num_items, 0)
        self.assertEqual(bag[ball_pocket_name].num_items, 0)
        self.assertEqual(bag[tmhm_pocket_name].num_items, 0)
        self.assertEqual(bag[berry_pocket_name].num_items, 0)
        for item in items:
            bag.add(item, 5)

        self.assertEqual(bag["Items"][0].item, pkmn.item.POTION)
        self.assertEqual(bag["Items"][0].amount, 5)
        self.assertEqual(bag["Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][1].amount, 0)

        self.assertEqual(bag["Key Items"][0].item, pkmn.item.MACH_BIKE)
        self.assertEqual(bag["Key Items"][0].amount, 5)
        self.assertEqual(bag["Key Items"][1].item, pkmn.item.WAILMER_PAIL)
        self.assertEqual(bag["Key Items"][1].amount, 5)
        self.assertEqual(bag["Key Items"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Key Items"][2].amount, 0)

        self.assertEqual(bag[ball_pocket_name][0].item, pkmn.item.GREAT_BALL)
        self.assertEqual(bag[ball_pocket_name][0].amount, 5)
        self.assertEqual(bag[ball_pocket_name][1].item, pkmn.item.MASTER_BALL)
        self.assertEqual(bag[ball_pocket_name][1].amount, 5)
        self.assertEqual(bag[ball_pocket_name][2].item, pkmn.item.NONE)
        self.assertEqual(bag[ball_pocket_name][2].amount, 0)

        self.assertEqual(bag[tmhm_pocket_name][0].item, pkmn.item.TM01)
        self.assertEqual(bag[tmhm_pocket_name][0].amount, 5)
        self.assertEqual(bag[tmhm_pocket_name][1].item, pkmn.item.HM04)
        self.assertEqual(bag[tmhm_pocket_name][1].amount, 5)
        self.assertEqual(bag[tmhm_pocket_name][2].item, pkmn.item.NONE)
        self.assertEqual(bag[tmhm_pocket_name][2].amount, 0)

        self.assertEqual(bag[berry_pocket_name][0].item, pkmn.item.ASPEAR_BERRY)
        self.assertEqual(bag[berry_pocket_name][0].amount, 5)
        self.assertEqual(bag[berry_pocket_name][1].item, pkmn.item.NONE)
        self.assertEqual(bag[berry_pocket_name][1].amount, 0)

        # Make sure removing items through the bag removes from the proper pocket.
        for item in items:
            bag.remove(item, 5)

        self.assertEqual(bag["Items"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][0].amount, 0)
        self.assertEqual(bag["Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][1].amount, 0)

        self.assertEqual(bag["Key Items"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Key Items"][0].amount, 0)
        self.assertEqual(bag["Key Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Key Items"][1].amount, 0)
        self.assertEqual(bag["Key Items"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["Key Items"][2].amount, 0)

        self.assertEqual(bag[ball_pocket_name][0].item, pkmn.item.NONE)
        self.assertEqual(bag[ball_pocket_name][0].amount, 0)
        self.assertEqual(bag[ball_pocket_name][1].item, pkmn.item.NONE)
        self.assertEqual(bag[ball_pocket_name][1].amount, 0)
        self.assertEqual(bag[ball_pocket_name][2].item, pkmn.item.NONE)
        self.assertEqual(bag[ball_pocket_name][2].amount, 0)

        self.assertEqual(bag[tmhm_pocket_name][0].item, pkmn.item.NONE)
        self.assertEqual(bag[tmhm_pocket_name][0].amount, 0)
        self.assertEqual(bag[tmhm_pocket_name][1].item, pkmn.item.NONE)
        self.assertEqual(bag[tmhm_pocket_name][1].amount, 0)
        self.assertEqual(bag[tmhm_pocket_name][2].item, pkmn.item.NONE)
        self.assertEqual(bag[tmhm_pocket_name][2].amount, 0)

        self.assertEqual(bag[berry_pocket_name][0].item, pkmn.item.NONE)
        self.assertEqual(bag[berry_pocket_name][0].amount, 0)
        self.assertEqual(bag[berry_pocket_name][1].item, pkmn.item.NONE)
        self.assertEqual(bag[berry_pocket_name][1].amount, 0)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            bag,
            self.__wrong_game_all_pocket_items
        )

    #
    # Ruby
    #

    def test_ruby_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.RUBY)
        self.__test_item_pocket(item_pocket, pkmn.game.RUBY)

    def test_ruby_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.RUBY)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.RUBY)

    def test_ruby_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.RUBY)
        self.__test_ball_pocket(ball_pocket, pkmn.game.RUBY)

    def test_ruby_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.RUBY)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.RUBY)

    def test_ruby_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", pkmn.game.RUBY)
        self.__test_berry_pocket(berry_pocket, pkmn.game.RUBY)

    def test_ruby_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.RUBY)
        self.__test_pc(pc, pkmn.game.RUBY)

    def test_ruby_item_bag(self):
        item_bag = pkmn.item_bag(pkmn.game.RUBY)
        self.__test_item_bag(item_bag, pkmn.game.RUBY)

    #
    # Sapphire
    #

    def test_sapphire_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.SAPPHIRE)
        self.__test_item_pocket(item_pocket, pkmn.game.SAPPHIRE)

    def test_sapphire_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.SAPPHIRE)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.SAPPHIRE)

    def test_sapphire_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.SAPPHIRE)
        self.__test_ball_pocket(ball_pocket, pkmn.game.SAPPHIRE)

    def test_sapphire_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.SAPPHIRE)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.SAPPHIRE)

    def test_sapphire_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", pkmn.game.SAPPHIRE)
        self.__test_berry_pocket(berry_pocket, pkmn.game.SAPPHIRE)

    def test_sapphire_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.SAPPHIRE)
        self.__test_pc(pc, pkmn.game.SAPPHIRE)

    def test_sapphire_item_bag(self):
        item_bag = pkmn.item_bag(pkmn.game.SAPPHIRE)
        self.__test_item_bag(item_bag, pkmn.game.SAPPHIRE)

    #
    # Emerald
    #

    def test_emerald_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.EMERALD)
        self.__test_item_pocket(item_pocket, pkmn.game.EMERALD)

    def test_emerald_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.EMERALD)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.EMERALD)

    def test_emerald_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.EMERALD)
        self.__test_ball_pocket(ball_pocket, pkmn.game.EMERALD)

    def test_emerald_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TMs & HMs", pkmn.game.EMERALD)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.EMERALD)

    def test_emerald_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", pkmn.game.EMERALD)
        self.__test_berry_pocket(berry_pocket, pkmn.game.EMERALD)

    def test_emerald_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.EMERALD)
        self.__test_pc(pc, pkmn.game.EMERALD)

    def test_emerald_item_bag(self):
        item_bag = pkmn.item_bag(pkmn.game.EMERALD)
        self.__test_item_bag(item_bag, pkmn.game.EMERALD)

    #
    # FireRed
    #

    def test_firered_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.FIRERED)
        self.__test_item_pocket(item_pocket, pkmn.game.FIRERED)

    def test_firered_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.FIRERED)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.FIRERED)

    def test_firered_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.FIRERED)
        self.__test_ball_pocket(ball_pocket, pkmn.game.FIRERED)

    def test_firered_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM Case", pkmn.game.FIRERED)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.FIRERED)

    def test_firered_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berry Pouch", pkmn.game.FIRERED)
        self.__test_berry_pocket(berry_pocket, pkmn.game.FIRERED)

    def test_firered_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.FIRERED)
        self.__test_pc(pc, pkmn.game.FIRERED)

    def test_firered_item_bag(self):
        item_bag = pkmn.item_bag(pkmn.game.FIRERED)
        self.__test_item_bag(item_bag, pkmn.game.FIRERED)

    #
    # LeafGreen
    #

    def test_leafgreen_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.LEAFGREEN)
        self.__test_item_pocket(item_pocket, pkmn.game.LEAFGREEN)

    def test_leafgreen_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.LEAFGREEN)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.LEAFGREEN)

    def test_leafgreen_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.LEAFGREEN)
        self.__test_ball_pocket(ball_pocket, pkmn.game.LEAFGREEN)

    def test_leafgreen_tmhm_pocket(self):
        tmhm_pocket = pkmn.item_list("TM Case", pkmn.game.LEAFGREEN)
        self.__test_tmhm_pocket(tmhm_pocket, pkmn.game.LEAFGREEN)

    def test_leafgreen_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berry Pouch", pkmn.game.LEAFGREEN)
        self.__test_berry_pocket(berry_pocket, pkmn.game.LEAFGREEN)

    def test_leafgreen_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.LEAFGREEN)
        self.__test_pc(pc, pkmn.game.LEAFGREEN)

    def test_leafgreen_item_bag(self):
        item_bag = pkmn.item_bag(pkmn.game.LEAFGREEN)
        self.__test_item_bag(item_bag, pkmn.game.LEAFGREEN)
