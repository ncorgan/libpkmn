/*
 * Copyright (c) 2016,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_BAG_HPP
#define PKMN_ITEM_BAG_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_list.hpp>

#include <memory>
#include <string>
#include <vector>

namespace pkmn {

    /*!
     * @brief A bag of items, sorted into different pockets.
     *
     * This class is an abstraction of the different in-game representation of a
     * trainer's item bag throughout the different games, providing a common API for
     * interacting with the bag.
     *
     * This class automatically creates item lists that correspond to the types and
     * capacities of the bag in the given game.
     */
    class PKMN_API item_bag
    {
        public:
            /*!
             * @brief The actual interface for using the item_bag class.
             *
             * The item_bag class itself is abstract and thus cannot be used directly.
             */
            typedef std::shared_ptr<item_bag> sptr;

            /*!
             * @brief The factory function for creating an item bag.
             *
             * This function uses the name of the game to figure out which underlying
             * representation to use. This function will fail if given invalid inputs.
             *
             * \param game Which game the bag corresponds to
             * \throws std::invalid_argument If either parameter is invalid
             * \throws std::runtime_error If an unimplemented game is given
             */
            static sptr make(
                const std::string& game
            );

            /*!
             * @brief Returns which game this item bag comes from.
             */
            virtual std::string get_game() = 0;

            /*!
             * @brief Returns an interface to the bag's pocket with the given game.
             *
             * The item_list::sptr returned from this function shares the same underlying
             * memory as the item_bag::sptr that created it, so calling item_bag::add or
             * item_bag::remove will affect the item_list::sptr corresponding to the
             * appropriate pocket.
             *
             * \param name Name of the pocket to return
             * \throws std::invalid_argument If the given name does not correspond to a
             *                               valid pocket
             */
            virtual pkmn::item_list::sptr get_pocket(
                const std::string& name
            ) = 0;

            /*!
             * @brief Returns a map from which pockets can be queried by name.
             */
            virtual const pkmn::item_pockets_t& get_pockets() = 0;

            /*!
             * @brief Returns a list of all valid pocket names.
             */
            virtual const std::vector<std::string>& get_pocket_names() = 0;

            /*!
             * @brief Adds the given amount of the given item to the appropriate pocket.
             *
             * This function figures out the appropriate pocket based on the given item and
             * adds the item to it.
             *
             * Any item_list::sptr returned by an item_bag function will share its underlying
             * memory, so this function will affect any item_lists corresponding to the same
             * pocket.
             *
             * \param item_name Name of the item to add
             * \param amount How many of the item to add
             * \throws std::invalid_argument If the item name is invalid
             * \throws std::invalid_argument If the item cannot be added to any of this game's
             *                               pockets
             * \throws std::out_of_range If the amount is not in the range [0,99]
             */
            virtual void add(
                const std::string& item_name,
                int amount
            ) = 0;

            /*!
             * @brief Removes the given amount of the given item to the appropriate pocket.
             *
             * This function figures out the appropriate pocket based on the given item and
             * adds the item to it.
             *
             * Any item_list::sptr returned by an item_bag function will share its underlying
             * memory, so this function will affect any item_lists corresponding to the same
             * pocket.
             *
             * \param item_name Name of the item to add
             * \param amount How many of the item to add
             * \throws std::invalid_argument If the item name is invalid
             * \throws std::invalid_argument If the item cannot be added to (and thus removed
             *                               from any of this game's pockets)
             * \throws std::out_of_range If the amount is not in the range [0,99]
             */
            virtual void remove(
                const std::string& item_name,
                int amount
            ) = 0;

            #ifndef __DOXYGEN__
            item_bag() {};
            virtual ~item_bag() {};
            virtual void* get_native() = 0;
            #endif
    };

}

#endif /* PKMN_ITEM_BAG_HPP */
