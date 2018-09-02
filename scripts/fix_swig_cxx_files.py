#!/usr/bin/env python
#
# Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to add Doxygen tags to
# SWIG's C# output and perform other miscellaneous fixes
################################################################

import datetime
from optparse import OptionParser
import os

#
# SWIG has issues with STL containers of enum classes, which are common in
# LibPKMN. It will erroneously add an extra "enum" in front of the type.
# For example, "pkmn::e_stat" becomes "enum pkmn::e_stat".
#
# See: https://github.com/swig/swig/issues/464
#
def remove_extra_enum_specifier(filepath):
    f = open(filepath, "r")
    flines = f.readlines()
    f.close()

    # Already done
    if "remove_extra_enum_specifier" in flines[0]:
        return

    # Find and fix the appropriate line
    for line_num in range(len(flines)):
        if "enum pkmn::e" in flines[line_num]:
            flines[line_num] = flines[line_num].replace(
                                   "enum pkmn::e",
                                   "pkmn::e"
                               )

    f = open(filepath, "w")
    f.write("// remove_extra_enum_specifier\n")
    for line in flines:
        f.write(line)
    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--source-file", type="string", help="SWIG-generated .cxx file")
    (options,args) = parser.parse_args()

    filepath = os.path.abspath(options.source_file)
    remove_extra_enum_specifier(filepath)
