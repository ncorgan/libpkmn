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
    typedef struct {
        //! Entry for move
        pkmn::database::move_entry move;
        //! Level at which the Pokémon learns the move
        int level;
    } levelup_move_t;

    #ifndef SWIG
    //! Equality check between two levelup_move_t instances.
    PKMN_INLINE bool operator==(const levelup_move_t &lhs, const levelup_move_t &rhs) {
        return (lhs.move == rhs.move) and (lhs.level == rhs.level);
    }

    //! Inequality check between two levelup_move_t instances.
    PKMN_INLINE bool operator!=(const levelup_move_t &lhs, const levelup_move_t &rhs) {
        return not (lhs == rhs);
    }
    #endif

    //! List of level-up moves.
    typedef std::vector<levelup_move_t> levelup_moves_t;
}}
#endif /* PKMN_DATABASE_LEVELUP_MOVE_HPP */
