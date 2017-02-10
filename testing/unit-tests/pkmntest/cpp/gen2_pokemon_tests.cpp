/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen2_pokemon_tests.hpp>
#include <pkmntest/pokemon_tests_common.hpp>

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>

#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/time.h>
#include <pksav/math/endian.h>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace fs = boost::filesystem;

static const std::map<std::string, pkmn::move_slot> none_move_slots = boost::assign::map_list_of
    ("Gold", pkmn::move_slot(
                 pkmn::database::move_entry("None", "Gold"),
                 0
             ))
    ("Silver", pkmn::move_slot(
                   pkmn::database::move_entry("None", "Silver"),
                   0
               ))
    ("Crystal", pkmn::move_slot(
                    pkmn::database::move_entry("None", "Crystal"),
                    0
                ))
;

static const std::map<std::string, pkmn::database::pokemon_entry> none_pokemon_entries = boost::assign::map_list_of
    ("Gold", pkmn::database::pokemon_entry("None", "Gold", ""))
    ("Silver", pkmn::database::pokemon_entry("None", "Silver", ""))
    ("Crystal", pkmn::database::pokemon_entry("None", "Crystal", ""))
;

namespace pkmntest {

    void gen2_unown_form_test(
        const std::string &game
    ) {
        pkmn::database::pokemon_entry unown_entry(
            "Unown",
            game,
            ""
        );
        std::vector<std::string> unown_forms = unown_entry.get_forms();

        for(int i = 0; i < 26; ++i) {
            pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                            "Unown",
                                            game,
                                            unown_forms[i],
                                            5
                                        );
            BOOST_CHECK_EQUAL(
                unown->get_form(),
                unown_forms[i]
            );

            // Make sure IVs are properly set
            const std::map<std::string, int>& IVs = unown->get_IVs();
            std::string form_from_IVs = pkmn::calculations::gen2_unown_form(
                                            IVs.at("Attack"),
                                            IVs.at("Defense"),
                                            IVs.at("Speed"),
                                            IVs.at("Special")
                                        );
            BOOST_CHECK_EQUAL(
                unown->get_form(),
                form_from_IVs
            );
        }

        pkmn::pokemon::sptr unown = pkmn::pokemon::make(
                                        "Unown",
                                        game,
                                        "A",
                                        5
                                    );
        const std::map<std::string, int>& IVs = unown->get_IVs();

        // Make sure setting the form properly changes the IVs.
        for(int i = 0; i < 26; ++i) {
            unown->set_form(unown_forms[i]);
            std::string form_from_IVs = pkmn::calculations::gen2_unown_form(
                                            IVs.at("Attack"),
                                            IVs.at("Defense"),
                                            IVs.at("Speed"),
                                            IVs.at("Special")
                                        );
            BOOST_CHECK_EQUAL(
                unown->get_form(),
                form_from_IVs
            );
        }

