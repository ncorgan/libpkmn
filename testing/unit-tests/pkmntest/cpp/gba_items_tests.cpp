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
        std::vector<std::string> wrong_items;
        wrong_items.emplace_back("Bicycle");
        wrong_items.emplace_back("Master Ball");
        wrong_items.emplace_back("HM01");
        wrong_items.emplace_back("Razz Berry");
        test_item_list_items_from_wrong_pocket(
            item_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("Potion");
        item_names.emplace_back("Orange Mail");
        item_names.emplace_back("Lava Cookie");
        item_names.emplace_back("Stardust");
        item_names.emplace_back("Shadow Mail");
        item_names.emplace_back("Pink Scarf");
        item_names.emplace_back("Antidote");
        item_names.emplace_back("Green Shard");
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
        std::vector<std::string> wrong_items;
        wrong_items.emplace_back("Potion");
        wrong_items.emplace_back("Master Ball");
        wrong_items.emplace_back("HM01");
        wrong_items.emplace_back("Razz Berry");
        test_item_list_items_from_wrong_pocket(
            key_item_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("Wailmer Pail");
        item_names.emplace_back("Basement Key");
        item_names.emplace_back("Meteorite");
        item_names.emplace_back("Old Rod");
        item_names.emplace_back("Red Orb");
        item_names.emplace_back("Root Fossil");
        item_names.emplace_back("Contest Pass");
        item_names.emplace_back("Eon Ticket");
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
        std::vector<std::string> wrong_items;
        wrong_items.emplace_back("Potion");
        wrong_items.emplace_back("Bicycle");
        wrong_items.emplace_back("HM01");
        wrong_items.emplace_back("Razz Berry");
        test_item_list_items_from_wrong_pocket(
            ball_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("Master Ball");
        item_names.emplace_back("Ultra Ball");
        item_names.emplace_back("Great Ball");
        item_names.emplace_back("Poké Ball");
        item_names.emplace_back("Safari Ball");
        item_names.emplace_back("Net Ball");
        item_names.emplace_back("Dive Ball");
        item_names.emplace_back("Nest Ball");
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
        std::vector<std::string> wrong_items;
        wrong_items.emplace_back("Potion");
        wrong_items.emplace_back("Bicycle");
        wrong_items.emplace_back("Great Ball");
        wrong_items.emplace_back("Razz Berry");
        test_item_list_items_from_wrong_pocket(
            tmhm_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("TM01");
        item_names.emplace_back("HM01");
        item_names.emplace_back("TM02");
        item_names.emplace_back("HM02");
        item_names.emplace_back("TM03");
        item_names.emplace_back("HM03");
        item_names.emplace_back("TM04");
        item_names.emplace_back("HM04");
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
        std::vector<std::string> wrong_items;
        wrong_items.emplace_back("Potion");
        wrong_items.emplace_back("Bicycle");
        wrong_items.emplace_back("Great Ball");
        wrong_items.emplace_back("HM02");
        test_item_list_items_from_wrong_pocket(
            berry_pocket,
            wrong_items
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("Cheri Berry");
        item_names.emplace_back("Razz Berry");
        item_names.emplace_back("Lum Berry");
        item_names.emplace_back("Pinap Berry");
        item_names.emplace_back("Aspear Berry");
        item_names.emplace_back("Iapapa Berry");
        item_names.emplace_back("Wiki Berry");
        item_names.emplace_back("Apicot Berry");
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

        // Start adding and removing stuff, and make sure the numbers are accurate.
        std::vector<std::string> item_names;
        item_names.emplace_back("Potion");
        item_names.emplace_back("Mach Bike");
        item_names.emplace_back("Great Ball");
        item_names.emplace_back("TM01");
        item_names.emplace_back("Aspear Berry");
        item_names.emplace_back("Wailmer Pail");
        item_names.emplace_back("Master Ball");
        item_names.emplace_back("HM04");
        test_item_list_add_remove(
            pc,
            none_entries.at(game),
            item_names
        );
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
