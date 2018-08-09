#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

from optparse import OptionParser
import os

IGNORE_FILES = ["enum_to_string.hpp"]

# So as to not conflict with Python keywords or internal libraries
PYTHON_REPLACEMENT_ENUM_NAMES = dict(
    stat = "pokemon_stat",
    type = "pokemon_type"
)

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--enum-dir", type="string", help="Enum directory")
    parser.add_option("--print-as-cmake-list-lua", action="store_true", default=False)
    parser.add_option("--print-as-cmake-list-python", action="store_true", default=False)
    parser.add_option("--print-as-lua-imports", action="store_true", default=False)
    parser.add_option("--print-as-python-imports", action="store_true", default=False)
    (options, args) = parser.parse_args()

    enum_dir = os.path.abspath(options.enum_dir)
    if not os.path.exists(enum_dir):
        raise RuntimeException(enum_dir + "does not exist")

    lua_enums = []
    python_enums = []
    for _, _, files in os.walk(enum_dir):
        for filename in files:
            if (filename not in IGNORE_FILES) and filename.endswith(".hpp"):
                enum_name = filename.split(".")[0]
                lua_enums += [enum_name]
                python_enums += [PYTHON_REPLACEMENT_ENUM_NAMES.get(enum_name, enum_name)]

    lua_enums.sort()
    python_enums.sort()

    if options.print_as_cmake_list_lua:
        print(";".join(lua_enums))

    if options.print_as_cmake_list_python:
        print(";".join(python_enums))

    if options.print_as_lua_imports:
        print("\n".join(["pkmn.{0} = require(\"{0}\")".format(enum) for enum in lua_enums]))

    if options.print_as_python_imports:
        print("\n".join(["from . import {0}".format(enum) for enum in python_enums]))
