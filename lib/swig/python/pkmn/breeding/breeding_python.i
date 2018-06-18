/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <python/python_init.i>
PKMN_PYTHON_INIT

%include <std_string.i>

%import <pkmn_python.i>
%import <stl_python.i>

%{
    #include <pkmn/breeding/child_info.hpp>
    #include <pkmn/breeding/compatibility.hpp>

    #include "cpp_wrappers/breeding.hpp"
%}

// Convert Doxygen docs to Python docstrings
%include <pkmn_python_docstrings.i>

%rename(gender) pkmn::e_gender;
%rename(stat) pkmn::e_stat;
%import <pkmn/enums/gender.hpp>
%import <pkmn/enums/stat.hpp>

%include <pkmn/breeding/child_info.hpp>
%include <pkmn/breeding/compatibility.hpp>
%include <cpp_wrappers/breeding.hpp>
