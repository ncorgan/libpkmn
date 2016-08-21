/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_CPP_TO_C_HPP
#define PKMN_C_CPP_TO_C_HPP

#include <pkmn/config.hpp>
#include <pkmn/database/move_entry.hpp>

#include <pkmn-c/types/string_types.h>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace pkmn {

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out
    );

    PKMN_INLINE void std_string_to_c_str(
        const std::string &str,
        char* c_str_out,
        size_t buffer_len,
        size_t* actual_strlen_out
    ) {
        std::strncpy(c_str_out, str.c_str(), buffer_len);
        *actual_strlen_out = str.size();
    }

    void std_pair_std_string_to_string_pair(
        const std::pair<std::string, std::string> &cpp_pair,
        pkmn_string_pair_t* c_pair_out
    );

    void std_vector_std_string_to_string_list(
        const std::vector<std::string> &vec,
        pkmn_string_list_t* string_list_out,
        size_t* list_len_out
    );

}

#endif /* PKMN_C_CPP_TO_C_HPP */
