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

    /*
     * TODO:
     *  * Fixes from unit test
     *  * See which ctors, move+copy ops, and ID getters can be constexpr'd
     *  * Can any other info be added?
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

            move_entry(
                const std::string &move_name,
                const std::string &game_name
            );

            std::string get_name() const;

            std::string get_game() const;

            std::string get_type() const;

            std::string get_description() const;

            std::string get_damage_class() const;

            int get_base_power() const;

            int get_pp(
                int num_pp_ups
            ) const;

            float get_accuracy() const;

            int get_priority() const;

            std::string get_effect() const;

            float get_effect_chance() const;

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
