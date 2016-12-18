/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "items_tests_common.hpp"

#include <pkmntest/gen2_items_tests.hpp>

#include <pkmn/database/lists.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>

#include <map>

namespace pkmntest {

    static const std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Gold",    pkmn::database::item_entry("None", "Gold"))
        ("Silver",  pkmn::database::item_entry("None", "Silver"))
        ("Crystal", pkmn::database::item_entry("None", "Crystal"))
    ;
    static const std::vector<std::string> all_pocket_item_names = boost::assign::list_of
        ("Potion")("Bicycle")("Great Ball")("TM28")
        ("Berry")("SquirtBottle")("Friend Ball")("HM01")
    ;

    void gen2_item_pocket_test(
        pkmn::item_list::sptr item_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(item_pocket->get_name(), "Items");
        BOOST_CHECK_EQUAL(item_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(item_pocket->get_capacity(), 20);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty.
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
        static const std::vector<std::string> wrong_items = boost::assign::list_of
            ("Bicycle")("Master Ball")("HM01")
        ;
        test_item_list_items_from_wrong_pocket(
            item_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Potion")("HP Up")("Wht Apricorn")("Lucky Egg")
            ("Flower Mail")("Burn Heal")("PSNCureBerry")("Stick")
        ;
        test_item_list_add_remove(
            item_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = item_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gen2_key_item_pocket_test(
        pkmn::item_list::sptr key_item_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(key_item_pocket->get_name(), "KeyItems");
        BOOST_CHECK_EQUAL(key_item_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(key_item_pocket->get_capacity(), 26);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty.
        test_item_list_empty_slots(
            key_item_pocket,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            key_item_pocket,
            "Bicycle"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_items = boost::assign::list_of
            ("Potion")("Master Ball")("HM01")
        ;
        test_item_list_items_from_wrong_pocket(
            key_item_pocket,
            wrong_items
        );

        // Crystal-specific items.
        if(game == "Crystal") {
            key_item_pocket->add("Clear Bell", 1);
            key_item_pocket->remove("Clear Bell", 1);

            key_item_pocket->add("GS Ball", 1);
            key_item_pocket->remove("GS Ball", 1);

            key_item_pocket->add("Blue Card", 1);
            key_item_pocket->remove("Blue Card", 1);

            key_item_pocket->add("Egg Ticket", 1);
            key_item_pocket->remove("Egg Ticket", 1);

            BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);
        } else {
            BOOST_CHECK_THROW(
                key_item_pocket->add("Clear Bell", 1);
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                key_item_pocket->add("GS Ball", 1);
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                key_item_pocket->add("Blue Card", 1);
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                key_item_pocket->add("Egg Ticket", 1);
            , std::invalid_argument);
            BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Bicycle")("Basement Key")("SecretPotion")("Mystery Egg")
            ("Silver Wing")("Lost Item")("SquirtBottle")("Rainbow Wing")
        ;
        test_item_list_add_remove(
            key_item_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = key_item_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gen2_ball_pocket_test(
        pkmn::item_list::sptr ball_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(ball_pocket->get_name(), "Balls");
        BOOST_CHECK_EQUAL(ball_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(ball_pocket->get_capacity(), 12);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty.
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
        static const std::vector<std::string> wrong_items = boost::assign::list_of
            ("Potion")("Bicycle")("HM01")
        ;
        test_item_list_items_from_wrong_pocket(
            ball_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        static const std::vector<std::string> item_names = boost::assign::list_of
            ("Great Ball")("Poké Ball")("Park Ball")("Fast Ball")
            ("Master Ball")("Friend Ball")("Love Ball")("Level Ball")
        ;
        test_item_list_add_remove(
            ball_pocket,
            none_entries.at(game),
            item_names
        );

        const std::vector<std::string>& valid_items = ball_pocket->get_valid_items();
        BOOST_CHECK_GT(valid_items.size(), 0);
    }

    void gen2_tmhm_pocket_test(
        pkmn::item_list::sptr tmhm_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(tmhm_pocket->get_name(), "TM/HM");
        BOOST_CHECK_EQUAL(tmhm_pocket->get_game(), game);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_capacity(), 57);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty.
        const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 57);
        for(int i = 1; i <= 50; ++i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "TM%02d", i);
            BOOST_CHECK_EQUAL(item_slots[i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[i-1].amount, 0);
        }
        for(int i = 1; i <= 7; ++i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "HM%02d", i);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            tmhm_pocket,
            "TM10"
        );

        // Make sure we can't add items from other pockets.
        static const std::vector<std::string> wrong_items = boost::assign::list_of
            ("Potion")("Master Ball")("Bicycle")
        ;
        test_item_list_items_from_wrong_pocket(
            tmhm_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        for(int i = 1; i <= 50; ++i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "TM%02d", i);
            tmhm_pocket->add(name, 50);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i);
            BOOST_CHECK_EQUAL(item_slots[i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[i-1].amount, 50);
        }
        for(int i = 50; i >= 1; --i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "TM%02d", i);
            tmhm_pocket->remove(name, 50);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i-1);
            BOOST_CHECK_EQUAL(item_slots[i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[i-1].amount, 0);
        }

        for(int i = 1; i <= 7; ++i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "HM%02d", i);
            tmhm_pocket->add(name, 1);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].amount, 1);
        }
        for(int i = 7; i >= 1; --i) {
            char name[5];
            std::snprintf(name, sizeof(name), "HM%02d", i);
            tmhm_pocket->remove(name, 1);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i-1);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[50+i-1].amount, 0);
        }

        const std::vector<std::string>& valid_items = tmhm_pocket->get_valid_items();
        BOOST_CHECK_EQUAL(valid_items.size(), 57);
    }

    void gen2_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), game);
        BOOST_CHECK_EQUAL(pc->get_capacity(), 50);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            pc,
            "Potion"
        );

        // Make sure item slots start as correctly empty.
        test_item_list_empty_slots(
            pc,
            none_entries.at(game)
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        test_item_list_add_remove(
            pc,
            none_entries.at(game),
            all_pocket_item_names
        );

        const std::vector<std::string>& valid_items = pc->get_valid_items();
        std::vector<std::string> full_item_list = pkmn::database::get_item_list(game);
        BOOST_CHECK_EQUAL(valid_items.size(), full_item_list.size());
    }

    void gen2_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(bag->get_game(), game);

        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 4);

        gen2_item_pocket_test(pockets.at("Items"), game);
        gen2_key_item_pocket_test(pockets.at("KeyItems"), game);
        gen2_ball_pocket_test(pockets.at("Balls"), game);
        gen2_tmhm_pocket_test(pockets.at("TM/HM"), game);

        // Make sure adding items through the bag adds to the proper pockets.
        BOOST_REQUIRE_EQUAL(pockets.at("Items")->get_num_items(), 0);
        BOOST_REQUIRE_EQUAL(pockets.at("KeyItems")->get_num_items(), 0);
        BOOST_REQUIRE_EQUAL(pockets.at("Balls")->get_num_items(), 0);
        BOOST_REQUIRE_EQUAL(pockets.at("TM/HM")->get_num_items(), 0);
        for(int i = 0; i < 8; ++i) {
            bag->add(
                all_pocket_item_names[i],
                5
            );
        }

        const pkmn::item_slots_t& item_slots = pockets.at("Items")->as_vector();
        const pkmn::item_slots_t& key_item_slots = pockets.at("KeyItems")->as_vector();
        const pkmn::item_slots_t& ball_slots = pockets.at("Balls")->as_vector();
        const pkmn::item_slots_t& tm_hm_slots = pockets.at("TM/HM")->as_vector();

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Berry");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(key_item_slots.at(0).item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(key_item_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(key_item_slots.at(1).item.get_name(), "SquirtBottle");
        BOOST_CHECK_EQUAL(key_item_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(key_item_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(key_item_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(ball_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(ball_slots.at(0).amount, 5);
        BOOST_CHECK_EQUAL(ball_slots.at(1).item.get_name(), "Friend Ball");
        BOOST_CHECK_EQUAL(ball_slots.at(1).amount, 5);
        BOOST_CHECK_EQUAL(ball_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(ball_slots.at(2).amount, 0);

        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).item.get_name(), "TM01");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).item.get_name(), "TM02");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(27).item.get_name(), "TM28");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(27).amount, 5);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(50).item.get_name(), "HM01");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(50).amount, 5);

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
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "None");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);

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

        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).item.get_name(), "TM01");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(0).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).item.get_name(), "TM02");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(1).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(27).item.get_name(), "TM28");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(27).amount, 0);
        BOOST_CHECK_EQUAL(tm_hm_slots.at(50).item.get_name(), "HM01");
        BOOST_CHECK_EQUAL(tm_hm_slots.at(50).amount, 0);
    }
}
