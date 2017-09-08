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

    /*! 
     * @brief Represents a move that a Pokémon learns leveling up.
     */
    struct levelup_move {
        /*!
         * @brief Default constructor.
         *
         * The move entry represents an invalid move.
         */
        PKMN_CONSTEXPR_OR_INLINE levelup_move():
            move(pkmn::database::move_entry()),
            level(0) {}

        /*!
         * @brief Constructor that takes in copies of each member.
         *
         * This calls the copy constructor for the move entry.
         *
         * \param lmove move entry
         * \param move_level level
         */
        PKMN_CONSTEXPR_OR_INLINE levelup_move(
            const pkmn::database::move_entry &lmove,
            int move_level
        ): move(lmove),
           level(move_level) {}

#ifndef SWIG
        /*!
         * @brief Constructor that takes in references each member.
         *
         * This calls the move constructor for the move entry.
         *
         * \param lmove move entry
         * \param move_level level
         */
        PKMN_CONSTEXPR_OR_INLINE levelup_move(
            pkmn::database::move_entry&& lmove,
            int move_level
        ): move(std::move(lmove)),
           level(move_level) {}
#endif

        /*!
         * @brief The entry for the given move.
         */
        pkmn::database::move_entry move;
        /*!
         * @brief The level at which the Pokémon learns the move.
         */
        int level;
    };

    #ifndef SWIG
    /*!
     * @brief Equality check between two levelup_move instances.
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator==(
        const levelup_move &lhs,
        const levelup_move &rhs
    ) {
        return (lhs.move == rhs.move) and \
               (lhs.level == rhs.level);
    }

    /*!
     * @brief Inequality check between two levelup_move instances.
     */
    PKMN_CONSTEXPR_OR_INLINE bool operator!=(
        const levelup_move &lhs,
        const levelup_move &rhs
    ) {
        return (lhs.move != rhs.move) or \
               (lhs.level != rhs.level);
    }
    #endif

    /*!
     * @brief List of level-up moves.
     */
    typedef std::vector<levelup_move> levelup_moves_t;
}}
#endif /* PKMN_DATABASE_LEVELUP_MOVE_HPP */
