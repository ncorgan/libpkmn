/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <java/java_init.i>
PKMN_JAVA_INIT

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

%include <std_string.i>

// std::map
PKMN_JAVA_MAP(std::string, bool,        String, boolean, StringBooleanMap)
PKMN_JAVA_MAP(std::string, std::string, String, String,  StringStringMap)
PKMN_JAVA_MAP(std::string, int,         String, int,     StringIntMap)

// std::pair
PKMN_JAVA_PAIR(int,         int,         int,    int,    IntPair)
PKMN_JAVA_PAIR(std::string, std::string, String, String, StringPair)

// std::vector
PKMN_JAVA_VECTOR(std::string, String, StringVector)
