/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H
#define PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H

enum pkmn_move_damage_class
{
    PKMN_MOVE_DAMAGE_CLASS_NONE = 0,
    PKMN_MOVE_DAMAGE_CLASS_STATUS = 1,
    PKMN_MOVE_DAMAGE_CLASS_PHYSICAL = 2,
    PKMN_MOVE_DAMAGE_CLASS_SPECIAL = 3,
};

#endif /* PKMN_C_ENUMS_MOVE_DAMAGE_CLASS_H */
