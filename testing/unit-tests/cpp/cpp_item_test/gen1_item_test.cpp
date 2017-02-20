/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/item_test.hpp>

#include <pkmn/exception.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

class gen1_item_list_test: public pkmntest::item_list_test {};

void gen1_item_pocket_test(
    pkmn::item_list::sptr item_pocket
) {
    ASSERT_EQ("Items", item_pocket->get_name());
}

void gen1_item_pc_test(
    pkmn::item_list::sptr item_pc
) {
    ASSERT_EQ("PC", item_pc->get_name());
}

static const pkmntest::item_list_test_fcns_t gen1_test_fcns = boost::assign::map_list_of
    ("Items", gen1_item_pocket_test)
    ("PC", gen1_item_pc_test)
;

TEST_P(gen1_item_list_test, item_list_test) {
    gen1_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<std::string, std::string>> item_list_params = {
    {"Red", "Items"},
    {"Red", "PC"},
    {"Blue", "Items"},
    {"Blue", "PC"},
    {"Yellow", "Items"},
    {"Yellow", "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_item_list_test,
    gen1_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gen1_item_bag_test: public pkmntest::item_bag_test {};

TEST_P(gen1_item_bag_test, item_bag_test) {
    pkmn::item_bag::sptr bag = get_item_bag();

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(1, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(1, pockets.size());
    ASSERT_EQ("Items", pocket_names[0]);

    gen1_item_pocket_test(pockets.at("Items"));

    // TODO: make sure adding to bag is reflected in pocket
}

static const std::vector<std::string> item_bag_params = {
    "Red", "Blue", "Yellow"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_item_bag_test,
    gen1_item_bag_test,
    ::testing::ValuesIn(item_bag_params)
);
