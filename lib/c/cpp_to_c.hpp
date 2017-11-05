/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_CPP_TO_C_HPP
#define PKMN_C_CPP_TO_C_HPP

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/item_slot.hpp>
#include <pkmn/move_slot.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/pokemon_pc.hpp>
#include <pkmn/calculations/hidden_power.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/levelup_move.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>

#include <pkmn-c/error.h>
#include <pkmn-c/database/item_entry.h>
#include <pkmn-c/database/move_entry.h>
#include <pkmn-c/database/pokemon_entry.h>
#include <pkmn-c/types/hidden_power.h>
#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/string_types.h>

#include <boost/thread/mutex.hpp>

#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "error_internal.hpp"

/*
 * These structs are opaque in the C API, but internally,
 * this is what we use.
 */

typedef struct {
    pkmn::game_save::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_game_save_t;

typedef struct {
    pkmn::pokemon::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_t;

typedef struct {
    pkmn::pokemon_box::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_box_t;

typedef struct {
    pkmn::pokemon_party::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_party_t;

typedef struct {
    pkmn::pokemon_pc::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_pc_t;

#define PKMN_GAME_SAVE_DECLARED
#define PKMN_ITEM_BAG_DECLARED
#define PKMN_ITEM_LIST_DECLARED
#define PKMN_POKEMON_DECLARED
#define PKMN_POKEMON_BOX_DECLARED
#define PKMN_POKEMON_PARTY_DECLARED
#define PKMN_POKEMON_PC_DECLARED

#include <pkmn-c/types/hidden_power.h>
#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/move_slot.h>
#include <pkmn-c/types/pokemon_list.h>
#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/string_types.h>

#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>

// Internal representations
typedef struct {
    pkmn::item_bag::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_item_bag_internal_t;

typedef struct {
    pkmn::item_list::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_item_list_internal_t;

// Calls to initialize or update internal representations.
void init_item_bag(
    pkmn_item_bag_t* item_bag
);
void update_item_bag(
    pkmn_item_bag_t* item_bag
);
void init_item_list(
    pkmn_item_list_t* item_list
);
void update_item_list(
    pkmn_item_list_t* item_list
);

namespace pkmn {

    void pkmn_hidden_power_cpp_to_c(
        const pkmn::calculations::hidden_power &hp_cpp,
        pkmn_hidden_power_t* hp_c
    );

    void pkmn_item_entry_cpp_to_c(
        const pkmn::database::item_entry &item_entry_cpp,
        pkmn_database_item_entry_t* item_entry_c
    );

    void pkmn_item_slot_cpp_to_c(
        const pkmn::item_slot &islot_cpp,
        pkmn_item_slot_t* islot_c
    );

    void pkmn_item_slots_cpp_to_c(
        const pkmn::item_slots_t &islots_cpp,
        pkmn_item_slots_t* islots_c
    );

    void pkmn_levelup_move_cpp_to_c(
        const pkmn::database::levelup_move &lmove_cpp,
        pkmn_levelup_move_t* lmove_c
    );

    void pkmn_levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t &lmoves_cpp,
        pkmn_levelup_moves_t* lmoves_c
    );

    void pkmn_move_entry_cpp_to_c(
        const pkmn::database::move_entry &move_entry_cpp,
        pkmn_database_move_entry_t* move_entry_c
    );

    void pkmn_move_list_to_string_list(
        const pkmn::database::move_list_t &move_list,
        pkmn_string_list_t* string_list_out
    );

    void pkmn_pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry &pokemon_entry_cpp,
        pkmn_database_pokemon_entry_t* pokemon_entry_c
    );

    void pkmn_move_slot_cpp_to_c(
        const pkmn::move_slot &mslot_cpp,
        pkmn_move_slot_t* mslot_c
    );

    void pkmn_move_slots_cpp_to_c(
        const pkmn::move_slots_t &mslots_cpp,
        pkmn_move_slots_t* mslots_c
    );

    void pkmn_pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t &pokemon_entries,
        pkmn_string_list_t* string_list_out
    );

    void pkmn_pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t &pokemon_list_cpp,
        pkmn_pokemon_list_t* pokemon_list_c
    );

    void pkmn_pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t &pokemon_box_list_cpp,
        pkmn_pokemon_box_list_t* pokemon_box_list_c
    );

    PKMN_INLINE void std_pair_int_to_int_pair(
        const std::pair<int, int> &pair_cpp,
        pkmn_int_pair_t* pair_c
    ) {
        pair_c->first = pair_cpp.first;
        pair_c->second = pair_cpp.second;
    }

    PKMN_INLINE pkmn_error_t std_string_to_c_str(
        const std::string &str,
        char* c_str_out,
        size_t buffer_len
    ) {
        PKMN_CHECK_BUFFER_LEN(buffer_len, (str.size()+1));
        std::strncpy(c_str_out, str.c_str(), buffer_len);
        c_str_out[buffer_len-1] = '\0';

        pkmn_set_error("None");
        return PKMN_ERROR_NONE;
    }

    template <typename handle_type>
    PKMN_INLINE pkmn_error_t std_string_to_c_str_with_handle(
        handle_type handle,
        const std::string &str,
        char* c_str_out,
        size_t buffer_len
    ) {
        PKMN_CHECK_BUFFER_LEN_WITH_HANDLE(buffer_len, (str.size()+1), handle);
        std::strncpy(c_str_out, str.c_str(), buffer_len);
        c_str_out[buffer_len-1] = '\0';

        pkmn_set_error("None");
        handle->last_error = "None";
        return PKMN_ERROR_NONE;
    }

    PKMN_INLINE void std_string_to_c_str_alloc(
        const std::string &str,
        char** c_str_out
    ) {
        *c_str_out = (char*)std::calloc(str.size()+1, sizeof(char));
        std::strncpy(*c_str_out, str.c_str(), str.size());
    }

    void std_pair_std_string_to_string_pair(
        const std::pair<std::string, std::string> &cpp_pair,
        pkmn_string_pair_t* c_pair_out
    );

    template <typename value_type>
    PKMN_INLINE void std_map_keys_to_string_list(
        const std::map<std::string, value_type> &string_map,
        pkmn_string_list_t* string_list_out
    ) {
        string_list_out->strings = (char**)std::malloc(sizeof(char*) * string_map.size());
        size_t i = 0;
        for(auto iter = string_map.begin();
            iter != string_map.end();
            ++iter, ++i)
        {
            string_list_out->strings[i] = (char*)std::malloc(iter->first.size() + 1);
            std::strcpy(string_list_out->strings[i], iter->first.c_str());
            string_list_out->strings[i][iter->first.size()] = '\0';
        }

        string_list_out->length = string_map.size();
    }

    void std_vector_std_string_to_string_list(
        const std::vector<std::string> &vec,
        pkmn_string_list_t* string_list_out
    );

}

#endif /* PKMN_C_CPP_TO_C_HPP */
