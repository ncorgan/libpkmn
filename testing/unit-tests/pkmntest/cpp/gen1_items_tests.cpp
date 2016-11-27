/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>

#include <pkmn/database/lists.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>

#include <map>

namespace pkmntest {

    static std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Red",    pkmn::database::item_entry("None", "Red"))
        ("Blue",  pkmn::database::item_entry("None", "Blue"))
        ("Yellow", pkmn::database::item_entry("None", "Yellow"))
    ;

    static void gen1_item_list_common(
        pkmn::item_list::sptr list,
        const std::string &game
    ) {

        // Make sure item slots start as correctly empty.
        const pkmn::item_slots_t& item_slots = list->as_vector();
        BOOST_REQUIRE_EQUAL(item_slots.size(), list->get_capacity());
        const pkmn::database::item_entry& none_entry = none_entries.at(game);
        for(auto iter = item_slots.begin(); iter != item_slots.end(); ++iter) {
            BOOST_CHECK(iter->item == none_entry);
            BOOST_CHECK_EQUAL(iter->amount, 0);
        }

        // Confirm exceptions are thrown when expected.
        BOOST_CHECK_THROW(
            list->add("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->add("Potion", 100)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove("Potion", 0)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            list->remove("Potion", 100)
        , std::out_of_range);

        // Start adding and removing stuff, and make sure the numbers are accurate.
        list->add("Potion", 30);
        list->add("Great Ball", 99);
        list->add("Ether", 1);

        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 1);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->add("Ether", 15);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 99);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove("Great Ball", 20);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->move(0, 1);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Potion");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 30);
        BOOST_CHECK_EQUAL(item_slots.at(2).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 16);
        BOOST_CHECK_EQUAL(list->get_num_items(), 3);

        list->remove("Potion", 30);
        BOOST_CHECK_EQUAL(item_slots.at(0).item.get_name(), "Great Ball");
        BOOST_CHECK_EQUAL(item_slots.at(0).amount, 79);
        BOOST_CHECK_EQUAL(item_slots.at(1).item.get_name(), "Ether");
        BOOST_CHECK_EQUAL(item_slots.at(1).amount, 16);
        BOOST_CHECK(item_slots.at(2).item == none_entry);
        BOOST_CHECK_EQUAL(item_slots.at(2).amount, 0);
        BOOST_CHECK_EQUAL(list->get_num_items(), 2);

        const std::vector<std::string>& valid_items = list->get_valid_items();
        std::vector<std::string> full_item_list = pkmn::database::get_item_list(game);
        BOOST_CHECK_EQUAL(valid_items.size(), full_item_list.size());
    }

    void gen1_item_list_test(
        pkmn::item_list::sptr items,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(items->get_name(), "Items");
        BOOST_CHECK_EQUAL(items->get_game(), game);
        BOOST_CHECK_EQUAL(items->get_capacity(), 20);
        BOOST_CHECK_EQUAL(items->get_num_items(), 0);

        gen1_item_list_common(items, game);
    }

    void gen1_pc_test(
        pkmn::item_list::sptr pc,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(pc->get_name(), "PC");
        BOOST_CHECK_EQUAL(pc->get_game(), game);
        BOOST_CHECK_EQUAL(pc->get_capacity(), 50);
        BOOST_CHECK_EQUAL(pc->get_num_items(), 0);

        gen1_item_list_common(pc, game);
    }

    void gen1_item_bag_test(
        pkmn::item_bag::sptr bag,
        const std::string &game
    ) {
        // Check unchanging and initial values.
        BOOST_CHECK_EQUAL(bag->get_game(), game);

        const pkmn::item_pockets_t& pockets = bag->get_pockets();
        BOOST_CHECK_EQUAL(pockets.size(), 1);
        gen1_item_list_test(pockets.at("Items"), game);
    }

}
