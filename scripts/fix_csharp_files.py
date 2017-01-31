#!/usr/bin/env python
#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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
              "ItemList.cs",
              "Pokemon.cs"]

# Can these by programatically grabbed from the *_base files?
SPTR_CTORS = dict(
                 ItemBag = """
public ItemBag(string game): this(PKMNPINVOKE.make_item_bag(game), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
""",
                 ItemList = """
public ItemList(string name, string game): this(PKMNPINVOKE.make_item_list(name, game), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
""",
                 Pokemon = """
public Pokemon(string name, string game, string form, int level): this(PKMNPINVOKE.make_pokemon__SWIG_0(name, game, form, level), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
public Pokemon(string filepath): this(PKMNPINVOKE.make_pokemon__SWIG_1(filepath), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
"""
             )

def hide_sptr_ctors(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-3]
    ctor = "public {0}(".format(class_name)

    # Already done
    if "hide_sptr_ctors" in flines[0]:
        return

    # Find first relevant ctor
    for i in range(len(flines)):
        if ctor in flines[i]:
            flines[i-1] = "// \\cond\n"
            flines[i+7] = "// \\endcond\n{0}\n".format(SPTR_CTORS[class_name])
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
