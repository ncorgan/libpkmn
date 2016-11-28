/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gba_items_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>

#include <map>

namespace pkmntest {

    static const std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Ruby",    pkmn::database::item_entry("None", "Ruby"))
        ("Sapphire",  pkmn::database::item_entry("None", "Sapphire"))
        ("Emerald", pkmn::database::item_entry("None", "Emerald"))
        ("FireRed", pkmn::database::item_entry("None", "FireRed"))
        ("LeafGreen", pkmn::database::item_entry("None", "LeafGreen"))
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
        const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), capacity);
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
        BOOST_CHECK_THROW(
            item_pocket->add("Razz Berry", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 0);
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
        const pkmn::item_slots_t& item_slots = key_item_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), capacity);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

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
        BOOST_CHECK_THROW(
            key_item_pocket->add("Razz Berry", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);
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
        const pkmn::item_slots_t& item_slots = ball_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), capacity);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            ball_pocket->add("Great Ball", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->add("Great Ball", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->remove("Great Ball", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            ball_pocket->remove("Great Ball", 100)
        , std::out_of_range);

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            ball_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            ball_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            ball_pocket->add("HM01", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            ball_pocket->add("Razz Berry", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 0);
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
        const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), capacity);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            tmhm_pocket->add("TM50", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("TM50", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->remove("TM50", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            tmhm_pocket->remove("TM50", 100)
        , std::out_of_range);

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Master Ball", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            tmhm_pocket->add("Razz Berry", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), 0);
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
        const pkmn::item_slots_t& item_slots = berry_pocket->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), capacity);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            berry_pocket->add("Razz Berry", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            berry_pocket->add("Razz Berry", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            berry_pocket->remove("Razz Berry", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            berry_pocket->remove("Razz Berry", 100)
        , std::out_of_range);

        // Make sure we can't add items from other pockets.
        BOOST_CHECK_THROW(
            berry_pocket->add("Bicycle", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            berry_pocket->add("Master Ball", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            berry_pocket->add("HM01", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            berry_pocket->add("Potion", 1);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(berry_pocket->get_num_items(), 0);
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
        const pkmn::item_slots_t& item_slots = pc->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), 50);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

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

        // Make sure we can add items from every pocket.
        pc->add("Bicycle", 1);
        pc->add("Master Ball", 1);
        pc->add("HM01", 1);
        pc->add("Razz Berry", 1);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 4);

        pc->remove("Bicycle", 1);
        pc->remove("Master Ball", 1);
        pc->remove("HM01", 1);
        pc->remove("Razz Berry", 1);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);
    }

    void gba_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(bag->get_game(), game);

        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 5);

        gba_item_pocket_test(pockets.at("Items"), game);
        gba_key_item_pocket_test(pockets.at("Key Items"), game);
        gba_ball_pocket_test(pockets.at("Poké Balls"), game);
        if(game == "FireRed" or game == "LeafGreen") {
            gba_tmhm_pocket_test(pockets.at("TM Case"), game);
            gba_berry_pocket_test(pockets.at("Berry Pouch"), game);
        } else {
            gba_tmhm_pocket_test(pockets.at("TMs & HMs"), game);
            gba_berry_pocket_test(pockets.at("Berries"), game);
        }
    }

}
