#!/usr/bin/env python
#
# Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
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

ignored_classes = ["numeric_attribute_map",
                   "string_attribute_map",
                   "boolean_attribute_map",
                   "PKMN_API"]

ignored_functions = ["size"]

ignored_files = ["config.hpp",
                 "exception.hpp",
                 "shared_ptr.hpp",
                 "attribute_engine.hpp"]

CppHeaderParser.ignoreSymbols += ["PKMN_API", "PKMN_INLINE", "PKMN_CONSTEXPR_OR_INLINE"]

replacements = dict(Ev = "EV",
                    Gb = "GB",
                    Id = "ID",
                    Iv = "IV",
                    Pp = "PP",
                    Pc = "PC",
                    Hp = "HP",
                    Tm = "TM",
                    Hm = "HM",
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
    elif "GetIDeal" in new_name:
        new_name = new_name.replace("GetIDeal", "GetIdeal")
    elif "TMp" in new_name:
        new_name = new_name.replace("TMp", "Tmp")

    return new_name

def generate_rename_line(old_name, pascal):
    return "" if (len(old_name) == 0 or "anon" in old_name.lower()) else "%%rename(%s) %s;" % (generate_new_name(old_name, pascal), old_name)

def convert_header(header,language):
    output = ""

    java = (language == "java")

    if language != "ruby":
        for fcn in header.functions:
            if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower() and fcn["name"] not in ignored_functions:
                output += generate_rename_line(str(fcn["name"]), (False if java else True)) + "\n"

    for cls in header.classes:
        if cls not in ignored_classes:
            output += generate_rename_line(str(cls), True) + "\n"

        if language != "ruby":
            for fcn in header.classes[cls]["methods"]["public"]:
                if "operator" not in fcn["name"].lower() and not fcn["constructor"] and not fcn["destructor"] and fcn["name"] not in ignored_functions:
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
    parser.add_option("--language", type="string", help="Output language")
    (options,args) = parser.parse_args()

    output = header_text + "\n\n"

    os.chdir(options.include_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            if file.endswith(".hpp") and file not in ignored_files:
                output += convert_header(CppHeaderParser.CppHeader(os.path.join(root, file)), options.language)

    f = open(options.output_file, 'a')
    f.write(output)
    f.close()
