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

#include <boost/format.hpp>

#include <pkmn/exception.hpp>

namespace pkmntest {

    void test_empty_pokemon_box(
        pkmn::pokemon_box::sptr box,
        const std::string &game
    ) {
        BOOST_CHECK_EQUAL(box->get_game(), game);

        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                (void)box->get_name();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_EQUAL(
                box->get_name(),
                ""
            );
        }

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

    void test_box_name(
        pkmn::pokemon_box::sptr box
    ) {
        std::string game = box->get_game();
        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                box->set_name("ABCDEFGH");
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)box->get_name();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_THROW(
                box->set_name("ABCDEFGHI");
            , std::invalid_argument);

            box->set_name("ABCDEFGH");
            BOOST_CHECK_EQUAL(
                box->get_name(),
                "ABCDEFGH"
            );
        }
    }

    void test_setting_pokemon(
        pkmn::pokemon_box::sptr box
    ) {
        std::string game = box->get_game();

        pkmn::pokemon::sptr original_first = box->get_pokemon(0);
        pkmn::pokemon::sptr original_second = box->get_pokemon(1);

        // Make sure we can't move these
        BOOST_CHECK_THROW(
            box->set_pokemon(2, original_first);
        , std::runtime_error);
        BOOST_CHECK_THROW(
            box->set_pokemon(3, original_second);
        , std::runtime_error);

        /*
         * Create new Pokémon and place in box. The original variables should
         * have the same underlying Pokémon.
         */
        pkmn::pokemon::sptr bulbasaur = pkmn::pokemon::make(
                                            "Bulbasaur",
                                            game,
                                            "",
                                            5
                                        );
        pkmn::pokemon::sptr charmander = pkmn::pokemon::make(
                                             "Charmander",
                                             game,
                                             "",
                                             5
                                         );
        pkmn::pokemon::sptr squirtle = pkmn::pokemon::make(
                                           "Squirtle",
                                           game,
                                           "",
                                           5
                                       );

        box->set_pokemon(0, bulbasaur);
        box->set_pokemon(1, charmander);

        // Make sure we can't do that again.
        BOOST_CHECK_THROW(
            box->set_pokemon(2, bulbasaur);
        , std::runtime_error);
        BOOST_CHECK_THROW(
            box->set_pokemon(3, charmander);
        , std::runtime_error);

        // Replace one of the new ones.
        box->set_pokemon(0, squirtle);

        /*
         * Now check everything we've created. Each variable should have the
         * same Pokémon underneath, even if the pointer has changed.
         */
        BOOST_CHECK_EQUAL(box->get_pokemon(0)->get_species(), "Squirtle");
        BOOST_CHECK_EQUAL(box->get_pokemon(1)->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(original_first->get_species(), "None");
        BOOST_CHECK_EQUAL(original_second->get_species(), "None");
        BOOST_CHECK_EQUAL(bulbasaur->get_species(), "Bulbasaur");
        BOOST_CHECK_EQUAL(charmander->get_species(), "Charmander");
        BOOST_CHECK_EQUAL(squirtle->get_species(), "Squirtle");

        // On the C++ level, make sure the expected equal pointers are equal.
        BOOST_CHECK_EQUAL(box->get_pokemon(0)->get_native_pc_data(), squirtle->get_native_pc_data());
        BOOST_CHECK_EQUAL(box->get_pokemon(1)->get_native_pc_data(), charmander->get_native_pc_data());
        BOOST_CHECK_NE(box->get_pokemon(0)->get_native_pc_data(), original_first->get_native_pc_data());
        BOOST_CHECK_NE(box->get_pokemon(1)->get_native_pc_data(), original_second->get_native_pc_data());
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

    void test_pc_box_names(
        pkmn::pokemon_pc::sptr pc
    ) {
        std::string game = pc->get_game();
        if(game == "Red" or game == "Blue" or game == "Yellow") {
            BOOST_CHECK_THROW(
                pc->get_box(0)->set_name("ABCDEFGH");
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)pc->get_box_names();
            , pkmn::feature_not_in_game_error);
        } else {
            for(int i = 0; i < pc->get_num_boxes(); ++i) {
                std::string box_name = str(boost::format("BOX%d") % (i+1));
                pc->get_box(i)->set_name(box_name);
            }

            const std::vector<std::string>& box_names = pc->get_box_names();
            BOOST_REQUIRE_EQUAL(box_names.size(), pc->get_num_boxes());
        }
    }

    void test_setting_pokemon_in_boxes(
        pkmn::pokemon_pc::sptr pc
    ) {
        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            test_setting_pokemon(
                pc->get_box(i)
            );
        }
        for(int i = 0; i < pc->get_num_boxes(); ++i) {
            BOOST_CHECK_EQUAL(pc->get_box(i)->get_pokemon(0)->get_species(), "Squirtle");
            BOOST_CHECK_EQUAL(pc->get_box(i)->get_pokemon(1)->get_species(), "Charmander");
        }
    }
}
