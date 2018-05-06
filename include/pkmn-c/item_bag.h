/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_BAG_H
#define PKMN_C_ITEM_BAG_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_list.h>
#include <pkmn-c/types/string_types.h>

struct pkmn_item_bag
{
    char* p_game;
    struct pkmn_string_list pocket_names;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_item_bag_init(
    const char* p_game,
    struct pkmn_item_bag* p_item_bag_out
);

PKMN_C_API enum pkmn_error pkmn_item_bag_free(
    struct pkmn_item_bag* p_item_bag
);

PKMN_C_API const char* pkmn_item_bag_strerror(
    struct pkmn_item_bag* p_item_bag
);

PKMN_C_API enum pkmn_error pkmn_item_bag_get_pocket(
    struct pkmn_item_bag* p_item_bag,
    const char* pocket_name,
    struct pkmn_item_list* p_item_list_out
);

PKMN_C_API enum pkmn_error pkmn_item_bag_add(
    struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
);

PKMN_C_API enum pkmn_error pkmn_item_bag_remove(
    struct pkmn_item_bag* p_item_bag,
    const char* p_item,
    size_t amount
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_BAG_H */
