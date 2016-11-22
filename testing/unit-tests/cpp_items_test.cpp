/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>
#include <pkmntest/gen2_items_tests.hpp>

#include <boost/test/unit_test.hpp>

// Red

BOOST_AUTO_TEST_CASE(gen1_red_item_list_test) {
    pkmn::item_list::sptr items = pkmn::item_list::make("Items", "Red");
    pkmntest::gen1_item_list_test(items, "Red");
}

BOOST_AUTO_TEST_CASE(gen1_red_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Red");
    pkmntest::gen1_pc_test(pc, "Red");
}

BOOST_AUTO_TEST_CASE(gen1_red_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Red");
    pkmntest::gen1_item_bag_test(bag, "Red");
}

// Blue

BOOST_AUTO_TEST_CASE(gen1_blue_item_list_test) {
    pkmn::item_list::sptr items = pkmn::item_list::make("Items", "Blue");
    pkmntest::gen1_item_list_test(items, "Blue");
}

BOOST_AUTO_TEST_CASE(gen1_blue_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Blue");
    pkmntest::gen1_pc_test(pc, "Blue");
}

BOOST_AUTO_TEST_CASE(gen1_blue_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Blue");
    pkmntest::gen1_item_bag_test(bag, "Blue");
}

// Yellow

BOOST_AUTO_TEST_CASE(gen1_yellow_item_list_test) {
    pkmn::item_list::sptr items = pkmn::item_list::make("Items", "Yellow");
    pkmntest::gen1_item_list_test(items, "Yellow");
}

BOOST_AUTO_TEST_CASE(gen1_yellow_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Yellow");
    pkmntest::gen1_pc_test(pc, "Yellow");
}

BOOST_AUTO_TEST_CASE(gen1_yellow_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Yellow");
    pkmntest::gen1_item_bag_test(bag, "Yellow");
}

// Gold

BOOST_AUTO_TEST_CASE(gen2_gold_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Gold");
    pkmntest::gen2_item_pocket_test(item_pocket, "Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("KeyItems", "Gold");
    pkmntest::gen2_key_item_pocket_test(key_item_pocket, "Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Balls", "Gold");
    pkmntest::gen2_ball_pocket_test(ball_pocket, "Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TM/HM", "Gold");
    pkmntest::gen2_tmhm_pocket_test(tmhm_pocket, "Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Gold");
    pkmntest::gen2_pc_test(pc, "Gold");
}

BOOST_AUTO_TEST_CASE(gen2_gold_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Gold");
    pkmntest::gen2_item_bag_test(bag, "Gold");
}

// Silver

BOOST_AUTO_TEST_CASE(gen2_silver_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Silver");
    pkmntest::gen2_item_pocket_test(item_pocket, "Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("KeyItems", "Silver");
    pkmntest::gen2_key_item_pocket_test(key_item_pocket, "Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Balls", "Silver");
    pkmntest::gen2_ball_pocket_test(ball_pocket, "Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TM/HM", "Silver");
    pkmntest::gen2_tmhm_pocket_test(tmhm_pocket, "Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Silver");
    pkmntest::gen2_pc_test(pc, "Silver");
}

BOOST_AUTO_TEST_CASE(gen2_silver_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Silver");
    pkmntest::gen2_item_bag_test(bag, "Silver");
}

// Crystal

BOOST_AUTO_TEST_CASE(gen2_crystal_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Crystal");
    pkmntest::gen2_item_pocket_test(item_pocket, "Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("KeyItems", "Crystal");
    pkmntest::gen2_key_item_pocket_test(key_item_pocket, "Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Balls", "Crystal");
    pkmntest::gen2_ball_pocket_test(ball_pocket, "Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TM/HM", "Crystal");
    pkmntest::gen2_tmhm_pocket_test(tmhm_pocket, "Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Crystal");
    pkmntest::gen2_pc_test(pc, "Crystal");
}

BOOST_AUTO_TEST_CASE(gen2_crystal_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Crystal");
    pkmntest::gen2_item_bag_test(bag, "Crystal");
}
