/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%begin %{
#define SWIG_PYTHON_2_UNICODE
%}

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need a super-ugly workaround
 * when using Python 2 and earlier versions of SWIG.
 */
#ifdef PKMN_WSTRING_WORKAROUND
%include <std_wstring.i>
%{
#include <boost/locale/encoding_utf.hpp>
%}
#endif

%define PKMN_PYTHON_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    //%include <pkmn_python_docstrings.i>
    %include <python/stl_macros.i>
%enddef
