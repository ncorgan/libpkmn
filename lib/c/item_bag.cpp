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

#define INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_bag_internal_t*>(ptr)
#define LIST_INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_list_internal_t*>(ptr)

// The caller is expected to be exception-safe.
void init_item_bag(
    pkmn_item_bag_t* item_bag
)
{
    pkmn::item_bag::sptr cpp = INTERNAL_RCAST(item_bag->_internal)->cpp;

    pkmn::string_cpp_to_c_alloc(
        cpp->get_game(),
        &item_bag->game
    );

    const pkmn::item_pockets_t& pockets_cpp = cpp->get_pockets();
    item_bag->pockets.num_pockets = pockets_cpp.size();
    item_bag->pockets.pockets =
        (pkmn_item_list_t*)std::calloc(sizeof(pkmn_item_list_t)*pockets_cpp.size(), 1);
    item_bag->pockets.pocket_names.strings =
        (char**)std::calloc(sizeof(char*)*pockets_cpp.size(), 1);
    item_bag->pockets.pocket_names.length = pockets_cpp.size();

    const std::vector<std::string>& pocket_names = cpp->get_pocket_names();
    for(size_t i = 0; i < pocket_names.size(); ++i)
    {
        item_bag->pockets.pockets[i]._internal = new pkmn_item_list_internal_t;
        LIST_INTERNAL_RCAST(item_bag->pockets.pockets[i]._internal)->cpp = pockets_cpp.at(pocket_names[i]);
        init_item_list(&item_bag->pockets.pockets[i]);
    }

    pkmn::string_list_cpp_to_c(
        pocket_names,
        &item_bag->pockets.pocket_names
    );

    update_item_bag(item_bag);
}

// The caller is expected to be exception-safe.
void update_item_bag(
    pkmn_item_bag_t* item_bag
)
{
    for(size_t i = 0; i < item_bag->pockets.num_pockets; ++i)
    {
        update_item_list(&item_bag->pockets.pockets[i]);
    }
}

pkmn_error_t pkmn_item_bag_init(
    const char* game,
    pkmn_item_bag_t* item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(item_bag_out);

    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(game);
        item_bag_out->_internal = new pkmn_item_bag_internal_t;
        INTERNAL_RCAST(item_bag_out->_internal)->cpp = cpp;

        init_item_bag(item_bag_out);
    )
}

pkmn_error_t pkmn_item_bag_free(
    pkmn_item_bag_t* item_bag
)
{
    PKMN_CHECK_NULL_PARAM(item_bag);

    item_bag->game = NULL;

    pkmn_item_pockets_free(&item_bag->pockets);

    PKMN_CPP_TO_C(
        delete INTERNAL_RCAST(item_bag->_internal);
        item_bag->_internal = NULL;
    )
}

const char* pkmn_item_bag_strerror(
    pkmn_item_bag_t* item_bag
)
{
    if(!item_bag)
    {
        return NULL;
    }

    try
    {
        pkmn_item_bag_internal_t* internal_ptr = INTERNAL_RCAST(item_bag->_internal);
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

pkmn_error_t pkmn_item_bag_get_pocket(
    pkmn_item_bag_t* item_bag,
    const char* pocket_name,
    pkmn_item_list_t** item_list_out
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag);
    pkmn_item_bag_internal_t* internal_ptr = INTERNAL_RCAST(item_bag->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pocket_name, internal_ptr);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_list_out, internal_ptr);

    bool was_pocket_found = false;

    for(size_t i = 0; i < item_bag->pockets.num_pockets; ++i)
    {
        if(!std::strcmp(pocket_name, item_bag->pockets.pockets[i].name))
        {
            *item_list_out = &item_bag->pockets.pockets[i];
            was_pocket_found = true;
            break;
        }
    }

    return was_pocket_found ? PKMN_ERROR_NONE : PKMN_ERROR_INVALID_ARGUMENT;
}

pkmn_error_t pkmn_item_bag_add(
    pkmn_item_bag_t* item_bag,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag);
    pkmn_item_bag_internal_t* internal_ptr = INTERNAL_RCAST(item_bag->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_bag::sptr cpp = internal_ptr->cpp;

        cpp->add(
            item,
            amount
        );

        // Only update the pocket the item corresponds to.
        pkmn::database::item_entry entry(item, cpp->get_game());
        for(size_t i = 0; i < item_bag->pockets.num_pockets; ++i)
        {
            if(!std::strcmp(entry.get_pocket().c_str(), item_bag->pockets.pocket_names.strings[i]))
            {
                update_item_list(&item_bag->pockets.pockets[i]);
            }
        }
    )
}

pkmn_error_t pkmn_item_bag_remove(
    pkmn_item_bag_t* item_bag,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_WRAPPER_PARAM(item_bag);
    pkmn_item_bag_internal_t* internal_ptr = INTERNAL_RCAST(item_bag->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::item_bag::sptr cpp = internal_ptr->cpp;

        cpp->remove(
            item,
            amount
        );

        // Only update the pocket the item corresponds to.
        pkmn::database::item_entry entry(item, cpp->get_game());
        for(size_t i = 0; i < item_bag->pockets.num_pockets; ++i)
        {
            if(!std::strcmp(entry.get_pocket().c_str(), item_bag->pockets.pocket_names.strings[i]))
            {
                update_item_list(&item_bag->pockets.pockets[i]);
            }
        }
    )
}
