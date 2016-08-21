/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_CPP_TO_C_HPP
#define PKMN_C_CPP_TO_C_HPP

#include <pkmn/config.hpp>

#include <cstring>
#include <string>
#include <vector>

PKMN_INLINE void std_string_to_c_str(
    const std::string &str,
    size_t buffer_len,
    char* c_str_out,
    size_t* actual_strlen_out
) {
    std::strncpy(c_str_out, str.c_str(), buffer_len);
    *actual_strlen_out = str.size();
}

void std_vector_std_string_to_c_strs(
    const std::vector<std::string> &vec,
    char*** c_strs_out,
    size_t* list_len_out
);

#endif /* PKMN_C_CPP_TO_C_HPP */
