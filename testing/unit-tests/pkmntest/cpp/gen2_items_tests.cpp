/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen2_items_tests.hpp>

#include <pkmn/database/lists.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/test/unit_test.hpp>

#include <map>

namespace pkmntest {

    static std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Gold",    pkmn::database::item_entry("None", "Gold"))
        ("Silver",  pkmn::database::item_entry("None", "Silver"))
        ("Crystal", pkmn::database::item_entry("None", "Crystal"))
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
        const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 20);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            item_pocket->add("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            item_pocket->add("Potion", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            item_pocket->remove("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            item_pocket->remove("Potion", 100)
        , std::out_of_range);

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            item_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            item_pocket->add("Master Ball", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            item_pocket->add("HM01", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 0);

        // Start adding and removing stuff, and make sure the numbers are accurate.
        item_pocket->add("Potion", 30);
        item_pocket->add("PSNCureBerry", 99);
        item_pocket->add("Leftovers", 1);

        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "PSNCureBerry");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Leftovers");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);

        item_pocket->add("Leftovers", 15);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "PSNCureBerry");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Leftovers");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);

        item_pocket->remove("PSNCureBerry", 20);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "PSNCureBerry");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Leftovers");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);

        item_pocket->move(0, 1);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "PSNCureBerry");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Leftovers");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 3);

        item_pocket->remove("Potion", 30);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 2);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "PSNCureBerry");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Leftovers");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK(item_slots.at(2).item == none_entry);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 2);

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

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            key_item_pocket->add("Bicycle", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            key_item_pocket->add("Bicycle", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            key_item_pocket->remove("Bicycle", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            key_item_pocket->remove("Bicycle", 100)
        , std::out_of_range);

        // Make sure item slots start as correctly empty.
        const pkmn::item_slots_t& item_slots = key_item_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 26);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            key_item_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            key_item_pocket->add("Master Ball", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            key_item_pocket->add("HM01", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);

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
        key_item_pocket->add("Bicycle", 30);
        key_item_pocket->add("Basement Key", 99);
        key_item_pocket->add("SecretPotion", 1);

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Basement Key");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "SecretPotion");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 3);

        key_item_pocket->add("SecretPotion", 15);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Basement Key");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "SecretPotion");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 3);

        key_item_pocket->remove("Basement Key", 20);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Basement Key");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "SecretPotion");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 3);

        key_item_pocket->move(0, 1);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Basement Key");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "SecretPotion");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 3);

        key_item_pocket->remove("Bicycle", 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Basement Key");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "SecretPotion");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK(item_slots.at(2).item == none_entry);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 2);

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

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            ball_pocket->add("Master Ball", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->add("Master Ball", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->remove("Master Ball", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->remove("Master Ball", 100)
        , std::out_of_range);

        // Make sure item slots start as correctly empty.
        const pkmn::item_slots_t& item_slots = ball_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 12);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            ball_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            ball_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            ball_pocket->add("HM01", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 0);

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ball_pocket->add("Great Ball", 30);
        ball_pocket->add("Fast Ball", 99);
        ball_pocket->add("Park Ball", 1);

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Fast Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Park Ball");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 3);

        ball_pocket->add("Park Ball", 15);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Fast Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Park Ball");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 3);

        ball_pocket->remove("Fast Ball", 20);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Fast Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Park Ball");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 3);

        ball_pocket->move(0, 1);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Fast Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Park Ball");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 3);

        ball_pocket->remove("Great Ball", 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Fast Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Park Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK(item_slots.at(2).item == none_entry);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 2);

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

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            tmhm_pocket->add("TM10", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("TM10", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->remove("TM10", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->remove("TM10", 100)
        , std::out_of_range);

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

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Master Ball", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), 0);

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
        BOOST_CHECK_THROW(
            pc->add("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            pc->add("Potion", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            pc->remove("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            pc->remove("Potion", 100)
        , std::out_of_range);

        // Make sure item slots start as correctly empty.
        const pkmn::item_slots_t& item_slots = pc->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 50);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Make sure we can add stuff from every pocket.
        pc->add("Potion", 5);
        pc->add("Bicycle", 1);
        pc->add("Great Ball", 10);
        pc->add("TM28", 8);

        BOOST_CHECK_EQUAL(pc->get_num_items(), 4);
        BOOST_CHECK_EQUAL(item_slots[0].item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots[0].amount, 5);
        BOOST_CHECK_EQUAL(item_slots[1].item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots[1].amount, 1);
        BOOST_CHECK_EQUAL(item_slots[2].item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots[2].amount, 10);
        BOOST_CHECK_EQUAL(item_slots[3].item.get_name(), "TM28");
        BOOST_CHECK_EQUAL(item_slots[3].amount, 8);

        pc->remove("Great Ball", 10);

        BOOST_CHECK_EQUAL(pc->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots[0].item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots[0].amount, 5);
        BOOST_CHECK_EQUAL(item_slots[1].item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots[1].amount, 1);
        BOOST_CHECK_EQUAL(item_slots[2].item.get_name(), "TM28");
        BOOST_CHECK_EQUAL(item_slots[2].amount, 8);

        pc->move(1, 2);

        BOOST_CHECK_EQUAL(pc->get_num_items(), 3);
        BOOST_CHECK_EQUAL(item_slots[0].item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots[0].amount, 5);
        BOOST_CHECK_EQUAL(item_slots[1].item.get_name(), "TM28");
        BOOST_CHECK_EQUAL(item_slots[1].amount, 8);
        BOOST_CHECK_EQUAL(item_slots[2].item.get_name(), "Bicycle");
        BOOST_CHECK_EQUAL(item_slots[2].amount, 1);

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
    }

}
