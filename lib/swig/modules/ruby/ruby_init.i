/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%define PKMN_RUBY_INIT
    %feature("autodoc", "2");

    %{
        // The Ruby headers define this but don't do anything with it. It messes
        // with us because of our game enum.
        #ifdef RUBY
        #    undef RUBY
        #endif
    %}

    %include <swig_exceptions.i>
    SWIG_CATCH_DEFAULT

    // In Ruby, != is just "not =="
    %ignore operator !=;
    %ignore operator +=;

    %include <ruby_CamelCase.i>
    %include <pkmn.i>
    %include <ruby/stl_macros.i>
%enddef
