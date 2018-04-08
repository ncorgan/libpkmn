/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

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

        pkmn::c::init_item_list(
            cpp,
            item_list_out
        );
    )
}

pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_t* item_list_ptr
)
{
    PKMN_CHECK_NULL_PARAM(item_list_ptr);

    pkmn::c::free_pointer_and_set_to_null(&item_list_ptr->name);
    pkmn::c::free_pointer_and_set_to_null(&item_list_ptr->game);
    item_list_ptr->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_item_list_internal_t**>(&item_list_ptr->_internal)
        );
    )
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_t* item_list_ptr
)
{
    if(!item_list_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
        if(!internal_ptr)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_t* item_list_ptr,
    size_t* num_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(num_items_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        *num_items_out = cpp->get_num_items();
    )
}

pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    struct pkmn_item_slot* item_slot_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_slot_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        pkmn::c::item_slot_cpp_to_c(
            cpp->at(int(position)),
            item_slot_out
        );
    )
}

pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->add(
            item,
            int(amount)
        );
    )
}

pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_t* item_list_ptr,
    const char* item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->remove(
            item,
            int(amount)
        );
    )
}

pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_t* item_list_ptr,
    size_t old_position,
    size_t new_position
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->move(
            int(old_position),
            int(new_position)
        );
    )
}

pkmn_error_t pkmn_item_list_set_item(
    pkmn_item_list_t* item_list_ptr,
    size_t position,
    const char* item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        cpp->set_item(
            int(position),
            item,
            int(amount)
        );
    )
}

pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_t* item_list_ptr,
    struct pkmn_string_list* valid_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(valid_items_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        pkmn::c::string_list_cpp_to_c(
            cpp->get_valid_items(),
            valid_items_out
        );
    )
}

pkmn_error_t pkmn_item_list_as_list(
    pkmn_item_list_t* item_list_ptr,
    struct pkmn_item_slots* item_slots_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_list_ptr);
    pkmn_item_list_internal_t* internal_ptr = ITEM_LIST_INTERNAL_RCAST(item_list_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_slots_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_list::sptr cpp = internal_ptr->cpp;

        pkmn::c::item_slots_cpp_to_c(
            cpp->as_vector(),
            item_slots_out
        );
    )
}
