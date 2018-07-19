/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef ITEM_TEST_COMMON_HPP
#define ITEM_TEST_COMMON_HPP

#include <pkmntest/config.hpp>

#include <pkmn/item_list.hpp>
#include <pkmn/item_bag.hpp>

#include <gtest/gtest.h>

#include <map>
#include <string>
#include <utility>

typedef void (*item_list_test_fcn_t)(const pkmn::item_list::sptr&);
typedef std::map<std::string, item_list_test_fcn_t> item_list_test_fcns_t;

class item_list_test: public ::testing::TestWithParam<std::pair<pkmn::e_game, std::string> >
{
    public:
        inline pkmn::item_list::sptr get_item_list()
        {
            return _item_list;
        }

        inline pkmn::e_game get_game()
        {
            return _game;
        }

        inline const std::string& get_name()
        {
            return _name;
        }

        inline void reset()
        {
             _item_list = pkmn::item_list::make(_name, _game);

            ASSERT_NE(nullptr, _item_list.get());
            ASSERT_EQ(_game, _item_list->get_game());
            ASSERT_EQ(_name, _item_list->get_name());
        }

    protected:
        void SetUp() {
            _game = GetParam().first;
            _name = GetParam().second;
            reset();
        }

    private:

        std::string _name;
        pkmn::e_game _game;
        pkmn::item_list::sptr _item_list;
};

void test_item_list_empty_slots(
    const pkmn::item_list::sptr& list
);

void test_item_list_out_of_range_error(
    const pkmn::item_list::sptr& list,
    pkmn::e_item item
);

void test_item_list_invalid_items(
    const pkmn::item_list::sptr& list,
    const std::vector<pkmn::e_item>& items
);

void test_item_list_add_remove(
    const pkmn::item_list::sptr& list,
    const std::vector<pkmn::e_item>& items
);

class item_bag_test: public ::testing::TestWithParam<pkmn::e_game> {
    public:
        // We need access to the specific sptr instance.
        inline const pkmn::item_bag::sptr& get_item_bag() {
            return _item_bag;
        }

        inline pkmn::e_game get_game() {
            return _game;
        }

        inline void reset() {
             _item_bag = pkmn::item_bag::make(_game);

            ASSERT_NE(nullptr, _item_bag.get());
            ASSERT_EQ(_game, _item_bag->get_game());
        }

    protected:
        void SetUp()
        {
            _game = GetParam();
            reset();
        }

    private:

        pkmn::e_game _game;
        pkmn::item_bag::sptr _item_bag;
};

void test_item_bag_invalid_items(
    const pkmn::item_bag::sptr& bag,
    const std::vector<pkmn::e_item>& items
);

#endif /* ITEM_TEST_COMMON_HPP */
