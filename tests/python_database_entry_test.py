#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn
import sys
import traceback

def check_close(num1,num2):
    return abs(num1-num2) < 0.0001

def item_entry_test():
    try:
        # Make sure trying to create an invalid entry results in an error
        try:
            entry = pkmn.database.item_entry("Not an item", "X")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        try:
            entry = pkmn.database.item_entry("Potion", "Not a game")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        entry = pkmn.database.item_entry("Salamencite", "Omega Ruby")

        name = entry.get_name()
        if name != "Salamencite":
            raise RuntimeError("name (\"{0}\") != \"Salamencite\"".format(name))

        game = entry.get_game()
        if game != "Omega Ruby":
            raise RuntimeError("game (\"{0}\") != \"Omega Ruby\"".format(game))

        category = entry.get_category()
        if category != "Mega Stones":
            raise RuntimeError("category (\"{0}\") != \"Mega Stones\"".format(category))

        pocket = entry.get_pocket()
        if pocket != "Items":
            raise RuntimeError("pocket (\"{0}\") != \"Items\"".format(pocket))

        # Just make sure it works
        description = entry.get_description()

        cost = entry.get_cost()
        if cost != 0:
            raise RuntimeError("cost ({0}) != 0".format(cost))

        holdable = entry.holdable()
        if not entry.holdable():
            raise RuntimeError("not entry.holdable()")

        fling_power = entry.get_fling_power()
        if fling_power != 0:
            raise RuntimeError("fling_power ({0}) != 0".format(fling_power))

        fling_effect = entry.get_fling_effect()
        if fling_effect != "None":
            raise RuntimeError("fling_effect (\"{0}\") != \"None\"".format(fling_effect))

        entry_same = pkmn.database.item_entry("Salamencite", "Omega Ruby")
        entry_different_item = pkmn.database.item_entry("Potion", "Omega Ruby")
        entry_different_game = pkmn.database.item_entry("Salamencite", "Alpha Sapphire")

        if entry != entry_same:
            raise RuntimeError("entry != entry_same")
        if entry == entry_different_item:
            raise RuntimeError("entry == entry_different_item")
        if entry == entry_different_game:
            raise RuntimeError("entry == entry_different_game")

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def move_entry_test():
    try:
        # Make sure trying to create an invalid entry results in an error
        try:
            entry = pkmn.database.move_entry("Not a move", "Pearl")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        try:
            entry = pkmn.database.move_entry("Bullet Punch", "Not a game")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        entry = pkmn.database.move_entry("Bullet Punch", "Pearl")

        name = entry.get_name()
        if name != "Bullet Punch":
            raise RuntimeError("name (\"{0}\") != \"Bullet Punch\"".format(name))

        game = entry.get_game()
        if game != "Pearl":
            raise RuntimeError("game (\"{0}\") != \"Pearl\"".format(game))

        move_type = entry.get_type()
        if move_type != "Steel":
            raise RuntimeError("move_type (\"{0}\") != \"Steel\"".format(move_type))

        # Just make sure it works
        description = entry.get_description()

        target = entry.get_target()
        if not target.startswith("Selected"):
            raise RuntimeError("not target.startswith(\"Selected\"), instead \"{0}\"".format(target))

        damage_class = entry.get_damage_class()
        if damage_class != "Physical":
            raise RuntimeError("damage_class (\"{0}\") != \"Physical\"".format(damage_class))

        base_power = entry.get_base_power()
        if base_power != 40:
            raise RuntimeError("base_power ({0}) != 40".format(base_power))

        pps = [30,36,42,48]
        for i in range(len(pps)):
            pp = entry.get_pp(i)
            if pp != pps[i]:
                raise RuntimeError("pp ({0}) != {1}".format(pp, pps[i]))

        accuracy = entry.get_accuracy()
        if not check_close(accuracy, 1.0):
            raise RuntimeError("accuracy ({0}) != 1.0".format(accuracy))

        priority = entry.get_priority()
        if priority != 1:
            raise RuntimeError("priority ({0}) != 1".format(priority))

        # Just make sure it works
        effect = entry.get_effect()

        contest_type = entry.get_contest_type()
        if contest_type != "Smart":
            raise RuntimeError("contest_type (\"{0}\") != \"Smart\"".format(contest_type))

        contest_effect = entry.get_contest_effect()
        if contest_effect != "None":
            raise RuntimeError("contest_effect (\"{0}\") != \"None\"".format(contest_effect))

        super_contest_effect = entry.get_super_contest_effect()
        if super_contest_effect == "None":
            raise RuntimeError("super_contest_effect (\"{0}\") == \"None\"".format(super_contest_effect))

        entry_same = pkmn.database.move_entry("Bullet Punch", "Pearl")
        entry_different_move = pkmn.database.move_entry("Roost", "Pearl")
        entry_different_game = pkmn.database.move_entry("Bullet Punch", "Diamond")

        if entry != entry_same:
            raise RuntimeError("entry != entry_same")
        if entry == entry_different_move:
            raise RuntimeError("entry == entry_different_move")
        if entry == entry_different_game:
            raise RuntimeError("entry == entry_different_game")

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def pokemon_entry_test():
    try:
        # Make sure trying to create an invalid entry results in an error
        try:
            entry = pkmn.database.pokemon_entry("Invalid", "Pearl", "")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        try:
            entry = pkmn.database.pokemon_entry("Bulbasaur", "Not a game", "")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        entry = pkmn.database.pokemon_entry("Shroomish", "XD", "")

        name = entry.get_name()
        if name != "Shroomish":
            raise RuntimeError("name (\"{0}\") != \"Shroomish\"".format(name))

        game = entry.get_game()
        if game != "XD":
            raise RuntimeError("game (\"{0}\") != \"XD\"".format(game))

        species = entry.get_species()
        if species != "Mushroom":
            raise RuntimeError("species (\"{0}\") != \"Mushroom\"".format(species))

        form = entry.get_form()
        if form != "Standard":
            raise RuntimeError("form (\"{0}\") != \"Standard\"".format(form))

        pokedex_entry = entry.get_pokedex_entry()
        if len(pokedex_entry) == 0:
            raise RuntimeError("len(pokedex_entry) == 0")

        height = entry.get_height()
        if not check_close(height, 0.4):
            raise RuntimeError("height ({0}) != 0.4", height)

        weight = entry.get_weight()
        if not check_close(weight, 4.5):
            raise RuntimeError("weight ({0}) != 4.5", weight)

        chance_male = entry.get_chance_male()
        if not check_close(chance_male, 0.5):
            raise RuntimeError("chance_male ({0}) != 0.5", chance_male)

        chance_female = entry.get_chance_female()
        if not check_close(chance_female, 0.5):
            raise RuntimeError("chance_female ({0}) != 0.5", chance_female)

        has_gender_differences = entry.has_gender_differences()
        if has_gender_differences:
            raise RuntimeError("has_gender_differences")

        base_happiness = entry.get_base_happiness()
        if base_happiness != 70:
            raise RuntimeError("base_happiness {0} != 70".format(base_happiness))

        types = entry.get_types()
        if types[0] != "Grass":
            raise RuntimeError("types[0] (\"{0}\") != \"Grass\"".format(types[0]))
        if types[1] != "None":
            raise RuntimeError("types[0] (\"{0}\") != \"None\"".format(types[1]))

        abilities = entry.get_abilities()
        if abilities[0] != "Effect Spore":
            raise RuntimeError("abilities[0] (\"{0}\") != \"Effect Spore\"".format(abilities[0]))
        if abilities[1] != "Poison Heal":
            raise RuntimeError("abilities[1] (\"{0}\") != \"Poison Heal\"".format(abilities[1]))

        hidden_ability = entry.get_hidden_ability()
        if hidden_ability != "None":
            raise RuntimeError("hidden_ability (\"{0}\") != \"None\"".format(hidden_ability))

        egg_groups = entry.get_egg_groups()
        if egg_groups[0] != "Fairy":
            raise RuntimeError("egg_groups[0] (\"{0}\") != \"Fairy\"".format(egg_groups[0]))
        if egg_groups[1] != "Grass":
            raise RuntimeError("egg_groups[1] (\"{0}\") != \"Grass\"".format(egg_groups[1]))

        base_stats = entry.get_base_stats()
        if base_stats["HP"] != 60:
            raise RuntimeError("base_stats[\"HP\"] ({0}) != 60".format(base_stats["HP"]))
        if base_stats["Attack"] != 40:
            raise RuntimeError("base_stats[\"Attack\"] ({0}) != 40".format(base_stats["Attack"]))
        if base_stats["Defense"] != 60:
            raise RuntimeError("base_stats[\"Defense\"] ({0}) != 60".format(base_stats["Defense"]))
        if base_stats["Speed"] != 35:
            raise RuntimeError("base_stats[\"Speed\"] ({0}) != 35".format(base_stats["Speed"]))
        if base_stats["Special Attack"] != 40:
            raise RuntimeError("base_stats[\"Special Attack\"] ({0}) != 40".format(base_stats["Special Attack"]))
        if base_stats["Special Defense"] != 60:
            raise RuntimeError("base_stats[\"Special Defense\"] ({0}) != 60".format(base_stats["Special Defense"]))

        EV_yields = entry.get_EV_yields()
        if EV_yields["HP"] != 1:
            raise RuntimeError("EV_yields[\"HP\"] ({0}) != 1".format(EV_yields["HP"]))
        if EV_yields["Attack"] != 0:
            raise RuntimeError("EV_yields[\"Attack\"] ({0}) != 0".format(EV_yields["Attack"]))
        if EV_yields["Defense"] != 0:
            raise RuntimeError("EV_yields[\"Defense\"] ({0}) != 0".format(EV_yields["Defense"]))
        if EV_yields["Speed"] != 0:
            raise RuntimeError("EV_yields[\"Speed\"] ({0}) != 0".format(EV_yields["Speed"]))
        if EV_yields["Special Attack"] != 0:
            raise RuntimeError("EV_yields[\"Special Attack\"] ({0}) != 0".format(EV_yields["Special Attack"]))
        if EV_yields["Special Defense"] != 0:
            raise RuntimeError("EV_yields[\"Special Defense\"] ({0}) != 0".format(EV_yields["Special Defense"]))

        experience_yield = entry.get_experience_yield()
        if experience_yield != 65:
            raise RuntimeError("experience_yield {0} != 65".format(experience_yield))

        experience_at_level = entry.get_experience_at_level(50)
        if experience_at_level != 142500:
            raise RuntimeError("experience_at_level {0} != 142500".format(experience_at_level))

        level_at_experience = entry.get_level_at_experience(200000)
        if level_at_experience != 55:
            raise RuntimeError("level_at_experience {0} != 55".format(level_at_experience))

        levelup_moves = entry.get_levelup_moves()
        if len(levelup_moves) == 0:
            raise RuntimeError("len(levelup_moves) == 0")

        tm_hm_moves = entry.get_tm_hm_moves()
        if len(tm_hm_moves) == 0:
            raise RuntimeError("len(tm_hm_moves) == 0")

        egg_moves = entry.get_egg_moves()
        if len(egg_moves) == 0:
            raise RuntimeError("len(egg_moves) == 0")

        tutor_moves = entry.get_tutor_moves()
        if len(tutor_moves) == 0:
            raise RuntimeError("len(tutor_moves) == 0")

        # TODO: Shadow
        forms = entry.get_forms()
        if len(forms) != 1:
            raise RuntimeError("len(forms) ({0}) != 1".format(len(forms)))

        evolutions = entry.get_evolutions()
        if len(evolutions) != 1:
            raise RuntimeError("len(evolutions) ({0}) != 1".format(len(evolutions)))

        # Use different Pokemon for testing (in)equality
        entry_first = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
        entry_same = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Standard")
        entry_different_pokemon = pkmn.database.pokemon_entry("Pichu", "Omega Ruby", "Standard")
        entry_different_game = pkmn.database.pokemon_entry("Pikachu", "Alpha Sapphire", "Standard")
        entry_different_form = pkmn.database.pokemon_entry("Pikachu", "Omega Ruby", "Belle")

        if entry_first != entry_same:
            raise RuntimeError("entry_first != entry_same")
        if entry_first == entry_different_pokemon:
            raise RuntimeError("entry_first == entry_different_pokemon")
        if entry_first == entry_different_game:
            raise RuntimeError("entry_first == entry_different_game")
        if entry_first == entry_different_form:
            raise RuntimeError("entry_first == entry_different_form")

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = True
    for test in [item_entry_test, move_entry_test, pokemon_entry_test]:
        sys.stdout.write("Starting test %s..." % test.__name__)
        result = test()
        print("success.\n" if result else "failed.\n")
        successful = successful and result

    sys.exit(0 if successful else 1)
