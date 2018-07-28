/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_MOVE_SLOT_HPP
#define PKMN_MOVE_SLOT_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/move.hpp>

#include <string>
#include <vector>

namespace pkmn {

    /*!
     * @brief A slot in an move list.
     *
     * A move list is made up of a list of move slots, each of which contains
     * a unique move type and the PP of that move in the given pocket. This
     * struct represents a single one of those slots.
     */
    struct move_slot
    {
        /*!
         * @brief Default constructor.
         *
         * The move entry represents an invalid/none move.
         */
        move_slot():
            move(pkmn::e_move::NONE),
            pp(0) {}

        /*!
         * @brief Constructor that takes in copies of each member.
         *
         * This calls the copy constructor for the move entry.
         *
         * \param move Move
         * \param move_pp Move's current PP
         */
        move_slot(
            pkmn::e_move slot_move,
            int slot_pp
        ): move(slot_move),
           pp(slot_pp) {}

        move_slot(const move_slot&) = default;
        move_slot& operator=(const move_slot&) = default;

#ifndef SWIG
        move_slot(move_slot&&) = default;
        move_slot& operator=(move_slot&&) = default;
#endif

        /*!
         * @brief Equality check.
         */
        inline bool operator==(const move_slot& rhs) const
        {
            return (this->move == rhs.move) &&
                   (this->pp == rhs.pp);
        }

        /*!
         * @brief Inequality check.
         */
        inline bool operator!=(const move_slot& rhs) const
        {
            return !operator==(rhs);
        }

        /*!
         * @brief The move in the given slot.
         */
        pkmn::e_move move;
        /*!
         * @brief Move's current PP is in the given slot.
         */
        int pp;
    };

    /*!
     * @brief List of move slots.
     *
     * This is effectively a vector representation of an move list.
     */
    typedef std::vector<move_slot> move_slots_t;
}

#endif /* PKMN_MOVE_SLOT_HPP */
