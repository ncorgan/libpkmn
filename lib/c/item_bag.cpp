/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_bag.h>

#include <cstdio>

enum pkmn_error pkmn_item_bag_init(
    enum pkmn_game game,
    struct pkmn_item_bag* p_item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(p_item_bag_out);

    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(
                                       static_cast<pkmn::e_game>(game)
                                   );
        pkmn::c::init_item_bag(
            cpp,
            p_item_bag_out
        );
    )
}

enum pkmn_error pkmn_item_bag_free(
    struct pkmn_item_bag* p_item_bag
)
{
    PKMN_CHECK_NULL_PARAM(p_item_bag);

    p_item_bag->game = PKMN_GAME_NONE;
    pkmn_string_list_free(&p_item_bag->pocket_names);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_item_bag_internal_t**>(&p_item_bag->p_internal)
        );
    )
}

const char* pkmn_item_bag_strerror(
    const struct pkmn_item_bag* p_item_bag
)
{
    if(!p_item_bag)
    {
        return nullptr;
    }

    try
    {
        pkmn_item_bag_internal_t* p_internal = ITEM_BAG_INTERNAL_RCAST(p_item_bag->p_internal);
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

enum pkmn_error pkmn_item_bag_get_pocket(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_pocket_name,
    struct pkmn_item_list* p_item_list_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_bag);
    pkmn_item_bag_internal_t* p_internal = ITEM_BAG_INTERNAL_RCAST(p_item_bag->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_pocket_name, p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item_list_out, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_bag::sptr cpp = p_internal->cpp;

        pkmn::item_list::sptr cpp_pocket = cpp->get_pocket(p_pocket_name);

        pkmn::c::init_item_list(
            cpp_pocket,
            p_item_list_out
        );
    )
}

enum pkmn_error pkmn_item_bag_add(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_bag);
    pkmn_item_bag_internal_t* p_internal = ITEM_BAG_INTERNAL_RCAST(p_item_bag->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_bag::sptr cpp = p_internal->cpp;

        cpp->add(
            p_item,
            int(amount)
        );
    )
}

enum pkmn_error pkmn_item_bag_remove(
    const struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(p_item_bag);
    pkmn_item_bag_internal_t* p_internal = ITEM_BAG_INTERNAL_RCAST(p_item_bag->p_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(p_item, p_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(p_internal,
        pkmn::item_bag::sptr cpp = p_internal->cpp;

        cpp->remove(
            p_item,
            int(amount)
        );
    )
}
