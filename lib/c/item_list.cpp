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

#define INTERNAL_RCAST(ptr) (reinterpret_cast<pkmn_item_list_internal_t*>(ptr))

// The caller is expected to be exception-safe.
void init_item_list(
    pkmn_item_list_t* item_list
)
{
    pkmn::item_list::sptr cpp = INTERNAL_RCAST(item_list->_internal)->cpp;

    pkmn::c::string_cpp_to_c_alloc(
        cpp->get_name(),
        &item_list->name
    );
    pkmn::c::string_cpp_to_c_alloc(
        cpp->get_game(),
        &item_list->game
    );

    item_list->num_items = cpp->get_num_items();
    item_list->capacity = cpp->get_capacity();

    item_list->item_slots.item_slots =
        (pkmn_item_slot_t*)std::malloc(
                               cpp->get_capacity()*sizeof(pkmn_item_slot_t)
                           );
    item_list->item_slots.length = item_list->capacity;

    update_item_list(item_list);
}

// The caller is expected to be exception-safe.
void update_item_list(
    pkmn_item_list_t* item_list
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

    item_list->num_items = cpp->get_num_items();
}

pkmn_error_t pkmn_item_list_init(
    const char* name,
    const char* game,
    pkmn_item_list_t* item_list_out
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
    )
}

pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_t* item_list
)
{
    PKMN_CHECK_NULL_PARAM(item_list);

    item_list->name = NULL;
    item_list->game = NULL;
    item_list->num_items = 0;
    item_list->capacity = 0;

    pkmn_item_slots_free(&item_list->item_slots);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_item_list_internal_t**>(&item_list->_internal)
        );
    )
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_t* item_list
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

pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_t* item_list,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->add(
            item,
            amount
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_t* item_list,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->remove(
            item,
            amount
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_t* item_list,
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

pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_t* item_list,
    int position,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->set_item(
            position,
            item,
            amount
        );

        update_item_list(item_list);
    )
}

pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_t* item_list,
    pkmn_string_list_t* valid_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list);
    pkmn_item_list_internal_t* internal_ptr = INTERNAL_RCAST(item_list->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(valid_items_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        pkmn::c::string_list_cpp_to_c(
            cpp->get_valid_items(),
            valid_items_out
        );
    )
}