        // Make sure setting IVs properly changes the form.
        unown->set_IV("Attack", 10);
        unown->set_IV("Defense", 9);
        unown->set_IV("Speed", 1);
        unown->set_IV("Special", 14);
        BOOST_CHECK_EQUAL(unown->get_form(), "G");
    }

    static void gen2_pokemon_check_stat_map(
        const std::map<std::string, int>& stat_map,
        bool is_stats
    ) {
        BOOST_CHECK_EQUAL(stat_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Speed"), 1);
        if(is_stats) {
            BOOST_CHECK_EQUAL(stat_map.count("Special"), 0);
            BOOST_CHECK_EQUAL(stat_map.count("Special Attack"), 1);
            BOOST_CHECK_EQUAL(stat_map.count("Special Defense"), 1);
        } else {
            BOOST_CHECK_EQUAL(stat_map.count("Special"), 1);
            BOOST_CHECK_EQUAL(stat_map.count("Special Attack"), 0);
            BOOST_CHECK_EQUAL(stat_map.count("Special Defense"), 0);
        }
    }

    void gen2_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        /*
         * Check starting values, and confirm that we can't query values
         * that didn't exist in Generation II.
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
        BOOST_CHECK_THROW(
            pokemon->get_trainer_secret_id();
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            (pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Male"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_friendship(),
            pokemon->get_database_entry().get_base_friendship()
        );
        BOOST_CHECK_THROW(
            pokemon->get_ability()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_ball()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            pokemon->get_level()
        );
        BOOST_CHECK_THROW(
            pokemon->get_location_met(true)
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            "Special"
        );
        BOOST_CHECK_THROW(
            pokemon->get_original_game();
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_personality();
        , pkmn::feature_not_in_game_error);
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
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_ribbons();
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_contest_stats();
        , pkmn::feature_not_in_game_error);

        const pkmn::move_slots_t& move_slots = pokemon->get_moves();
        BOOST_CHECK_EQUAL(move_slots.size(), 4);
        for(auto iter = move_slots.begin(); iter != move_slots.end(); ++iter) {
            BOOST_CHECK((*iter) == none_move_slots.at(game));
        }

        gen2_pokemon_check_stat_map(
            pokemon->get_EVs(),
            false
        );
        gen2_pokemon_check_stat_map(
            pokemon->get_IVs(),
            false
        );
        gen2_pokemon_check_stat_map(
            pokemon->get_stats(),
            true
        );

        BOOST_CHECK(fs::exists(pokemon->get_icon_filepath()));
        BOOST_CHECK(fs::exists(pokemon->get_sprite_filepath()));

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

        // Gender affects IVs, so make sure the abstraction reflects that.
        const std::map<std::string, int>& IVs = pokemon->get_IVs();
        pokemon->set_gender("Male");
        BOOST_CHECK_EQUAL(
            IVs.at("Attack"),
            15
        );
        pokemon->set_gender("Female");
        BOOST_CHECK_LT(
            IVs.at("Attack"),
            15
        );

        pokemon->set_IV("Attack", 0);
        BOOST_CHECK_EQUAL(
            pokemon->get_gender(),
            "Female"
        );
        pokemon->set_IV("Attack", 15);
        BOOST_CHECK_EQUAL(
            pokemon->get_gender(),
            "Male"
        );

        // Shininess affects IVs, so make sure the abstraction reflects that. Also check filepaths.
        std::string sprite_filepath;

        pokemon->set_shininess(false);
        BOOST_CHECK(not pokemon->is_shiny());
        BOOST_CHECK_EQUAL(
            IVs.at("Attack"),
            13
        );
        sprite_filepath = pokemon->get_sprite_filepath();
        BOOST_CHECK(fs::exists(sprite_filepath));

        // This will fail if "shiny" is anywhere in the filepath.
        BOOST_CHECK(sprite_filepath.find("shiny") == std::string::npos);

        pokemon->set_shininess(true);
        BOOST_CHECK(pokemon->is_shiny());
        BOOST_CHECK(fs::exists(pokemon->get_sprite_filepath()));
        BOOST_CHECK_EQUAL(
            IVs.at("Attack"),
            15
        );
        BOOST_CHECK_EQUAL(
            IVs.at("Defense"),
            10
        );
        BOOST_CHECK_EQUAL(
            IVs.at("Speed"),
            10
        );
        BOOST_CHECK_EQUAL(
            IVs.at("Special"),
            10
        );
        sprite_filepath = pokemon->get_sprite_filepath();
        BOOST_CHECK(fs::exists(sprite_filepath));
        BOOST_CHECK(sprite_filepath.find("shiny") != std::string::npos);

        BOOST_CHECK_THROW(
            pokemon->set_held_item("Not an item");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        BOOST_CHECK_THROW(
            pokemon->set_held_item("Razz Berry");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        BOOST_CHECK_THROW(
            pokemon->set_held_item("Bicycle");
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "None"
        );
        pokemon->set_held_item("Berry");
        BOOST_CHECK_EQUAL(
            pokemon->get_held_item().get_name(),
            "Berry"
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
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_id(),
            10001
        );

        BOOST_CHECK_THROW(
            pokemon->set_ability("");
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_ball("Great Ball");
        , pkmn::feature_not_in_game_error);

        // On the C++ level, make sure functions that affect the same PKSav field don't impact each other.
        std::string location_met_before_change = pokemon->get_location_met(false);
        std::string trainer_gender_before_change = pokemon->get_trainer_gender();
        int level_met_before_change = pokemon->get_level_met();

        const pksav_gen2_pc_pokemon_t* native_pc = reinterpret_cast<const pksav_gen2_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
        uint16_t time_caught_before_change = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
        time_caught_before_change >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;

        // Setting location caught shouldn't affect level caught, trainer gender, or time of day caught
        BOOST_CHECK_THROW(
            pokemon->set_location_met("Pallet Town", true);
        , pkmn::feature_not_in_game_error);
        pokemon->set_location_met("Pallet Town", false);
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            "Pallet Town"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            trainer_gender_before_change
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            level_met_before_change
        );

        uint16_t time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
        time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
        BOOST_CHECK_EQUAL(
            time_caught,
            time_caught_before_change
        );

        pokemon->set_location_met(location_met_before_change, false);

        // Setting trainer gender shouldn't affect level caught, location caught, or time of day caught.
        pokemon->set_trainer_gender("Female");
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Female"
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            location_met_before_change
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            level_met_before_change
        );

        time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
        time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
        BOOST_CHECK_EQUAL(
            time_caught,
            time_caught_before_change
        );

        pokemon->set_trainer_gender(trainer_gender_before_change);

        // Setting level caught shouldn't affect location caught, trainer gender, or time of day caught.
        pokemon->set_level_met(3);
        BOOST_CHECK_EQUAL(
            pokemon->get_level_met(),
            3
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_location_met(false),
            location_met_before_change
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            trainer_gender_before_change
        );

        time_caught = (native_pc->caught_data & PKSAV_GEN2_TIME_OF_DAY_MASK);
        time_caught >>= PKSAV_GEN2_TIME_OF_DAY_OFFSET;
        BOOST_CHECK_EQUAL(
            time_caught,
            time_caught_before_change
        );

        pokemon->set_friendship(123);
        BOOST_CHECK_EQUAL(
            pokemon->get_friendship(),
            123
        );

        BOOST_CHECK_THROW(
            pokemon->set_original_game("Gold");
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_marking("Circle", true);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_ribbon("Cool", false);
        , pkmn::feature_not_in_game_error);

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
        , pkmn::range_error);
        pokemon->set_move("Synthesis", 1);
        BOOST_CHECK_THROW(
            pokemon->set_move("Bounce", 0);
        , std::invalid_argument);
        BOOST_CHECK_EQUAL(
            move_slots.at(0).move.get_name(),
            "None"
        );
        BOOST_CHECK_EQUAL(
            move_slots.at(1).move.get_name(),
            "Synthesis"
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
        , pkmn::range_error);
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
        , pkmn::range_error);
        pokemon->set_IV("Attack", 12);
        BOOST_CHECK_EQUAL(
            pokemon->get_IVs().at("Attack"),
            12
        );

        /*
         * On the C++ level, check the underlying PKSav struct and make
         * sure our abstractions match.
         */
        const pksav_gen2_pokemon_party_data_t* native_party_data = reinterpret_cast<const pksav_gen2_pokemon_party_data_t*>(
                                                                       pokemon->get_native_party_data()
                                                                   );

        BOOST_CHECK_EQUAL(
            uint8_t(pokemon->get_database_entry().get_pokemon_index()),
            native_pc->species
        );
        BOOST_CHECK_EQUAL(
            uint8_t(pokemon->get_held_item().get_item_index()),
            native_pc->held_item
        );
        for(int i = 0; i < 4; ++i) {
            BOOST_CHECK_EQUAL(
                native_pc->moves[i],
                uint8_t(move_slots.at(i).move.get_move_id())
            );
            BOOST_CHECK_EQUAL(
                (native_pc->move_pps[i] & PKSAV_GEN2_MOVE_PP_MASK),
                uint8_t(move_slots.at(i).pp)
            );
        }
        BOOST_CHECK_EQUAL(
            pksav_bigendian16(native_pc->ot_id),
            uint16_t(pokemon->get_trainer_id())
        );
        BOOST_CHECK_EQUAL(
            pksav_bigendian16(native_pc->ot_id),
            pokemon->get_trainer_public_id()
        );

        const std::map<std::string, int>& EVs = pokemon->get_EVs();
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ev_hp)),
            EVs.at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ev_atk)),
            EVs.at("Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ev_def)),
            EVs.at("Defense")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ev_spd)),
            EVs.at("Speed")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ev_spcl)),
            EVs.at("Special")
        );

        uint8_t IV = 0;

        PKSAV_CALL(
            pksav_get_gb_IV(
                &native_pc->iv_data,
                PKSAV_STAT_HP,
                &IV
            )
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("HP")
        );
        PKSAV_CALL(
            pksav_get_gb_IV(
                &native_pc->iv_data,
                PKSAV_STAT_ATTACK,
                &IV
            )
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Attack")
        );
        PKSAV_CALL(
            pksav_get_gb_IV(
                &native_pc->iv_data,
                PKSAV_STAT_DEFENSE,
                &IV
            )
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Defense")
        );
        PKSAV_CALL(
            pksav_get_gb_IV(
                &native_pc->iv_data,
                PKSAV_STAT_SPEED,
                &IV
            )
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Speed")
        );
        PKSAV_CALL(
            pksav_get_gb_IV(
                &native_pc->iv_data,
                PKSAV_STAT_SPECIAL,
                &IV
            )
        )
        BOOST_CHECK_EQUAL(
            int(IV),
            IVs.at("Special")
        );
        BOOST_CHECK_EQUAL(
            int(native_pc->level),
            pokemon->get_level()
        );

        /*
         * Party data
         */
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->max_hp)),
            pokemon->get_stats().at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->atk)),
            pokemon->get_stats().at("Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->def)),
            pokemon->get_stats().at("Defense")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->spd)),
            pokemon->get_stats().at("Speed")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->spatk)),
            pokemon->get_stats().at("Special Attack")
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_party_data->spdef)),
            pokemon->get_stats().at("Special Defense")
        );
    }
}
