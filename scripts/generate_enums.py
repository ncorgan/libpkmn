#!/usr/bin/env python
#
# Copyright (c) 2014-2015,2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

###############################################################
# This script is to be manually run when LibPKMN's database
# is updated. This isn't called at build-time to remove build-time
# dependencies on PySQLite3 and Unidecode.
###############################################################

from optparse import OptionParser
import os
import sqlite3
from unidecode import unidecode

def get_abilities(c):
    c.execute("SELECT ability_id,name FROM ability_names WHERE local_language_id=9 AND ability_id<10000 ORDER BY ability_id")
    db_responses = c.fetchall()
    abilities = ["NONE = 0"]

    for db_response in db_responses:
        abilities += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").upper().replace("SYNCHRONIZE","SYNCHRONIZE_ABILITY"), db_response[0])]

    return abilities

def get_balls(c):
    c.execute("SELECT id,name FROM balls ORDER BY id")
    db_responses = c.fetchall()
    balls = ["NONE = 0"]

    for db_response in db_responses:
        balls += ["{0} = {1}".format(str(unidecode(db_response[1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper(), db_response[0])]

    balls += ["INVALID = 30000"]

    return balls

def get_contest_stats(c):
    c.execute("SELECT id,identifier FROM contest_types ORDER BY id")
    db_responses = c.fetchall()
    contest_stats = ["NONE = 0"]

    for db_response in db_responses:
        contest_stats += ["{0} = {1}".format(str(db_response[1]).upper(), db_response[0])]

    contest_stats += ["FEEL = 6", "SHEEN = 7"]

    return contest_stats

def get_egg_groups(c):
    c.execute("SELECT id,identifier FROM egg_groups ORDER BY id")
    db_responses = c.fetchall()
    egg_groups = ["NONE = 0"]

    for db_response in db_responses:
        egg_groups += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").upper().replace("NO_EGGS","UNDISCOVERED"), db_response[0])]

    return egg_groups

def get_game_save_types():
    return [
        "NONE = 0",
        "RED_BLUE = 1",
        "YELLOW = 2",
        "GOLD_SILVER = 3",
        "CRYSTAL = 4",
        "RUBY_SAPPHIRE = 5",
        "EMERALD = 6",
        "FIRERED_LEAFGREEN = 7",
        "COLOSSEUM_XD = 8"
    ]

def get_items(c):
    c.execute("SELECT item_id,name FROM item_names WHERE local_language_id=9 ORDER BY item_id")
    db_responses = c.fetchall()
    items = ["NONE = 0"]

    dna_sample_num = 1

    for db_response in db_responses:
        item_name = str(unidecode(db_response[1])).upper()

        tokens_to_replace_with_underscore = ["-","(",")"," "]
        tokens_to_remove = [".","'",","]

        # Is there a nice way to do this that works in both Python 2+3?
        for token in tokens_to_replace_with_underscore:
            item_name = item_name.replace(token, "_")
        for token in tokens_to_remove:
            item_name = item_name.replace(token, "")

        if "DNA_SAMPLE" in item_name:
            item_name = "DNA_SAMPLE_{0}".format(dna_sample_num)
            dna_sample_num += 1

        items += ["{0} = {1}".format(item_name, db_response[0])]

    items += ["INVALID = 30000"]

    return items

def get_markings():
    markings = [
        "NONE = 0",
        "CIRCLE = 1",
        "TRIANGLE = 2",
        "SQUARE = 3",
        "HEART = 4",
        "STAR = 5",
        "DIAMOND = 6",
    ]

    return markings

def get_moves(c):
    c.execute("SELECT move_id,name FROM move_names WHERE local_language_id=9 ORDER BY move_id")
    db_responses = c.fetchall()
    moves = ["NONE = 0"]

    for db_response in db_responses:
        moves += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").replace("'","").upper().replace("SING","SING_MOVE"), db_response[0])]

    moves += ["INVALID = 30000"]

    return moves

def get_move_damage_classes(c):
    c.execute("SELECT id,identifier FROM move_damage_classes ORDER BY id")
    db_responses = c.fetchall()
    move_damage_classes = ["NONE = 0"]

    for db_response in db_responses:
        move_damage_classes += ["{0} = {1}".format(str(db_response[1]).upper(), db_response[0])]

    return move_damage_classes

def get_move_targets(c):
    c.execute("SELECT id,identifier FROM move_targets ORDER BY id")
    db_responses = c.fetchall()
    move_targets = ["NONE = 0"]

    for db_response in db_responses:
        move_targets += ["{0} = {1}".format(str(unidecode(db_response[1])).upper().replace("-","_"), db_response[0])]

    return move_targets

def get_natures(c):
    c.execute("SELECT game_index,identifier FROM natures ORDER BY game_index")
    db_responses = c.fetchall()
    natures = ["NONE = 0"]

    for db_response in db_responses:
        natures += ["{0} = {1}".format(str(db_response[1]).upper(), int(db_response[0])+1)]

    return natures

def get_species(c):
    c.execute("SELECT pokemon_species_id,name FROM pokemon_species_names WHERE local_language_id=9 ORDER BY pokemon_species_id")
    db_responses = c.fetchall()
    species = ["NONE = 0"]

    for db_response in db_responses:
        if db_response[0] == 29:
            species += ["NIDORAN_F = 29"]
        elif db_response[0] == 32:
            species += ["NIDORAN_M = 32"]
        else:
            species_name = str(unidecode(db_response[1])).upper()
            tokens_to_replace_with_underscore = ["-", ": ", " ", ":"]
            tokens_to_remove = ["'", "."]

            # Is there a nice way to do this that works in both Python 2+3?
            for token in tokens_to_replace_with_underscore:
                species_name = species_name.replace(token, "_")
            for token in tokens_to_remove:
                species_name = species_name.replace(token, "")

            species += ["{0} = {1}".format(species_name, db_response[0])]

    species += ["INVALID = 30000"]

    return species

def get_stats():
    stats = [
        "NONE = 0",
        "HP = 1",
        "ATTACK = 2",
        "DEFENSE = 3",
        "SPEED = 4",
        "SPECIAL = 5",
        "SPECIAL_ATTACK = 6",
        "SPECIAL_DEFENSE = 7"
    ]

    return stats

def get_types(c):
    c.execute("SELECT type_id,name FROM type_names WHERE local_language_id=9 ORDER BY type_id")
    db_responses = c.fetchall()
    types = ["NONE = 0"]

    for db_response in db_responses:
        types += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper(), db_response[0])]

    types += ["INVALID = 30000"]

    return types

def generate_cpp_enum_file(enum_name, enum_values, has_pair_using = False):
    file_contents = """/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_{0}_HPP
#define PKMN_ENUMS_{0}_HPP

""".format(enum_name.upper())

    if has_pair_using:
        file_contents += """#include <utility>

"""

    file_contents += """namespace pkmn
{{
    enum class e_{0}
    {{""".format(enum_name)

    for enum_value in enum_values:
        file_contents += """
        {0},""".format(enum_value)

    file_contents += """
    };"""

    if has_pair_using:
        file_contents += """

    using {0}_pair_t = std::pair<pkmn::e_{0}, pkmn::e_{0}>;""".format(enum_name)

    file_contents += """
}}

#endif /* PKMN_ENUMS_{0}_HPP */
""".format(enum_name.upper())

    with open("{0}.hpp".format(enum_name), "w") as output:
        output.write(file_contents)

def generate_c_enum_file(enum_name, enum_values, has_pair_struct = False, has_map = False, map_value_type = "int"):
    file_contents = """/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_{0}_H
#define PKMN_C_ENUMS_{0}_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_{1}
{{""".format(enum_name.upper(), enum_name)

    for enum_value in enum_values:
        file_contents += """
    PKMN_{0}_{1},""".format(enum_name.upper(), enum_value)

    file_contents += """
}};

struct pkmn_{0}_enum_list
{{
    enum pkmn_{0}* p_enums;
    size_t length;
}};""".format(enum_name)

    if has_pair_struct:
        file_contents += """

struct pkmn_{0}_enum_pair
{{
    enum pkmn_{0} first;
    enum pkmn_{0} second;
}};""".format(enum_name)

    if has_map:
        file_contents += """

struct pkmn_{0}_enum_map
{{
    {1}* p_values;
    size_t length;
}};""".format(enum_name, map_value_type)

    file_contents += """

#ifdef __cplusplus
extern \"C\" {{
#endif

PKMN_C_API enum pkmn_error pkmn_{0}_enum_list_free(
    struct pkmn_{0}_enum_list* p_{0}_enum_list
);""".format(enum_name)

    if has_map:
        file_contents += """

PKMN_C_API enum pkmn_error pkmn_{0}_enum_map_free(
    struct pkmn_{0}_enum_map* p_{0}_enum_map
);""".format(enum_name)

    file_contents += """

#ifdef __cplusplus
}}
#endif

#endif /* PKMN_C_ENUMS_{0}_H */
""".format(enum_name.upper())

    with open("{0}.h".format(enum_name), "w") as output:
        output.write(file_contents)

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--database-path", type="string", help="LibPKMN database location")
    parser.add_option("--include-dir", type="string", help="Output directory")
    parser.add_option("--cpp", action="store_true", help="Generate C++ enums")
    parser.add_option("--c", action="store_true", help="Generate C enums")
    parser.add_option("--all", action="store_true", help="Generate all enums")
    (options,args) = parser.parse_args()

    conn = sqlite3.connect(options.database_path)
    c = conn.cursor()

    include_dir = ""
    if os.path.isabs(options.include_dir):
        include_dir = options.include_dir
    else:
        script_dir = os.path.dirname(os.path.realpath(__file__))
        include_dir = os.path.join(script_dir, options.include_dir)

    abilities = get_abilities(c)
    balls = get_balls(c)
    contest_stats = get_contest_stats(c)
    egg_groups = get_egg_groups(c)
    game_save_types = get_game_save_types()
    items = get_items(c)
    moves = get_moves(c)
    markings = get_markings()
    move_damage_classes = get_move_damage_classes(c)
    move_targets = get_move_targets(c)
    natures = get_natures(c)
    species = get_species(c)
    stats = get_stats()
    types = get_types(c)

    if options.cpp or options.all:
        os.chdir(os.path.join(include_dir, "pkmn/enums"))

        generate_cpp_enum_file("ability", abilities, True)
        generate_cpp_enum_file("ball", balls)
        generate_cpp_enum_file("contest_stat", contest_stats)
        generate_cpp_enum_file("egg_group", egg_groups, True)
        generate_cpp_enum_file("game_save_type", game_save_types)
        generate_cpp_enum_file("item", items)
        generate_cpp_enum_file("marking", markings)
        generate_cpp_enum_file("move", moves)
        generate_cpp_enum_file("move_damage_class", move_damage_classes)
        generate_cpp_enum_file("move_target", move_targets)
        generate_cpp_enum_file("nature", natures)
        generate_cpp_enum_file("species", species)
        generate_cpp_enum_file("stat", stats)
        generate_cpp_enum_file("type", types, True)

    if options.c or options.all:
        os.chdir(os.path.join(include_dir, "pkmn-c/enums"))

        generate_c_enum_file("ability", abilities, True)
        generate_c_enum_file("ball", balls)
        generate_c_enum_file("contest_stat", contest_stats, has_map=True)
        generate_c_enum_file("egg_group", egg_groups, True)
        generate_c_enum_file("game_save_type", game_save_types)
        generate_c_enum_file("item", items)
        generate_c_enum_file("marking", markings, has_map = True, map_value_type = "bool")
        generate_c_enum_file("move", moves)
        generate_c_enum_file("move_damage_class", move_damage_classes)
        generate_c_enum_file("move_target", move_targets)
        generate_c_enum_file("nature", natures)
        generate_c_enum_file("species", species)
        generate_c_enum_file("stat", stats, has_map=True)
        generate_c_enum_file("type", types, True)
