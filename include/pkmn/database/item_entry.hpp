/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_ITEM_ENTRY_HPP
#define PKMN_DATABASE_ITEM_ENTRY_HPP

#include <pkmn/config.hpp>

#include <string>

namespace pkmn { namespace database {

    /*
     * TODO:
     *  * Fixes from unit test
     *  * See which ctors, move+copy ops, and ID getters can be constexpr'd
     *  * Can any other info be added?
     */
    class PKMN_API item_entry {
        public:
            #ifndef __DOXYGEN__
            PKMN_CONSTEXPR_OR_INLINE item_entry():
                _item_id(0),
                _item_index(0),
                _item_list_id(0),
                _game_id(0),
                _generation(0),
                _version_group_id(0),
                _none(false),
                _invalid(true)
            {}

            item_entry(
                int item_index,
                int game_id
            );
            #endif

            item_entry(
                const std::string &item_name,
                const std::string &game_name
            );

            /*!
             * @brief Returns the item's name.
             *
             * This function accounts for name changes between generations
             * and returns the name corresponding to the given game.
             *
             * If this entry corresponds to a "None" item or an empty item slot,
             * this function will return the string "None".
             *
             * If this entry corresponds to an invalid item, this function
             * will return the string "Invalid (0xXX)", where XX corresponds to
             * the hex value of the item's in-game index.
             */
            std::string get_name() const;

            /*!
             * @brief Returns the name of the game associated with this entry.
             */
            std::string get_game() const;

            /*!
             * @brief Returns this item's category.
             *
             * If this entry corresponds to a "None" item or an empty item slot,
             * this function will return the string "None".
             *
             * If this entry corresponds to an invalid item, this function will
             * return the string "Unknown".
             */
            std::string get_category() const;

            /*!
             * @brief Returns which pocket this item would correspond to in
             *        the given game.
             */
            std::string get_pocket() const;

            /*!
             * @brief Returns a description of this item.
             *
             * If this entry corresponds to a TM/HM, this function will return the
             * string "Teaches the move $name.", where $name corresponds to the move
             * name.
             *
             * If this entry corresponds to a "None" item or an empty item slot,
             * this function will return the string "None".
             *
             * If this entry corresponds to an invalid item, this function will
             * return the string "Unknown".
             */
            std::string get_description() const;

            /*!
             * @brief Returns the price of this item if bought at a Poké Mart.
             *
             * If this item is a key item (and thus cannot be bought or sold), this
             * function will return 0.
             *
             * If this item corresponds to a "None" item, an empty item slot, or
             * an invalid item, this function will return -1.
             */
            int get_cost() const;

            /*!
             * @brief Returns whether or not a Pokémon can hold this item.
             *
             * Pokémon could not hold items in Generation I, so if this entry
             * corresponds to an item in Red, Blue, or Yellow, this function will
             * return false.
             *
             * If this item corresponds to a "None" item, an empty item slot, or
             * an invalid item, this function will return false.
             */
            bool holdable() const;

            /*!
             * @brief Returns the base power of the move Fling when this item is held.
             *
             * The move Fling was introduced in Generation IV, so if this entry
             * corresponds to a game from a previous generation, this function will
             * return -1.
             *
             * If this item cannot be used with the move Fling, this function will
             * return -1.
             *
             * If this item corresponds to a "None" item, an empty item slot, or
             * an invalid item, this function will return -1.
             */
            int get_fling_power() const;

            /*!
             * @brief Returns the effect of the move Fling when this item is held.
             *
             * \todo verify database storage of effect
             */
            std::string get_fling_effect() const;

            #ifndef __DOXYGEN__
            PKMN_CONSTEXPR_OR_INLINE int get_item_id() const {
                return _item_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_item_index() const {
                return _item_index;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_item_list_id() const {
                return _item_list_id;
            }

            PKMN_CONSTEXPR_OR_INLINE int get_game_id() const {
                return _game_id;
            }
            #endif

            //! Equality check between two item entries
            PKMN_CONSTEXPR_OR_INLINE bool operator==(
                const item_entry &rhs
            ) const {
                return ((this->_game_id == rhs._game_id) and
                        (this->_item_id == rhs._item_id));
            }

            //! Inequality check between two item entries
            PKMN_CONSTEXPR_OR_INLINE bool operator!=(
                const item_entry &rhs
            ) const {
                return ((this->_game_id != rhs._game_id) or
                        (this->_item_id != rhs._item_id));
            }

        private:
            int _item_id, _item_index, _item_list_id;
            int _game_id, _generation, _version_group_id;
            bool _none, _invalid;
    };
}}
#endif /* PKMN_DATABASE_ITEM_ENTRY_HPP */
