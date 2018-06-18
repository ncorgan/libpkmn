/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%{
    #include <pkmn/enums/stat.hpp>
%}

/*
 * This file contains STL containers with either pure STL types or with LibPKMN
 * enums.
 */

%include <pkmn/enums/stat.hpp>
%include <std_string.i>

// std::map
PKMN_CSHARP_MAP(pkmn::e_stat, int, Stat, int, StatDict);

// std::pair
PKMN_CSHARP_PAIR(int,         int,         int,    int,    IntPair)
PKMN_CSHARP_PAIR(std::string, std::string, string, string, StringPair)

// std::vector
PKMN_CSHARP_VECTOR(int,          int,       IntList)
PKMN_CSHARP_VECTOR(std::string,  string,    StringList)
PKMN_CSHARP_VECTOR(pkmn::e_stat, Stat, StatList);
