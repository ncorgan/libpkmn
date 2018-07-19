/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_HPP
#define PKMN_ITEM_LIST_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_slot.hpp>

#include <pkmn/enums/game.hpp>
#include <pkmn/enums/item.hpp>

#include <map>
#include <memory>
#include <string>

namespace pkmn {

    /*!
     * @brief A list of items, corresponding to an in-game bag pocket or item PC.
     *
     * This class is an abstraction of the different in-game representations of an
     * item pocket or PC throughout the different games, providing a common API for
     * interacting with the items.
     *
     * The rules for interacting with different types of lists, as well as the set
     * of items that can be inserted into a given list, matches the behavior of the list
     * in the given game. As such, this class will never represent an invalid in-game
     * state unless the original item list was already in an invalid state.
     */
    class PKMN_API item_list
    {
        public:
            /*!
             * @brief The actual interface for using the item_list class.
             *
             * The item_list class itself is abstract and thus cannot be used directly.
             */
            typedef std::shared_ptr<item_list> sptr;

            /*!
             * @brief The factory function for instantiating an item list.
             *
             * This function uses the name of the pocket and the corresponding game
             * to figure out which underlying implementation to use. This function will
             * fail if given invalid inputs.
             *
             * \param name Item list name
             * \param game Which game the list corresponds to
             * \throws std::invalid_argument If either parameter is invalid
             * \throws std::runtime_error If an unimplemented game is given
             */
            static sptr make(
                const std::string& name,
                pkmn::e_game game
            );

            /*!
             * @brief Returns the name of the item list.
             */
            virtual std::string get_name() = 0;

            /*!
             * @brief Returns which game this item list comes from.
             */
            virtual pkmn::e_game get_game() = 0;

            /*!
             * @brief Returns how many unique items can be stored in this list.
             */
            virtual int get_capacity() = 0;

            /*!
             * @brief Returns how many unique items are currently in this list.
             */
            virtual int get_num_items() = 0;

            /*!
             * @brief Returns the item slot at the given position in the list (read-only).
             *
             * \param position 0-based position of the item to return
             * \throws std::out_of_range If position is outside the range [0,capacity-1]
             */
            virtual const pkmn::item_slot& at(
                int position
            ) = 0;

            /*!
             * @brief Adds some amount of the given item to the list.
             *
             * If some of this item is already in the pocket, the amount in that slot is
             * increased by the given amount. If not, the next empty slot will be used,
             * assuming one is available.
             *
             * \param item The item to add
             * \param amount How many of the item to add
             * \throws std::invalid_argument If the item cannot be included in the given list
             * \throws std::out_of_range If amount is outside the range [1,99]
             */
            virtual void add(
                pkmn::e_item item,
                int amount
            ) = 0;

            /*!
             * @brief Removes some amount of the given item to the list.
             *
             * If some of this item is already in the pocket, the amount in that slot is
             * decreased by the given amount. If all of this item is removed, the slot will
             * be cleared, and all later slots are shifted back.
             *
             * \param item The item to remove
             * \param amount How many of the item to remove
             * \throws std::invalid_argument If the item cannot be included in the given list
             * \throws std::out_of_range If amount is outside the range [1,99]
             */
            virtual void remove(
                pkmn::e_item item,
                int amount
            ) = 0;

            /*!
             * @brief Moves the items in one given slot into another.
             *
             * \param old_position The position of the slot to move
             * \param new_position Where to move the given slot
             * \throws std::out_of_range If either parameter is outside the range [0,num_items-1]
             * \throws std::runtime_error If items in the given list cannot be moved
             */
            virtual void move(
                int old_position,
                int new_position
            ) = 0;

            /*!
             * @brief Sets the given item slot to the given item and amount.
             *
             * \param position The position for which to set the item
             * \param item_name The name of the item to set
             * \param amount How much of the item to put in the slot
             * \throws std::invalid_argument If the item cannot be included in the given list
             * \throws std::out_of_range If position is outside the range [0,num_items-1]
             * \throws std::runtime_error If the pocket's items are locked in one slot and the
             *                            given item does not match that slot
             */
            virtual void set_item(
                int position,
                pkmn::e_item item,
                int amount
            ) = 0;

            virtual const std::vector<pkmn::e_item>& get_valid_items() = 0;

            /*!
             * @brief Returns a list of the items that can be added to this list.
             *
             * If this item list corresponds to an item PC, this list of names will include almost
             * every item in the given game, minus certain Key Items.
             */
            virtual const std::vector<std::string>& get_valid_item_names() = 0;

            /*!
             * @brief Returns a vector representation of the given item list.
             */
            virtual const pkmn::item_slots_t& as_vector() = 0;

            #ifndef __DOXYGEN__
            item_list() {};
            virtual ~item_list() {};
            virtual void* get_native() = 0;
            #endif
    };

    /*!
     * @brief A data representation that allows a set of pockets to be accessed by name.
     */
    typedef std::map<std::string, pkmn::item_list::sptr> item_pockets_t;

}

#endif /* PKMN_ITEM_LIST_HPP */
