/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gba_pokemon_tests.hpp>
#include "pokemon_tests_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gba/pokemon.h>
#include <pksav/math/endian.h>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

static const std::map<std::string, pkmn::move_slot> none_move_slots = boost::assign::map_list_of
    ("Ruby", pkmn::move_slot(
                 pkmn::database::move_entry("None", "Ruby"),
                 0
             ))
    ("Sapphire", pkmn::move_slot(
                     pkmn::database::move_entry("None", "Sapphire"),
                     0
                 ))
    ("Emerald", pkmn::move_slot(
                    pkmn::database::move_entry("None", "Emerald"),
                    0
                ))
    ("FireRed", pkmn::move_slot(
                    pkmn::database::move_entry("None", "FireRed"),
                    0
                ))
    ("LeafGreen", pkmn::move_slot(
                      pkmn::database::move_entry("None", "LeafGreen"),
                      0
                  ))
;

static const std::map<std::string, pkmn::database::pokemon_entry> none_pokemon_entries = boost::assign::map_list_of
    ("Ruby", pkmn::database::pokemon_entry("None", "Ruby", ""))
    ("Sapphire", pkmn::database::pokemon_entry("None", "Sapphire", ""))
    ("Emerald", pkmn::database::pokemon_entry("None", "Emerald", ""))
    ("FireRed", pkmn::database::pokemon_entry("None", "FireRed", ""))
    ("LeafGreen", pkmn::database::pokemon_entry("None", "LeafGreen", ""))
;

namespace pkmntest {

    void gba_invalid_pokemon_test(
        const std::string &game
    ) {
        pkmntest::test_invalid_pokemon(game);
    }

    static void check_markings_map(
        const std::map<std::string, bool>& markings_map
    ) {
        BOOST_CHECK_EQUAL(markings_map.count("Circle"), 1);
        BOOST_CHECK(not markings_map.at("Circle"));
        BOOST_CHECK_EQUAL(markings_map.count("Triangle"), 1);
        BOOST_CHECK(not markings_map.at("Triangle"));
        BOOST_CHECK_EQUAL(markings_map.count("Square"), 1);
        BOOST_CHECK(not markings_map.at("Square"));
        BOOST_CHECK_EQUAL(markings_map.count("Heart"), 1);
        BOOST_CHECK(not markings_map.at("Heart"));

        BOOST_CHECK_EQUAL(markings_map.count("Star"), 0);
        BOOST_CHECK_EQUAL(markings_map.count("Diamond"), 0);
    }

