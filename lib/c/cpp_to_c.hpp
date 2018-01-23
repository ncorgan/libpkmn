/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pkmn/pokedex.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/pokemon_pc.hpp>

#include <pkmn/calculations/moves/hidden_power.hpp>
#include <pkmn/calculations/moves/natural_gift.hpp>

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
#include <pkmn-c/types/natural_gift.h>
#include <pkmn-c/types/string_types.h>

#include <boost/assert.hpp>
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
#include <pkmn-c/pokedex.h>
#include <pkmn-c/pokemon2.h>

// Internal representations
typedef struct
{
    pkmn::item_bag::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_item_bag_internal_t;

typedef struct
{
    pkmn::item_list::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_item_list_internal_t;

typedef struct
{
    pkmn::pokemon::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;

    int generation;
} pkmn_pokemon_internal_t;

typedef struct
{
    pkmn::pokedex::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokedex_internal_t;

// Calls to initialize or update internal representations.
void init_item_bag(
    pkmn::item_bag::sptr cpp_item_bag,
    pkmn_item_bag_t* item_bag_ptr
);
void init_item_list(
    pkmn::item_list::sptr cpp_item_list,
    pkmn_item_list_t* item_list_ptr
);
void init_pokedex(
    pkmn::pokedex::sptr cpp_pokedex,
    pkmn_pokedex_t* pokedex_ptr
);

namespace pkmn { namespace c {

    template <typename pointer_type>
    inline void delete_pointer_and_set_to_null(pointer_type** pointer_ptr)
    {
        delete (*pointer_ptr);
        *pointer_ptr = nullptr;
    }

    template <typename pointer_type>
    inline void free_pointer_and_set_to_null(pointer_type** pointer_ptr)
    {
        std::free(*pointer_ptr);
        *pointer_ptr = nullptr;
    }

    template <typename T>
    void list_cpp_to_c(
        const std::vector<T>& list_cpp,
        T* list_ptr,
        size_t buffer_len,
        size_t* num_values_out
    )
    {
        BOOST_ASSERT(list_ptr);

        if(!list_cpp.empty())
        {
            std::memcpy(
                list_ptr,
                list_cpp.data(),
                buffer_len * sizeof(T)
            );
        }

        if(num_values_out)
        {
            *num_values_out = list_cpp.size();
        }
    }

    inline void int_pair_cpp_to_c(
        const std::pair<int, int>& int_pair_cpp,
        pkmn_int_pair_t* int_pair_ptr
    )
    {
        BOOST_ASSERT(int_pair_ptr);

        int_pair_ptr->first = int_pair_cpp.first;
        int_pair_ptr->second = int_pair_cpp.second;
    }

    void string_cpp_to_c(
        const std::string& string_cpp,
        char* c_str_ptr,
        size_t buffer_len,
        size_t* string_length_out
    );

    inline void string_cpp_to_c_alloc(
        const std::string& string_cpp,
        char** c_str_ptr
    )
    {
        BOOST_ASSERT(c_str_ptr);

        *c_str_ptr = (char*)std::calloc(string_cpp.size()+1, sizeof(char));
        std::strncpy(
            *c_str_ptr,
            string_cpp.c_str(),
            string_cpp.size()
        );
    }

    inline void string_cpp_to_c_realloc(
        const std::string& string_cpp,
        char** c_str_ptr
    )
    {
        BOOST_ASSERT(c_str_ptr);

        std::free(*c_str_ptr);
        string_cpp_to_c_alloc(
            string_cpp,
            c_str_ptr
        );
    }

    void string_list_cpp_to_c(
        const std::vector<std::string>& string_list_cpp,
        pkmn_string_list_t* string_list_c_ptr
    );

    inline void string_pair_cpp_to_c(
        const std::pair<std::string, std::string>& string_pair_cpp,
        pkmn_string_pair_t* c_pair_ptr
    )
    {
        BOOST_ASSERT(c_pair_ptr);

        string_cpp_to_c_alloc(string_pair_cpp.first, &c_pair_ptr->first);
        string_cpp_to_c_alloc(string_pair_cpp.second, &c_pair_ptr->second);
    }

    template <typename value_type>
    void string_map_keys_to_string_list(
        const std::map<std::string, value_type>& map_cpp,
        pkmn_string_list_t* string_list_c_ptr
    )
    {
        BOOST_ASSERT(string_list_c_ptr);

        size_t num_keys = map_cpp.size();
        if(num_keys > 0)
        {
            string_list_c_ptr->strings = (char**)std::calloc(
                                                     num_keys,
                                                     sizeof(char*)
                                                 );
            size_t index = 0;
            for(auto map_iter = map_cpp.begin();
                (map_iter != map_cpp.end()) && (index < num_keys);
                ++map_iter, ++index
               )
            {
                string_cpp_to_c_alloc(
                    map_iter->first,
                    &string_list_c_ptr->strings[index]
                );
            }
        }

        string_list_c_ptr->length = num_keys;
    }

    inline void hidden_power_cpp_to_c(
        const pkmn::calculations::hidden_power& hidden_power_cpp,
        pkmn_hidden_power_t* hidden_power_c_ptr
    )
    {
        BOOST_ASSERT(hidden_power_c_ptr);

        string_cpp_to_c_alloc(
            hidden_power_cpp.type,
            &hidden_power_c_ptr->type
        );
        hidden_power_c_ptr->base_power = hidden_power_cpp.base_power;
    }

    inline void natural_gift_cpp_to_c(
        const pkmn::calculations::natural_gift& natural_gift_cpp,
        pkmn_natural_gift_t* natural_gift_c_ptr
    )
    {
        BOOST_ASSERT(natural_gift_c_ptr);

        string_cpp_to_c_alloc(
            natural_gift_cpp.type,
            &natural_gift_c_ptr->type
        );
        natural_gift_c_ptr->base_power = natural_gift_cpp.base_power;
    }

    inline void item_slot_cpp_to_c(
        const pkmn::item_slot& item_slot_cpp,
        pkmn_item_slot_t* item_slot_c_ptr
    )
    {
        BOOST_ASSERT(item_slot_c_ptr);

        string_cpp_to_c_alloc(
            item_slot_cpp.item,
            &item_slot_c_ptr->item
        );
        item_slot_c_ptr->amount = item_slot_cpp.amount;
    }

    void item_slots_cpp_to_c(
        const pkmn::item_slots_t& item_slots_cpp,
        pkmn_item_slots_t* item_slots_c_ptr
    );

    inline void levelup_move_cpp_to_c(
        const pkmn::database::levelup_move& levelup_move_cpp,
        pkmn_levelup_move_t* levelup_move_c_ptr
    )
    {
        BOOST_ASSERT(levelup_move_c_ptr);

        string_cpp_to_c_alloc(
            levelup_move_cpp.move.get_name(),
            &levelup_move_c_ptr->move
        );
        levelup_move_c_ptr->level = levelup_move_cpp.level;
    }

    void levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t& levelup_moves_cpp,
        pkmn_levelup_moves_t* levelup_moves_c_ptr
    );

    void move_list_to_string_list(
        const pkmn::database::move_list_t& move_list,
        pkmn_string_list_t* string_list_ptr
    );

    inline void move_slot_cpp_to_c(
        const pkmn::move_slot& move_slot_cpp,
        pkmn_move_slot_t* move_slot_c_ptr
    )
    {
        BOOST_ASSERT(move_slot_c_ptr);

        string_cpp_to_c_alloc(
            move_slot_cpp.move,
            &move_slot_c_ptr->move
        );
        move_slot_c_ptr->pp = move_slot_cpp.pp;
    }

    void move_slots_cpp_to_c(
        const pkmn::move_slots_t& move_slots_cpp,
        pkmn_move_slots_t* move_slots_c_ptr
    );

    void pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t& pokemon_entries,
        pkmn_string_list_t* string_list_ptr
    );

    void item_entry_cpp_to_c(
        const pkmn::database::item_entry& item_entry_cpp,
        pkmn_database_item_entry_t* item_entry_c_ptr
    );

    void move_entry_cpp_to_c(
        const pkmn::database::move_entry& move_entry_cpp,
        pkmn_database_move_entry_t* move_entry_c_ptr
    );

    void pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry& pokemon_entry_cpp,
        pkmn_database_pokemon_entry_t* pokemon_entry_c_ptr
    );

    void pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t& pokemon_list_cpp,
        pkmn_pokemon_list_t* pokemon_list_c_ptr
    );

    void pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t& pokemon_box_list_cpp,
        pkmn_pokemon_box_list_t* pokemon_box_list_c_ptr
    );
}
}

#endif /* PKMN_C_CPP_TO_C_HPP */
