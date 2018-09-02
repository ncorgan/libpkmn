/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_ENUMS_MOVE_TARGET_HPP
#define PKMN_ENUMS_MOVE_TARGET_HPP

namespace pkmn
{
    enum class e_move_target
    {
        NONE = 0,
        SPECIFIC_MOVE = 1,
        SELECTED_POKEMON_ME_FIRST = 2,
        ALLY = 3,
        USERS_FIELD = 4,
        USER_OR_ALLY = 5,
        OPPONENTS_FIELD = 6,
        USER = 7,
        RANDOM_OPPONENT = 8,
        ALL_OTHER_POKEMON = 9,
        SELECTED_POKEMON = 10,
        ALL_OPPONENTS = 11,
        ENTIRE_FIELD = 12,
        USER_AND_ALLIES = 13,
        ALL_POKEMON = 14,
    };
}

#endif /* PKMN_ENUMS_MOVE_TARGET_HPP */
