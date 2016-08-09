#!/usr/bin/env python
#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

################################################################
# This script is called at build-time to generate SWIG
# docstrings.
################################################################

import CppHeaderParser
import swigdoc_converter
import datetime
from optparse import OptionParser
import os

header_text = """/*
 * This file was generated: %s
 */""" % datetime.datetime.now()

ignored_files = ["config.hpp",
                 "shared_ptr.hpp.in"]

CppHeaderParser.ignoreSymbols += ["PKMN_API", "PKMN_INLINE", "PKMN_CONSTEXPR_OR_INLINE"]

def get_csharp_docs(header):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower() and "doxygen" in fcn:
            output += "%s\n" % swigdoc_converter.documentation(fcn).swig_csharp_docs()

    for cls in header.classes:
        if "doxygen" in header.classes[cls]:
            cls_csharp_docs = "%s\n" % swigdoc_converter.documentation(header.classes[cls]).swig_csharp_docs()
            output += cls_csharp_docs

            if "sptr" in str(header):
                full_name = swigdoc_converter.assemble_full_name(header.classes[cls], True)
                cls_csharp_docs = cls_csharp_docs.replace(full_name, ("pkmn::shared_ptr<%s>" % full_name))
                output += cls_csharp_docs

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["destructor"] and "doxygen" in fcn:
                output += "%s\n" % swigdoc_converter.documentation(fcn).swig_csharp_docs()

    return output

def get_javadocs(header):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower() and "doxygen" in fcn:
            output += "%s\n" % swigdoc_converter.documentation(fcn).swig_javadoc()

    for cls in header.classes:
        if "doxygen" in header.classes[cls]:
            cls_javadoc = "%s\n" % swigdoc_converter.documentation(header.classes[cls]).swig_javadoc()
            output += cls_javadoc

            if len(header.classes[cls]["typedefs"]["public"]) > 0:
                full_name = swigdoc_converter.assemble_full_name(header.classes[cls], True)
                cls_javadoc = cls_javadoc.replace(full_name, ("pkmn::shared_ptr<%s>" % full_name))
                output += cls_javadoc

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["destructor"] and "doxygen" in fcn:
                output += "%s\n" % swigdoc_converter.documentation(fcn).swig_javadoc()

    return output

def get_python_docstrings(header):
    output = ""

    for fcn in header.functions:
        if "operator" not in fcn["name"].lower() and "anon" not in fcn["name"].lower() and "doxygen" in fcn:
            output += "%s\n" % swigdoc_converter.documentation(fcn).swig_python_docstring()

    for cls in header.classes:
        if "doxygen" in header.classes[cls]:
            cls_python_docstring = "%s\n" % swigdoc_converter.documentation(header.classes[cls]).swig_python_docstring()
            output += cls_python_docstring

            if len(header.classes[cls]["typedefs"]["public"]) > 0:
                full_name = swigdoc_converter.assemble_full_name(header.classes[cls], True)
                cls_python_docstring = cls_python_docstring.replace(full_name, ("pkmn::shared_ptr<%s>" % full_name))
                output += cls_python_docstring

        for fcn in header.classes[cls]["methods"]["public"]:
            if "operator" not in fcn["name"].lower() and not fcn["destructor"] and "doxygen" in fcn:
                output += "%s\n" % swigdoc_converter.documentation(fcn).swig_python_docstring()

        for var in header.classes[cls]["properties"]["public"]:
            if "doxygen" in var:
                output += "%s\n" % swigdoc_converter.documentation(var).swig_python_docstring()

    return output

SWIG_DOC_FUNCTIONS = dict(csharp = get_csharp_docs,
                          java = get_javadocs,
                          python = get_python_docstrings)

SWIG_DOC_FILENAMES = dict(csharp = "pkmn_csharp_docs.i",
                          java = "pkmn_javadocs.i",
                          python = "pkmn_python_docstrings.i")

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--include-dir", type="string", help="LibPKMN include directory")
    parser.add_option("--output-dir", type="string", help="Output directory")
    parser.add_option("--language", type="string", help="Language")
    (options,args) = parser.parse_args()

    output = header_text + "\n\n"

    os.chdir(options.include_dir)
    for root, dirs, files in os.walk(os.getcwd()):
        for file in files:
            if file.endswith(".hpp") and file not in ignored_files:
                output += "{0}\n".format(SWIG_DOC_FUNCTIONS[options.language](CppHeaderParser.CppHeader(os.path.join(root, file))))

    os.chdir(options.output_dir)
    f = open(SWIG_DOC_FILENAMES[options.language], 'w')
    f.write(output)
    f.close()
