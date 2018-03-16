/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_RUBY_INIT
    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    // In Ruby, != is just "not =="
    %ignore operator !=;
    %ignore operator +=;

    %include <ruby_CamelCase.i>
    %include <pkmn.i>
    %include <ruby/stl_macros.i>
%enddef
