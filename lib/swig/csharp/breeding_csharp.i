/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/csharp_init.i>
PKMN_CSHARP_INIT

%include <std_string.i>

%import <pkmn_csharp.i>
%import <stl_csharp.i>

%{
    #include <pkmn/breeding/child_info.hpp>
    #include <pkmn/breeding/compatibility.hpp>

    #include "cpp_wrappers/breeding.hpp"
%}

// Convert Doxygen docs to C# docs
%include <pkmn_csharp_docs.i>

%import <pkmn/enums/gender.hpp>

%include <pkmn/breeding/child_info.hpp>
%include <pkmn/breeding/compatibility.hpp>
%include <cpp_wrappers/breeding.hpp>
