/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_CSHARP_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    %include <pkmn_csharp_docs.i>
    %include <csharp_CamelCase.i>
    %include <csharp/stl_macros.i>
    %typemap(csclassmodifiers) SWIGTYPE "public partial class"

    %ignore operator ==;
    %ignore operator !=;
    %ignore operator +;
    %ignore operator +=;
%enddef
