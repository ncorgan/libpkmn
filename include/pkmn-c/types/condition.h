/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_CONDITION_H
#define PKMN_C_TYPES_CONDITION_H

typedef enum {
    PKMN_CONDITION_NONE = 0,
    PKMN_CONDITION_ASLEEP,
    PKMN_CONDITION_POISON,
    PKMN_CONDITION_BURN,
    PKMN_CONDITION_FROZEN,
    PKMN_CONDITION_PARALYSIS,
    PKMN_CONDITION_BAD_POISON
} pkmn_condition_t;

#endif /* PKMN_C_TYPES_CONDITION_H */
