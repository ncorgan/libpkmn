/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_LEVELUP_MOVE_HPP
#define PKMN_DATABASE_LEVELUP_MOVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/database/move_entry.hpp>

#include <vector>

namespace pkmn { namespace database {

    //! Represents a move that a Pokémon learns leveling up.
    struct levelup_move {
        PKMN_INLINE levelup_move() {}

        PKMN_INLINE levelup_move(
            const pkmn::database::move_entry &lmove,
            int move_level
        ): move(lmove),
           level(move_level) {}

#ifndef SWIG
        PKMN_INLINE levelup_move(
            pkmn::database::move_entry&& lmove,
            int move_level
        ): move(lmove),
           level(move_level) {}
#endif

        //! Entry for move
        pkmn::database::move_entry move;
        //! Level at which the Pokémon learns the move
        int level;
    };

    #ifndef SWIG
    //! Equality check between two levelup_move instances.
    PKMN_INLINE bool operator==(
        const levelup_move &lhs,
        const levelup_move &rhs
    ) {
        return (lhs.move == rhs.move) and \
               (lhs.level == rhs.level);
    }

    //! Inequality check between two levelup_move instances.
    PKMN_INLINE bool operator!=(
        const levelup_move &lhs,
        const levelup_move &rhs
    ) {
        return (lhs.move != rhs.move) or \
               (lhs.level != rhs.level);
    }
    #endif

    //! List of level-up moves.
    typedef std::vector<levelup_move> levelup_moves_t;
}}
#endif /* PKMN_DATABASE_LEVELUP_MOVE_HPP */
