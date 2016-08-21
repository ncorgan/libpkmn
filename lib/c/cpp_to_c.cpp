/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

void std_vector_std_string_to_c_strs(
    const std::vector<std::string> &vec,
    char*** c_strs_out,
    size_t* list_len_out
) {
    *c_strs_out = (char**)std::malloc(sizeof(char*) * vec.size());
    for(size_t i = 0; i < vec.size(); ++i) {
        (*c_strs_out)[i] = (char*)std::malloc(vec[i].size() + 1);
        std::strcpy((*c_strs_out)[i], vec[i].c_str());
        (*c_strs_out)[i][vec[i].size()] = '\0';
    }
    *list_len_out = vec.size();
}
