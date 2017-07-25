#!/usr/bin/env python
#
# Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to generate the CamelCase
# names for LibPKMN's classes and functions.
################################################################

import CppHeaderParser
import datetime
from optparse import OptionParser
import os

header_text = """/*
 * This file was generated: %s
 */""" % datetime.datetime.now()

ignored_classes = ["item_bag",
                   "item_list",
                   "PKMN_API"]

ignored_files = ["config.hpp",
                 "shared_ptr.hpp"]

CppHeaderParser.ignoreSymbols += ["PKMN_API", "PKMN_INLINE", "PKMN_CONSTEXPR_OR_INLINE"]

replacements = dict(Ev = "EV",
                    Gb = "GB",
                    Id = "ID",
                    Iv = "IV",
                    Pp = "PP",
                    Pc = "PC",
                    TmHm = "TMHM",
                    Pksav = "PKSav",
                    Libpkmgc = "LibPkmGC",
                    Sqlite3 = "SQLite3",
                    Sqlitecpp = "SQLiteCpp",
                    Generatespindaspriteatfilepath = "GenerateSpindaSpriteAtFilepath"
                   )

def generate_new_name(old_name, pascal):
    if old_name.endswith("_t"):
        old_name = old_name.replace("_t","")

    if pascal:
        new_name = old_name[0].upper() + ''.join(x for x in old_name.title()[1:] if not (x.isspace() or x == '_'))
    else:
        new_name = old_name[0].lower() + ''.join(x for x in old_name.title()[1:] if not (x.isspace() or x == '_'))

    for replacement in replacements:
        new_name = new_name.replace(replacement, replacements[replacement])

    if new_name == "GetEVolutions":
        new_name = "GetEvolutions"
    elif new_name == "getEVolutions":
        new_name = "getEvolutions"
    elif new_name == "GetType":
        new_name = "GetMoveType"

    return new_name

def generate_rename_line(old_name, pascal):
    return "" if (len(old_name) == 0 or "anon" in old_name.lower()) else "%%rename(%s) %s;" % (generate_new_name(old_name, pascal), old_name)

def convert_header(header,java):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower():
            output += generate_rename_line(str(fcn["name"]), (False if java else True)) + "\n"

    for cls in header.classes:
        if cls not in ignored_classes:
            output += generate_rename_line(str(cls), True) + "\n"

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["constructor"] and not fcn["destructor"]:
                output += generate_rename_line(fcn["name"], (False if java else True)) + "\n"

        for var in header.classes[cls]["properties"]["public"]:
            output += generate_rename_line(str(var["name"]), (False if java else True)) + "\n"

        for enum in header.classes[cls]._public_enums:
            output += generate_rename_line(str(header.classes[cls]._public_enums[enum]["name"]), True) + "\n"

    return output

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-dir", type="string", help="LibPKMN include directory")
    parser.add_option("--output-file", type="string", help="Output file")
    parser.add_option("--java", action="store_true", help="Building Java?")
    (options,args) = parser.parse_args()

    output = header_text + "\n\n"

    os.chdir(options.include_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            if file.endswith(".hpp") and file not in ignored_files:
                output += convert_header(CppHeaderParser.CppHeader(os.path.join(root, file)), options.java)

    f = open(options.output_file, 'w')
    f.write(output)
    f.close()