    static void check_contest_stats_map(
        const std::map<std::string, int>& contest_stats_map
    ) {
        BOOST_CHECK_EQUAL(contest_stats_map.count("Cool"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Cool"), 0);
        BOOST_CHECK_EQUAL(contest_stats_map.count("Beauty"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Beauty"), 0);
        BOOST_CHECK_EQUAL(contest_stats_map.count("Cute"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Cute"), 0);
        BOOST_CHECK_EQUAL(contest_stats_map.count("Smart"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Smart"), 0);
        BOOST_CHECK_EQUAL(contest_stats_map.count("Tough"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Tough"), 0);
        BOOST_CHECK_EQUAL(contest_stats_map.count("Feel"), 1);
        BOOST_CHECK_EQUAL(contest_stats_map.at("Feel"), 0);

        BOOST_CHECK_EQUAL(contest_stats_map.count("Sheen"), 0);
    }

    static void check_stats_map(
        const std::map<std::string, int>& stats_map
    ) {
        BOOST_CHECK_EQUAL(stats_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stats_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stats_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stats_map.count("Speed"), 1);
        BOOST_CHECK_EQUAL(stats_map.count("Special"), 0);
        BOOST_CHECK_EQUAL(stats_map.count("Special Attack"), 1);
        BOOST_CHECK_EQUAL(stats_map.count("Special Defense"), 1);
    }

    static void test_markings(
        pkmn::pokemon::sptr pokemon,
        const std::string &field
    ) {
        std::map<std::string, bool> markings_before = pokemon->get_markings();
        pokemon->set_marking(field, true);
        const std::map<std::string, bool>& markings_after = pokemon->get_markings();
        for(auto markings_iter = markings_after.begin();
            markings_iter != markings_after.end();
            ++markings_iter)
        {
            if(markings_iter->first == field) {
                BOOST_CHECK(markings_iter->second);
            } else {
                BOOST_CHECK_EQUAL(markings_iter->second, markings_before.at(markings_iter->first));
            }
        }
    }

    static void test_contest_fields(
        pkmn::pokemon::sptr pokemon,
        const std::string &field
    ) {
        std::map<std::string, int> contest_stats_before = pokemon->get_contest_stats();
        pokemon->set_contest_stat(field, 123);
        const std::map<std::string, int>& contest_stats_after = pokemon->get_contest_stats();
        for(auto contest_stats_iter = contest_stats_after.begin();
            contest_stats_iter != contest_stats_after.end();
            ++contest_stats_iter)
        {
            if(contest_stats_iter->first == field) {
                BOOST_CHECK_EQUAL(contest_stats_iter->second, 123);
            } else {
                BOOST_CHECK_EQUAL(contest_stats_iter->second, contest_stats_before.at(contest_stats_iter->first));
            }
        }
    }

    static void test_IVs(
        pkmn::pokemon::sptr pokemon,
        const std::string &field
    ) {
        std::map<std::string, int> IVs_before = pokemon->get_IVs();
        pokemon->set_IV(field, 10);
        const std::map<std::string, int>& IVs_after = pokemon->get_IVs();
        for(auto IVs_iter = IVs_after.begin();
            IVs_iter != IVs_after.end();
            ++IVs_iter)
        {
            if(IVs_iter->first == field) {
                BOOST_CHECK_EQUAL(IVs_iter->second, 10);
            } else {
                BOOST_CHECK_EQUAL(IVs_iter->second, IVs_before.at(IVs_iter->first));
            }
        }
    }

    void gba_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        /*
         * Check known starting values, and confirm that we can't query values
         * that didn't exist in Generation III.
         */
        
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
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_name(),
            std::string(pkmn::pokemon::LIBPKMN_OT_NAME)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            uint16_t(pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_secret_id(),
            uint16_t((pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF0000) >> 16)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Male"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_friendship(),
            pokemon->get_database_entry().get_base_friendship()
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_ability(),
            "Blaze"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_ball(),
            "Premier Ball"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            pokemon->get_level()
        );
        BOOST_CHECK_THROW(
            pokemon->get_location_met(true)
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            "Fateful encounter"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_original_game(),
            pokemon->get_game()
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_experience(),
            pokemon->get_database_entry().get_experience_at_level(30)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_level(), 
            30
        );

        check_markings_map(
            pokemon->get_markings()
        );
        // TODO: ribbons
        check_contest_stats_map(
            pokemon->get_contest_stats()
        );

        const pkmn::move_slots_t& move_slots = pokemon->get_moves();
        BOOST_CHECK_EQUAL(move_slots.size(), 4);
        for(auto iter = move_slots.begin(); iter != move_slots.end(); ++iter) {
            BOOST_CHECK((*iter) == none_move_slots.at(game));
        }

        check_stats_map(
            pokemon->get_EVs()
        );
        check_stats_map(
            pokemon->get_IVs()
        );
        check_stats_map(
            pokemon->get_stats()
        );

        /*
         * Make sure the getters and setters agree. Also make sure it fails when
         * expected.
         */

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

        // Setting shininess should affect personality.
        pokemon->set_shininess(false);
        BOOST_CHECK(not pokemon->is_shiny());
        uint32_t personality = pokemon->get_personality();
        pokemon->set_shininess(true);
        BOOST_CHECK(pokemon->is_shiny());
        BOOST_CHECK_NE(personality, pokemon->get_personality());

        BOOST_CHECK_THROW(
            pokemon->set_held_item("Not an item");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        BOOST_CHECK_THROW(
            pokemon->set_held_item("Berry");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        BOOST_CHECK_THROW(
            pokemon->set_held_item("Mach Bike");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        pokemon->set_held_item("Razz Berry");
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "Razz Berry"
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

        pokemon->set_trainer_id(0x1234ABCD);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            0x1234ABCD
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            0xABCD
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_secret_id(),
            0x1234
        );

        pokemon->set_trainer_public_id(0x1A2B);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            0x12341A2B
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            0x1A2B
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_secret_id(),
            0x1234
        );

        pokemon->set_trainer_secret_id(0x3C4D);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            0x3C4D1A2B
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_public_id(),
            0x1A2B
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_secret_id(),
            0x3C4D
        );

        pokemon->set_trainer_gender("Male");
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Male"
        );
        pokemon->set_trainer_gender("Female");
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Female"
        );
        BOOST_CHECK_THROW(
            pokemon->set_trainer_gender("Genderless");
        , std::invalid_argument);

        pokemon->set_friendship(123);
        BOOST_CHECK_EQUAL(
            pokemon->get_friendship(),
            123
        );
        BOOST_CHECK_THROW(
            pokemon->set_friendship(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_friendship(256);
        , pkmn::range_error);

        pokemon->set_ability("Blaze");
        BOOST_CHECK_EQUAL(
            pokemon->get_ability(),
            "Blaze"
        );
        BOOST_CHECK_THROW(
            pokemon->set_ability("None");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_ability("Torrent"); // Invalid
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_ability("Speed Boost"); // Hidden ability
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_ability(),
            "Blaze"
        );

        pokemon->set_ball("Great Ball");
        BOOST_CHECK_EQUAL(
            pokemon->get_ball(),
            "Great Ball"
        );
        BOOST_CHECK_THROW(
            pokemon->set_ball("Friend Ball"); // Not in Generation III
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_ball(),
            "Great Ball"
        );

        pokemon->set_level_met(67);
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            67
        );
        BOOST_CHECK_THROW(
            pokemon->set_level_met(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_level_met(101);
        , pkmn::range_error);

        std::string location;
        if(game == "FireRed" or game == "LeafGreen") {
            location = "Viridian Forest";
        } else {
            location = "Petalburg Woods";
        }
        pokemon->set_location_met(location, false);
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            location
        );
        BOOST_CHECK_THROW(
            pokemon->set_location_met(location, true);
        , pkmn::feature_not_in_game_error);

        pokemon->set_personality(0x7F3AB3A8);
        BOOST_CHECK_EQUAL(
            pokemon->get_personality(),
            0x7F3AB3A8
        );

        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Cool", -1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Cool", 256);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Not a stat", -1);
        , std::invalid_argument);

        // Make sure functions that affect the same PKSav field don't impact each other.
        static const std::vector<std::string> markings = boost::assign::list_of
            ("Circle")("Triangle")("Square")("Heart")
        ;
        for(auto marking_iter = markings.begin();
            marking_iter != markings.end();
            ++marking_iter)
        {
            test_markings(pokemon, *marking_iter);
        }

        static const std::vector<std::string> contest_fields = boost::assign::list_of
            ("Cool")("Beauty")("Cute")("Smart")("Tough")("Feel")
        ;
        for(auto contest_field_iter = contest_fields.begin();
            contest_field_iter != contest_fields.end();
            ++contest_field_iter)
        {
            test_contest_fields(pokemon, *contest_field_iter);
        }
        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Not a stat", 10);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Cool", -1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_contest_stat("Cool", 256);
        , pkmn::range_error);

        pokemon->set_EV("Attack", 100);
        BOOST_CHECK_EQUAL(
            pokemon->get_EVs().at("Attack"),
            100
        );
        BOOST_CHECK_THROW(
            pokemon->set_EV("Not a stat", 100);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_EV("Attack", -1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_EV("Attack", 256);
        , pkmn::range_error);

        static const std::vector<std::string> IV_fields = boost::assign::list_of
            ("HP")("Attack")("Defense")("Speed")("Special Attack")("Special Defense")
        ;
        for(auto IV_field_iter = IV_fields.begin();
            IV_field_iter != IV_fields.end();
            ++IV_field_iter)
        {
            test_IVs(pokemon, *IV_field_iter);
        }
        BOOST_CHECK_THROW(
            pokemon->set_IV("Not a stat", 10);
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_IV("Attack", -1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_IV("Attack", 32);
        , pkmn::range_error);
    }

}
