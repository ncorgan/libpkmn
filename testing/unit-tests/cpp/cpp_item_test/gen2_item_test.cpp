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
#include <pksav/gen2/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

#include <algorithm>

class gen2_item_list_test: public pkmntest::item_list_test {};

void gen2_item_pocket_test(
    pkmn::item_list::sptr item_pocket
) {
    ASSERT_EQ("Items", item_pocket->get_name());
}

void gen2_key_item_pocket_test(
    pkmn::item_list::sptr key_item_pocket
) {
    ASSERT_EQ("KeyItems", key_item_pocket->get_name());
}

void gen2_ball_pocket_test(
    pkmn::item_list::sptr ball_pocket
) {
    ASSERT_EQ("Balls", ball_pocket->get_name());
}

void gen2_tmhm_pocket_test(
    pkmn::item_list::sptr tmhm_pocket
) {
    ASSERT_EQ("TM/HM", tmhm_pocket->get_name());
}

void gen2_item_pc_test(
    pkmn::item_list::sptr item_pc
) {
    ASSERT_EQ("PC", item_pc->get_name());
}

static const pkmntest::item_list_test_fcns_t gen2_test_fcns = boost::assign::map_list_of
    ("Items", gen2_item_pocket_test)
    ("KeyItems", gen2_key_item_pocket_test)
    ("Balls", gen2_ball_pocket_test)
    ("TM/HM", gen2_tmhm_pocket_test)
    ("PC", gen2_item_pc_test)
;

TEST_P(gen2_item_list_test, item_list_test) {
    gen2_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<std::string, std::string>> item_list_params = {
    {"Gold", "Items"},
    {"Gold", "KeyItems"},
    {"Gold", "Balls"},
    {"Gold", "TM/HM"},
    {"Gold", "PC"},
    {"Silver", "Items"},
    {"Silver", "KeyItems"},
    {"Silver", "Balls"},
    {"Silver", "TM/HM"},
    {"Silver", "PC"},
    {"Crystal", "Items"},
    {"Crystal", "KeyItems"},
    {"Crystal", "Balls"},
    {"Crystal", "TM/HM"},
    {"Crystal", "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_item_list_test,
    gen2_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gen2_item_bag_test: public pkmntest::item_bag_test {};

TEST_P(gen2_item_bag_test, item_bag_test) {
    pkmn::item_bag::sptr bag = get_item_bag();

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(4, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());
    ASSERT_EQ("KeyItems", pockets.at("KeyItems")->get_name());
    ASSERT_EQ(get_game(), pockets.at("KeyItems")->get_game());
    ASSERT_EQ("Balls", pockets.at("Balls")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Balls")->get_game());
    ASSERT_EQ("TM/HM", pockets.at("TM/HM")->get_name());
    ASSERT_EQ(get_game(), pockets.at("TM/HM")->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(4, pockets.size());
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "KeyItems"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Balls"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "TM/HM"));

    gen2_item_pocket_test(pockets.at("Items"));
    gen2_key_item_pocket_test(pockets.at("KeyItems"));
    gen2_ball_pocket_test(pockets.at("Balls"));
    gen2_tmhm_pocket_test(pockets.at("TM/HM"));

    // TODO: make sure adding to bag is reflected in pocket
}

static const std::vector<std::string> item_bag_params = {
    "Gold", "Silver", "Crystal"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen2_item_bag_test,
    gen2_item_bag_test,
    ::testing::ValuesIn(item_bag_params)
);
