#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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

SPTR_FILES = ["ItemBag.cs",
              "ItemList.cs"]

def hide_sptr_ctors(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-3]
    ctor = "public {0}".format(class_name)

    # Already done
    if "hide_sptr_ctors" in flines[0]:
        return

    # Find first relevant ctor
    for i in range(len(flines)):
        if ctor in flines[i]:
            flines[i-1] = "// \\cond\n"
            flines[i+7] = "// \\endcond\n"
            break

    f = open(filename, "w")
    f.write("// hide_sptr_ctors\n")
    for line in flines:
        f.write(line)
    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--csharp-dir", type="string", help="SWIG *.cs output directory")
    (options,args) = parser.parse_args()

    os.chdir(options.csharp_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for filename in files:
            if filename.endswith(".cs"):
                if filename in SPTR_FILES:
                    hide_sptr_ctors(filename)