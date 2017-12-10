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

GUI_FILES = ["GUI.cs"]

SPTR_FILES = ["GameSave.cs",
              "ItemBag.cs",
              "ItemList.cs",
              "Pokemon.cs",
              "PokemonBox.cs",
              "PokemonParty.cs",
              "PokemonPC.cs"]

# Can these by programatically grabbed from the *_base files?
SPTR_CTORS = dict(
                 GameSave = """
public GameSave(string filepath): this(PKMNPINVOKE.make_game_save(filepath), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
""",
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
""",
                PokemonBox = """
public PokemonBox(string game): this(PKMNPINVOKE.make_pokemon_box(game), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
""",
                PokemonParty = """
public PokemonParty(string game): this(PKMNPINVOKE.make_pokemon_party(game), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
""",
                PokemonPC = """
public PokemonPC(string game): this(PKMNPINVOKE.make_pokemon_pc(game), true) {
    if(PKMNPINVOKE.SWIGPendingException.Pending) {
        throw PKMNPINVOKE.SWIGPendingException.Retrieve();
    }
}
"""
             )

def make_gui_a_partial_class(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()

    # Already done
    if "make_gui_a_partial_class" in flines[0]:
        return

    # Find and fix the appropriate line
    for i in range(len(flines)):
        if "class GUI" in flines[i]:
            flines[i] = flines[i].replace("class GUI", "partial class GUI")

    f = open(filename, "w")
    f.write("// make_gui_a_partial_class\n")
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
    f.write("// remove_class_with_attributes_inheritance")
    for line in flines:
        if "base.Dispose();" not in line:
            f.write(line)
    f.close()

def fix_sptr_file(filename):
    f = open(filename, "r")
    flines = f.readlines()
    f.close()
    class_name = filename[:-3]
    ctor = "public {0}(".format(class_name)

    # Already done
    if "fix_sptr_file" in flines[0]:
        return

    # Find first relevant ctor
    for i in range(len(flines)):
        if ctor in flines[i]:
            flines[i-1] = "// \\cond\n"
            flines[i+7] = "// \\endcond\n{0}\n".format(SPTR_CTORS[class_name])
            break

    # For some reason, the CamelCase changes don't propagate to sptr files.
    for i in range(len(flines)):
        flines[i] = flines[i].replace(" get_numeric_attribute_names", " GetNumericAttributeNames")
        flines[i] = flines[i].replace(" get_numeric_attribute", " GetNumericAttribute")
        flines[i] = flines[i].replace(" set_numeric_attribute", " SetNumericAttribute")
        flines[i] = flines[i].replace(" get_string_attribute_names", " GetStringAttributeNames")
        flines[i] = flines[i].replace(" get_string_attribute", " GetStringAttribute")
        flines[i] = flines[i].replace(" set_string_attribute", " SetStringAttribute")
        flines[i] = flines[i].replace(" attribute_name", " attributeName")

    f = open(filename, "w")
    f.write("// fix_sptr_file\n")
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
                if "_base" in filename:
                    remove_class_with_attributes_inheritance(filename)
                if filename in GUI_FILES:
                    make_gui_a_partial_class(filename)
                if filename in SPTR_FILES:
                    fix_sptr_file(filename)
