/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_LEVELUP_MOVE_HPP
#define PKMN_DATABASE_LEVELUP_MOVE_HPP

#include <pkmn/enums/move.hpp>

#include <vector>

namespace pkmn { namespace database {

    struct levelup_move
    {
        levelup_move():
            move(pkmn::e_move::NONE),
            level(0)
        {}

        levelup_move(
            pkmn::e_move move_enum,
            int move_level
        ): move(move_enum),
           level(move_level)
        {}

        levelup_move(const levelup_move&) = default;
        levelup_move& operator=(const levelup_move&) = default;

#ifndef SWIG
        levelup_move(levelup_move&&) = default;
        levelup_move& operator=(levelup_move&&) = default;
#endif

        bool operator==(const levelup_move& rhs) const
        {
            return (move == rhs.move) && (level == rhs.level);
        }

        bool operator!=(const levelup_move& rhs) const
        {
            return !operator==(rhs);
        }

        pkmn::e_move move;
        int level;
    };

    using levelup_moves_t = std::vector<levelup_move>;
}}
#endif /* PKMN_DATABASE_LEVELUP_MOVE_HPP */
