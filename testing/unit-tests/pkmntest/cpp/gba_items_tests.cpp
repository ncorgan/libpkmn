/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gba_items_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/test/unit_test.hpp>

#include <map>

namespace pkmntest {

    static std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Ruby",    pkmn::database::item_entry("None", "Ruby"))
        ("Sapphire",  pkmn::database::item_entry("None", "Sapphire"))
        ("Emerald", pkmn::database::item_entry("None", "Emerald"))
        ("FireRed", pkmn::database::item_entry("None", "FireRed"))
        ("LeafGreen", pkmn::database::item_entry("None", "LeafGreen"))
    ;

    void gba_item_pocket_test(
        pkmn::item_list::sptr item_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(item_pocket->get_name(), "Items");
        BOOST_CHECK_EQUAL(item_pocket->get_game(), game);
        //BOOST_CHECK_EQUAL(item_pocket->get_capacity(), 20);
        BOOST_CHECK_EQUAL(item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        const pkmn::item_slots_t& item_slots = item_pocket->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 20);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
    }

    void gba_key_item_pocket_test(
        pkmn::item_list::sptr key_item_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(key_item_pocket->get_name(), "Key Items");
        BOOST_CHECK_EQUAL(key_item_pocket->get_game(), game);
        //BOOST_CHECK_EQUAL(key_item_pocket->get_capacity(), 26);
        BOOST_CHECK_EQUAL(key_item_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        const pkmn::item_slots_t& item_slots = key_item_pocket->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 26);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
    }

    void gba_ball_pocket_test(
        pkmn::item_list::sptr ball_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(ball_pocket->get_name(), "Poké Balls");
        BOOST_CHECK_EQUAL(ball_pocket->get_game(), game);
        //BOOST_CHECK_EQUAL(ball_pocket->get_capacity(), 12);
        BOOST_CHECK_EQUAL(ball_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        const pkmn::item_slots_t& item_slots = ball_pocket->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 12);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
    }

    void gba_tmhm_pocket_test(
        pkmn::item_list::sptr tmhm_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values
        //BOOST_CHECK_EQUAL(tmhm_pocket->get_name(), "TM/HM");
        BOOST_CHECK_EQUAL(tmhm_pocket->get_game(), game);
        //BOOST_CHECK_EQUAL(tmhm_pocket->get_capacity(), 57);
        BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        //const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 57);
    }

    void gba_berry_pocket_test(
        pkmn::item_list::sptr berry_pocket,
        const std::string &game
    ) {
        // Check unchanging and initial values
        //BOOST_CHECK_EQUAL(berry_pocket->get_name(), "Berries");
        BOOST_CHECK_EQUAL(berry_pocket->get_game(), game);
        //BOOST_CHECK_EQUAL(berry_pocket->get_capacity(), 12);
        BOOST_CHECK_EQUAL(berry_pocket->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        const pkmn::item_slots_t& item_slots = berry_pocket->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 12);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
    }

    void gba_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    ) {
        // Check unchanging and initial values
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), game);
        //BOOST_CHECK_EQUAL(pc->get_capacity(), 50);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);

        // Make sure item slots start as correctly empty
        const pkmn::item_slots_t& item_slots = pc->as_vector();
        //BOOST_REQUIRE_EQUAL(item_slots.size(), 50);
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }
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
            gba_tmhm_pocket_test(pockets.at("Berry Pouch"), game);
        } else {
            gba_tmhm_pocket_test(pockets.at("TMs & HMs"), game);
            gba_tmhm_pocket_test(pockets.at("Berries"), game);
        }
    }

}
