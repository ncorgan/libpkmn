/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_EGG_GROUP_HPP
#define PKMN_ENUMS_EGG_GROUP_HPP

namespace pkmn
{
    enum class e_egg_group
    {
        NONE = 0,
        MONSTER = 1,
        WATER1 = 2,
        BUG = 3,
        FLYING = 4,
        GROUND = 5,
        FAIRY = 6,
        PLANT = 7,
        HUMANSHAPE = 8,
        WATER3 = 9,
        MINERAL = 10,
        INDETERMINATE = 11,
        WATER2 = 12,
        DITTO = 13,
        DRAGON = 14,
        UNDISCOVERED = 15,
    };
}

#endif /* PKMN_ENUMS_EGG_GROUP_HPP */
