/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_MOVE_ENTRY_HPP
#define PKMN_DATABASE_MOVE_ENTRY_HPP

#include <pkmn/config.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace database {

    /*!
     * @brief Class representing a database entry for an in-game move.
     *
     * This class abstracts the database calls needed to various tables in order
     * to get relevant information about in-game moves. It takes into account
     * changes in the move between games.
     *
     * \todo Distinguish contest types,effects between Contests (Gen III), Super Contests (Gen IV),
     *       and Contest Spectaculars (Gen VI)
     * \todo Override target for Generation I-II
     */
    class PKMN_API move_entry {
        public:
            #ifndef __DOXYGEN__
            PKMN_CONSTEXPR_OR_INLINE move_entry():
                _move_id(0),
                _game_id(0),
                _generation(0),
                _none(false),
                _invalid(true)
            {}

            move_entry(
                int move_id,
                int game_id
            );
            #endif

            /*!
             * @brief Constructor that instantiates a move entry based on its name and game.
             *
             * This class allows for querying information regarding the move in the given game.
             * It accounts for differences between games, such as changes in base PP or priority.
             *
             * \param move_name Move's name (does not need to match name in given game)
             * \param game_name Which game this move is from
             * \throw std::invalid_argument If the move or game name is invalid
             * \throw std::invalid_argument If the given move was not in the given game
             */
            move_entry(
                const std::string &move_name,
                const std::string &game_name
            );

            /*!
             * @brief Returns the move's name.
             *
             * This function accounts for name changes between generations
             * and returns the name corresponding to the given game.
             *
             * If this entry corresponds to a "None" move or an empty move slot,
             * this function will return the string "None".
             *
             * If this entry corresponds to an invalid move, this function
             * will return the string "Invalid (0xXX)", where XX corresponds to
             * the hex value of the move's in-game index.
             */
            std::string get_name() const;

            /*!
             * @brief Returns the name of the game associated with this entry.
             */
            std::string get_game() const;

            /*!
             * @brief Returns the move's type.
             *
             * This function accounts for type changes between generations
             * and returns the type corresponding to the given game.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_type() const;

            /*!
             * @brief Returns a description of this item.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_description() const;

            std::string get_target() const;

            std::string get_damage_class() const;

            int get_base_power() const;

            int get_pp(
                int num_pp_ups
            ) const;

            float get_accuracy() const;

            int get_priority() const;

            std::string get_effect() const;

            std::string get_contest_type() const;

            std::string get_contest_effect() const;

            std::string get_super_contest_effect() const;

            #ifndef __DOXYGEN__
            PKMN_CONSTEXPR_OR_INLINE int get_move_id() const {
                return _move_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_game_id() const {
                return _game_id;
            }
            #endif

            //! Equality check between two move entries
            PKMN_CONSTEXPR_OR_INLINE bool operator==(
                const move_entry &rhs
            ) const {
                return ((this->_game_id == rhs._game_id) and
                        (this->_move_id == rhs._move_id));
            }

            //! Inequality check between two move entries
            PKMN_CONSTEXPR_OR_INLINE bool operator!=(
                const move_entry &rhs
            ) const {
                return ((this->_game_id != rhs._game_id) or
                        (this->_move_id != rhs._move_id));
            }

        private:
            int _move_id, _game_id, _generation;
            bool _none, _invalid;
    };

    //! List of moves.
    typedef std::vector<move_entry> move_list_t;
}}
#endif /* PKMN_DATABASE_MOVE_ENTRY_HPP */
