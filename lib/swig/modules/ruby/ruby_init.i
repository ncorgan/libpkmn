/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_RUBY_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    %include <pkmn.i>
    %include <ruby_CamelCase.i>
    %include <ruby/stl_macros.i>
%enddef
