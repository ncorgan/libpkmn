/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
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
    struct levelup_move
    {
        /*!
         * @brief Default constructor. Corresponds to an invalid move.
         */
        levelup_move():
            move(pkmn::database::move_entry()),
            level(0) {}

        /*!
         * @brief Constructor that takes in copies of each member.
         *
         * This calls the copy constructor for the move entry.
         *
         * \param entry move entry
         * \param move_level level
         */
        levelup_move(
            const pkmn::database::move_entry& entry,
            int move_level
        ): move(entry),
           level(move_level) {}

        levelup_move(const levelup_move&) = default;
        levelup_move& operator=(const levelup_move&) = default;

#ifndef SWIG
        /*!
         * @brief Constructor that takes in references to each member.
         *
         * This calls the move constructor for the move entry.
         *
         * \param entry move entry
         * \param move_level level
         */
        levelup_move(
            pkmn::database::move_entry&& entry,
            int move_level
        ): move(std::move(entry)),
           level(move_level) {}

        levelup_move(levelup_move&&) = default;
        levelup_move& operator=(levelup_move&&) = default;
#endif

        /*!
         * @brief Equality check.
         */
        inline bool operator==(const levelup_move& rhs) const
        {
            return (this->move == rhs.move) and (this->level == rhs.level);
        }

        /*!
         * @brief Inequality check.
         */
        inline bool operator!=(const levelup_move& rhs) const
        {
            return !operator==(rhs);
        }

        /*!
         * @brief The entry for the given move.
         */
        pkmn::database::move_entry move;
        /*!
         * @brief The level at which the Pokémon learns the move.
         */
        int level;
    };

    /*!
     * @brief List of level-up moves.
     */
    typedef std::vector<levelup_move> levelup_moves_t;
}}
#endif /* PKMN_DATABASE_LEVELUP_MOVE_HPP */
