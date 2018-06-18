/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%{
    #include <pkmn/enums/stat.hpp>
%}

/*
 * This file contains STL wrappers with either pure STL types or LibPKMN enums.
 */

%rename(stat) pkmn::e_stat;
%include <pkmn/enums/stat.hpp>

%include <std_string.i>

// std::map
PKMN_PYTHON_MAP(pkmn::e_stat, int, stat_dict);

// std::pair
PKMN_PYTHON_PAIR(std::string, std::string, string_pair)

// std::vector
PKMN_PYTHON_VECTOR(int,          int_list)
PKMN_PYTHON_VECTOR(std::string,  string_list)
PKMN_PYTHON_VECTOR(pkmn::e_stat, stat_list);
