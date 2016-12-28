#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import unittest

class game_save_tests(unittest.TestCase):

    def setUp(self):
        # TODO: replace with pokemon fields
        self.__LIBPKMN_OT_NAME = "LibPKMN"
        self.__TOO_LONG_OT_NAME = "LibPKMNLibPKMN"
        self.__LIBPKMN_OT_ID = 2105214279
        self.__LIBPKMN_OT_PID = 1351
        self.__LIBPKMN_OT_SID = 32123
        self.__MONEY_MAX_VALUE = 999999

        self.__GB_GAMES = ["Red", "Blue", "Yellow",
                           "Gold", "Silver", "Crystal"]
        self.__RIVAL_NAME_SET_GAMES = ["Ruby", "Sapphire", "Emerald",
                                       "Black", "White",
                                       "X", "Y"]
        self.__MALE_ONLY_GAMES = ["Red", "Blue", "Yellow",
                                  "Gold", "Silver"]

    def __test_trainer_name(self, save):
        with self.assertRaises(ValueError):
            save.set_trainer_name("")
        with self.assertRaises(ValueError):
            save.set_trainer_name(self.__TOO_LONG_OT_NAME)

        save.set_trainer_name(self.__LIBPKMN_OT_NAME)
        self.assertEqual(save.get_trainer_name(), self.__LIBPKMN_OT_NAME)

    def __test_rival_name(self, save):
        if save.get_game() in self.__RIVAL_NAME_SET_GAMES:
            with self.assertRaises(ValueError):
                save.set_rival_name(self.__LIBPKMN_OT_NAME)
        else:
            with self.assertRaises(ValueError):
                save.set_rival_name("")
            with self.assertRaises(ValueError):
                save.set_rival_name(self.__TOO_LONG_OT_NAME)

            save.set_rival_name(self.__LIBPKMN_OT_NAME)
            self.assertEqual(save.get_rival_name(), self.__LIBPKMN_OT_NAME)

    def __test_trainer_id(self, save):
        is_gb_game = (save.get_game() in self.__GB_GAMES)
        self.assertEqual(
            save.get_trainer_id(),
            (self.__LIBPKMN_OT_PID if is_gb_game else self.__LIBPKMN_OT_ID)
        )
        self.assertEqual(save.get_trainer_public_id(), self.__LIBPKMN_OT_PID)
        if is_gb_game:
            with self.assertRaises(RuntimeError):
                save.get_trainer_secret_id()
        else:
            self.assertEqual(save.get_trainer_secret_id(), self.__LIBPKMN_OT_SID)

    def __test_game_save_common_fields(self, save):
        self.__test_trainer_name(save)

        is_gb_game = (save.get_game() in self.__GB_GAMES)
        save.set_trainer_id(
            (self.__LIBPKMN_OT_PID if is_gb_game else self.__LIBPKMN_OT_ID)
        )
        self.__test_trainer_id(save)

        self.__test_rival_name(save)

        if is_gb_game:
            with self.assertRaises(IndexError):
                save.set_trainer_id(self.__LIBPKMN_OT_ID)
            with self.assertRaises(RuntimeError):
                save.set_trainer_secret_id(self.__LIBPKMN_OT_SID)
        else:
            save.set_trainer_secret_id(self.__LIBPKMN_OT_SID)
            self.__test_trainer_id(save)

        if save.get_game() in self.__MALE_ONLY_GAMES:
            self.assertEqual(save.get_trainer_gender(), "Male")
            with self.assertRaises(RuntimeError):
                save.set_trainer_gender("Male")
            with self.assertRaises(RuntimeError):
                save.set_trainer_gender("Female")
        else:
            save.set_trainer_gender("Male")
            self.assertEqual(save.get_trainer_gender(), "Male")
            save.set_trainer_gender("Female")
            self.assertEqual(save.get_trainer_gender(), "Female")
            with self.assertRaises(ValueError):
                save.set_trainer_gender("Genderless")

        with self.assertRaises(IndexError):
            save.set_money(-1)
        with self.assertRaises(IndexError):
            save.set_money(self.__MONEY_MAX_VALUE+1)

        save.set_money(123456)
        self.assertEqual(save.get_money(), 123456)

    def __test_game_save(filepath, game):
        self.assertTrue(game in pkmn.detect_game_save_type(filepath))

        save = pkmn.game_save(filepath)
        self.assertEqual(save.get_game(), game)
        self.__test_game_save_common_fields(save)

    #
    # Red
    #
    def test_red(self):
        self.__test_game_save("@POKEMON_RED_SAV@", "Red")

    #
    # TODO: Blue
    #

    #
    # Yellow
    #
    def test_yellow(self):
        self.__test_game_save("@POKEMON_YELLOW_SAV@", "Yellow")

    #
    # Gold
    #
    def test_gold(self):
        self.__test_game_save("@POKEMON_GOLD_SAV@", "Gold")

    #
    # TODO: Silver
    #

    #
    # Crystal
    #
    def test_crystal(self):
        self.__test_game_save("@POKEMON_CRYSTAL_SAV@", "Crystal")

    #
    # Ruby
    #
    def test_ruby(self):
        self.__test_game_save("@POKEMON_RUBY_SAV@", "Ruby")

    #
    # TODO: Sapphire
    #

    #
    # Emerald
    #
    def test_emerald(self):
        self.__test_game_save("@POKEMON_EMERALD_SAV@", "Emerald")

    #
    # FireRed
    #
    def test_firered(self):
        self.__test_game_save("@POKEMON_FIRERED_SAV@", "FireRed")

    #
    # TODO: LeafGreen
    #
