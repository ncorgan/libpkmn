/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT

%include <std_string.i>

/*
 * Java has no reliable and non-hacky way of setting environment variables for
 * underlying native methods, so we must expose setenv to be able to tell LibPKMN
 * to look for the database where we extract it from the standalone JAR.
 */

%rename(getEnv)   pkmn_getenv;
%rename(setEnv)   pkmn_setenv;
%rename(unsetEnv) pkmn_unsetenv;

%{
    #include "env.hpp"
%}

%include <env.hpp>
