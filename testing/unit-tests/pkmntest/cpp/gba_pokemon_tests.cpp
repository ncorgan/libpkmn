/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gba_pokemon_tests.hpp>
#include <pkmntest/pokemon_tests_common.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gba/pokemon.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

static bool seeded = false;

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

static const std::vector<std::string> contest_types = boost::assign::list_of
    ("Cool")("Beauty")("Cute")("Smart")("Tough")
;

static const std::vector<std::string> contest_levels = boost::assign::list_of
    ("")(" Super")(" Hyper")(" Master")
;

static const std::vector<std::string> ribbons = boost::assign::list_of
    ("Champion")("Winning")("Victory")("Artist")
    ("Effort")("Marine")("Land")("Sky")
    ("Country")("National")("Earth")("World")
;

namespace pkmntest {

    void gba_unown_form_test(
        const std::string &game
    ) {
        pkmn::database::pokemon_entry unown_entry(
            "Unown",
            game,
            ""
        );
        std::vector<std::string> unown_forms = unown_entry.get_forms();

        for(auto form_iter = unown_forms.begin(); form_iter != unown_forms.end(); ++form_iter) {
            pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                            "Unown",
                                            game,
                                            *form_iter,
                                            5
                                        );
            BOOST_CHECK_EQUAL(
                unown->get_form(),
                *form_iter
            );
        }

