/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%begin %{
#define SWIG_PYTHON_2_UNICODE
%}

%define PKMN_PYTHON_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    //%include <pkmn_python_docstrings.i>
    %include <python/stl_macros.i>
%enddef
