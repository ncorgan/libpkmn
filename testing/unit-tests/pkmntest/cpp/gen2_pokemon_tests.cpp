/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen2_pokemon_tests.hpp>
#include "pokemon_tests_common.hpp"

#include <pkmn/exception.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/math/endian.h>

// Don't create the main in a library
#undef BOOST_TEST_MAIN

#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

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

    void gen2_invalid_pokemon_test(
        const std::string &game
    ) {
        pkmntest::test_invalid_pokemon(game);
    }

    static void gen2_pokemon_check_stat_map(
        const std::map<std::string, int>& stat_map,
        bool is_stats
    ) {
        BOOST_CHECK_EQUAL(stat_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Speed"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special"), 1);
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
            std::string(pkmn::pokemon::LIBPKMN_OT_NAME)
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
            uint16_t(pkmn::pokemon::LIBPKMN_OT_ID & 0xFFFF)
        );
        BOOST_CHECK_EQUAL(
            pokemon->get_trainer_gender(),
            "Male"
        );
        BOOST_CHECK_THROW(
            pokemon->get_ability()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_ball()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_EQUAL(
            pokemon->get_location_caught(),
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
    }
}
