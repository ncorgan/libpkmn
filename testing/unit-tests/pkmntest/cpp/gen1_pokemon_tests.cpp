/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

static const std::map<std::string, pkmn::move_slot> none_move_slots = boost::assign::map_list_of
    ("Red", pkmn::move_slot(
                pkmn::database::move_entry("None", "Red"),
                0
            ))
    ("Blue", pkmn::move_slot(
                 pkmn::database::move_entry("None", "Blue"),
                 0
             ))
    ("Yellow", pkmn::move_slot(
                   pkmn::database::move_entry("None", "Yellow"),
                   0
               ))
;

static const std::map<std::string, pkmn::database::pokemon_entry> none_pokemon_entries = boost::assign::map_list_of
    ("Red", pkmn::database::pokemon_entry("None", "Red", ""))
    ("Blue", pkmn::database::pokemon_entry("None", "Blue", ""))
    ("Yellow", pkmn::database::pokemon_entry("None", "Yellow", ""))
;

namespace pkmntest {

    static void gen1_pokemon_check_stat_map(
        const std::map<std::string, int>& stat_map
    ) {
        BOOST_CHECK_EQUAL(stat_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Speed"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special Attack"), 0);
        BOOST_CHECK_EQUAL(stat_map.count("Special Defense"), 0);
    }

    void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        // Check starting values
        BOOST_CHECK_EQUAL(
            pokemon->get_species(),
            species
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_form(),
            "Standard"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_game(),
            game
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_nickname(),
            boost::algorithm::to_upper_copy(species)
        );
        BOOST_CHECK_THROW(
            pokemon->get_held_item();
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_name(),
            std::string(pkmn::pokemon::LIBPKMN_OT_NAME)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            uint16_t(pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF)
        );
        BOOST_CHECK_THROW(
            pokemon->get_trainer_secret_id();
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            uint16_t(pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Male"
        );
        BOOST_CHECK_THROW(
            pokemon->get_ability()
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->get_ball()
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->get_location_caught();
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->get_original_game();
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->get_personality();
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_experience(),
            pokemon->get_database_entry().get_experience_at_level(30)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_level(), 
            30
        );
        BOOST_CHECK_THROW(
            pokemon->get_markings();
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->get_ribbons();
        , std::runtime_error);

        const pkmn::move_slots_t& move_slots = pokemon->get_moves();
        BOOST_CHECK_EQUAL(move_slots.size(), 4);
        for(auto iter = move_slots.begin(); iter != move_slots.end(); ++iter) {
            BOOST_CHECK((*iter) == none_move_slots.at(game));
        }

        gen1_pokemon_check_stat_map(
            pokemon->get_EVs()
        );
        gen1_pokemon_check_stat_map(
            pokemon->get_IVs()
        );
        gen1_pokemon_check_stat_map(
            pokemon->get_stats()
        );

        // Set values. Make sure it works or fails when expected.

        BOOST_CHECK_THROW(
            pokemon->set_nickname(""),
        std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_nickname("Too long nickname"),
        std::invalid_argument);
        pokemon->set_nickname("foobarbaz");
        BOOST_CHECK_EQUAL(
            pokemon->get_nickname(),
            "foobarbaz"
        );

        BOOST_CHECK_THROW(
            pokemon->set_trainer_name(""),
        std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_trainer_name("Too long trainer name"),
        std::invalid_argument);
        pokemon->set_trainer_name("foobar");
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_name(),
            "foobar"
        );

        pokemon->set_trainer_id(10001);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            10001
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            10001
        );

        BOOST_CHECK_THROW(
            pokemon->set_trainer_secret_id(54321);
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            10001
        );

        BOOST_CHECK_THROW(
            pokemon->set_ability("");
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->set_ball("Great Ball");
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->set_location_caught("Victory Road");
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->set_original_game("Blue");
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->set_marking("Circle", true);
        , std::runtime_error);
        BOOST_CHECK_THROW(
            pokemon->set_ribbon("Cool", false);
        , std::runtime_error);

        pokemon->set_experience(12345);
        BOOST_CHECK_EQUAL(
            pokemon->get_experience(),
            12345
        );

        pokemon->set_level(45);
        BOOST_CHECK_EQUAL(
            pokemon->get_level(),
            45
        );

        BOOST_CHECK_THROW(
            pokemon->set_move("Ember", -1)
        , std::out_of_range);
        BOOST_CHECK_THROW(
            pokemon->set_move("Synthesis", 0)
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            move_slots.at(0).move.get_name(),
            "None"
        );

        pokemon->set_move("Ember", 0);
        pokemon->set_move("Flamethrower", 1);
        pokemon->set_move("Slash", 2);
        pokemon->set_move("Fire Blast", 3);

        BOOST_CHECK_EQUAL(
            move_slots.at(0).move.get_name(),
            "Ember"
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(0).pp,
            move_slots.at(0).move.get_pp(0)
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(1).move.get_name(),
            "Flamethrower"
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(1).pp,
            move_slots.at(1).move.get_pp(0)
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(2).move.get_name(),
            "Slash"
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(2).pp,
            move_slots.at(2).move.get_pp(0)
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(3).move.get_name(),
            "Fire Blast"
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(3).pp,
            move_slots.at(3).move.get_pp(0)
        );

        BOOST_CHECK_THROW(
            pokemon->set_EV("Not a stat", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_EV("Attack", 65536);
        , std::out_of_range);
        pokemon->set_EV("Attack", 12345);
        BOOST_CHECK_EQUAL(
            pokemon->get_EVs().at("Attack"),
            12345
        );

        BOOST_CHECK_THROW(
            pokemon->set_IV("Not a stat", 1);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_IV("Attack", 16);
        , std::out_of_range);
        pokemon->set_IV("Attack", 12);
        BOOST_CHECK_EQUAL(
            pokemon->get_IVs().at("Attack"),
            12
        );
    }

    void gen1_pokemon_box_test(
        pkmn::pokemon_box::sptr pokemon_box,
        const std::string &game
    ) {
        // Check starting values
        BOOST_CHECK_THROW(
            pokemon_box->get_name()
        , std::runtime_error);
        BOOST_CHECK_EQUAL(
            pokemon_box->get_game(),
            game
        );
        BOOST_CHECK_EQUAL(
            pokemon_box->get_capacity(),
            20
        );

        const pkmn::pokemon_list_t& as_vector = pokemon_box->as_vector();
        BOOST_REQUIRE_EQUAL(as_vector.size(), 20);
        for(auto iter = as_vector.begin(); iter != as_vector.end(); ++iter) {
            BOOST_CHECK((*iter)->get_database_entry() == none_pokemon_entries.at(game));
            const pkmn::move_slots_t& move_slots = (*iter)->get_moves();
            BOOST_CHECK_EQUAL(move_slots.size(), 4);
            for(auto iter = move_slots.begin(); iter != move_slots.end(); ++iter) {
                BOOST_CHECK((*iter) == none_move_slots.at(game));
            }
        }
    }

    void gen1_pokemon_pc_test(
        pkmn::pokemon_pc::sptr pokemon_pc,
        const std::string &game
    ) {
        // Check starting values
        BOOST_CHECK_EQUAL(
            pokemon_pc->get_game(),
            game
        );
        BOOST_CHECK_EQUAL(
            pokemon_pc->get_num_boxes(),
            12
        );
        BOOST_CHECK_EQUAL(
            pokemon_pc->get_box_names().size(),
            12
        );

        const pkmn::pokemon_box_list_t& as_vector = pokemon_pc->as_vector();
        BOOST_REQUIRE_EQUAL(as_vector.size(), 12);
        for(auto iter = as_vector.begin(); iter != as_vector.end(); ++iter) {
            gen1_pokemon_box_test((*iter), game);
        }
    }
}
