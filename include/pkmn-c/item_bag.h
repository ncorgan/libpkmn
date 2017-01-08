/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_BAG_H
#define PKMN_C_ITEM_BAG_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_list.h>
#include <pkmn-c/types/string_types.h>

#if !defined(PKMN_ITEM_BAG_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_item_bag_t;
typedef struct pkmn_item_bag_t pkmn_item_bag_t;
#define PKMN_ITEM_BAG_DECLARED
#endif

typedef pkmn_item_bag_t* pkmn_item_bag_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_item_bag_make(
    pkmn_item_bag_handle_t* handle_ptr,
    const char* game_name
);

PKMN_API pkmn_error_t pkmn_item_bag_free(
    pkmn_item_bag_handle_t* handle_ptr
);

PKMN_API const char* pkmn_item_bag_strerror(
    pkmn_item_bag_handle_t handle
);

PKMN_API pkmn_error_t pkmn_item_bag_get_game(
    pkmn_item_bag_handle_t handle,
    char* game_out,
    size_t buffer_len
);

PKMN_API pkmn_error_t pkmn_item_bag_get_pocket(
    pkmn_item_bag_handle_t handle,
    const char* name,
    pkmn_item_list_handle_t* item_list_out
);

PKMN_API pkmn_error_t pkmn_item_bag_get_pocket_names(
    pkmn_item_bag_handle_t handle,
    pkmn_string_list_t* pocket_names_out
);

PKMN_API pkmn_error_t pkmn_item_bag_add(
    pkmn_item_bag_handle_t handle,
    const char* item_name,
    int amount
);

PKMN_API pkmn_error_t pkmn_item_bag_remove(
    pkmn_item_bag_handle_t handle,
    const char* item_name,
    int amount
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_BAG_H */
