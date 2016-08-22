/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_CPP_TO_C_HPP
#define PKMN_C_CPP_TO_C_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/item_slot.hpp>
#include <pkmn/database/levelup_move.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/string_types.h>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

/*
 * These structs are opaque in the C API, but internally,
 * this is what we use.
 */

typedef struct {
    pkmn::item_list::sptr cpp;
    std::string last_error;
} pkmn_item_list_t;

#define PKMN_ITEM_LIST_DECLARED

namespace pkmn {

    void pkmn_item_slot_cpp_to_c(
        const pkmn::item_slot &islot_cpp,
        pkmn_item_slot_t* islot_c
    );

    void pkmn_item_slots_cpp_to_c(
        const pkmn::item_slots_t &islots_cpp,
        pkmn_item_slots_t* islots_c,
        size_t* list_length_out
    );

    void pkmn_levelup_move_cpp_to_c(
        const pkmn::database::levelup_move &lmove_cpp,
        pkmn_levelup_move_t* lmove_c
    );

    void pkmn_levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t &lmoves_cpp,
        pkmn_levelup_moves_t* lmoves_c,
        size_t* list_length_out
    );

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out,
        size_t* list_length_out
    );

    void pkmn_pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t &pokemon_entries,
        pkmn_string_list_t* string_list_out,
        size_t* list_length_out
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
        size_t* list_length_out
    );

}

#endif /* PKMN_C_CPP_TO_C_HPP */
