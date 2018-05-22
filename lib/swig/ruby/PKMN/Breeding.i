/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::Breeding"

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%import <STL.i>
%import <PKMNRuby.i>

%{
    #include <pkmn/breeding/child_info.hpp>
    #include <pkmn/breeding/compatibility.hpp>

    #include "cpp_wrappers/breeding.hpp"
%}

%include <pkmn/breeding/child_info.hpp>
%include <pkmn/breeding/compatibility.hpp>
%include <cpp_wrappers/breeding.hpp>
