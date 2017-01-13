#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import CppHeaderParser

import sys
import unicodedata

PYTHON_MAJOR_VERSION = sys.version_info[0]

"""
C# exceptions corresponding to C++ exceptions.
"""
CSHARP_EXCEPTIONS = {
    "boost::exception"      : "System.ApplicationException",
    "std::bad_exception"    : "System.ApplicationException",
    "std::invalid_argument" : "System.ArgumentException",
    "std::domain_error"     : "System.ArgumentOutOfRangeException",
    "std::length_error"     : "System.IndexOutOfRangeException",
    "std::out_of_range"     : "System.IndexOutOfRangeException",
    "std::logic_error"      : "System.ApplicationException",
    "std::range_error"      : "System.ApplicationException",
    "std::overflow_error"   : "System.OverflowException",
    "std::underflow_error"  : "System.UnderflowException",
    "std::runtime_error"    : "System.ApplicationException",
    "std::exception"        : "System.ApplicationException",
}

"""
Java exceptions corresponding to C++ exceptions.
"""
JAVA_EXCEPTIONS = {
    "boost::exception"      : "java.lang.RuntimeException",
    "std::bad_exception"    : "java.lang.RuntimeException",
    "std::invalid_argument" : "java.lang.IllegalArgumentException",
    "std::domain_error"     : "java.lang.RuntimeException",
    "std::length_error"     : "java.lang.IndexOutOfBoundsException",
    "std::out_of_range"     : "java.lang.IndexOutOfBoundsException",
    "std::logic_error"      : "java.lang.RuntimeException",
    "std::range_error"      : "java.lang.RuntimeException",
    "std::overflow_error"   : "java.lang.IndexOutOfBoundsException",
    "std::underflow_error"  : "java.lang.RuntimeException",
    "std::runtime_error"    : "java.lang.RuntimeException",
    "std::exception"        : "java.lang.RuntimeException",
}

"""
Python exceptions corresponding to C++ exceptions.
"""
PYTHON_EXCEPTIONS = {
    "boost::exception"      : "RuntimeError",
    "std::bad_exception"    : "SystemError",
    "std::invalid_argument" : "ValueError",
    "std::domain_error"     : "ValueError",
    "std::length_error"     : "IndexError",
    "std::out_of_range"     : "IndexError",
    "std::logic_error"      : "SystemError",
    "std::range_error"      : "RuntimeError",
    "std::overflow_error"   : "OverflowError",
    "std::underflow_error"  : "RuntimeError",
    "std::runtime_error"    : "RuntimeError",
    "std::exception"        : "SystemError",
}

"""
Returns namespace + class name (if applicable) + function name (if applicable).
"""
def assemble_full_name(cpp_input, is_class):
    full_name = ""

    if cpp_input.get("parent",None) != None:
        full_name = "%s::%s" % (assemble_full_name(cpp_input["parent"], True), cpp_input["name"])
        if not is_class:
            full_name += "("
            for param in cpp_input["parameters"]:
                full_name += "%s %s, " % (param["type"], param["name"])
            if len(cpp_input["parameters"]) > 0:
                full_name = full_name[:-2]
            full_name += ")"
            if cpp_input["const"]:
                full_name += " const"
    else:
        if is_class:
            return "%s::%s" % (cpp_input["namespace"], cpp_input["name"])
        else:
            return "%s%s" % (cpp_input["namespace"], cpp_input["name"])

    return full_name

