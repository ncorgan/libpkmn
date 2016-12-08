/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT("paths_java")

%include <std_string.i>

%{
    #include <pkmn/utils/paths.hpp>
%}

%include <pkmn/utils/paths.hpp>
