/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT

%include <std_string.i>

%{
    #include <pkmn/build_info.hpp>

    PKMN_INLINE std::string getSWIGVersion() {
        return "@SWIG_VERSION@";
    }
%}

std::string getSWIGVersion();

%include <pkmn/build_info.hpp>
