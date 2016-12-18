/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.hpp"

#include <pkmntest/gba_items_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>

#include <map>

namespace pkmntest {

    static const std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Ruby",      pkmn::database::item_entry("None", "Ruby"))
        ("Sapphire",  pkmn::database::item_entry("None", "Sapphire"))
        ("Emerald",   pkmn::database::item_entry("None", "Emerald"))
        ("FireRed",   pkmn::database::item_entry("None", "FireRed"))
        ("LeafGreen", pkmn::database::item_entry("None", "LeafGreen"))
    ;
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

    void gba_item_pocket_test(
        pkmn::item_list::sptr item_pocket,
        const std::string &game
    ) {
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

        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(item_pocket->get_name(), "Items");
        BOOST_CHECK_EQUAL(item_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(item_pocket->get_capacity(), capacity);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            item_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            item_pocket,
            "Potion"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
            ("Bicycle")("Master Ball")("HM01")("Razz Berry")
        ;
        test_item_list_invalid_items(
            item_pocket,
            wrong_pocket_items
        );

        // Make sure we can't add items from other generations.
        static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
            ("Pink Bow")("Black Sludge")("Binding Band")("Beedrillite")
        ;
        test_item_list_invalid_items(
            item_pocket,
            wrong_generation_items
        );

        // Make sure we can't add items from Gamecube games.
        static const std::vector<std::string> gcn_items = boost::assign::list_of
            ("Time Flute")
            ("Poké Snack")
        ;
        test_item_list_invalid_items(
            item_pocket,
            gcn_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Potion")("Orange Mail")("Lava Cookie")("Stardust")
            ("Shadow Mail")("Pink Scarf")("Antidote")("Green Shard")
        ;
        test_item_list_add_remove(
            item_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = item_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gba_key_item_pocket_test(
        pkmn::item_list::sptr key_item_pocket,
        const std::string &game
    ) {
        int game_id = key_item_pocket->at(0).item.get_game_id();

        int capacity = 0;
        if(game_id == RUBY or game_id == SAPPHIRE) {
            capacity = 20;
        } else {
            capacity = 30;
        }

        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(key_item_pocket->get_name(), "Key Items");
        BOOST_CHECK_EQUAL(key_item_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(key_item_pocket->get_capacity(), capacity);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            key_item_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            key_item_pocket,
            "Basement Key"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
            ("Potion")("Master Ball")("HM01")("Razz Berry")
        ;
        test_item_list_invalid_items(
            key_item_pocket,
            wrong_pocket_items
        );

        // Make sure we can't add items from other generations.
        static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
            ("GS Ball")("Poffin Items")("DNA Splicers")("Aqua Suit")
        ;
        test_item_list_invalid_items(
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
        test_item_list_invalid_items(
            key_item_pocket,
            gcn_items
        );
        if(game == "Ruby" or game == "Sapphire") {
            test_item_list_invalid_items(
                key_item_pocket,
                frlg_items
            );
        }
        if(game != "Emerald") {
            test_item_list_invalid_items(
                key_item_pocket,
                emerald_items
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Wailmer Pail")("Basement Key")("Meteorite")("Old Rod")
            ("Red Orb")("Root Fossil")("Contest Pass")("Eon Ticket")
        ;
        test_item_list_add_remove(
            key_item_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = key_item_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gba_ball_pocket_test(
        pkmn::item_list::sptr ball_pocket,
        const std::string &game
    ) {
        int game_id = ball_pocket->at(0).item.get_game_id();

        int capacity = 0;
        if(game_id == FIRERED or game_id == LEAFGREEN) {
            capacity = 13;
        } else {
            capacity = 16;
        }

        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(ball_pocket->get_name(), "Poké Balls");
        BOOST_CHECK_EQUAL(ball_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(ball_pocket->get_capacity(), capacity);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            ball_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            ball_pocket,
            "Master Ball"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
            ("Potion")("Bicycle")("HM01")("Razz Berry")
        ;
        test_item_list_invalid_items(
            ball_pocket,
            wrong_pocket_items
        );

        // Make sure we can't add items from other generations.
        static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
            ("Moon Ball")("Heal Ball")("Dream Ball")
        ;
        test_item_list_invalid_items(
            ball_pocket,
            wrong_generation_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Master Ball")("Ultra Ball")("Great Ball")("Poké Ball")
            ("Safari Ball")("Net Ball")("Dive Ball")("Nest Ball")
        ;
        test_item_list_add_remove(
            ball_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = ball_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gba_tmhm_pocket_test(
        pkmn::item_list::sptr tmhm_pocket,
        const std::string &game
    ) {
        int game_id = tmhm_pocket->at(0).item.get_game_id();

        int capacity = 0;
        std::string pocket_name;
        if(game_id == FIRERED or game_id == LEAFGREEN) {
            capacity = 58;
            pocket_name = "TM Case";
        } else {
            capacity = 64;
            pocket_name = "TMs & HMs";
        }

        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(tmhm_pocket->get_name(), pocket_name);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_capacity(), capacity);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            tmhm_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            tmhm_pocket,
            "TM01"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
            ("Potion")("Bicycle")("Great Ball")("Razz Berry")
        ;
        test_item_list_invalid_items(
            tmhm_pocket,
            wrong_pocket_items
        );

        // Make sure we can't add items from other generations.
        static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
            ("TM51")
        ;
        test_item_list_invalid_items(
            tmhm_pocket,
            wrong_generation_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("TM01")("HM01")("TM02")("HM02")
            ("TM03")("HM03")("TM04")("HM04")
        ;
        test_item_list_add_remove(
            tmhm_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = tmhm_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gba_berry_pocket_test(
        pkmn::item_list::sptr berry_pocket,
        const std::string &game
    ) {
        int game_id = berry_pocket->at(0).item.get_game_id();

        int capacity = 0;
        std::string pocket_name;
        if(game_id == FIRERED or game_id == LEAFGREEN) {
            capacity = 43;
            pocket_name = "Berry Pouch";
        } else {
            capacity = 46;
            pocket_name = "Berries";
        }

        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(berry_pocket->get_name(), pocket_name);
        BOOST_CHECK_EQUAL(berry_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(berry_pocket->get_capacity(), capacity);
        BOOST_CHECK_EQUAL(berry_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            berry_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            berry_pocket,
            "Razz Berry"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_pocket_items = boost::assign::list_of
            ("Potion")("Bicycle")("Great Ball")("HM02")
        ;
        test_item_list_invalid_items(
            berry_pocket,
            wrong_pocket_items
        );

        // Make sure we can't add items from other generations.
        static const std::vector<std::string> wrong_generation_items = boost::assign::list_of
            ("Berry")("Occa Berry")("Roseli Berry")
        ;
        test_item_list_invalid_items(
            berry_pocket,
            wrong_generation_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Cheri Berry")("Razz Berry")("Lum Berry")("Pinap Berry")
            ("Aspear Berry")("Iapapa Berry")("Wiki Berry")("Apicot Berry")
        ;
        test_item_list_add_remove(
            berry_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = berry_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gba_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    ) {
        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), game);
        BOOST_CHECK_EQUAL(pc->get_capacity(), 50);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        test_item_list_empty_slots(
            pc,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            pc,
            "Potion"
        );

        // Make sure we can't add items from other generations or invalid Generation III games.
        test_item_list_invalid_items(
            pc,
            wrong_game_all_pocket_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        test_item_list_add_remove(
            pc,
            none_entries.at(game),
            all_pocket_item_names
        );
    }

    void gba_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(bag->get_game(), game);

        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 5);

        bool frlg = (game == "FireRed" or game == "LeafGreen");

        gba_item_pocket_test(pockets.at("Items"), game);
        gba_key_item_pocket_test(pockets.at("Key Items"), game);
        gba_ball_pocket_test(pockets.at("Poké Balls"), game);
        if(frlg) {
            BOOST_CHECK_EQUAL(pockets.count("TMs & HMs"), 0);
            BOOST_CHECK_EQUAL(pockets.count("Berries"), 0);
            gba_tmhm_pocket_test(pockets.at("TM Case"), game);
            gba_berry_pocket_test(pockets.at("Berry Pouch"), game);
        } else {
            BOOST_CHECK_EQUAL(pockets.count("TM Case"), 0);
            BOOST_CHECK_EQUAL(pockets.count("Berry Pouch"), 0);
            gba_tmhm_pocket_test(pockets.at("TMs & HMs"), game);
            gba_berry_pocket_test(pockets.at("Berries"), game);
        }

        // Make sure adding items through the bag adds to the proper pockets.
        BOOST_REQUIRE_EQUAL(pockets.at("Items")->get_num_items(), 0);
        BOOST_REQUIRE_EQUAL(pockets.at("Key Items")->get_num_items(), 0);
        BOOST_REQUIRE_EQUAL(pockets.at("Poké Balls")->get_num_items(), 0);
        if(frlg) {
            BOOST_REQUIRE_EQUAL(pockets.at("TM Case")->get_num_items(), 0);
            BOOST_REQUIRE_EQUAL(pockets.at("Berry Pouch")->get_num_items(), 0);
        } else {
            BOOST_REQUIRE_EQUAL(pockets.at("TMs & HMs")->get_num_items(), 0);
            BOOST_REQUIRE_EQUAL(pockets.at("Berries")->get_num_items(), 0);
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
        const pkmn::item_slots_t& tm_hm_slots = pockets.at(frlg ? "TM Case" : "TMs & HMs")->as_vector();
        const pkmn::item_slots_t& berry_slots = pockets.at(frlg ? "Berry Pouch" : "Berries")->as_vector();

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 0);

        BOOST_CHECK_EQUAL(key_item_slots.at(0).item.get_name(), "Mach Bike");
        BOOST_CHECK_EQUAL(key_item_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(key_item_slots.at(1).item.get_name(), "Wailmer Pail");
        BOOST_CHECK_EQUAL(key_item_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(key_item_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(key_item_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(ball_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(ball_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(ball_slots.at(1).item.get_name(), "Master Ball");
        BOOST_CHECK_EQUAL(ball_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(ball_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(ball_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).item.get_name(), "TM01");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).item.get_name(), "HM04");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(berry_slots.at(0).item.get_name(), "Aspear Berry");
        BOOST_CHECK_EQUAL(berry_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(berry_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(berry_slots.at(1).amount, 0);

        // Make sure removing items through the bag removes from the proper pockets.
        for(int i = 0; i < 8; ++i) {
            bag->remove(
                all_pocket_item_names[i],
                5
            );
        }

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "None");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 0);

        BOOST_CHECK_EQUAL(key_item_slots.at(0).item.get_name(), "None");
        BOOST_CHECK_EQUAL(key_item_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(key_item_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(key_item_slots.at(1).amount, 0);
        BOOST_CHECK_EQUAL(key_item_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(key_item_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(ball_slots.at(0).item.get_name(), "None");
        BOOST_CHECK_EQUAL(ball_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(ball_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(ball_slots.at(1).amount, 0);
        BOOST_CHECK_EQUAL(ball_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(ball_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).item.get_name(), "None");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(berry_slots.at(0).item.get_name(), "None");
        BOOST_CHECK_EQUAL(berry_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(berry_slots.at(1).item.get_name(), "None");
        BOOST_CHECK_EQUAL(berry_slots.at(1).amount, 0);

        // Make sure we can't add items from other generations or invalid Generation III games.
        test_item_bag_invalid_items(
            bag,
            wrong_game_all_pocket_items
        );
    }
}
