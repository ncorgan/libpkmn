/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

/*
 * This file contains all pure STL wrappers. Other .i files can declare maps,
 * pairs, and vectors of their classes, but there's no reason for multiple
 * files to declare string vectors, etc.
 */

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier versions of SWIG.
 */
#if SWIG_VERSION < 0x030008

%include <std_wstring.i>

// std::map
PKMN_PYTHON_MAP(std::wstring, bool,        string_bool_dict)
PKMN_PYTHON_MAP(std::wstring, std::string, string_string_dict)
PKMN_PYTHON_MAP(std::wstring, int,         string_int_dict)

// std::pair
PKMN_PYTHON_PAIR(std::wstring, std::wstring, string_pair)

// std::vector
PKMN_PYTHON_VECTOR(std::wstring, string_list)

#else

%include <std_string.i>

// std::map
PKMN_PYTHON_MAP(std::string, bool,        string_bool_dict)
PKMN_PYTHON_MAP(std::string, std::string, string_string_dict)
PKMN_PYTHON_MAP(std::string, int,         string_int_dict)

// std::pair
PKMN_PYTHON_PAIR(std::string, std::string, string_pair)

// std::vector
PKMN_PYTHON_VECTOR(std::string, string_list)

#endif
