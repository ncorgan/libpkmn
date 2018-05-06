/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_bag.h>

#include <cstdio>

enum pkmn_error pkmn_item_bag_init(
    const char* game,
    struct pkmn_item_bag* item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(item_bag_out);

    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(game);

        pkmn::c::init_item_bag(
            cpp,
            item_bag_out
        );
    )
}

enum pkmn_error pkmn_item_bag_free(
    struct pkmn_item_bag* item_bag_ptr
)
{
    PKMN_CHECK_NULL_PARAM(item_bag_ptr);

    pkmn::c::free_pointer_and_set_to_null(&item_bag_ptr->game);
    pkmn_string_list_free(&item_bag_ptr->pocket_names);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_item_bag_internal_t**>(&item_bag_ptr->p_internal)
        );
    )
}

const char* pkmn_item_bag_strerror(
    struct pkmn_item_bag* item_bag_ptr
)
{
    if(!item_bag_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_item_bag_internal_t* internal_ptr = ITEM_BAG_INTERNAL_RCAST(item_bag_ptr->p_internal);
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

enum pkmn_error pkmn_item_bag_get_pocket(
    struct pkmn_item_bag* item_bag_ptr,
    const char* pocket_name,
    struct pkmn_item_list* item_list_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag_ptr);
    pkmn_item_bag_internal_t* internal_ptr = ITEM_BAG_INTERNAL_RCAST(item_bag_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pocket_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_list_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_bag::sptr cpp = internal_ptr->cpp;

        pkmn::item_list::sptr cpp_pocket = cpp->get_pocket(pocket_name);

        pkmn::c::init_item_list(
            cpp_pocket,
            item_list_out
        );
    )
}

enum pkmn_error pkmn_item_bag_add(
    struct pkmn_item_bag* item_bag_ptr,
    const char* item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag_ptr);
    pkmn_item_bag_internal_t* internal_ptr = ITEM_BAG_INTERNAL_RCAST(item_bag_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_bag::sptr cpp = internal_ptr->cpp;

        cpp->add(
            item,
            int(amount)
        );
    )
}

enum pkmn_error pkmn_item_bag_remove(
    struct pkmn_item_bag* item_bag_ptr,
    const char* item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag_ptr);
    pkmn_item_bag_internal_t* internal_ptr = ITEM_BAG_INTERNAL_RCAST(item_bag_ptr->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_bag::sptr cpp = internal_ptr->cpp;

        cpp->remove(
            item,
            int(amount)
        );
    )
}