        // Make sure setting the personality properly sets the form.
        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        "Unown",
                                        game,
                                        "A",
                                        5
                                    );
        unown->set_personality(0x4C07DE71);
        BOOST_CHECK_EQUAL(unown->get_form(), "B");

        // Make sure setting the form properly sets the personality.
        for(auto form_iter = unown_forms.begin(); form_iter != unown_forms.end(); ++form_iter) {
            unown->set_form(*form_iter);
            BOOST_CHECK_EQUAL(
                unown->get_form(),
                *form_iter
            );
            std::string form_from_personality = pkmn::calculations::gen3_unown_form(
                                                    unown->get_personality()
                                                );
            BOOST_CHECK_EQUAL(
                form_from_personality,
                *form_iter
            );
        }
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

    static void check_ribbons_map(
        const std::map<std::string, bool>& ribbons_map
    ) {
        for(auto contest_type_iter = contest_types.begin();
            contest_type_iter != contest_types.end();
            ++contest_type_iter)
        {
            for(auto contest_level_iter = contest_levels.begin();
                contest_level_iter != contest_levels.end();
                ++contest_level_iter)
            {
                std::string ribbon_name = str(boost::format("%s%s")
                                              % contest_type_iter->c_str()
                                              % contest_level_iter->c_str());
                BOOST_CHECK_EQUAL(ribbons_map.count(ribbon_name), 1);
                BOOST_CHECK(not ribbons_map.at(ribbon_name));
            }
        }

        for(auto ribbon_iter = ribbons.begin();
            ribbon_iter != ribbons.end();
            ++ribbon_iter)
        {
            BOOST_CHECK_EQUAL(ribbons_map.count(*ribbon_iter), 1);
            BOOST_CHECK(not ribbons_map.at(*ribbon_iter));
        }
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

    static void test_contest_ribbons(
        pkmn::pokemon::sptr pokemon,
        const std::string &contest_type
    ) {
        std::string ribbon_name = contest_type;
        std::string super_ribbon_name = contest_type + std::string(" Super");
        std::string hyper_ribbon_name = contest_type + std::string(" Hyper");
        std::string master_ribbon_name = contest_type + std::string(" Master");
        const std::map<std::string, bool>& ribbons = pokemon->get_ribbons();

        pokemon->set_ribbon(hyper_ribbon_name, true);
        BOOST_CHECK(ribbons.at(ribbon_name));
        BOOST_CHECK(ribbons.at(super_ribbon_name));
        BOOST_CHECK(ribbons.at(hyper_ribbon_name));
        BOOST_CHECK(not ribbons.at(master_ribbon_name));

        pokemon->set_ribbon(super_ribbon_name, false);
        BOOST_CHECK(ribbons.at(ribbon_name));
        BOOST_CHECK(not ribbons.at(super_ribbon_name));
        BOOST_CHECK(not ribbons.at(hyper_ribbon_name));
        BOOST_CHECK(not ribbons.at(master_ribbon_name));
    }

    static void test_ribbons(
        pkmn::pokemon::sptr pokemon,
        const std::string &ribbon
    ) {
        std::map<std::string, bool> ribbons_before = pokemon->get_ribbons();
        pokemon->set_ribbon(ribbon, true);
        const std::map<std::string, bool>& ribbons_after = pokemon->get_ribbons();
        for(auto ribbons_iter = ribbons_after.begin();
            ribbons_iter != ribbons_after.end();
            ++ribbons_iter)
        {
            if(ribbons_iter->first == ribbon) {
                BOOST_CHECK(ribbons_iter->second);
            } else {
                BOOST_CHECK_EQUAL(ribbons_iter->second, ribbons_before.at(ribbons_iter->first));
            }
        }
    }

    static void test_contest_stats(
        pkmn::pokemon::sptr pokemon,
        const std::string &field
    ) {
        int value = std::rand() % 255;

        std::map<std::string, int> contest_stats_before = pokemon->get_contest_stats();
        pokemon->set_contest_stat(field, value);
        const std::map<std::string, int>& contest_stats_after = pokemon->get_contest_stats();
        for(auto contest_stats_iter = contest_stats_after.begin();
            contest_stats_iter != contest_stats_after.end();
            ++contest_stats_iter)
        {
            if(contest_stats_iter->first == field) {
                BOOST_CHECK_EQUAL(contest_stats_iter->second, value);
            } else {
                BOOST_CHECK_EQUAL(contest_stats_iter->second, contest_stats_before.at(contest_stats_iter->first));
            }
        }
    }

    static void test_IVs(
        pkmn::pokemon::sptr pokemon,
        const std::string &field
    ) {
        int value = std::rand() % 31;

        std::map<std::string, int> IVs_before = pokemon->get_IVs();
        pokemon->set_IV(field, value);
        const std::map<std::string, int>& IVs_after = pokemon->get_IVs();
        for(auto IVs_iter = IVs_after.begin();
            IVs_iter != IVs_after.end();
            ++IVs_iter)
        {
            if(IVs_iter->first == field) {
                BOOST_CHECK_EQUAL(IVs_iter->second, value);
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
        if(not seeded) {
            std::srand((unsigned int)std::time(NULL));
            seeded = true;
        }

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
            pkmn::pokemon::LIBPKMN_OT_NAME
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
            pokemon->get_trainer_id(),
            pkmn::pokemon::LIBPKMN_OT_ID
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
        check_ribbons_map(
            pokemon->get_ribbons()
        );
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

        pokemon->set_original_game("Ruby");
        BOOST_CHECK_EQUAL(
            pokemon->get_original_game(),
            "Ruby"
        );

        // Make sure we can't set invalid games.
        BOOST_CHECK_THROW(
            pokemon->set_original_game("Not a game");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_original_game("Red"); // Impossible
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            pokemon->set_original_game("HeartGold"); // From a later game
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_original_game(),
            "Ruby"
        );

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
        for(auto contest_type_iter = contest_types.begin();
            contest_type_iter != contest_types.end();
            ++contest_type_iter)
        {
            test_contest_ribbons(pokemon, *contest_type_iter);
        }

        for(auto ribbon_iter = ribbons.begin();
            ribbon_iter != ribbons.end();
            ++ribbon_iter)
        {
            test_ribbons(pokemon, *ribbon_iter);
        }

        static const std::vector<std::string> contest_fields = boost::assign::list_of
            ("Cool")("Beauty")("Cute")("Smart")("Tough")("Feel")
        ;
        for(auto contest_field_iter = contest_fields.begin();
            contest_field_iter != contest_fields.end();
            ++contest_field_iter)
        {
            test_contest_stats(pokemon, *contest_field_iter);
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

        BOOST_CHECK_THROW(
            pokemon->set_move("Pound", -1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_move("Pound", 4);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            pokemon->set_move("Shadow Sky", 0);
        , std::invalid_argument);

        pokemon->set_move("Swallow", 0);
        pokemon->set_move("Flamethrower", 1);
        pokemon->set_move("Return", 2);
        pokemon->set_move("Fire Blast", 3);

        BOOST_CHECK_EQUAL(
            move_slots.at(0).move.get_name(),
            "Swallow"
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
            "Return"
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

        /*
         * On the C++ level, check the underlying PKSav struct and make
         * sure our abstractions match.
         */
        const pksav_gba_pc_pokemon_t* native_pc_data = reinterpret_cast<const pksav_gba_pc_pokemon_t*>(
                                                           pokemon->get_native_pc_data()
                                                       );
        const pksav_gba_pokemon_growth_t* growth = &native_pc_data->blocks.growth;
        const pksav_gba_pokemon_attacks_t* attacks = &native_pc_data->blocks.attacks;
        const pksav_gba_pokemon_effort_t* effort = &native_pc_data->blocks.effort;
        const pksav_gba_pokemon_misc_t* misc = &native_pc_data->blocks.misc;

        BOOST_CHECK_EQUAL(
            pksav_littleendian32(native_pc_data->personality),
            pokemon->get_personality()
        );
        BOOST_CHECK_EQUAL(
            pksav_littleendian32(native_pc_data->ot_id.id),
            pokemon->get_trainer_id()
        );
        BOOST_CHECK_EQUAL(
            pksav_littleendian16(native_pc_data->ot_id.pid),
            pokemon->get_trainer_public_id()
        );
        BOOST_CHECK_EQUAL(
            pksav_littleendian16(native_pc_data->ot_id.sid),
            pokemon->get_trainer_secret_id()
        );

        char nickname[11] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                native_pc_data->nickname,
                nickname,
                10
            );
        );
        BOOST_CHECK_EQUAL(
            std::string(nickname),
            pokemon->get_nickname()
        );

        // TODO: language

        char otname[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                native_pc_data->otname,
                otname,
                7
            );
        );
        BOOST_CHECK_EQUAL(
            std::string(otname),
            pokemon->get_trainer_name()
        );

        BOOST_CHECK(native_pc_data->markings & PKSAV_MARKING_CIRCLE);
        BOOST_CHECK(native_pc_data->markings & PKSAV_MARKING_TRIANGLE);
        BOOST_CHECK(native_pc_data->markings & PKSAV_MARKING_SQUARE);
        BOOST_CHECK(native_pc_data->markings & PKSAV_MARKING_HEART);

        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(growth->species)),
            pokemon->get_database_entry().get_pokemon_index()
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(growth->held_item)),
            pokemon->get_held_item().get_item_index()
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian32(growth->exp)),
            pokemon->get_experience()
        );
        // TODO: PP Up
        BOOST_CHECK_EQUAL(
            int(growth->friendship),
            pokemon->get_friendship()
        );

        for(int i = 0; i < 4; ++i) {
            BOOST_CHECK_EQUAL(
                int(attacks->moves[i]),
                move_slots.at(i).move.get_move_id()
            );
            BOOST_CHECK_EQUAL(
                int(attacks->move_pps[i]),
                move_slots.at(i).pp
            );
        }

        const std::map<std::string, int>& EVs = pokemon->get_EVs();
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_hp)),
            EVs.at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_atk)),
            EVs.at("Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_def)),
            EVs.at("Defense")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_spd)),
            EVs.at("Speed")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_spatk)),
            EVs.at("Special Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(effort->ev_spdef)),
            EVs.at("Special Defense")
        );

        const std::map<std::string, int>& contest_stats = pokemon->get_contest_stats();
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.cool),
            contest_stats.at("Cool")
        );
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.beauty),
            contest_stats.at("Beauty")
        );
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.cute),
            contest_stats.at("Cute")
        );
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.smart),
            contest_stats.at("Smart")
        );
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.tough),
            contest_stats.at("Tough")
        );
        BOOST_CHECK_EQUAL(
            int(effort->contest_stats.feel),
            contest_stats.at("Feel")
        );

        // TODO: Pokérus
        // TODO: get location indices for what we set

        BOOST_CHECK_EQUAL(
            int(misc->origin_info & PKSAV_GBA_LEVEL_MET_MASK),
            pokemon->get_level_met()
        );

        // TODO: get ball ID for what we set
        BOOST_CHECK(misc->origin_info & PKSAV_GBA_OTGENDER_MASK);

        const std::map<std::string, int>& IVs = pokemon->get_IVs();
        uint8_t IV = 0;

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_HP,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("HP")
        );

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_ATTACK,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Attack")
        );

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_DEFENSE,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Defense")
        );

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_SPEED,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Speed")
        );

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_SPATK,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Special Attack")
        );

        PKSAV_CALL(
            pksav_get_IV(
                &misc->iv_egg_ability,
                PKSAV_STAT_SPDEF,
                &IV
            );
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Special Defense")
        );

        BOOST_CHECK(!(misc->iv_egg_ability & PKSAV_GBA_EGG_MASK));
        BOOST_CHECK(!(misc->iv_egg_ability & PKSAV_GBA_ABILITY_MASK));

        /*
         * Party data
         */
        const pksav_gba_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gba_pokemon_party_data_t*>(
                                                                      pokemon->get_native_party_data()
                                                                  );
        // TODO: condition

        BOOST_CHECK_EQUAL(
            int(native_party_data->level),
            pokemon->get_level()
        );

        // TODO: Pokérus

        const std::map<std::string, int>& stats = pokemon->get_stats();

        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->current_hp)),
            stats.at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->max_hp)),
            stats.at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->atk)),
            stats.at("Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->def)),
            stats.at("Defense")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->spd)),
            stats.at("Speed")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->spatk)),
            stats.at("Special Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_littleendian16(native_party_data->spdef)),
            stats.at("Special Defense")
        );
    }

}
