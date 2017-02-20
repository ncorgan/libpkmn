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
#include <pksav/gba/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>

class gba_item_list_test: public pkmntest::item_list_test {};

void gba_item_pocket_test(
    pkmn::item_list::sptr item_pocket
) {
    ASSERT_EQ("Items", item_pocket->get_name());
}

void gba_key_item_pocket_test(
    pkmn::item_list::sptr key_item_pocket
) {
    ASSERT_EQ("Key Items", key_item_pocket->get_name());
}

void gba_ball_pocket_test(
    pkmn::item_list::sptr ball_pocket
) {
    ASSERT_EQ("Poké Balls", ball_pocket->get_name());
}

void gba_tmhm_pocket_test(
    pkmn::item_list::sptr tmhm_pocket
) {
    std::string game = tmhm_pocket->get_game();
    if(game == "FireRed" or game == "LeafGreen") {
        ASSERT_EQ("TM Case", tmhm_pocket->get_name());
    } else {
        ASSERT_EQ("TMs & HMs", tmhm_pocket->get_name());
    }
}

void gba_berry_pocket_test(
    pkmn::item_list::sptr berry_pocket
) {
    std::string game = berry_pocket->get_game();
    if(game == "FireRed" or game == "LeafGreen") {
        ASSERT_EQ("Berry Pouch", berry_pocket->get_name());
    } else {
        ASSERT_EQ("Berries", berry_pocket->get_name());
    }
}

void gba_item_pc_test(
    pkmn::item_list::sptr item_pc
) {
    ASSERT_EQ("PC", item_pc->get_name());
}

static const pkmntest::item_list_test_fcns_t gba_test_fcns = boost::assign::map_list_of
    ("Items", gba_item_pocket_test)
    ("Key Items", gba_key_item_pocket_test)
    ("Poké Balls", gba_ball_pocket_test)
    ("TMs & HMs", gba_tmhm_pocket_test)
    ("TM Case", gba_tmhm_pocket_test)
    ("Berries", gba_berry_pocket_test)
    ("Berry Pouch", gba_berry_pocket_test)
    ("PC", gba_item_pc_test)
;

TEST_P(gba_item_list_test, item_list_test) {
    gba_test_fcns.at(get_name())(get_item_list());
}

static const std::vector<std::pair<std::string, std::string>> item_list_params = {
    {"Ruby", "Items"},
    {"Ruby", "Key Items"},
    {"Ruby", "Poké Balls"},
    {"Ruby", "TMs & HMs"},
    {"Ruby", "Berries"},
    {"Ruby", "PC"},
    {"Sapphire", "Items"},
    {"Sapphire", "Key Items"},
    {"Sapphire", "Poké Balls"},
    {"Sapphire", "TMs & HMs"},
    {"Sapphire", "Berries"},
    {"Sapphire", "PC"},
    {"Emerald", "Items"},
    {"Emerald", "Key Items"},
    {"Emerald", "Poké Balls"},
    {"Emerald", "TMs & HMs"},
    {"Emerald", "Berries"},
    {"Emerald", "PC"},
    {"FireRed", "Items"},
    {"FireRed", "Key Items"},
    {"FireRed", "Poké Balls"},
    {"FireRed", "TM Case"},
    {"FireRed", "Berry Pouch"},
    {"FireRed", "PC"},
    {"LeafGreen", "Items"},
    {"LeafGreen", "Key Items"},
    {"LeafGreen", "Poké Balls"},
    {"LeafGreen", "TM Case"},
    {"LeafGreen", "Berry Pouch"},
    {"LeafGreen", "PC"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_item_list_test,
    gba_item_list_test,
    ::testing::ValuesIn(item_list_params)
);

class gba_item_bag_test: public pkmntest::item_bag_test {};

TEST_P(gba_item_bag_test, item_bag_test) {
}

static const std::vector<std::string> item_bag_params = {
    "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_item_bag_test,
    gba_item_bag_test,
    ::testing::ValuesIn(item_bag_params)
);
