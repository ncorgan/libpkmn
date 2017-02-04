/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN2_ITEMS_TESTS_HPP
#define GEN2_ITEMS_TESTS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>

#include <string>

namespace pkmntest {

    PKMNTEST_API void gen2_item_pocket_test(
        pkmn::item_list::sptr item_pocket,
        const std::string &game
    );

    PKMNTEST_API void gen2_key_item_pocket_test(
        pkmn::item_list::sptr key_item_pocket,
        const std::string &game
    );

    PKMNTEST_API void gen2_ball_pocket_test(
        pkmn::item_list::sptr ball_pocket,
        const std::string &game
    );

    PKMNTEST_API void gen2_tmhm_pocket_test(
        pkmn::item_list::sptr tmhm_pocket,
        const std::string &game
    );

    PKMNTEST_API void gen2_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    );

    PKMNTEST_API void gen2_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    );

}

#endif /* GEN2_ITEMS_TESTS_HPP */
