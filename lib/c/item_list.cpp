/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

#include <cstdio>

#define INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_list_internal_t*>(ptr)

// The caller is expected to be exception-safe.
void init_item_list(
    pkmn_item_list2_t* item_list
)
{
    pkmn::item_list::sptr cpp = INTERNAL_RCAST(item_list->_internal)->cpp;

    std::string name = cpp->get_name();
    item_list->name = (char*)std::calloc(name.size() + 1, 1);
    std::strcpy(item_list->name, name.c_str());
    item_list->name[name.size()] = '\0';

    std::string game = cpp->get_game();
    item_list->game = (char*)std::calloc(game.size() + 1, 1);
    std::strcpy(item_list->game, game.c_str());
    item_list->game[game.size()] = '\0';

    item_list->num_items = cpp->get_num_items();
    item_list->capacity = cpp->get_capacity();

    item_list->item_slots.item_slots =
        (pkmn_item_slot_t*)std::malloc(
                               cpp->get_capacity()*sizeof(pkmn_item_slot_t)
                           );
    item_list->item_slots.length = item_list->capacity;
}

// The caller is expected to be exception-safe.
void update_item_list(
    pkmn_item_list2_t* item_list
)
{
    pkmn::item_list::sptr cpp = INTERNAL_RCAST(item_list->_internal)->cpp;

    for(size_t i = 0; i < item_list->capacity; ++i)
    {
        const pkmn::item_slot& slot_cpp = cpp->at(int(i));

        std::strncpy(
            item_list->item_slots.item_slots[i].item,
            slot_cpp.item.c_str(),
            sizeof(item_list->item_slots.item_slots[i].item)
        );
        item_list->item_slots.item_slots[i].amount = slot_cpp.amount;
    }
}

pkmn_error_t pkmn_item_list2_init(
    const char* name,
    const char* game,
    pkmn_item_list2_t* item_list_out
)
{
    PKMN_CHECK_NULL_PARAM(name);
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(item_list_out);

    PKMN_CPP_TO_C(
        pkmn::item_list::sptr cpp = pkmn::item_list::make(name, game);
        item_list_out->_internal = new pkmn_item_list_internal_t;
        INTERNAL_RCAST(item_list_out->_internal)->cpp = cpp;
        INTERNAL_RCAST(item_list_out->_internal)->last_error = "None";

        init_item_list(item_list_out);
        update_item_list(item_list_out);
    )
}

pkmn_error_t pkmn_item_list2_free(
    pkmn_item_list2_t* item_list
)
{
    PKMN_CHECK_NULL_PARAM(item_list);

    item_list->name = NULL;
    item_list->game = NULL;
    item_list->num_items = 0;
    item_list->capacity = 0;
    std::free(item_list->item_slots.item_slots);
    item_list->item_slots.length = 0;

    PKMN_CPP_TO_C(
        delete INTERNAL_RCAST(item_list->_internal);
        item_list->_internal = NULL;
    )
}

const char* pkmn_item_list2_strerror(
    pkmn_item_list2_t* item_list
)
{
    if(!item_list)
    {
        return NULL;
    }

    try
    {
        pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);
        if(!internal_ptr)
        {
            return NULL;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return NULL;
    }
}

pkmn_error_t pkmn_item_list2_add(
    pkmn_item_list2_t* item_list,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->add(
            item,
            amount
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list2_remove(
    pkmn_item_list2_t* item_list,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->remove(
            item,
            amount
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list2_move(
    pkmn_item_list2_t* item_list,
    int old_position,
    int new_position
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->move(
            old_position,
            new_position
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list2_set_item(
    pkmn_item_list2_t* item_list,
    int position,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->set_item(
            position,
            {item, amount} 
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list2_get_valid_items(
    pkmn_item_list2_t* item_list,
    pkmn_string_list_t* valid_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        pkmn::std_vector_std_string_to_string_list(
            cpp->get_valid_items(),
            valid_items_out
        );
    )
}

// OLD BELOW

pkmn_error_t pkmn_item_list_make(
    pkmn_item_list_handle_t* handle_ptr,
    const char* item_list_name,
    const char* game_name
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(item_list_name);
    PKMN_CHECK_NULL_PARAM(game_name);

    PKMN_CPP_TO_C(
        pkmn::item_list::sptr cpp = pkmn::item_list::make(
                                        item_list_name,
                                        game_name
                                    );

        (*handle_ptr) = new pkmn_item_list_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
) {
    if(!handle) {
        return NULL;
    }

    try {
        boost::mutex::scoped_lock lock(handle->error_mutex);
        return handle->last_error.c_str();
    } catch(...) {
        return NULL;
    }
}

pkmn_error_t pkmn_item_list_get_name(
    pkmn_item_list_handle_t handle,
    char* name_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_item_list_handle_t>(
                   handle,
                   handle->cpp->get_name(),
                   name_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_item_list_get_game(
    pkmn_item_list_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_item_list_handle_t>(
                   handle,
                   handle->cpp->get_game(),
                   game_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_item_list_get_capacity(
    pkmn_item_list_handle_t handle,
    int* capacity_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(capacity_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *capacity_out = handle->cpp->get_capacity();
    )
}

pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_handle_t handle,
    int* num_items_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_items_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *num_items_out = handle->cpp->get_num_items();
    )
}

pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_handle_t handle,
    int position,
    pkmn_item_slot_t* item_slot_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_slot_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_item_slot_cpp_to_c(
            handle->cpp->at(position),
            item_slot_out
        );
    )
}

pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->add(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->remove(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_handle_t handle,
    int old_position,
    int new_position
) {
    PKMN_CHECK_NULL_PARAM(handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->move(
            old_position,
            new_position
        );
    )
}

pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_handle_t handle,
    int position,
    const char* item_name,
    int amount
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM(item_name);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->set_item(
            position,
            {item_name, amount}
        );
    )
}

pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_handle_t handle,
    pkmn_string_list_t* string_list_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(string_list_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_vector_std_string_to_string_list(
            handle->cpp->get_valid_items(),
            string_list_out
        );
    )
}

pkmn_error_t pkmn_item_list_as_array(
    pkmn_item_list_handle_t handle,
    pkmn_item_slots_t* array_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(array_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_item_slots_cpp_to_c(
            handle->cpp->as_vector(),
            array_out
        );
    )
}
