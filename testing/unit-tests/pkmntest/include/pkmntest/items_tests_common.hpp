/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef ITEMS_TESTS_COMMON_HPP
#define ITEMS_TESTS_COMMON_HPP

#include <pkmn/item_list.hpp>

#include <string>
#include <vector>

namespace pkmntest {

    void test_item_list_empty_slots(
        pkmn::item_list::sptr list,
        const pkmn::database::item_entry &none_entry
    );

    void test_item_list_out_of_range_error(
        pkmn::item_list::sptr list,
        const std::string &item_name
    );

    void test_item_list_items_from_wrong_pocket(
        pkmn::item_list::sptr list,
        const std::vector<std::string> &item_names
    );

    void test_item_list_add_remove(
        pkmn::item_list::sptr list,
        const pkmn::database::item_entry &none_entry,
        const std::string &item1_name,
        const std::string &item2_name,
        const std::string &item3_name
    );

}

#endif /* ITEMS_TESTS_COMMON_HPP */
