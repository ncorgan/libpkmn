#!/usr/bin/env python
#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

from .items_tests import items_tests

import sys
import unittest

PYTHON_MAJOR_VERSION = sys.version_info[0]

COLOSSEUM_ALL_POCKET_ITEMS = [
    pkmn.item.POTION, pkmn.item.EIN_FILE_S,
    pkmn.item.GREAT_BALL, pkmn.item.TM01,
    pkmn.item.TM02, pkmn.item.ASPEAR_BERRY,
    pkmn.item.JOY_SCENT, pkmn.item.EXCITE_SCENT
]
XD_ALL_POCKET_ITEMS = [
    pkmn.item.POTION, pkmn.item.GONZAPS_KEY,
    pkmn.item.GREAT_BALL, pkmn.item.TM01,
    pkmn.item.TM02, pkmn.item.ASPEAR_BERRY,
    pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01
]
COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS = [
    pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
    pkmn.item.GS_BALL, pkmn.item.GONZAPS_KEY, pkmn.item.POFFIN_CASE,
    pkmn.item.TM51, pkmn.item.HM01,
    pkmn.item.BERRY, pkmn.item.OCCA_BERRY
]
XD_WRONG_GAME_ALL_POCKET_ITEMS = [
    pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
    pkmn.item.GS_BALL, pkmn.item.EIN_FILE_S, pkmn.item.POFFIN_CASE,
    pkmn.item.TM51, pkmn.item.HM01,
    pkmn.item.BERRY, pkmn.item.OCCA_BERRY
]

