#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to add Doxygen tags to
# SWIG's Java output and perform other miscellaneous fixes
################################################################

import datetime
from optparse import OptionParser
import os

SPTR_FILES = ["ItemBag.java",
              "ItemList.java"]

SPTR_FCNS = dict(
                ItemBag = "make_item_bag",
                ItemList = "make_item_list"
            )

# Make sure no spaces between commas
SPTR_PARAMS = dict(
                  ItemBag = "String game",
                  ItemList = "String name,String game"
              )

def hide_ctors(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-5]
    ctor1 = "protected {0}(long cPtr, boolean cMemoryOwn)".format(class_name)
    ctor2 = "public {0}()".format(class_name)

    # Already done
    if "hide_ctors" in flines[0] or "hide_ctors" in flines[1]:
        return

    # Find protected constructor
    first_ctor_end = 0
    for i in range(len(flines)):
        if ctor1 in flines[i]:
            flines[i-1] = "// \\cond\n"
            flines[i+4] = "// \\endcond\n"
            first_ctor_end = i + 5
            break

    # If this is an sptr file, hide other ctors
    if filename in SPTR_FILES:
        for i in range(first_ctor_end, len(flines)):
            if ctor2 in flines[i]:
                flines[i-1] = "// \\cond\n"
                flines[i+7] = "// \\endcond\n"
                break

    f = open(filename, "w")
    f.write("// hide_ctors\n")
    for line in flines:
        f.write(line)
    f.close()

def add_sptr_ctor(filename):
    if filename not in SPTR_FILES:
        return

    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-5]

    # Already done
    if "add_sptr_ctor" in flines[0] or "add_sptr_ctor" in flines[1]:
        return

    # What to pass into function
    shortened_params = ""
    for var in SPTR_PARAMS[class_name].split(","):
        shortened_params += var.split(" ")[1]
        shortened_params += ", "
    shortened_params = shortened_params[:-2]

    # Construct new constructor
    new_ctor = """
public {0}({1}) {{
    this(PKMNJNI.{2}({3}), true);
}}
    """.format(class_name, SPTR_PARAMS[class_name], SPTR_FCNS[class_name], shortened_params)

    # Put new string at beginning of class
    for i in range(len(flines)):
        if "public class {0}".format(class_name) in flines[i]:
            flines = flines[0:i+1] + [new_ctor] + flines[i+1:]
            break

    f = open(filename, "w")
    f.write("// add_sptr_ctor\n")
    for line in flines:
        f.write(line)
    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--java-dir", type="string", help="SWIG *.java output directory")
    (options,args) = parser.parse_args()

    os.chdir(options.java_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for filename in files:
            if filename.endswith(".java"):
                hide_ctors(filename)
                if filename in SPTR_FILES:
                    add_sptr_ctor(filename)
