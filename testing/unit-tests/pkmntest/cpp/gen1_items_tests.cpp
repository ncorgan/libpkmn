/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_items_tests.hpp>
#include <pkmntest/items_tests_common.hpp>

#include <pkmn/database/lists.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>

#include <map>

namespace pkmntest {

    static const std::map<std::string, pkmn::database::item_entry> none_entries = boost::assign::map_list_of
        ("Red",    pkmn::database::item_entry("None", "Red"))
        ("Blue",  pkmn::database::item_entry("None", "Blue"))
        ("Yellow", pkmn::database::item_entry("None", "Yellow"))
    ;

    static void gen1_item_list_common(
        pkmn::item_list::sptr list,
        const std::string &game
    ) {
        // Make sure item slots start as correctly empty.
        test_item_list_empty_slots(
            list,
            none_entries.at(game)
        );

        // Confirm exceptions are thrown when expected.
        test_item_list_out_of_range_error(
            list,
            "Potion"
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        test_item_list_add_remove(
            list,
            none_entries.at(game),
            "Potion",
            "Great Ball",
            "Ether"
        );

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
