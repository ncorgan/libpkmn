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

static const std::vector<std::string> all_pocket_item_names = boost::assign::list_of
    ("Potion")("Mach Bike")("Great Ball")("TM01")
    ("Aspear Berry")("Wailmer Pail")("Master Ball")("HM04")
;
static const std::vector<std::string> wrong_game_all_pocket_items = boost::assign::list_of
    ("Pink Bow")("Black Sludge")
    ("Ein File S")("Gonzap's Key")
    ("GS Ball")("Poffin Items")
    ("TM51")
    ("Berry")("Occa Berry")
;

BOOST_STATIC_CONSTEXPR int RUBY      = 7;
BOOST_STATIC_CONSTEXPR int SAPPHIRE  = 8;
BOOST_STATIC_CONSTEXPR int EMERALD   = 9;
BOOST_STATIC_CONSTEXPR int FIRERED   = 10;
BOOST_STATIC_CONSTEXPR int LEAFGREEN = 11;

class gba_item_list_test: public pkmntest::item_list_test {};

void gba_item_pocket_test(
    pkmn::item_list::sptr item_pocket
) {
    ASSERT_EQ("Items", item_pocket->get_name());

    int capacity = 0;
    switch(item_pocket->at(0).item.get_game_id()) {
        case RUBY:
        case SAPPHIRE:
            capacity = 20;
            break;

        case EMERALD:
            capacity = 30;
            break;

        default:
            capacity = 42;
            break;
    }
    ASSERT_EQ(capacity, item_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(item_pocket);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        item_pocket,
        "Potion"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Bicycle")("Master Ball")("HM01")("Razz Berry")
    ;
    pkmntest::test_item_list_invalid_items(
        item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Pink Bow")("Black Sludge")("Binding Band")("Beedrillite")
    ;
    pkmntest::test_item_list_invalid_items(
        item_pocket,
        wrong_generation_items
    );

    // Make sure we can't add items from Gamecube games.
    static const std::vector<std::string> gcn_items = boost::assign::list_of
        ("Time Flute")
        ("Poké Snack")
    ;
    pkmntest::test_item_list_invalid_items(
        item_pocket,
        gcn_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Potion")("Orange Mail")("Lava Cookie")("Stardust")
        ("Shadow Mail")("Pink Scarf")("Antidote")("Green Shard")
    ;
    pkmntest::test_item_list_add_remove(
        item_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = item_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

void gba_key_item_pocket_test(
    pkmn::item_list::sptr key_item_pocket
) {
    ASSERT_EQ("Key Items", key_item_pocket->get_name());

    int game_id = key_item_pocket->at(0).item.get_game_id();
    std::string game = key_item_pocket->get_game();

    int capacity = 0;
    if(game_id == RUBY or game_id == SAPPHIRE) {
        capacity = 20;
    } else {
        capacity = 30;
    }
    ASSERT_EQ(capacity, key_item_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), key_item_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(key_item_pocket);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        key_item_pocket,
        "Basement Key"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Master Ball")("HM01")("Razz Berry")
    ;
    pkmntest::test_item_list_invalid_items(
        key_item_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("GS Ball")("Poffin Items")("DNA Splicers")("Aqua Suit")
    ;
    pkmntest::test_item_list_invalid_items(
        key_item_pocket,
        wrong_generation_items
    );

    // Make sure we can't add items from incompatible Generation III games.
    static const std::vector<std::string> gcn_items = boost::assign::list_of
        ("Ein File S")("Powerup Part")
        ("Gonzap's Key")("Krane Memo 1")
    ;
    static const std::vector<std::string> frlg_items = boost::assign::list_of
        ("Helix Fossil")("Tea")("Ruby")
    ;
    static const std::vector<std::string> emerald_items = boost::assign::list_of
        ("Magma Emblem")("Old Sea Map")
    ;
    pkmntest::test_item_list_invalid_items(
        key_item_pocket,
        gcn_items
    );
    if(game == "Ruby" or game == "Sapphire") {
        pkmntest::test_item_list_invalid_items(
            key_item_pocket,
            frlg_items
        );
    }
    if(game != "Emerald") {
        pkmntest::test_item_list_invalid_items(
            key_item_pocket,
            emerald_items
        );
    }

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Wailmer Pail")("Basement Key")("Meteorite")("Old Rod")
        ("Red Orb")("Root Fossil")("Contest Pass")("Eon Ticket")
    ;
    pkmntest::test_item_list_add_remove(
        key_item_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = key_item_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

void gba_ball_pocket_test(
    pkmn::item_list::sptr ball_pocket
) {
    ASSERT_EQ("Poké Balls", ball_pocket->get_name());

    int game_id = ball_pocket->at(0).item.get_game_id();

    int capacity = 0;
    if(game_id == FIRERED or game_id == LEAFGREEN) {
        capacity = 13;
    } else {
        capacity = 16;
    }
    ASSERT_EQ(capacity, ball_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), ball_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(ball_pocket);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        ball_pocket,
        "Master Ball"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("HM01")("Razz Berry")
    ;
    pkmntest::test_item_list_invalid_items(
        ball_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Moon Ball")("Heal Ball")("Dream Ball")
    ;
    pkmntest::test_item_list_invalid_items(
        ball_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Master Ball")("Ultra Ball")("Great Ball")("Poké Ball")
        ("Safari Ball")("Net Ball")("Dive Ball")("Nest Ball")
    ;
    pkmntest::test_item_list_add_remove(
        ball_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = ball_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

void gba_tmhm_pocket_test(
    pkmn::item_list::sptr tmhm_pocket
) {
    int game_id = tmhm_pocket->at(0).item.get_game_id();

    int capacity = 0;
    if(game_id == FIRERED or game_id == LEAFGREEN) {
        ASSERT_EQ("TM Case", tmhm_pocket->get_name());
        capacity = 58;
    } else {
        ASSERT_EQ("TMs & HMs", tmhm_pocket->get_name());
        capacity = 64;
    }
    ASSERT_EQ(capacity, tmhm_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), tmhm_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(tmhm_pocket);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        tmhm_pocket,
        "TM01"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("Great Ball")("Razz Berry")
    ;
    pkmntest::test_item_list_invalid_items(
        tmhm_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("TM51")
    ;
    pkmntest::test_item_list_invalid_items(
        tmhm_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("TM01")("HM01")("TM02")("HM02")
        ("TM03")("HM03")("TM04")("HM04")
    ;
    pkmntest::test_item_list_add_remove(
        tmhm_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = tmhm_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

void gba_berry_pocket_test(
    pkmn::item_list::sptr berry_pocket
) {
    int game_id = berry_pocket->at(0).item.get_game_id();

    int capacity = 0;
    if(game_id == FIRERED or game_id == LEAFGREEN) {
        ASSERT_EQ("Berry Pouch", berry_pocket->get_name());
        capacity = 43;
    } else {
        ASSERT_EQ("Berries", berry_pocket->get_name());
        capacity = 46;
    }
    ASSERT_EQ(capacity, berry_pocket->get_capacity());
    ASSERT_EQ(size_t(capacity), berry_pocket->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(berry_pocket);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        berry_pocket,
        "Razz Berry"
    );

    // Make sure we can't add items from other pockets.
    static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
        ("Potion")("Bicycle")("Great Ball")("HM02")
    ;
    pkmntest::test_item_list_invalid_items(
        berry_pocket,
        wrong_pocket_items
    );

    // Make sure we can't add items from other generations.
    static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
        ("Berry")("Occa Berry")("Roseli Berry")
    ;
    pkmntest::test_item_list_invalid_items(
        berry_pocket,
        wrong_generation_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    static const std::vector<std::string> item_names = boost::assign::list_of
        ("Cheri Berry")("Razz Berry")("Lum Berry")("Pinap Berry")
        ("Aspear Berry")("Iapapa Berry")("Wiki Berry")("Apicot Berry")
    ;
    pkmntest::test_item_list_add_remove(
        berry_pocket,
        item_names
    );

    const std::vector<std::string>& valid_items = berry_pocket->get_valid_items();
    EXPECT_GT(valid_items.size(), 0);
}

void gba_item_pc_test(
    pkmn::item_list::sptr item_pc
) {
    ASSERT_EQ("PC", item_pc->get_name());
    ASSERT_EQ(50, item_pc->get_capacity());
    ASSERT_EQ(50, item_pc->as_vector().size());

    // Make sure item slots start as correctly empty
    pkmntest::test_item_list_empty_slots(item_pc);

    // Confirm exceptions are thrown when expected.
    pkmntest::test_item_list_out_of_range_error(
        item_pc,
        "Potion"
    );

    // Make sure we can't add items from other generations or incompatible Generation III games.
    pkmntest::test_item_list_invalid_items(
        item_pc,
        wrong_game_all_pocket_items
    );

    // Start adding and removing stuff, and make sure the numbers are accurate.
    pkmntest::test_item_list_add_remove(
        item_pc,
        all_pocket_item_names
    );
}

static const pkmntest::item_list_test_fcns_t gba_test_fcns = boost::assign::map_list_of
    ("Items", &gba_item_pocket_test)
    ("Key Items", &gba_key_item_pocket_test)
    ("Poké Balls", &gba_ball_pocket_test)
    ("TMs & HMs", &gba_tmhm_pocket_test)
    ("TM Case", &gba_tmhm_pocket_test)
    ("Berries", &gba_berry_pocket_test)
    ("Berry Pouch", &gba_berry_pocket_test)
    ("PC", &gba_item_pc_test)
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
    pkmn::item_bag::sptr bag = get_item_bag();

    const std::string& game = get_game();
    bool is_frlg = (game == "FireRed" or game == "LeafGreen");
    std::string berry_pocket_name = is_frlg ? "Berry Pouch" : "Berries";
    std::string tmhm_pocket_name = is_frlg ? "TM Case" : "TMs & HMs";

    const pkmn::item_pockets_t& pockets = bag->get_pockets();
    ASSERT_EQ(5, pockets.size());
    ASSERT_EQ("Items", pockets.at("Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Items")->get_game());
    ASSERT_EQ("Key Items", pockets.at("Key Items")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Key Items")->get_game());
    ASSERT_EQ("Poké Balls", pockets.at("Poké Balls")->get_name());
    ASSERT_EQ(get_game(), pockets.at("Poké Balls")->get_game());
    ASSERT_EQ(berry_pocket_name, pockets.at(berry_pocket_name)->get_name());
    ASSERT_EQ(get_game(), pockets.at(berry_pocket_name)->get_game());
    ASSERT_EQ(tmhm_pocket_name, pockets.at(tmhm_pocket_name)->get_name());
    ASSERT_EQ(get_game(), pockets.at(tmhm_pocket_name)->get_game());

    const std::vector<std::string>& pocket_names = bag->get_pocket_names();
    ASSERT_EQ(5, pockets.size());
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Key Items"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), "Poké Balls"));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), berry_pocket_name));
    ASSERT_NE(pocket_names.end(), std::find(pocket_names.begin(), pocket_names.end(), tmhm_pocket_name));

    gba_item_pocket_test(pockets.at("Items"));
    gba_key_item_pocket_test(pockets.at("Key Items"));
    gba_ball_pocket_test(pockets.at("Poké Balls"));
    gba_berry_pocket_test(pockets.at(berry_pocket_name));
    gba_tmhm_pocket_test(pockets.at(tmhm_pocket_name));

    // Make sure adding items through the bag adds to the proper pockets.
    ASSERT_EQ(0, pockets.at("Items")->get_num_items());
    ASSERT_EQ(0, pockets.at("Key Items")->get_num_items());
    ASSERT_EQ(0, pockets.at("Poké Balls")->get_num_items());
    if(is_frlg) {
        ASSERT_EQ(0, pockets.at("TM Case")->get_num_items());
        ASSERT_EQ(0, pockets.at("Berry Pouch")->get_num_items());
    } else {
        ASSERT_EQ(0, pockets.at("TMs & HMs")->get_num_items());
        ASSERT_EQ(0, pockets.at("Berries")->get_num_items());
    }
    for(int i = 0; i < 8; ++i) {
        bag->add(
            all_pocket_item_names[i],
            5
        );
    }

    const pkmn::item_slots_t& item_slots = pockets.at("Items")->as_vector();
    const pkmn::item_slots_t& key_item_slots = pockets.at("Key Items")->as_vector();
    const pkmn::item_slots_t& ball_slots = pockets.at("Poké Balls")->as_vector();
    const pkmn::item_slots_t& tm_hm_slots = pockets.at(tmhm_pocket_name)->as_vector();
    const pkmn::item_slots_t& berry_slots = pockets.at(berry_pocket_name)->as_vector();

    EXPECT_EQ("Potion", item_slots.at(0).item.get_name());
    EXPECT_EQ(5, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item.get_name());
    EXPECT_EQ(0, item_slots.at(1).amount);

    EXPECT_EQ("Mach Bike", key_item_slots.at(0).item.get_name());
    EXPECT_EQ(5, key_item_slots.at(0).amount);
    EXPECT_EQ("Wailmer Pail", key_item_slots.at(1).item.get_name());
    EXPECT_EQ(5, key_item_slots.at(1).amount);
    EXPECT_EQ("None", key_item_slots.at(2).item.get_name());
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ("Great Ball", ball_slots.at(0).item.get_name());
    EXPECT_EQ(5, ball_slots.at(0).amount);
    EXPECT_EQ("Master Ball", ball_slots.at(1).item.get_name());
    EXPECT_EQ(5, ball_slots.at(1).amount);
    EXPECT_EQ("None", ball_slots.at(2).item.get_name());
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ("TM01", tm_hm_slots.at(0).item.get_name());
    EXPECT_EQ(5, tm_hm_slots.at(0).amount);
    EXPECT_EQ("HM04", tm_hm_slots.at(1).item.get_name());
    EXPECT_EQ(5, tm_hm_slots.at(1).amount);
    EXPECT_EQ("None", tm_hm_slots.at(2).item.get_name());
    EXPECT_EQ(0, tm_hm_slots.at(2).amount);

    EXPECT_EQ("Aspear Berry", berry_slots.at(0).item.get_name());
    EXPECT_EQ(5, berry_slots.at(0).amount);
    EXPECT_EQ("None", berry_slots.at(1).item.get_name());
    EXPECT_EQ(0, berry_slots.at(1).amount);

    // Make sure removing items through the bag removes from the proper pockets.
    for(int i = 0; i < 8; ++i) {
        bag->remove(
            all_pocket_item_names[i],
            5
        );
    }

    EXPECT_EQ("None", item_slots.at(0).item.get_name());
    EXPECT_EQ(0, item_slots.at(0).amount);
    EXPECT_EQ("None", item_slots.at(1).item.get_name());
    EXPECT_EQ(0, item_slots.at(1).amount);

    EXPECT_EQ("None", key_item_slots.at(0).item.get_name());
    EXPECT_EQ(0, key_item_slots.at(0).amount);
    EXPECT_EQ("None", key_item_slots.at(1).item.get_name());
    EXPECT_EQ(0, key_item_slots.at(1).amount);
    EXPECT_EQ("None", key_item_slots.at(2).item.get_name());
    EXPECT_EQ(0, key_item_slots.at(2).amount);

    EXPECT_EQ("None", ball_slots.at(0).item.get_name());
    EXPECT_EQ(0, ball_slots.at(0).amount);
    EXPECT_EQ("None", ball_slots.at(1).item.get_name());
    EXPECT_EQ(0, ball_slots.at(1).amount);
    EXPECT_EQ("None", ball_slots.at(2).item.get_name());
    EXPECT_EQ(0, ball_slots.at(2).amount);

    EXPECT_EQ("None", tm_hm_slots.at(0).item.get_name());
    EXPECT_EQ(0, tm_hm_slots.at(0).amount);
    EXPECT_EQ("None", tm_hm_slots.at(1).item.get_name());
    EXPECT_EQ(0, tm_hm_slots.at(1).amount);
    EXPECT_EQ("None", tm_hm_slots.at(2).item.get_name());
    EXPECT_EQ(0, tm_hm_slots.at(2).amount);

    EXPECT_EQ("None", berry_slots.at(0).item.get_name());
    EXPECT_EQ(0, berry_slots.at(0).amount);
    EXPECT_EQ("None", berry_slots.at(1).item.get_name());
    EXPECT_EQ(0, berry_slots.at(1).amount);

    // Make sure we can't add items from other generations or invalid Generation III games.
    pkmntest::test_item_bag_invalid_items(
        bag,
        wrong_game_all_pocket_items
    );
}

static const std::vector<std::string> item_bag_params = {
    "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gba_item_bag_test,
    gba_item_bag_test,
    ::testing::ValuesIn(item_bag_params)
);
