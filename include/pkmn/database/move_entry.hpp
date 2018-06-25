/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_MOVE_ENTRY_HPP
#define PKMN_DATABASE_MOVE_ENTRY_HPP

#include <pkmn/config.hpp>

#include <pkmn/enums/game.hpp>
#include <pkmn/enums/move.hpp>
#include <pkmn/enums/move_damage_class.hpp>
#include <pkmn/enums/type.hpp>

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
    class PKMN_API move_entry
    {
        public:
            /*!
             * Default constructor. Corresponds to an invalid item.
             */
            move_entry():
                _move_id(0),
                _game_id(0),
                _generation(0),
                _none(false),
                _invalid(true)
            {}

#if !defined(__DOXYGEN__) && !defined(SWIG)
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
             * \param game Which game this move is from
             * \throw std::invalid_argument If the move or game name is invalid
             * \throw std::invalid_argument If the given move was not in the given game
             */
            move_entry(
                const std::string& move_name,
                pkmn::e_game game
            );

            move_entry(const move_entry&) = default;
            move_entry& operator=(const move_entry&) = default;

#ifndef SWIG
            move_entry(move_entry&&) = default;
            move_entry& operator=(move_entry&&) = default;
#endif

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
             * @brief Returns the game associated with this entry.
             */
            pkmn::e_game get_game() const;

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
             * @brief Returns a description of this move.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_description() const;

            /*!
             * @brief Returns which Pokémon this move will target in battle.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_target() const;

            /*!
             * @brief Returns this move's damage class (Physical, Special, Status).
             *
             * This value affects whether Attack+Defense or Special Attack+Defense will
             * be used in calculating a move's damage.
             *
             * In Generation I through the Game Boy Advance games, this value is tied to
             * the move's type. However, starting with the Gamecube games, their values were
             * separated. For example, Fire Punch is a Special move in Generation I but a
             * Physical move in Generation IV.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_damage_class() const;

            /*!
             * @brief Returns this move's base power.
             *
             * This value, alongside the attacker's (Special) Attack and the defender's (Special)
             * Defense, is used in determining how much damage an attack does.
             *
             * If this entry corresponds to a "None" or "Invalid" move, this function will
             * return -1.
             */
            int get_base_power() const;

            /*!
             * @brief Return the maximum number of Power Points (PP) this move has with the given amount of PP Up.
             *
             * If this entry corresponds to a "None" or "Invalid" move, this function will
             * return -1.
             *
             * \param num_pp_ups Number of PP Ups applied (0-3, 3 for PP Max)
             * \throws std::out_of_range if num_pp_ups is not [0-3]
             */
            int get_pp(
                int num_pp_ups
            ) const;

            /*!
             * @brief Returns the move's accuracy (0.0-1.0).
             *
             * If this entry corresponds to a "None" or "Invalid" move, this function will
             * return -1.0.
             */
            float get_accuracy() const;

            /*!
             * @brief Returns the move's priority (-8 - +8).
             *
             * In a single turn, a move with a higher priority will always be performed before a move with a
             * lower priority. If the moves have the same priority, the Pokémons' Speed stats will be
             * used to determine the order.
             *
             * If this entry corresponds to a "None" or "Invalid" move, this function will
             * return -9.
             */
            int get_priority() const;

            /*!
             * @brief Returns a move's effect, if any, other than damage.
             *
             * If this entry corresponds to a "None" move, this function will
             * return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_effect() const;

            /*!
             * @brief Return the move's type when used in a Contest.
             *
             * If this entry corresponds to a "None" move, or this move's game is Generation I-II
             * or Gamecube, this function will return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_contest_type() const;

            /*!
             * @brief Return the move's effect when used in a Contest.
             *
             * If this entry corresponds to a "None" move, or this move's game is Generation I-II
             * or Gamecube, this function will return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_contest_effect() const;

            /*!
             * @brief Return the move's effect when used in a Super Contest.
             *
             * If this entry corresponds to a "None" move, or this move's game is 
             * not from Generation IV, this function will return the string "None".
             *
             * If this entry corresponds to an invalid move, this function will
             * return the string "Unknown".
             */
            std::string get_super_contest_effect() const;

            #ifndef __DOXYGEN__
            inline int get_move_id() const
            {
                return _move_id;
            }

            inline int get_game_id() const
            {
                return _game_id;
            }
            #endif

            //! Equality check between two move entries
            inline bool operator==(const move_entry &rhs) const
            {
                return (this->_game_id == rhs._game_id) and
                       (this->_move_id == rhs._move_id);
            }

            //! Inequality check between two move entries
            inline bool operator!=(const move_entry &rhs) const
            {
                return !operator==(rhs);
            }

        private:
            int _move_id, _game_id, _generation;
            bool _none, _invalid;
    };

    //! List of moves.
    typedef std::vector<move_entry> move_list_t;
}}
#endif /* PKMN_DATABASE_MOVE_ENTRY_HPP */
