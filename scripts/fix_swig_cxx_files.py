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
import re

NUM_CHECKS = 2

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
    if "remove_extra_enum_specifier" in "".join(flines[:NUM_CHECKS]):
        return

    # Find and fix the appropriate lines
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

#
# If a Python exception is thrown by a SWIG director function, the C++ exception
# raised in the SWIG director class merely says "Error detected when calling 'foo'",
# which tells us essentially nothing. This even happens when the C++ function that
# hasn't been overridden throws a C++ exception, as SWIG translates this into a
# Python exception. Effectively, using SWIG directors in Python means no exception
# information is preserved.
#
def improve_python_director_exceptions(filepath):
    f = open(filepath, "r")
    flines = f.readlines()
    f.close()

    # Already done
    if "improve_python_director_exceptions" in "".join(flines[:NUM_CHECKS]):
        return

    director_error_code = """
{
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);

    std::string error_message(PyString_AsString(pvalue));

    PyErr_Restore(ptype, pvalue, ptraceback);

    Swig::DirectorMethodException::raise(error_message.c_str());
}
    """

    # Find and fix the appropriate lines
    for line_num in range(len(flines)):
        if "Swig::DirectorMethodException::raise(\"Error detected when calling" in flines[line_num]:
            flines[line_num] = re.sub(
                                   flines[line_num],
                                   r"Swig::DirectorMethodException.*",
                                   director_error_code
                               )

    f = open(filepath, "w")
    f.write("// improve_python_director_exceptions\n")
    for line in flines:
        f.write(line)
    f.close()

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("--source-file", type="string", help="SWIG-generated .cxx file")
    (options,args) = parser.parse_args()

    filepath = os.path.abspath(options.source_file)
    remove_extra_enum_specifier(filepath)

    if "PYTHON" in filepath:
        improve_python_director_exceptions(filepath)
