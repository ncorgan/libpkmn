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

import datetime
from optparse import OptionParser
import os
import sqlite3
from unidecode import unidecode

def get_abilities(c):
    c.execute("SELECT ability_id,name FROM ability_names WHERE local_language_id=9 AND ability_id<10000 ORDER BY ability_id")
    db_responses = c.fetchall()
    abilities = ["NONE = 0"]

    for db_response in db_responses:
        abilities += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").upper(), db_response[0])]

    return abilities

def get_balls(c):
    c.execute("SELECT id,name FROM balls ORDER BY id")
    db_responses = c.fetchall()
    balls = ["NONE = 0"]

    for db_response in db_responses:
        balls += ["{0} = {1}".format(str(unidecode(db_response[1])).replace("-","_").replace(" ","_").replace(".","").replace("'","").upper(), db_response[0])]

    balls += ["INVALID = 30000"]

    return balls

def get_egg_groups(c):
    c.execute("SELECT id,identifier FROM egg_groups ORDER BY id")
    db_responses = c.fetchall()
    egg_groups = ["NONE = 0"]

    for db_response in db_responses:
        egg_groups += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").upper().replace("NO_EGGS","UNDISCOVERED"), db_response[0])]

    return egg_groups

def get_items(c):
    c.execute("SELECT item_id,name FROM item_names WHERE local_language_id=9 ORDER BY item_id")
    db_responses = c.fetchall()
    items = ["NONE = 0"]

    dna_sample_num = 1

    for db_response in db_responses:
        item_name = str(unidecode(db_response[1])).upper()

        tokens_to_replace_with_underscore = ["-","(",")"," "]
        tokens_to_remove = [".","'"]

        # Is there a nice way to do this that works in both Python 2+3?
        for token in tokens_to_replace_with_underscore:
            item_name = item_name.replace(token, "_")
        for token in tokens_to_remove:
            item_name = item_name.replace(token, "")

        if item_name == "DNA_SAMPLE":
            item_name += "_{0}".format(dna_sample_num)
            dna_sample_num += 1

        items += ["{0} = {1}".format(item_name, db_response[0])]

    items += ["INVALID = 30000"]

    return items

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

def get_natures(c):
    c.execute("SELECT nature_id,name FROM nature_names WHERE local_language_id=9 ORDER BY nature_id")
    db_responses = c.fetchall()
    natures = ["NONE = 0"]

    for db_response in db_responses:
        natures += ["{0} = {1}".format(str(db_response[1]).upper(), db_response[0])]

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

def get_types(c):
    c.execute("SELECT type_id,name FROM type_names WHERE local_language_id=9 ORDER BY type_id")
    db_responses = c.fetchall()
    types = ["NONE = 0"]

    for db_response in db_responses:
        types += ["{0} = {1}".format(str(db_response[1]).replace("-","_").replace(" ","_").replace("???","QUESTION_MARK").upper(), db_response[0])]

    types += ["INVALID = 30000"]

    return types

def generate_enum_file(enum_name, enum_values, output_dir):
    file_contents = """/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 *
 * This file was generated: {0}
 */

#ifndef PKMN_ENUMS_{1}_HPP
#define PKMN_ENUMS_{1}_HPP

namespace pkmn
{{
    enum class e_{2}
    {{""".format(datetime.datetime.now(), enum_name.upper(), enum_name)

    for enum_value in enum_values:
        file_contents += """
        {0},""".format(enum_value)

    file_contents += """
    }};
}}

#endif /* PKMN_ENUMS_{0}_HPP */
""".format(enum_name.upper())

    with open("{0}.hpp".format(enum_name), "w") as output:
        output.write(file_contents)

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--database-path", type="string", help="LibPKMN database location")
    parser.add_option("--output-dir", type="string", help="Output directory")
    (options,args) = parser.parse_args()

    conn = sqlite3.connect(options.database_path)
    c = conn.cursor()

    os.chdir(options.output_dir)

    generate_enum_file("ability", get_abilities(c), options.output_dir)
    generate_enum_file("ball", get_balls(c), options.output_dir)
    generate_enum_file("egg_group", get_egg_groups(c), options.output_dir)
    generate_enum_file("item", get_items(c), options.output_dir)
    generate_enum_file("move", get_moves(c), options.output_dir)
    generate_enum_file("move_damage_class", get_move_damage_classes(c), options.output_dir)
    generate_enum_file("nature", get_natures(c), options.output_dir)
    generate_enum_file("species", get_species(c), options.output_dir)
    generate_enum_file("type", get_types(c), options.output_dir)
