/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_MOVE_DAMAGE_CLASS_HPP
#define PKMN_ENUMS_MOVE_DAMAGE_CLASS_HPP

namespace pkmn
{
    enum class e_move_damage_class
    {
        NONE = 0,
        STATUS = 1,
        PHYSICAL = 2,
        SPECIAL = 3,
    };
}

#endif /* PKMN_ENUMS_MOVE_DAMAGE_CLASS_HPP */
