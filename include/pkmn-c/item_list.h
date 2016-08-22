/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_LIST_H
#define PKMN_C_ITEM_LIST_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#if !defined(PKMN_ITEM_LIST_DECLARED) && !defined(__DOXYGEN__)
struct pkmn_item_list_t;
typedef struct pkmn_item_list_t pkmn_item_list_t;
#define PKMN_ITEM_LIST_DECLARED
#endif

typedef pkmn_item_list_t* pkmn_item_list_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API pkmn_error_t pkmn_item_list_make(
    pkmn_item_list_handle_t* handle_ptr,
    const char* item_name,
    const char* game_name
);

PKMN_API void pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
);

PKMN_API const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_LIST_H */
