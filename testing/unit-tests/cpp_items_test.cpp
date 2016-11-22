/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>
#include <pkmntest/gen2_items_tests.hpp>
#include <pkmntest/gba_items_tests.hpp>

#include <boost/test/unit_test.hpp>

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

// Ruby

BOOST_AUTO_TEST_CASE(gba_ruby_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Ruby");
    pkmntest::gba_item_pocket_test(item_pocket, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("Key Items", "Ruby");
    pkmntest::gba_key_item_pocket_test(key_item_pocket, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Poké Balls", "Ruby");
    pkmntest::gba_ball_pocket_test(ball_pocket, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TMs & HMs", "Ruby");
    pkmntest::gba_tmhm_pocket_test(tmhm_pocket, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_berry_pocket_test) {
    pkmn::item_list::sptr berry_pocket = pkmn::item_list::make("Berries", "Ruby");
    pkmntest::gba_berry_pocket_test(berry_pocket, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Ruby");
    pkmntest::gba_pc_test(pc, "Ruby");
}

BOOST_AUTO_TEST_CASE(gba_ruby_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Ruby");
    pkmntest::gba_item_bag_test(bag, "Ruby");
}

// Sapphire

BOOST_AUTO_TEST_CASE(gba_sapphire_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Sapphire");
    pkmntest::gba_item_pocket_test(item_pocket, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("Key Items", "Sapphire");
    pkmntest::gba_key_item_pocket_test(key_item_pocket, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Poké Balls", "Sapphire");
    pkmntest::gba_ball_pocket_test(ball_pocket, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TMs & HMs", "Sapphire");
    pkmntest::gba_tmhm_pocket_test(tmhm_pocket, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_berry_pocket_test) {
    pkmn::item_list::sptr berry_pocket = pkmn::item_list::make("Berries", "Sapphire");
    pkmntest::gba_berry_pocket_test(berry_pocket, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Sapphire");
    pkmntest::gba_pc_test(pc, "Sapphire");
}

BOOST_AUTO_TEST_CASE(gba_sapphire_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Sapphire");
    pkmntest::gba_item_bag_test(bag, "Sapphire");
}

// Emerald

BOOST_AUTO_TEST_CASE(gba_emerald_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "Emerald");
    pkmntest::gba_item_pocket_test(item_pocket, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("Key Items", "Emerald");
    pkmntest::gba_key_item_pocket_test(key_item_pocket, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Poké Balls", "Emerald");
    pkmntest::gba_ball_pocket_test(ball_pocket, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TMs & HMs", "Emerald");
    pkmntest::gba_tmhm_pocket_test(tmhm_pocket, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_berry_pocket_test) {
    pkmn::item_list::sptr berry_pocket = pkmn::item_list::make("Berries", "Emerald");
    pkmntest::gba_berry_pocket_test(berry_pocket, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "Emerald");
    pkmntest::gba_pc_test(pc, "Emerald");
}

BOOST_AUTO_TEST_CASE(gba_emerald_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("Emerald");
    pkmntest::gba_item_bag_test(bag, "Emerald");
}

// FireRed

BOOST_AUTO_TEST_CASE(gba_firered_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "FireRed");
    pkmntest::gba_item_pocket_test(item_pocket, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("Key Items", "FireRed");
    pkmntest::gba_key_item_pocket_test(key_item_pocket, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Poké Balls", "FireRed");
    pkmntest::gba_ball_pocket_test(ball_pocket, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TM Case", "FireRed");
    pkmntest::gba_tmhm_pocket_test(tmhm_pocket, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_berry_pocket_test) {
    pkmn::item_list::sptr berry_pocket = pkmn::item_list::make("Berry Pouch", "FireRed");
    pkmntest::gba_berry_pocket_test(berry_pocket, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "FireRed");
    pkmntest::gba_pc_test(pc, "FireRed");
}

BOOST_AUTO_TEST_CASE(gba_firered_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("FireRed");
    pkmntest::gba_item_bag_test(bag, "FireRed");
}

// LeafGreen

BOOST_AUTO_TEST_CASE(gba_leafgreen_item_pocket_test) {
    pkmn::item_list::sptr item_pocket = pkmn::item_list::make("Items", "LeafGreen");
    pkmntest::gba_item_pocket_test(item_pocket, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_key_item_pocket_test) {
    pkmn::item_list::sptr key_item_pocket = pkmn::item_list::make("Key Items", "LeafGreen");
    pkmntest::gba_key_item_pocket_test(key_item_pocket, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_ball_pocket_test) {
    pkmn::item_list::sptr ball_pocket = pkmn::item_list::make("Poké Balls", "LeafGreen");
    pkmntest::gba_ball_pocket_test(ball_pocket, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_tmhm_pocket_test) {
    pkmn::item_list::sptr tmhm_pocket = pkmn::item_list::make("TM Case", "LeafGreen");
    pkmntest::gba_tmhm_pocket_test(tmhm_pocket, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_berry_pocket_test) {
    pkmn::item_list::sptr berry_pocket = pkmn::item_list::make("Berry Pouch", "LeafGreen");
    pkmntest::gba_berry_pocket_test(berry_pocket, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_pc_test) {
    pkmn::item_list::sptr pc = pkmn::item_list::make("PC", "LeafGreen");
    pkmntest::gba_pc_test(pc, "LeafGreen");
}

BOOST_AUTO_TEST_CASE(gba_leafgreen_item_bag_test) {
    pkmn::item_bag::sptr bag = pkmn::item_bag::make("LeafGreen");
    pkmntest::gba_item_bag_test(bag, "LeafGreen");
}
