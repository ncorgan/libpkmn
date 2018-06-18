/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::STL"

%{
    #include <pkmn/enums/stat.hpp>
%}

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%include <std_map.i>
%include <std_string.i>

%include <pkmn/enums/stat.hpp>

// std::map
PKMN_RUBY_MAP(pkmn::e_stat, int, StatHash);

// std::pair
PKMN_RUBY_PAIR(std::string, std::string, StringPair);

// std::vector
PKMN_RUBY_VECTOR(int,          IntList);
PKMN_RUBY_VECTOR(std::string,  StringList);
PKMN_RUBY_VECTOR(pkmn::e_stat, StatList);
