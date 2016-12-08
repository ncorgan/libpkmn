/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT("calculations_java")

%import <stl_java.i>

%{
    #include <pkmn/calculations/form.hpp>
    #include <pkmn/calculations/gender.hpp>
    #include <pkmn/calculations/shininess.hpp>
    #include <pkmn/calculations/stats.hpp>
%}

%include <pkmn/calculations/form.hpp>
%include <pkmn/calculations/gender.hpp>
%include <java/pkmn_calculations_hidden_power.i>
%include <pkmn/calculations/shininess.hpp>
%include <java/pkmn_calculations_spinda_spots.i>
%include <pkmn/calculations/stats.hpp>
