#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import CppHeaderParser

from optparse import OptionParser
import os

# Enum classes will be parsed as classes without this. None of our
# enum headers have classes, so it will be fine.
CppHeaderParser.ignoreSymbols += ["class"]

IGNORE_FILES = ["enum_to_string.hpp"]

# So as to not conflict with Python keywords or internal libraries
PYTHON_REPLACEMENT_FILENAMES = dict(
    stat = "pokemon_stat",
    type = "pokemon_type"
)

def generate_python_enum(header_path, output_dir):
    header = CppHeaderParser.CppHeader(header_path)
    for enum in header.enums:
        enum_name = enum["name"][2:]
        filename = "{0}.py".format(PYTHON_REPLACEMENT_FILENAMES.get(enum_name, enum_name))
        output_text= """#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#
"""
        for value in enum["values"]:
            output_text += "\n{0} = {1}".format(value["name"], value["value"])

        output_path = os.path.join(output_dir, filename)
        with open(output_path, "w") as f:
            f.write(output_text)

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--python", action="store_true", default=False)
    parser.add_option("--enum-dir", type="string", help="Enum directory")
    parser.add_option("--output-dir", type="string", help="Where in the build directory to output files")
    (options, args) = parser.parse_args()

    enum_dir = os.path.abspath(options.enum_dir)
    output_dir = os.path.abspath(options.output_dir)

    if not os.path.exists(enum_dir):
        raise RuntimeError("Invalid enum dir: " + enum_dir)
    if not os.path.exists(output_dir):
        raise RuntimeError("Invalid output dir: " + output_dir)

    for root, _, files in os.walk(enum_dir):
        for filename in files:
            if (filename not in IGNORE_FILES) and filename.endswith(".hpp"):
                if options.python:
                    generate_python_enum(os.path.join(root, filename), output_dir)
