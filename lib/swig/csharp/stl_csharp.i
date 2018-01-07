/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

%include <std_string.i>

// std::map
PKMN_CSHARP_MAP(std::string, int, string, int, StringIntDict)

// std::pair
PKMN_CSHARP_PAIR(int, int, int, int, IntPair)
PKMN_CSHARP_PAIR(std::string, std::string, string, string, StringPair)

// std::vector
PKMN_CSHARP_VECTOR(std::string, string, StringList)
