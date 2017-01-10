/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_pc_tests.hpp> 

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include "pkmn_boost_unit_test.hpp"

namespace pkmntest {

    void test_empty_pokemon_box(
        pkmn::pokemon_box::sptr box,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(box->get_game(), game);

        const pkmn::pokemon_list_t& pokemon_list = box->as_vector();
        BOOST_CHECK_EQUAL(box->get_capacity(), pokemon_list.size());

        for(int i = 0; i < box->get_capacity(); ++i) {
            pkmn::pokemon::sptr entry = pokemon_list.at(i);
            BOOST_CHECK_EQUAL(entry->get_species(), "None");
            BOOST_CHECK_EQUAL(entry->get_game(), game);

            const pkmn::move_slots_t& move_slots = entry->get_moves();
            BOOST_REQUIRE_EQUAL(move_slots.size(), 4);
            for(int j = 0; j < 4; ++j) {
                BOOST_CHECK_EQUAL(move_slots.at(j).move.get_name(), "None");
                BOOST_CHECK_EQUAL(move_slots.at(j).move.get_game(), game);
                BOOST_CHECK_EQUAL(move_slots.at(j).pp, 0);
            }
        }
    }

    void test_empty_pokemon_pc(
        pkmn::pokemon_pc::sptr pc,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(pc->get_game(), game);

        const pkmn::pokemon_box_list_t& pokemon_box_list = pc->as_vector();
        BOOST_CHECK_EQUAL(pc->get_num_boxes(), pokemon_box_list.size());

        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            test_empty_pokemon_box(
                pokemon_box_list.at(i),
                game
            );
        }
    }
}
