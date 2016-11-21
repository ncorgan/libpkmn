/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen2_items_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <iostream>

namespace pkmntest {

    void gen2_item_pocket_test(
        pkmn::item_list::sptr item_pocket,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(item_pocket->get_name(), "Items");
        BOOST_CHECK_EQUAL(item_pocket->get_game(), game);
    }

    void gen2_key_item_pocket_test(
        pkmn::item_list::sptr key_item_pocket,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(key_item_pocket->get_name(), "KeyItems");
        BOOST_CHECK_EQUAL(key_item_pocket->get_game(), game);

        if(game == "Crystal") {
            key_item_pocket->add("GS Ball", 1);
        } else {
            BOOST_CHECK_THROW(
                key_item_pocket->add("GS Ball", 1);
            , std::invalid_argument);
        }
    }

    void gen2_ball_pocket_test(
        pkmn::item_list::sptr ball_pocket,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(ball_pocket->get_name(), "Balls");
        BOOST_CHECK_EQUAL(ball_pocket->get_game(), game);
    }

    void gen2_tmhm_pocket_test(
        pkmn::item_list::sptr tmhm_pocket,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(tmhm_pocket->get_name(), "TM/HM");
        BOOST_CHECK_EQUAL(tmhm_pocket->get_game(), game);

        const pkmn::item_slots_t& item_slots = tmhm_pocket->as_vector();

        for(int i = 1; i <= 50; ++i) {
            char name[5] = "";
            std::snprintf(name, sizeof(name), "TM%02d", i);
            tmhm_pocket->add(name, 50);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i);
            BOOST_CHECK_EQUAL(item_slots[i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[i-1].amount, 50);
        }
        for(int i = 50; i >= 1; --i) {
            char name[5];
            std::snprintf(name, sizeof(name), "TM%02d", i);
            tmhm_pocket->remove(name, 50);
            BOOST_CHECK_EQUAL(tmhm_pocket->get_num_items(), i-1);
            BOOST_CHECK_EQUAL(item_slots[i-1].item.get_name(), name);
            BOOST_CHECK_EQUAL(item_slots[i-1].amount, 0);
        }

        for(int i = 1; i <= 7; ++i) {
            char name[5];
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
    }

    void gen2_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), game);
    }

    void gen2_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(bag->get_game(), game);

        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 4);
        gen2_item_pocket_test(pockets.at("Items"), game);
        gen2_key_item_pocket_test(pockets.at("KeyItems"), game);
        gen2_ball_pocket_test(pockets.at("Balls"), game);
        gen2_tmhm_pocket_test(pockets.at("TM/HM"), game);
    }

}
