#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

PARTIAL_CLASS_FILES = ["GUI.cs", "PKMN.cs"]

def make_a_partial_class(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()

    # Already done
    if "make_a_partial_class" in flines[0]:
        return

    # Find and fix the appropriate line
    for i in range(len(flines)):
        for class_name in ["GUI", "PKMN"]:
            if "class {0}".format(class_name) in flines[i]:
                flines[i] = flines[i].replace(
                                "class {0}".format(class_name),
                                "partial class {0}".format(class_name)
                            )

    f = open(filename, "w")
    f.write("// make_a_partial_class\n")
    for line in flines:
        f.write(line)
    f.close()

def remove_class_with_attributes_inheritance(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-3]

    # Already done
    if "remove_class_with_attributes_inheritance" in flines[0]:
        return

    base_class_ctor = " : base(PKMNPINVOKE.{0}_SWIGUpcast(cPtr), cMemoryOwn)".format(class_name)

    for i in range(len(flines)):
        if "class_with_attributes" in flines[i]:
            flines[i] = flines[i].replace(" : class_with_attributes", "")
        if base_class_ctor in flines[i]:
            flines[i] = flines[i].replace(base_class_ctor, "")
        if "Dispose() {" in flines[i]:
            flines[i] = flines[i].replace("override void", "void")
        if "HandleRef swigCPtr" in flines[i]:
            flines[i+1] = "  protected bool swigCMemOwn;"
        if "bool cMemoryOwn" in flines[i]:
            flines[i] = flines[i] + " swigCMemOwn = cMemoryOwn;"

    f = open(filename, "w")
    f.write("// remove_class_with_attributes_inheritance\n")
    for line in flines:
        if "base.Dispose();" not in line:
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
                if "_base" in filename:
                    remove_class_with_attributes_inheritance(filename)
                if filename in PARTIAL_CLASS_FILES:
                    make_a_partial_class(filename)
