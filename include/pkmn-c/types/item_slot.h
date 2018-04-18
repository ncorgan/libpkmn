/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_ITEM_SLOT_H
#define PKMN_C_TYPES_ITEM_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

struct pkmn_item_slot
{
    char* item;
    size_t amount;
};

struct pkmn_item_slots
{
    struct pkmn_item_slot* item_slots;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_item_slot_free(
    struct pkmn_item_slot* item_slot_ptr
);

PKMN_C_API enum pkmn_error pkmn_item_slots_free(
    struct pkmn_item_slots* item_slots_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_ITEM_SLOT_H */