class gcn_items_test(items_tests):
    #
    # Helper functions
    #

    def __test_item_pocket(self, item_pocket, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 20 if colosseum else 30

        # Check unchanging and initial values.
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.name, "Items")
        self.assertEqual(item_pocket.game, game)
        self.assertEqual(len(item_pocket), expected_capacity)
        self.assertEqual(item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(item_pocket, pkmn.item.POTION)

        # Make sure we can't add items from other pockets.'
        wrong_pocket_items = [
            pkmn.item.EIN_FILE_S, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT] if colosseum else \
           [pkmn.item.MIROR_RADAR, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            item_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            item_pocket,
            [pkmn.item.PINK_BOW, pkmn.item.BLACK_SLUDGE,
             pkmn.item.BINDING_BAND, pkmn.item.BEEDRILLITE]
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
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 43

        # Check unchanging and initial values.
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.name, "Key Items")
        self.assertEqual(key_item_pocket.game, game)
        self.assertEqual(len(key_item_pocket), expected_capacity)
        self.assertEqual(key_item_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(key_item_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(
            key_item_pocket,
            pkmn.item.EIN_FILE_S if colosseum else pkmn.item.MIROR_RADAR
        )

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = [
            pkmn.item.POTION, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT] if colosseum else \
           [pkmn.item.POTION, pkmn.item.GREAT_BALL,
            pkmn.item.TM01, pkmn.item.ORAN_BERRY,
            pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            key_item_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.GS_BALL, pkmn.item.POFFIN_CASE,
             pkmn.item.DNA_SPLICERS, pkmn.item.AQUA_SUIT]
        )

        # Make sure we can't add items from incompatible Generation III games.
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.HELIX_FOSSIL, pkmn.item.TEA, pkmn.item.RUBY]
        )
        self.item_class_test_invalid_items(
            key_item_pocket,
            [pkmn.item.MAGMA_EMBLEM, pkmn.item.OLD_SEA_MAP]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        items = [pkmn.item.JAIL_KEY, pkmn.item.ELEVATOR_KEY,
                 pkmn.item.SMALL_TABLET, pkmn.item.F_DISK,
                 pkmn.item.R_DISK, pkmn.item.L_DISK,
                 pkmn.item.D_DISK, pkmn.item.U_DISK] if colosseum else \
                [pkmn.item.KRANE_MEMO_1, pkmn.item.KRANE_MEMO_2,
                 pkmn.item.KRANE_MEMO_3, pkmn.item.KRANE_MEMO_4,
                 pkmn.item.KRANE_MEMO_5, pkmn.item.VOICE_CASE_1,
                 pkmn.item.VOICE_CASE_2, pkmn.item.VOICE_CASE_3]
        self.item_list_test_add_remove(
            key_item_pocket,
            items
        )

        self.assertGreater(len(key_item_pocket.valid_items), 0)
        self.assertEqual(len(key_item_pocket.valid_items), len(key_item_pocket.valid_item_names))

    def __test_ball_pocket(self, ball_pocket, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 16

        # Check unchanging and initial values.
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertStringEqual(ball_pocket.name, u"Pok\u00e9 Balls")
        self.assertEqual(ball_pocket.game, game)
        self.assertEqual(len(ball_pocket), expected_capacity)
        self.assertEqual(ball_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(ball_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(ball_pocket, pkmn.item.MASTER_BALL)

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = [pkmn.item.EIN_FILE_S, pkmn.item.POTION,
                              pkmn.item.TM01, pkmn.item.ORAN_BERRY,
                              pkmn.item.JOY_SCENT] if colosseum else \
                             [pkmn.item.MIROR_RADAR, pkmn.item.POTION,
                              pkmn.item.TM01, pkmn.item.ORAN_BERRY,
                              pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            ball_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            ball_pocket,
            [pkmn.item.MOON_BALL, pkmn.item.HEAL_BALL, pkmn.item.DREAM_BALL]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        item_names = [pkmn.item.MASTER_BALL, pkmn.item.ULTRA_BALL,
                      pkmn.item.GREAT_BALL, pkmn.item.POKE_BALL,
                      pkmn.item.SAFARI_BALL, pkmn.item.NET_BALL,
                      pkmn.item.DIVE_BALL, pkmn.item.NEST_BALL]
        self.item_list_test_add_remove(
            ball_pocket,
            item_names
        )

        self.assertGreater(len(ball_pocket.valid_items), 0)
        self.assertEqual(len(ball_pocket.valid_items), len(ball_pocket.valid_item_names))

    def __test_tm_pocket(self, tm_pocket, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 64

        # Check unchanging and initial values.
        self.assertEqual(len(tm_pocket), expected_capacity)
        self.assertEqual(tm_pocket.name, "TMs")
        self.assertEqual(tm_pocket.game, game)
        self.assertEqual(len(tm_pocket), expected_capacity)
        self.assertEqual(tm_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(tm_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(tm_pocket, pkmn.item.TM01)

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = [pkmn.item.EIN_FILE_S, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.ORAN_BERRY,
                              pkmn.item.JOY_SCENT] if colosseum else \
                             [pkmn.item.MIROR_RADAR, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.ORAN_BERRY,
                              pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            tm_pocket,
            wrong_pocket_items
        )

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            tm_pocket,
            [pkmn.item.TM51]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            tm_pocket,
            [pkmn.item.TM01, pkmn.item.TM02, pkmn.item.TM03, pkmn.item.TM04,
             pkmn.item.TM05, pkmn.item.TM06, pkmn.item.TM07, pkmn.item.TM08]
        )

        self.assertGreater(len(tm_pocket.valid_items), 0)
        self.assertEqual(len(tm_pocket.valid_items), len(tm_pocket.valid_item_names))

    def __test_berry_pocket(self, berry_pocket, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 46

        # Check unchanging and initial values.
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.name, "Berries")
        self.assertEqual(berry_pocket.game, game)
        self.assertEqual(len(berry_pocket), expected_capacity)
        self.assertEqual(berry_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(berry_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(berry_pocket, pkmn.item.RAZZ_BERRY)

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = [pkmn.item.EIN_FILE_S, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.TM01,
                              pkmn.item.JOY_SCENT] if colosseum else \
                             [pkmn.item.MIROR_RADAR, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.TM01,
                              pkmn.item.JOY_SCENT, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            berry_pocket,
            wrong_pocket_items
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

    def __test_cologne_pocket(self, cologne_pocket, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 3

        # Check unchanging and initial values.
        self.assertEqual(len(cologne_pocket), expected_capacity)
        self.assertEqual(cologne_pocket.name, "Colognes")
        self.assertEqual(cologne_pocket.game, game)
        self.assertEqual(len(cologne_pocket), expected_capacity)
        self.assertEqual(cologne_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(cologne_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(cologne_pocket, pkmn.item.JOY_SCENT)

        # Make sure we can't add items from other pockets.
        wrong_pocket_items = [pkmn.item.EIN_FILE_S, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.TM01,
                              pkmn.item.ORAN_BERRY] if colosseum else \
                             [pkmn.item.MIROR_RADAR, pkmn.item.POTION,
                              pkmn.item.GREAT_BALL, pkmn.item.TM01,
                              pkmn.item.ORAN_BERRY, pkmn.item.BATTLE_CD_01]
        self.item_class_test_invalid_items(
            cologne_pocket,
            wrong_pocket_items
        )

        # Since this pocket can only have 3 items, we can't use our typical function, which requires 8.
        cologne_pocket.add(pkmn.item.JOY_SCENT, 3)
        cologne_pocket.add(pkmn.item.EXCITE_SCENT, 3)
        cologne_pocket.add(pkmn.item.VIVID_SCENT, 3)

        cologne_pocket.remove(pkmn.item.EXCITE_SCENT, 3)
        cologne_pocket.remove(pkmn.item.VIVID_SCENT, 1)

        self.assertEquals(pkmn.item.JOY_SCENT, cologne_pocket[0].item)
        self.assertEquals(3, cologne_pocket[0].amount)
        self.assertEquals(pkmn.item.VIVID_SCENT, cologne_pocket[1].item)
        self.assertEquals(2, cologne_pocket[1].amount)
        self.assertEquals(pkmn.item.NONE, cologne_pocket[2].item)
        self.assertEquals(0, cologne_pocket[2].amount)

        cologne_pocket.remove(pkmn.item.JOY_SCENT, 3)
        cologne_pocket.remove(pkmn.item.VIVID_SCENT, 2)

        self.assertEquals(pkmn.item.NONE, cologne_pocket[0].item)
        self.assertEquals(0, cologne_pocket[0].amount)
        self.assertEquals(pkmn.item.NONE, cologne_pocket[1].item)
        self.assertEquals(0, cologne_pocket[1].amount)
        self.assertEquals(pkmn.item.NONE, cologne_pocket[2].item)
        self.assertEquals(0, cologne_pocket[2].amount)

        self.assertEquals(len(cologne_pocket.valid_items), 3)
        self.assertEquals(len(cologne_pocket.valid_item_names), 3)

    def __test_battle_cd_pocket(self, battle_cd_pocket, game):
        expected_capacity = 60

        # Check unchanging and initial values.
        self.assertEqual(len(battle_cd_pocket), expected_capacity)
        self.assertEqual(battle_cd_pocket.name, "Battle CDs")
        self.assertEqual(battle_cd_pocket.game, game)
        self.assertEqual(len(battle_cd_pocket), expected_capacity)
        self.assertEqual(battle_cd_pocket.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(battle_cd_pocket)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(battle_cd_pocket, pkmn.item.BATTLE_CD_01)

        # Make sure we can't add items from other pockets.
        self.item_class_test_invalid_items(
            battle_cd_pocket,
            [pkmn.item.MIROR_RADAR, pkmn.item.POTION,
             pkmn.item.GREAT_BALL, pkmn.item.TM01,
             pkmn.item.JOY_SCENT, pkmn.item.ORAN_BERRY]
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            battle_cd_pocket,
            [pkmn.item.BATTLE_CD_01, pkmn.item.BATTLE_CD_02,
             pkmn.item.BATTLE_CD_03, pkmn.item.BATTLE_CD_04,
             pkmn.item.BATTLE_CD_05, pkmn.item.BATTLE_CD_06,
             pkmn.item.BATTLE_CD_07, pkmn.item.BATTLE_CD_08]
        )

        self.assertEquals(len(battle_cd_pocket.valid_items), 60)
        self.assertEquals(len(battle_cd_pocket.valid_item_names), 60)

    def __test_pc(self, pc, game):
        colosseum = (game == pkmn.game.COLOSSEUM)
        expected_capacity = 235

        # Check unchanging and initial values.
        self.assertEqual(len(pc), expected_capacity)
        self.assertEqual(pc.name, "PC")
        self.assertEqual(pc.game, game)
        self.assertEqual(len(pc), expected_capacity)
        self.assertEqual(pc.num_items, 0)

        # Make sure item slots start as correctly empty.
        self.item_list_test_empty_slot(pc)

        # Confirm errors are thrown when expected.
        self.item_list_test_out_of_range_error(pc, pkmn.item.POTION)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            pc,
            COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS if colosseum else XD_WRONG_GAME_ALL_POCKET_ITEMS
        )

        # Start adding and removing stuff, and make sure the numbers are accurate.
        self.item_list_test_add_remove(
            pc,
            COLOSSEUM_ALL_POCKET_ITEMS if colosseum else XD_ALL_POCKET_ITEMS
        )

        self.assertGreater(len(pc.valid_items), 0)
        self.assertEqual(len(pc.valid_items), len(pc.valid_item_names))

    def __test_item_bag(self, bag, game):
        colosseum = (game == pkmn.game.COLOSSEUM)

        # Check unchanging and initial values.
        self.assertEqual(bag.game, game)

        expected_num_pockets = 6 if colosseum else 7
        self.assertEqual(len(bag), expected_num_pockets)
        self.assertEqual(len(bag.pocket_names), expected_num_pockets)
        self.item_bag_test_get_pockets_with_both_text_types(bag)

        unicode_pocket_names = []
        if PYTHON_MAJOR_VERSION == 2:
            unicode_pocket_names = [pocket_name.decode("utf-8") for pocket_name in bag.pocket_names]
        else:
            unicode_pocket_names = bag.pocket_names

        self.assertTrue("Items" in unicode_pocket_names)
        self.assertTrue("Key Items" in unicode_pocket_names)
        self.assertTrue(u"Pok\u00e9 Balls" in unicode_pocket_names)
        self.assertTrue("TMs" in unicode_pocket_names)
        self.assertTrue("Berries" in unicode_pocket_names)
        self.assertTrue("Colognes" in unicode_pocket_names)
        if not colosseum:
            self.assertTrue("Battle CDs" in unicode_pocket_names)

        # Make sure we can't add items from other generations.
        self.item_class_test_invalid_items(
            bag,
            COLOSSEUM_WRONG_GAME_ALL_POCKET_ITEMS if colosseum else XD_WRONG_GAME_ALL_POCKET_ITEMS
        )

        self.__test_item_pocket(bag["Items"], game)
        self.__test_key_item_pocket(bag["Key Items"], game)
        self.__test_ball_pocket(bag[u"Pok\u00e9 Balls"], game)
        self.__test_tm_pocket(bag["TMs"], game)
        self.__test_berry_pocket(bag["Berries"], game)
        self.__test_cologne_pocket(bag["Colognes"], game)
        if not colosseum:
            self.__test_battle_cd_pocket(bag["Battle CDs"], game)

        items = COLOSSEUM_ALL_POCKET_ITEMS if colosseum else XD_ALL_POCKET_ITEMS

        # Make sure adding items through the bag adds to the proper pocket.
        self.assertEqual(bag["Items"].num_items, 0)
        self.assertEqual(bag["Key Items"].num_items, 0)
        self.assertEqual(bag[u"Pok\u00e9 Balls"].num_items, 0)
        self.assertEqual(bag["TMs"].num_items, 0)
        self.assertEqual(bag["Berries"].num_items, 0)
        self.assertEqual(bag["Colognes"].num_items, 0)
        if not colosseum:
            self.assertEqual(bag["Battle CDs"].num_items, 0)
        for item in items:
            bag.add(item, 5)

        self.assertEqual(bag["Items"][0].item, pkmn.item.POTION)
        self.assertEqual(bag["Items"][0].amount, 5)
        self.assertEqual(bag["Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Items"][1].amount, 0)

        key_item = pkmn.item.EIN_FILE_S if colosseum else pkmn.item.GONZAPS_KEY
        self.assertEqual(bag["Key Items"][0].item, key_item)
        self.assertEqual(bag["Key Items"][0].amount, 5)
        self.assertEqual(bag["Key Items"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Key Items"][1].amount, 0)

        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].item, pkmn.item.GREAT_BALL)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].amount, 5)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].item, pkmn.item.NONE)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].amount, 0)

        self.assertEqual(bag["TMs"][0].item, pkmn.item.TM01)
        self.assertEqual(bag["TMs"][0].amount, 5)
        self.assertEqual(bag["TMs"][1].item, pkmn.item.TM02)
        self.assertEqual(bag["TMs"][1].amount, 5)
        self.assertEqual(bag["TMs"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["TMs"][2].amount, 0)

        self.assertEqual(bag["Berries"][0].item, pkmn.item.ASPEAR_BERRY)
        self.assertEqual(bag["Berries"][0].amount, 5)
        self.assertEqual(bag["Berries"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Berries"][1].amount, 0)

        self.assertEqual(bag["Colognes"][0].item, pkmn.item.JOY_SCENT)
        self.assertEqual(bag["Colognes"][0].amount, 5)
        if colosseum:
            self.assertEqual(bag["Colognes"][1].item, pkmn.item.EXCITE_SCENT)
            self.assertEqual(bag["Colognes"][1].amount, 5)
            self.assertEqual(bag["Colognes"][2].item, pkmn.item.NONE)
            self.assertEqual(bag["Colognes"][2].amount, 0)
        else:
            self.assertEqual(bag["Colognes"][1].item, pkmn.item.NONE)
            self.assertEqual(bag["Colognes"][1].amount, 0)

            self.assertEqual(bag["Battle CDs"][0].item, pkmn.item.BATTLE_CD_01)
            self.assertEqual(bag["Battle CDs"][0].amount, 5)
            self.assertEqual(bag["Battle CDs"][1].item, pkmn.item.NONE)
            self.assertEqual(bag["Battle CDs"][1].amount, 0)

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

        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].item, pkmn.item.NONE)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][0].amount, 0)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].item, pkmn.item.NONE)
        self.assertEqual(bag[u"Pok\u00e9 Balls"][1].amount, 0)

        self.assertEqual(bag["TMs"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["TMs"][0].amount, 0)
        self.assertEqual(bag["TMs"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["TMs"][1].amount, 0)
        self.assertEqual(bag["TMs"][2].item, pkmn.item.NONE)
        self.assertEqual(bag["TMs"][2].amount, 0)

        self.assertEqual(bag["Berries"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Berries"][0].amount, 0)
        self.assertEqual(bag["Berries"][1].item, pkmn.item.NONE)
        self.assertEqual(bag["Berries"][1].amount, 0)

        self.assertEqual(bag["Colognes"][0].item, pkmn.item.NONE)
        self.assertEqual(bag["Colognes"][0].amount, 0)
        if colosseum:
            self.assertEqual(bag["Colognes"][1].item, pkmn.item.NONE)
            self.assertEqual(bag["Colognes"][1].amount, 0)
            self.assertEqual(bag["Colognes"][2].item, pkmn.item.NONE)
            self.assertEqual(bag["Colognes"][2].amount, 0)
        else:
            self.assertEqual(bag["Colognes"][1].item, pkmn.item.NONE)
            self.assertEqual(bag["Colognes"][1].amount, 0)

            self.assertEqual(bag["Battle CDs"][0].item, pkmn.item.NONE)
            self.assertEqual(bag["Battle CDs"][0].amount, 0)
            self.assertEqual(bag["Battle CDs"][1].item, pkmn.item.NONE)
            self.assertEqual(bag["Battle CDs"][1].amount, 0)

    #
    # Colosseum
    #

    def test_colosseum_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.COLOSSEUM)
        self.__test_item_pocket(item_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.COLOSSEUM)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.COLOSSEUM)
        self.__test_ball_pocket(ball_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_tm_pocket(self):
        tm_pocket = pkmn.item_list("TMs", pkmn.game.COLOSSEUM)
        self.__test_tm_pocket(tm_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", pkmn.game.COLOSSEUM)
        self.__test_berry_pocket(berry_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_cologne_pocket(self):
        cologne_pocket = pkmn.item_list("Colognes", pkmn.game.COLOSSEUM)
        self.__test_cologne_pocket(cologne_pocket, pkmn.game.COLOSSEUM)

    def test_colosseum_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.COLOSSEUM)
        self.__test_pc(pc, pkmn.game.COLOSSEUM)

    def test_colosseum_bag(self):
        bag = pkmn.item_bag(pkmn.game.COLOSSEUM)
        self.__test_item_bag(bag, pkmn.game.COLOSSEUM)

    #
    # XD
    #

    def test_xd_item_pocket(self):
        item_pocket = pkmn.item_list("Items", pkmn.game.XD)
        self.__test_item_pocket(item_pocket, pkmn.game.XD)

    def test_xd_key_item_pocket(self):
        key_item_pocket = pkmn.item_list("Key Items", pkmn.game.XD)
        self.__test_key_item_pocket(key_item_pocket, pkmn.game.XD)

    def test_xd_ball_pocket(self):
        ball_pocket = pkmn.item_list(u"Pok\u00e9 Balls", pkmn.game.XD)
        self.__test_ball_pocket(ball_pocket, pkmn.game.XD)

    def test_xd_tm_pocket(self):
        tm_pocket = pkmn.item_list("TMs", pkmn.game.XD)
        self.__test_tm_pocket(tm_pocket, pkmn.game.XD)

    def test_xd_berry_pocket(self):
        berry_pocket = pkmn.item_list("Berries", pkmn.game.XD)
        self.__test_berry_pocket(berry_pocket, pkmn.game.XD)

    def test_xd_cologne_pocket(self):
        cologne_pocket = pkmn.item_list("Colognes", pkmn.game.XD)
        self.__test_cologne_pocket(cologne_pocket, pkmn.game.XD)

    def test_xd_battle_cd_pocket(self):
        battle_cd_pocket = pkmn.item_list("Battle CDs", pkmn.game.XD)
        self.__test_battle_cd_pocket(battle_cd_pocket, pkmn.game.XD)

    def test_xd_pc(self):
        pc = pkmn.item_list("PC", pkmn.game.XD)
        self.__test_pc(pc, pkmn.game.XD)

    def test_xd_bag(self):
        bag = pkmn.item_bag(pkmn.game.XD)
        self.__test_item_bag(bag, pkmn.game.XD)
