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

def ability_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            ability_list = pkmn.database.get_ability_list(0)
        except IndexError:
            pass
        except:
            raise RuntimeError("This should have thrown an IndexError.")

        ability_list = pkmn.database.get_ability_list(6)
        if len(ability_list) != 191:
            raise RuntimeError("len(ability_list) ({0}) != 191".format(len(ability_list)))

        if ability_list[0] != "Adaptability":
            raise RuntimeError("ability_list[0] (\"{0}\") != \"Adaptability\"".format(ability_list[0]))
        if ability_list[-1] != "Zen Mode":
            raise RuntimeError("ability_list[-1] (\"{0}\") != \"Zen Mode\"".format(ability_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def game_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            game_list = pkmn.database.get_game_list(7, False)
        except IndexError:
            pass
        except:
            raise RuntimeError("This should have thrown an IndexError.")

        game_list = pkmn.database.get_game_list(6, True)
        if len(game_list) != 26:
            raise RuntimeError("len(game_list) ({0}) != 26".format(len(game_list)))

        if game_list[0] != "Red":
            raise RuntimeError("game_list[0] (\"{0}\") != \"Red\"".format(game_list[0]))
        if game_list[-1] != "Alpha Sapphire":
            raise RuntimeError("game_list[-1] (\"{0}\") != \"Alpha Sapphire\"".format(game_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def item_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            item_list = pkmn.database.get_item_list("Not a game")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        item_list = pkmn.database.get_item_list("HeartGold")
        if len(item_list) != 513:
            raise RuntimeError("len(item_list) ({0}) != 513".format(len(item_list)))

        if item_list[0] != "Adamant Orb":
            raise RuntimeError("item_list[0] (\"{0}\") != \"Adamant Orb\"".format(item_list[0]))
        if item_list[-1] != "Zoom Lens":
            raise RuntimeError("item_list[-1] (\"{0}\") != \"Zoom Lens\"".format(item_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def location_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            location_list = pkmn.database.get_location_list("Not a game", False)
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        location_list = pkmn.database.get_location_list("Emerald", False)
        if len(location_list) != 106:
            raise RuntimeError("len(location_list) ({0}) != 106".format(len(location_list)))

        if location_list[0] != "Abandoned Ship":
            raise RuntimeError("location_list[0] (\"{0}\") != \"Abandoned Ship\"".format(location_list[0]))
        if location_list[-1] != "Victory Road":
            raise RuntimeError("location_list[-1] (\"{0}\") != \"Victory Road\"".format(location_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def move_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            move_list = pkmn.database.get_move_list("Not a game")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        move_list = pkmn.database.get_move_list("Red")
        if len(move_list) != 165:
            raise RuntimeError("len(move_list) ({0}) != 165".format(len(move_list)))

        if move_list[0] != "Pound":
            raise RuntimeError("move_list[0] (\"{0}\") != \"Pound\"".format(move_list[0]))
        if move_list[-1] != "Struggle":
            raise RuntimeError("move_list[-1] (\"{0}\") != \"Struggle\"".format(move_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def nature_list_test():
    try:
        nature_list = pkmn.database.get_nature_list()
        if len(nature_list) != 25:
            raise RuntimeError("len(nature_list) ({0}) != 25".format(len(nature_list)))

        if nature_list[0] != "Adamant":
            raise RuntimeError("nature_list[0] (\"{0}\") != \"Adamant\"".format(nature_list[0]))
        if nature_list[-1] != "Timid":
            raise RuntimeError("nature_list[-1] (\"{0}\") != \"Timid\"".format(nature_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def pokemon_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            pokemon_list = pkmn.database.get_pokemon_list(0, True)
        except IndexError:
            pass
        except:
            raise RuntimeError("This should have thrown an IndexError.")

        pokemon_list = pkmn.database.get_pokemon_list(1, True)
        if len(pokemon_list) != 151:
            raise RuntimeError("len(pokemon_list) ({0}) != 151".format(len(pokemon_list)))

        if pokemon_list[0] != "Bulbasaur":
            raise RuntimeError("pokemon_list[0] (\"{0}\") != \"Bulbasaur\"".format(pokemon_list[0]))
        if pokemon_list[-1] != "Mew":
            raise RuntimeError("pokemon_list[-1] (\"{0}\") != \"Mew\"".format(pokemon_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def region_list_test():
    try:
        region_list = pkmn.database.get_region_list()
        if len(region_list) != 7:
            raise RuntimeError("len(region_list) ({0}) != 7".format(len(region_list)))

        if region_list[0] != "Kanto":
            raise RuntimeError("region_list[0] (\"{0}\") != \"Kanto\"".format(region_list[0]))
        if region_list[-1] != "Kalos":
            raise RuntimeError("region_list[-1] (\"{0}\") != \"Kalos\"".format(region_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

# TODO when database branch is merged in
def ribbon_list_test():
    ribbon_list = pkmn.database.get_ribbon_list(1)
    return True

def super_training_medal_list_test():
    try:
        super_training_medal_list = pkmn.database.get_super_training_medal_list()
        if len(super_training_medal_list) != 30:
            raise RuntimeError("len(super_training_medal_list) ({0}) != 30".format(len(super_training_medal_list)))

        if super_training_medal_list[0] != "Sp. Atk Level 1":
            raise RuntimeError("super_training_medal_list[0] (\"{0}\") != \"Sp. Atk Level 1\"".format(super_training_medal_list[0]))
        if super_training_medal_list[-1] != "The Battle for the Best!":
            raise RuntimeError("super_training_medal_list[-1] (\"{0}\") != \"The Battle for the Best!\"".format(super_training_medal_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def type_list_test():
    try:
        # Make sure trying to create an invalid list results in an error
        try:
            type_list = pkmn.database.get_type_list("Not a game")
        except ValueError:
            pass
        except:
            raise RuntimeError("This should have thrown a ValueError.")

        type_list = pkmn.database.get_type_list("Alpha Sapphire")
        if len(type_list) != 18:
            raise RuntimeError("len(type_list) ({0}) != 18".format(len(type_list)))

        if type_list[0] != "Normal":
            raise RuntimeError("type_list[0] (\"{0}\") != \"Normal\"".format(type_list[0]))
        if type_list[-1] != "Fairy":
            raise RuntimeError("type_list[-1] (\"{0}\") != \"Fairy\"".format(type_list[-1]))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = True
    for test in [ability_list_test, game_list_test, item_list_test, location_list_test, \
                 move_list_test, nature_list_test, pokemon_list_test, region_list_test, \
                 ribbon_list_test, super_training_medal_list_test, type_list_test]:
        sys.stdout.write("Starting test %s..." % test.__name__)
        result = test()
        print("success.\n" if result else "failed.\n")
        successful = successful and result

    sys.exit(0 if successful else 1)
