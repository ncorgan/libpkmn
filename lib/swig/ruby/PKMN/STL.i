/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::STL"

%include <ruby/ruby_init.i>
PKMN_RUBY_INIT

%include <std_map.i>
%include <std_string.i>

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

// std::map
PKMN_RUBY_MAP(std::string, bool, StringBooleanHash);
PKMN_RUBY_MAP(std::string, std::string, StringStringHash);
PKMN_RUBY_MAP(std::string, int, StringIntHash);

// std::pair
PKMN_RUBY_PAIR(std::string, std::string, StringPair);

// std::vector
PKMN_RUBY_VECTOR(std::string, StringList);
