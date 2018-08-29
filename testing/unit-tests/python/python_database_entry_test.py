#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import os
import sys
import unittest

class database_entry_test(unittest.TestCase):

    def test_item_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry(
                        pkmn.item.INVALID,
                        pkmn.game.SOULSILVER
                    )
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry(
                        pkmn.item.FAST_BALL,
                        pkmn.game.NONE
                    )
        with self.assertRaises(ValueError):
            entry = pkmn.database.item_entry(
                        pkmn.item.FAST_BALL,
                        pkmn.game.RED
                    )

        entry = pkmn.database.item_entry(
                    pkmn.item.FAST_BALL,
                    pkmn.game.SOULSILVER
                )

        self.assertEqual(entry.item, pkmn.item.FAST_BALL)
        self.assertEqual(entry.name, "Fast Ball")
        self.assertEqual(entry.game, pkmn.game.SOULSILVER)
        self.assertEqual(entry.category, "Apricorn balls")
        self.assertTrue(entry.pocket.startswith("Pok"))

        # Just make sure it works
        description = entry.description

        self.assertEqual(entry.cost, 300)
        self.assertTrue(entry.is_holdable)
        self.assertEqual(entry.fling_power, 0)
        self.assertEqual(entry.fling_effect, "None")

    def test_move_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry(
                        pkmn.move.INVALID,
                        pkmn.game.SILVER
                    )
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry(
                        pkmn.move.OCTAZOOKA,
                        pkmn.game.NONE
                    )
        with self.assertRaises(ValueError):
            entry = pkmn.database.move_entry(
                        pkmn.move.OCTAZOOKA,
                        pkmn.game.RED
                    )

        entry = pkmn.database.move_entry(
                    pkmn.move.OCTAZOOKA,
                    pkmn.game.SILVER
                )

        self.assertEqual(entry.move, pkmn.move.OCTAZOOKA)
        self.assertEqual(entry.name, "Octazooka")
        self.assertEqual(entry.game, pkmn.game.SILVER)
        self.assertEqual(entry.type, pkmn.pokemon_type.WATER)

        # Just make sure it works
        description = entry.description

        self.assertEqual(entry.target, pkmn.move_target.SELECTED_POKEMON)
        self.assertEqual(entry.damage_class, pkmn.move_damage_class.SPECIAL)
        self.assertEqual(entry.base_power, 65)

        pps = [10,12,14,16]
        for i in range(4):
            pp = entry.get_pp(i)
            self.assertEqual(pp, pps[i])

        self.assertAlmostEqual(entry.accuracy, 0.85)
        self.assertEqual(entry.priority, 0)

        # Just make sure it works
        effect = entry.effect

        self.assertEqual(entry.contest_type, pkmn.contest_stat.NONE)
        self.assertEqual(entry.contest_effect, "None")
        self.assertEqual(entry.super_contest_effect, "None")

    def test_pokemon_entry(self):
        # Make sure trying to create an invalid entry results in an error
        with self.assertRaises(ValueError):
            entry = pkmn.database.pokemon_entry(
                        pkmn.species.CASTFORM,
                        pkmn.game.NONE,
                        "Sunny"
                    )
        with self.assertRaises(ValueError):
            entry = pkmn.database.pokemon_entry(
                        pkmn.species.CASTFORM,
                        pkmn.game.BLACK2,
                        "Not a form"
                    )

        entry = pkmn.database.pokemon_entry(
                    pkmn.species.STUNFISK,
                    pkmn.game.BLACK2,
                    ""
                )

        self.assertEqual(entry.species, pkmn.species.STUNFISK)
        self.assertEqual(entry.species_name, "Stunfisk")
        self.assertEqual(entry.game, pkmn.game.BLACK2)
        self.assertEqual(entry.category, "Trap")
        self.assertEqual(entry.form, "Standard")
        self.assertGreater(len(entry.pokedex_entry), 0)
        self.assertAlmostEqual(entry.height, 0.7)
        self.assertAlmostEqual(entry.weight, 11.0)
        self.assertAlmostEqual(entry.chance_male, 0.5)
        self.assertAlmostEqual(entry.chance_female, 0.5)
        self.assertFalse(entry.has_gender_differences)
        self.assertEqual(entry.base_friendship, 70)

        types = entry.types
        self.assertEqual(types[0], pkmn.pokemon_type.GROUND)
        self.assertEqual(types[1], pkmn.pokemon_type.ELECTRIC)

        abilities = entry.abilities
        self.assertEqual(abilities[0], pkmn.ability.STATIC)
        self.assertEqual(abilities[1], pkmn.ability.LIMBER)

        self.assertEqual(entry.hidden_ability, pkmn.ability.SAND_VEIL)

        egg_groups = entry.egg_groups
        self.assertEqual(egg_groups[0], pkmn.egg_group.WATER1)
        self.assertEqual(egg_groups[1], pkmn.egg_group.INDETERMINATE)

        base_stats = entry.base_stats
        self.assertEqual(base_stats[pkmn.pokemon_stat.HP], 109)
        self.assertEqual(base_stats[pkmn.pokemon_stat.ATTACK], 66)
        self.assertEqual(base_stats[pkmn.pokemon_stat.DEFENSE], 84)
        self.assertEqual(base_stats[pkmn.pokemon_stat.SPEED], 32)
        self.assertEqual(base_stats[pkmn.pokemon_stat.SPECIAL_ATTACK], 81)
        self.assertEqual(base_stats[pkmn.pokemon_stat.SPECIAL_DEFENSE], 99)

        EV_yields = entry.EV_yields
        self.assertEqual(EV_yields[pkmn.pokemon_stat.HP], 2)
        self.assertEqual(EV_yields[pkmn.pokemon_stat.ATTACK], 0)
        self.assertEqual(EV_yields[pkmn.pokemon_stat.DEFENSE], 0)
        self.assertEqual(EV_yields[pkmn.pokemon_stat.SPEED], 0)
        self.assertEqual(EV_yields[pkmn.pokemon_stat.SPECIAL_ATTACK], 0)
        self.assertEqual(EV_yields[pkmn.pokemon_stat.SPECIAL_DEFENSE], 0)

        self.assertEqual(entry.experience_yield, 165)
        self.assertEqual(entry.get_experience_at_level(50), 125000)
        self.assertEqual(entry.get_level_at_experience(200000), 58)
        self.assertGreater(len(entry.levelup_moves), 0)
        self.assertGreater(len(entry.tm_hm_moves), 0)
        self.assertGreater(len(entry.egg_moves), 0)
        self.assertGreater(len(entry.tutor_moves), 0)
        self.assertEqual(len(entry.forms), 1)
        self.assertEqual(len(entry.evolutions), 0)

        self.assertTrue(os.path.exists(entry.get_icon_filepath(False)))
        self.assertTrue(os.path.exists(entry.get_icon_filepath(True)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(False,False)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(False,True)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(True,False)))
        self.assertTrue(os.path.exists(entry.get_sprite_filepath(True,True)))

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(database_entry_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
