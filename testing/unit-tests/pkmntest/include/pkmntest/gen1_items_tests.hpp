/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_ITEMS_TESTS_HPP
#define GEN1_ITEMS_TESTS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>

namespace pkmntest {

    PKMNTEST_API void gen1_item_list_test(
        pkmn::item_list::sptr items,
        const std::string &game
    );

    PKMNTEST_API void gen1_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    );

    PKMNTEST_API void gen1_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    );

}

#endif /* GEN1_ITEMS_TESTS_HPP */