# TODO: sanitize quotes
class documentation():

    def __init__(self, cpp_input):
        if not ("name" in cpp_input and "doxygen" in cpp_input and "namespace" in cpp_input):
            raise Exception("Invalid input.")

        self.__input = cpp_input
        self.__class = ("CppClass" in str(type(self.__input)))
        self.__full_name = assemble_full_name(self.__input, self.__class)
        self.__short_doc = ""
        self.__long_doc = ""
        self.__returns = ""
        self.__params = dict()
        self.__throws = dict()

        # Parse Doxygen input
        doc = self.__input["doxygen"]
        in_brief = False
        if doc != "":
            for line in doc.split("\n"):
                if line.startswith("//!"):
                    self.__short_doc = "/".join(line.split("/")[2:]).split("! ")[1].replace("\"","\\\"").rstrip("/*!")
                elif line.startswith("* \\param"):
                    self.__params[line.split(" ")[2]] = " ".join(line.split(" ")[3:]).replace("\"","\\\"")
                elif line.startswith("* \\throw"):
                    self.__throws[line.split(" ")[2]] = " ".join(line.split(" ")[3:]).replace("\"","\\\"")
                elif line.startswith("* \\return"):
                    self.__returns = " ".join(line.split(" ")[2:]).replace("\"","\\\"")
                elif line.startswith("* @brief"):
                    in_brief = True
                elif line.startswith("*/") and in_brief:
                    in_brief = False
                elif line.startswith("* "):
                    if in_brief:
                        self.__short_doc += "%s\n" % line[2:].replace("\"","\\\"")
                    else:
                        self.__long_doc += "%s\n" % line[2:].replace("\"","\\\"")

    # http://stackoverflow.com/a/925630
    def __remove_html(self, string):
        if PYTHON_MAJOR_VERSION == 2:
            from HTMLParser import HTMLParser

            class MLStripper(HTMLParser):
                def __init__(self):
                    self.reset()
                    self.fed = []
                def handle_data(self, d):
                    self.fed.append(d)
                def get_data(self):
                    return ''.join(self.fed)

        else:
            from html.parser import HTMLParser

            class MLStripper(HTMLParser):
                def __init__(self):
                    self.reset()
                    self.strict = False
                    self.convert_charrefs= True
                    self.fed = []
                def handle_data(self, d):
                    self.fed.append(d)
                def get_data(self):
                    return ''.join(self.fed)

        s = MLStripper()
        s.feed(string)
        return s.get_data()

    def __cleanup_python_string(self, string):
        if PYTHON_MAJOR_VERSION == 2:
            string = unicodedata.normalize("NFKD", string.decode("utf-8")).encode("ascii", "ignore")

        try:
            string = self.__remove_html(string)
        except:
            pass

        return string

    """
    Returns the original C++ Doxygen documentation. CppHeaderParser ruins the Doxygen
    syntax of the original documentation, so this returns how it should look.
    """
    def cpp_doc(self):
        output = ""
        if self.__short_doc != "":
            output += "//! %s\n" % self.__short_doc
        if self.__long_doc != "":
            output += "/*!\n"
            for line in self.__long_doc:
                output += " * %s\n" % line
        for key in self.__throws:
            output += " * \\throws %s %s\n" % (key, self.__throws[key])
        for key in self.__params:
            output += " * \\param %s %s\n" % (key, self.__params[key])
        if self.__returns != "":
            output += " * \\return %s\n" % self.__returns
        output += "*/"

        return output

    """
    Returns a C# XML comment corresponding to the given class/function.
    """
    def __csharp_docs(self):
        output = ""
        if self.__short_doc != "":
            output += "/// <summary>%s</summary>" % self.__remove_html(self.__short_doc).replace("\n","")
        if self.__long_doc != "":
            output += "\n/// <remarks>\n"
            for line in self.__long_doc.split("\n"):
                output += "/// %s\n" % self.__remove_html(line).replace("\n","")
            output += "/// </remarks>"

        if self.__class:
            return "using System;\nusing System.Runtime.InteropServices;\n%s" % output

        for key in self.__throws:
            output += "\n/// <exception cref=\\\"%s\\\">%s</exception>" % (CSHARP_EXCEPTIONS.get(key, "System.ApplicationException"), self.__remove_html(self.__throws[key]).replace("\n",""))
        for key in self.__params:
            output += "\n/// <param name=\\\"%s\\\">%s</param>" % (key, self.__remove_html(self.__params[key]).replace("\n",""))
        if self.__returns != "":
            output += "\n/// <returns>%s</returns>" % self.__remove_html(self.__returns).replace("\n","")

        return output

    """
    Returns a SWIG C# documentation line corresponding to the given class/function.
    """
    def swig_csharp_docs(self):
        if self.__class:
            return "%%typemap(csimports) %s \"\n%s\"" % (self.__full_name, self.__csharp_docs())
        else:
            return "%%csmethodmodifiers %s \"\n%s\npublic\";" % (self.__full_name, self.__csharp_docs())

    def __javadoc(self):
        output = "/**\n"
        if self.__short_doc != "":
            output += " * %s\n *\n" % self.__short_doc
        if self.__long_doc != "":
            for line in self.__long_doc.split("\n"):
                output += " * %s\n" % line
            output += " *\n"
        for key in self.__throws:
            output += " * @throws %s %s\n" % (JAVA_EXCEPTIONS.get(key, "java.lang.RuntimeException"), self.__throws[key])
        for key in self.__params:
            output += " * @param %s %s\n" % (key, self.__params[key])
        if self.__returns != "":
            output += " * @return %s\n" % self.__returns
        output += " */"

        return output

    """
    Returns a SWIG Javadoc line corresponding to the given class/function.
    """
    def swig_javadoc(self):
        if self.__class:
            return "%%typemap(javaimports) %s \"\n%s\"" % (self.__full_name, self.__javadoc())
        else:
            return "%%javamethodmodifiers %s \"\n%s\npublic\";" % (self.__full_name, self.__javadoc())

    """
    Returns a SWIG %feature("docstring") line corresponding to the given class/function.

    All HTML is stripped from the documentation, and Unicode is converted to ASCII in Python 2.
    """
    def swig_python_docstring(self):
        output = "%%feature(\"docstring\") %s \"" % self.__full_name
        if self.__short_doc != "":
            output += "%s\n\n" % self.__cleanup_python_string(self.__short_doc)
        if self.__long_doc != "":
            output += "%s\n\n" % self.__cleanup_python_string(self.__long_doc)
        if len(self.__throws.keys()) > 0:
            output += "\nThrows:\n"
            for key in self.__throws:
                output += "    %s : %s\n" % (PYTHON_EXCEPTIONS.get(key, "RuntimeError"), self.__cleanup_python_string(self.__throws[key]))
        if len(self.__params.keys()) > 0:
            output += "\nArgs:\n"
            for key in self.__params:
                output += "    %s : %s\n" % (key, self.__cleanup_python_string(self.__params[key]))
        output += "\""

        return output
