/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%module "PKMN::STL"

%include <std_map.i>
%include <std_string.i>
%include <std_vector.i>

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

// std::map
%template(StringBooleanHash) std::map<std::string, bool>;
%template(StringStringHash)  std::map<std::string, std::string>;
%template(StringIntHash)     std::map<std::string, int>;

// std::pair
%template(StringPair) std::pair<std::string, std::string>;

// std::vector
%template(StringList) std::vector<std::string>;
