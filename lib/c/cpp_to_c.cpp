/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"

namespace pkmn {

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out
    ) {
        *string_list_out = (pkmn_string_list_t)std::malloc(sizeof(char*) * move_list.size());
        for(size_t i = 0; i < move_list.size(); ++i) {
            std::string move_name = move_list[i].get_name();
            (*string_list_out)[i] = (char*)std::malloc(move_name.size() + 1);
            std::strcpy((*string_list_out)[i], move_name.c_str());
            (*string_list_out)[i][move_name.size()] = '\0';
        }
    }

    void std_pair_std_string_to_string_pair(
        const std::pair<std::string, std::string> &cpp_pair,
        pkmn_string_pair_t* c_pair_out
    ) {
        c_pair_out->first = (char*)std::malloc(cpp_pair.first.size() + 1);
        std::strcpy(c_pair_out->first, cpp_pair.first.c_str());
        c_pair_out->first[cpp_pair.first.size()] = '\0';

        c_pair_out->second = (char*)std::malloc(cpp_pair.second.size() + 1);
        std::strcpy(c_pair_out->second, cpp_pair.second.c_str());
        c_pair_out->second[cpp_pair.second.size()] = '\0';
    }

    void std_vector_std_string_to_string_list(
        const std::vector<std::string> &vec,
        pkmn_string_list_t* string_list_out,
        size_t* list_len_out
    ) {
        *string_list_out = (pkmn_string_list_t)std::malloc(sizeof(char*) * vec.size());
        for(size_t i = 0; i < vec.size(); ++i) {
            (*string_list_out)[i] = (char*)std::malloc(vec[i].size() + 1);
            std::strcpy((*string_list_out)[i], vec[i].c_str());
            (*string_list_out)[i][vec[i].size()] = '\0';
        }
        *list_len_out = vec.size();
    }

}
