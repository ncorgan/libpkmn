/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_JAVA_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    %include <java/unsigned_nums_typemap.i> 
    //%include <pkmn_javadocs.i>
    %include <java_CamelCase.i>
    //%include <java/equality.i>
    %include <java/stl_macros.i>
%enddef
