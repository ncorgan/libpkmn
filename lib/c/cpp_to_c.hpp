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

#include <pkmn-c/types/hidden_power.h>
#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/move_slot.h>
#include <pkmn-c/types/pokemon_list.h>
#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/string_types.h>

#include <pkmn-c/game_save.h>
#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>
#include <pkmn-c/pokedex.h>
#include <pkmn-c/pokemon.h>
#include <pkmn-c/pokemon_box.h>
#include <pkmn-c/pokemon_party.h>
#include <pkmn-c/pokemon_pc.h>

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
    pkmn::pokedex::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokedex_internal_t;

typedef struct
{
    pkmn::pokemon::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;

    int generation;
} pkmn_pokemon_internal_t;

typedef struct
{
    pkmn::pokemon_box::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_box_internal_t;

typedef struct
{
    pkmn::pokemon_party::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_party_internal_t;

typedef struct
{
    pkmn::pokemon_pc::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;
} pkmn_pokemon_pc_internal_t;

typedef struct
{
    pkmn::game_save::sptr cpp;
    boost::mutex error_mutex;
    std::string last_error;

    int generation;
} pkmn_game_save_internal_t;

#define ITEM_BAG_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_item_bag_internal_t*>(ptr))
#define ITEM_LIST_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_item_list_internal_t*>(ptr))
#define POKEDEX_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokedex_internal_t*>(ptr))
#define POKEMON_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_internal_t*>(ptr))
#define POKEMON_BOX_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_box_internal_t*>(ptr))
#define POKEMON_PARTY_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_party_internal_t*>(ptr))
#define POKEMON_PC_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_pokemon_pc_internal_t*>(ptr))
#define GAME_SAVE_INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_game_save_internal_t*>(ptr))

namespace pkmn { namespace c {

    // Calls to initialize internal representations.
    void init_item_bag(
        const pkmn::item_bag::sptr& cpp_item_bag,
        struct pkmn_item_bag* item_bag_ptr
    );

    void init_item_list(
        const pkmn::item_list::sptr& cpp_item_list,
        struct pkmn_item_list* item_list_ptr
    );

    void init_pokedex(
        const pkmn::pokedex::sptr& cpp_pokedex,
        struct pkmn_pokedex* pokedex_ptr
    );

    void init_pokemon(
        const pkmn::pokemon::sptr& cpp_pokemon,
        struct pkmn_pokemon* pokemon_ptr
    );

    void init_pokemon_box(
        const pkmn::pokemon_box::sptr& cpp_pokemon_box,
        struct pkmn_pokemon_box* pokemon_box_ptr
    );

    void init_pokemon_party(
        const pkmn::pokemon_party::sptr& cpp_pokemon_party,
        struct pkmn_pokemon_party* pokemon_party_ptr
    );

    void init_pokemon_pc(
        const pkmn::pokemon_pc::sptr& cpp_pokemon_pc,
        struct pkmn_pokemon_pc* pokemon_pc_ptr
    );

    void init_game_save(
        const pkmn::game_save::sptr& cpp_game_save,
        struct pkmn_game_save* game_save_ptr
    );

    template <typename pointer_type>
    inline void delete_pointer_and_set_to_null(pointer_type** pointer_ptr)
    {
        BOOST_ASSERT(pointer_ptr != nullptr);

        delete (*pointer_ptr);
        *pointer_ptr = nullptr;
    }

    template <typename pointer_type>
    inline void free_pointer_and_set_to_null(pointer_type** pointer_ptr)
    {
        BOOST_ASSERT(pointer_ptr != nullptr);

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
        BOOST_ASSERT(list_ptr != nullptr);

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
        struct pkmn_int_pair* int_pair_ptr
    )
    {
        BOOST_ASSERT(int_pair_ptr != nullptr);

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
        BOOST_ASSERT(c_str_ptr != nullptr);

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
        BOOST_ASSERT(c_str_ptr != nullptr);

        std::free(*c_str_ptr);
        string_cpp_to_c_alloc(
            string_cpp,
            c_str_ptr
        );
    }

    void string_list_cpp_to_c(
        const std::vector<std::string>& string_list_cpp,
        struct pkmn_string_list* string_list_c_ptr
    );

    inline void string_pair_cpp_to_c(
        const std::pair<std::string, std::string>& string_pair_cpp,
        struct pkmn_string_pair* c_pair_ptr
    )
    {
        BOOST_ASSERT(c_pair_ptr != nullptr);

        string_cpp_to_c_alloc(string_pair_cpp.first, &c_pair_ptr->first);
        string_cpp_to_c_alloc(string_pair_cpp.second, &c_pair_ptr->second);
    }

    template <typename value_type>
    void string_map_keys_to_string_list(
        const std::map<std::string, value_type>& map_cpp,
        struct pkmn_string_list* string_list_c_ptr
    )
    {
        BOOST_ASSERT(string_list_c_ptr != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_string_list temp_string_list =
        {
            nullptr,       // strings
            map_cpp.size() // length
        };

        if(temp_string_list.length > 0)
        {
            temp_string_list.strings = (char**)std::calloc(
                                                   temp_string_list.length,
                                                   sizeof(char*)
                                               );
            size_t index = 0;
            for(auto map_iter = map_cpp.begin();
                (map_iter != map_cpp.end()) && (index < temp_string_list.length);
                ++map_iter, ++index)
            {
                string_cpp_to_c_alloc(
                    map_iter->first,
                    &temp_string_list.strings[index]
                );
            }
        }

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *string_list_c_ptr = std::move(temp_string_list);
    }

    inline void hidden_power_cpp_to_c(
        const pkmn::calculations::hidden_power& hidden_power_cpp,
        struct pkmn_hidden_power* hidden_power_c_ptr
    )
    {
        BOOST_ASSERT(hidden_power_c_ptr != nullptr);

        string_cpp_to_c_alloc(
            hidden_power_cpp.type,
            &hidden_power_c_ptr->type
        );
        hidden_power_c_ptr->base_power = hidden_power_cpp.base_power;
    }

    inline void natural_gift_cpp_to_c(
        const pkmn::calculations::natural_gift& natural_gift_cpp,
        struct pkmn_natural_gift* natural_gift_c_ptr
    )
    {
        BOOST_ASSERT(natural_gift_c_ptr != nullptr);

        string_cpp_to_c_alloc(
            natural_gift_cpp.type,
            &natural_gift_c_ptr->type
        );
        natural_gift_c_ptr->base_power = natural_gift_cpp.base_power;
    }

    inline void item_slot_cpp_to_c(
        const pkmn::item_slot& item_slot_cpp,
        struct pkmn_item_slot* item_slot_c_ptr
    )
    {
        BOOST_ASSERT(item_slot_c_ptr != nullptr);

        string_cpp_to_c_alloc(
            item_slot_cpp.item,
            &item_slot_c_ptr->item
        );
        item_slot_c_ptr->amount = item_slot_cpp.amount;
    }

    void item_slots_cpp_to_c(
        const pkmn::item_slots_t& item_slots_cpp,
        struct pkmn_item_slots* item_slots_c_ptr
    );

    inline void levelup_move_cpp_to_c(
        const pkmn::database::levelup_move& levelup_move_cpp,
        struct pkmn_levelup_move* levelup_move_c_ptr
    )
    {
        BOOST_ASSERT(levelup_move_c_ptr != nullptr);

        string_cpp_to_c_alloc(
            levelup_move_cpp.move.get_name(),
            &levelup_move_c_ptr->move
        );
        levelup_move_c_ptr->level = levelup_move_cpp.level;
    }

    void levelup_moves_cpp_to_c(
        const pkmn::database::levelup_moves_t& levelup_moves_cpp,
        struct pkmn_levelup_moves* levelup_moves_c_ptr
    );

    void move_list_to_string_list(
        const pkmn::database::move_list_t& move_list,
        struct pkmn_string_list* string_list_ptr
    );

    inline void move_slot_cpp_to_c(
        const pkmn::move_slot& move_slot_cpp,
        struct pkmn_move_slot* move_slot_c_ptr
    )
    {
        BOOST_ASSERT(move_slot_c_ptr != nullptr);

        string_cpp_to_c_alloc(
            move_slot_cpp.move,
            &move_slot_c_ptr->move
        );
        move_slot_c_ptr->pp = move_slot_cpp.pp;
    }

    void move_slots_cpp_to_c(
        const pkmn::move_slots_t& move_slots_cpp,
        struct pkmn_move_slots* move_slots_c_ptr
    );

    void pokemon_entries_to_string_list(
        const pkmn::database::pokemon_entries_t& pokemon_entries,
        struct pkmn_string_list* string_list_ptr
    );

    void item_entry_cpp_to_c(
        const pkmn::database::item_entry& item_entry_cpp,
        struct pkmn_database_item_entry* item_entry_c_ptr
    );

    void move_entry_cpp_to_c(
        const pkmn::database::move_entry& move_entry_cpp,
        struct pkmn_database_move_entry* move_entry_c_ptr
    );

    void pokemon_entry_cpp_to_c(
        const pkmn::database::pokemon_entry& pokemon_entry_cpp,
        struct pkmn_database_pokemon_entry* pokemon_entry_c_ptr
    );

    void pokemon_list_cpp_to_c(
        const pkmn::pokemon_list_t& pokemon_list_cpp,
        struct pkmn_pokemon_list* pokemon_list_c_ptr
    );

    void pokemon_box_list_cpp_to_c(
        const pkmn::pokemon_box_list_t& pokemon_box_list_cpp,
        struct pkmn_pokemon_box_list* pokemon_box_list_c_ptr
    );

    template <typename sptr_type>
    void get_attribute_names_from_sptr(
        const std::shared_ptr<sptr_type>& libpkmn_sptr,
        struct pkmn_attribute_names* attribute_names_out
    )
    {
        BOOST_ASSERT(libpkmn_sptr.get() != nullptr);
        BOOST_ASSERT(attribute_names_out != nullptr);

        // Make all C++ calls and operate on a second struct until we
        // know everything succeeds before changing any user output.
        // If this fails, we'll leak, but it's small enough to not be
        // a concern.
        struct pkmn_attribute_names temp_attribute_names =
        {
            // numeric_attribute_names
            {
                nullptr, // strings
                0ULL     // length
            },
            // string_attribute_names
            {
                nullptr, // strings
                0ULL     // length
            },
            // boolean_attribute_names
            {
                nullptr, // strings
                0ULL     // length
            }
        };

        string_list_cpp_to_c(
            libpkmn_sptr->get_numeric_attribute_names(),
            &temp_attribute_names.numeric_attribute_names
        );
        string_list_cpp_to_c(
            libpkmn_sptr->get_string_attribute_names(),
            &temp_attribute_names.string_attribute_names
        );
        string_list_cpp_to_c(
            libpkmn_sptr->get_boolean_attribute_names(),
            &temp_attribute_names.boolean_attribute_names
        );

        // Everything succeeded, so move it into the pointer the caller
        // provided.
        *attribute_names_out = std::move(temp_attribute_names);
    }
}
}

#endif /* PKMN_C_CPP_TO_C_HPP */
