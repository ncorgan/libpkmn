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

    /*!
     * @brief Class representing a database entry for an in-game item.
     *
     * This class abstracts the database calls needed to various tables in order
     * to get relevant information about in-game items. It takes into account
     * changes in the item between games.
     *
     * \todo some differences between version groups need to be done manually
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
                _none(true),
                _invalid(true)
            {}

            item_entry(
                int item_index,
                int game_id
            );
            #endif

            /*!
             * @brief Constructor that instantiates an item entry based on its name and game.
             *
             * This class allows for querying information regarding the item in the given game.
             * It accounts for differences between games, such as which pocket the item
             * corresponds to in a particular game.
             *
             * \param item_name Item's name (does not need to match name in given game)
             * \param game_name Which game this item is from
             * \throw std::invalid_argument If the item or game name is invalid
             * \throw std::invalid_argument If the given item was not in the given game
             */
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
             * @brief Returns which pocket would store this item in the given game.
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
