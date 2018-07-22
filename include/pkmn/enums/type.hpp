/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_TYPE_HPP
#define PKMN_ENUMS_TYPE_HPP

#include <utility>

namespace pkmn
{
    enum class e_type
    {
        NONE = 0,
        NORMAL = 1,
        FIGHTING = 2,
        FLYING = 3,
        POISON = 4,
        GROUND = 5,
        ROCK = 6,
        BUG = 7,
        GHOST = 8,
        STEEL = 9,
        FIRE = 10,
        WATER = 11,
        GRASS = 12,
        ELECTRIC = 13,
        PSYCHIC = 14,
        ICE = 15,
        DRAGON = 16,
        DARK = 17,
        FAIRY = 18,
        QUESTION_MARK = 10001,
        SHADOW = 10002,
        INVALID = 30000,
    };

    using type_pair_t = std::pair<pkmn::e_type, pkmn::e_type>;
}

#endif /* PKMN_ENUMS_TYPE_HPP */
