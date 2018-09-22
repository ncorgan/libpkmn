#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import random
import unittest

from nose_parameterized import parameterized

def test_name_func(testcase_func, param_num, param):
    return "{0}_{1}".format(testcase_func.__name__, param.args[0])

LIBPKMN_TEST_FILES = os.environ["LIBPKMN_TEST_FILES"]
TMP_DIR = pkmn.paths.get_tmp_dir()

class pokemon_subclass(pkmn.pokemon):
    def __init__(self, species, game):
        pkmn.pokemon.__init__(self)

        self.__species = species
        self.__game = game
        self.__form = ""
        self.__is_egg = False
        self.__condition = pkmn.condition.NONE
        self.__nickname = pkmn.species_to_string(species)
        self.__gender = pkmn.gender.NONE
        self.__is_shiny = False
        self.__held_item = pkmn.item.NONE
        self.__nature = pkmn.nature.NONE
        self.__pokerus_duration = 0
        self.__original_trainer_name = pkmn.pokemon.DEFAULT_TRAINER_NAME
        self.__original_trainer_id = pkmn.pokemon.DEFAULT_TRAINER_ID
        self.__original_trainer_gender = pkmn.gender.NONE
        self.__language = pkmn.language.NONE
        self.__current_trainer_friendship = 0
        self.__ability = pkmn.ability.NONE
        self.__ball = pkmn.ball.NONE
        self.__level_met = 0
        self.__location_met = ""
        self.__location_met_as_egg = ""
        self.__original_game = game
        self.__personality = 0
        self.__experience = 0
        self.__level = 0
        self.__current_hp = 0

        self.initialize_default_values(species, game)

    def clone(self):
        return pokemon_subclass(self.species, this.game)

    def to_game(self, game):
        return pokemon_subclass(self.species, game)

    def export_to_file(self):
        return

    def _get_species_internal(self):
        return self.__species

    def _get_game_internal(self):
        return self.__game

    def _get_form_internal(self):
        return self.__form

    def _set_form_internal(self, form):
        self.__form = form

    def _get_is_egg_internal(self):
        return self.__is_egg

    def _set_is_egg_internal(self, is_egg):
        self.__is_egg = is_egg

    def _get_condition_internal(self):
        return self.__condition

    def _set_condition_internal(self, condition):
        self.__condition = condition

    def _get_nickname_internal(self):
        return self.__nickname

    def _set_nickname_internal(self, nickname):
        self.__nickname = nickname

    def _get_gender_internal(self):
        return self.__gender

    def _set_gender_internal(self, gender):
        self.__gender = gender

    def _get_is_shiny_internal(self):
        return self.__is_shiny

    def _set_is_shiny_internal(self, is_shiny):
        self.__is_shiny = is_shiny

    def _get_held_item_internal(self):
        return self.__held_item

    def _set_held_item_internal(self, held_item):
        self.__held_item = held_item

    def _get_nature_internal(self):
        return self.__nature

    def _set_nature_internal(self, nature):
        self.__nature = nature

    def _get_pokerus_duration_internal(self):
        return self.__pokerus_duration

    def _set_pokerus_duration_internal(self, pokerus_duration):
        self.__pokerus_duration = pokerus_duration

    def _get_original_trainer_name_internal(self):
        return self.__original_trainer_name

    def _set_original_trainer_name_internal(self, original_trainer_name):
        self.__original_trainer_name = original_trainer_name

    def _get_original_trainer_id_internal(self):
        return self.__original_trainer_id

    def _set_original_trainer_id_internal(self, original_trainer_id):
        self.__original_trainer_id = original_trainer_id

    def _get_original_trainer_public_id_internal(self):
        return (self.__original_trainer_id & 0xFFFF)

    def _set_original_trainer_public_id_internal(self, original_trainer_public_id):
        self.__original_trainer_id = (self.__original_trainer_id & 0xFFFF) | original_trainer_public_id

    def _get_original_trainer_secret_id_internal(self):
        return ((self.__original_trainer_id & 0xFFFF0000) >> 16)

    def _set_original_trainer_secret_id_internal(self, original_trainer_secret_id):
        self.__original_trainer_id = (self.__original_trainer_id & 0xFFFF0000) | (original_trainer_secret_id << 16)

    def _get_original_trainer_gender_internal(self):
        return self.__original_trainer_gender

    def _set_original_trainer_gender_internal(self, original_trainer_gender):
        self.__original_trainer_gender = original_trainer_gender

    def _get_language_internal(self):
        return self.__language

    def _set_language_internal(self, language):
        self.__language = language

    def _get_current_trainer_friendship_internal(self):
        return self.__current_trainer_friendship

    def _set_current_trainer_friendship_internal(self, current_trainer_friendship):
        self.__current_trainer_friendship = current_trainer_friendship

    def _get_ability_internal(self):
        return self.__ability

    def _set_ability_internal(self, ability):
        self.__ability = ability

    def _get_ball_internal(self):
        return self.__ball

    def _set_ball_internal(self, ball):
        self.__ball = ball

    def _get_level_met_internal(self):
        return self.__level_met

    def _set_level_met_internal(self, level_met):
        self.__level_met = level_met

    def _get_location_met_internal(self):
        return self.__location_met

    def _set_location_met_internal(self, location_met):
        self.__location_met = location_met

    def _get_location_met_as_egg_internal(self):
        return self.__location_met_as_egg

    def _set_location_met_as_egg_internal(self, location_met_as_egg):
        self.__location_met_as_egg = location_met_as_egg

    def _get_original_game_internal(self):
        return self.__original_game

    def _set_original_game_internal(self, original_game):
        self.__original_game = original_game

    def _get_personality_internal(self):
        return self.__personality

    def _set_personality_internal(self, personality):
        self.__personality = personality

    def _get_experience_internal(self):
        return self.__experience

    def _set_experience_internal(self, experience):
        self.__experience = experience

    def _get_level_internal(self):
        return self.__level

    def _set_level_internal(self, level):
        self.__level = level

    def _get_current_hp_internal(self):
        return self.__current_hp

    def _set_current_hp_internal(self, current_hp):
        self.__current_hp = current_hp

    # Just set the abstraction.
    def _set_marking_internal(self, marking, value):
        self._set_markings_map_value_internal(marking, value)

    # Just set the abstraction.
    def _set_ribbon_internal(self, ribbon, value):
        self._set_ribbons_map_value_internal(ribbon, value)

    # Just set the abstraction.
    def _set_contest_stat_internal(self, contest_stat, value):
        self._set_contest_stats_map_value_internal(contest_stat, value)

    # Just set the abstraction.
    def _set_move_internal(self, move, index):
        self._set_moves_list_move_value_internal(move, index)

    # Just set the abstraction.
    def _set_move_pp_internal(self, index, pp):
        self._set_moves_list_pp_value_internal(index, pp)

    # Just set the abstraction.
    def _set_EV_internal(self, EV, value):
        self._set_EVs_map_value_internal(EV, value)

    # Just set the abstraction.
    def _set_IV_internal(self, IV, value):
        self._set_IVs_map_value_internal(IV, value)
