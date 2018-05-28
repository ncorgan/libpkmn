/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_list.h>

enum pkmn_error pkmn_item_list_init(
    const char* p_name,
    const char* p_game,
    struct pkmn_item_list* p_item_list_out
)
{
    PKMN_CHECK_NULL_PARAM(p_name);
    PKMN_CHECK_NULL_PARAM(p_game);
    PKMN_CHECK_NULL_PARAM(p_item_list_out);

    PKMN_CPP_TO_C(
        pkmn::item_list::sptr cpp = pkmn::item_list::make(p_name, p_game);

        pkmn::c::init_item_list(
            cpp,
            p_item_list_out
        );
    )
}

enum pkmn_error pkmn_item_list_free(
    struct pkmn_item_list* p_item_list
)
{
    PKMN_CHECK_NULL_PARAM(p_item_list);

    pkmn::c::free_pointer_and_set_to_null(&p_item_list->p_name);
    pkmn::c::free_pointer_and_set_to_null(&p_item_list->p_game);
    p_item_list->capacity = 0;

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_item_list_internal_t**>(&p_item_list->p_internal)
        );
    )
}

const char* pkmn_item_list_strerror(
    const struct pkmn_item_list* p_item_list
)
{
    if(!p_item_list)
    {
        return nullptr;
    }

    try
    {
        pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
        if(!p_internal)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(p_internal->error_mutex);
        return p_internal->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

enum pkmn_error pkmn_item_list_get_num_items(
    const struct pkmn_item_list* p_item_list,
    size_t* p_num_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_num_items_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        *p_num_items_out = cpp->get_num_items();
    )
}

enum pkmn_error pkmn_item_list_at(
    const struct pkmn_item_list* p_item_list,
    size_t position,
    struct pkmn_item_slot* p_item_slot_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item_slot_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        pkmn::c::item_slot_cpp_to_c(
            cpp->at(int(position)),
            p_item_slot_out
        );
    )
}

enum pkmn_error pkmn_item_list_add(
    const struct pkmn_item_list* p_item_list,
    const char* p_item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        cpp->add(
            p_item,
            int(amount)
        );
    )
}

enum pkmn_error pkmn_item_list_remove(
    const struct pkmn_item_list* p_item_list,
    const char* p_item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        cpp->remove(
            p_item,
            int(amount)
        );
    )
}

enum pkmn_error pkmn_item_list_move(
    const struct pkmn_item_list* p_item_list,
    size_t old_position,
    size_t new_position
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        cpp->move(
            int(old_position),
            int(new_position)
        );
    )
}

enum pkmn_error pkmn_item_list_set_item(
    const struct pkmn_item_list* p_item_list,
    size_t position,
    const char* p_item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        cpp->set_item(
            int(position),
            p_item,
            int(amount)
        );
    )
}

enum pkmn_error pkmn_item_list_get_valid_items(
    const struct pkmn_item_list* p_item_list,
    struct pkmn_string_list* p_valid_items_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_valid_items_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        pkmn::c::string_list_cpp_to_c(
            cpp->get_valid_items(),
            p_valid_items_out
        );
    )
}

enum pkmn_error pkmn_item_list_as_list(
    const struct pkmn_item_list* p_item_list,
    struct pkmn_item_slots* p_item_slots_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_list);
    pkmn_item_list_internal_t* p_internal = ITEM_LIST_INTERNAL_RCAST(p_item_list->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item_slots_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_list::sptr cpp = p_internal->cpp;

        pkmn::c::item_slots_cpp_to_c(
            cpp->as_vector(),
            p_item_slots_out
        );
    )
}
