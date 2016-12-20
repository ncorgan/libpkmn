/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/gen1_pokemon_tests.hpp>
#include "pokemon_tests_common.hpp"

#include <pkmn/exception.hpp>
#include "pksav/pksav_call.hpp"

#include <pksav/common/stats.h>
#include <pksav/gen1/pokemon.h>
#include <pksav/math/endian.h>

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

    void gen1_invalid_pokemon_test(
        const std::string &game
    ) {
        pkmntest::test_invalid_pokemon(game);
    }

    void gen1_friendship_test(
        const std::string &game
    ) {
        pkmn::pokemon::sptr pikachu = pkmn::pokemon::make(
                                          "Pikachu",
                                          game,
                                          "",
                                          5
                                      );
        if(game == "Yellow") {
            pikachu->set_friendship(123);
            BOOST_CHECK_EQUAL(
                pikachu->get_friendship(),
                123
            );

            // Also check a non-Pikachu
            pkmn::pokemon::sptr mewtwo = pkmn::pokemon::make(
                                             "Mewtwo",
                                             game,
                                             "",
                                             70
                                         );
            BOOST_CHECK_THROW(
                mewtwo->set_friendship(123);
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)mewtwo->get_friendship();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_THROW(
                pikachu->set_friendship(123);
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                (void)pikachu->get_friendship();
            , pkmn::feature_not_in_game_error);
        }
    }

    static void gen1_pokemon_check_stat_map(
        const std::map<std::string, int>& stat_map
    ) {
        BOOST_CHECK_EQUAL(stat_map.count("HP"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Attack"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Defense"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Speed"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special"), 1);
        BOOST_CHECK_EQUAL(stat_map.count("Special Attack"), 0);
        BOOST_CHECK_EQUAL(stat_map.count("Special Defense"), 0);
    }

    void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    ) {
        /*
         * Check starting values, and confirm that we can't query values
         * that didn't exist in Generation I.
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
        BOOST_CHECK_THROW(
            pokemon->get_held_item();
        , pkmn::feature_not_in_game_error);
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
            pokemon->get_friendship()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_ability()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_ball()
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_level_caught();
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_location_met(true);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->get_location_met(false);
        , pkmn::feature_not_in_game_error);
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

        gen1_pokemon_check_stat_map(
            pokemon->get_EVs()
        );
        gen1_pokemon_check_stat_map(
            pokemon->get_IVs()
        );
        gen1_pokemon_check_stat_map(
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

        BOOST_CHECK_THROW(
            pokemon->set_held_item("Potion");
        , pkmn::feature_not_in_game_error);

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
            pokemon->set_trainer_gender("Male");
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_friendship(123);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_ability("");
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_ball("Great Ball");
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_level_caught(78);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_location_met("Victory Road", true);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_location_met("Victory Road", false);
        , pkmn::feature_not_in_game_error);
        BOOST_CHECK_THROW(
            pokemon->set_original_game("Blue");
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
        const pksav_gen1_pc_pokemon_t* native_pc = reinterpret_cast<pksav_gen1_pc_pokemon_t*>(
                                                       pokemon->get_native_pc_data()
                                                   );
        const pksav_gen1_pokemon_party_data_t* native_party_data = reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(
                                                                       pokemon->get_native_party_data()
                                                                   );

        /*
         * PC data
         */
        BOOST_CHECK_EQUAL(
            int(native_pc->species),
            pokemon->get_database_entry().get_pokemon_index()
        );
        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->current_hp)),
            pokemon->get_stats().at("HP")
        );
        BOOST_CHECK_EQUAL(
            int(native_pc->level),
            pokemon->get_level()
        );
        // TODO: change condition and check
        BOOST_CHECK_EQUAL(
            native_pc->condition,
            0
        );
        // TODO: programmatic check for types, catch rate

        const pkmn::move_slots_t& moves = pokemon->get_moves();
        for(size_t i = 0; i < 4; ++i) {
            BOOST_CHECK_EQUAL(
                int(native_pc->moves[i]),
                moves.at(i).move.get_move_id()
            );
            BOOST_CHECK_EQUAL(
                int(native_pc->move_pps[i]),
                moves.at(i).pp
            );
        }

        BOOST_CHECK_EQUAL(
            int(pksav_bigendian16(native_pc->ot_id)),
            pokemon->get_trainer_id()
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

        const std::map<std::string, int>& IVs = pokemon->get_IVs();
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

        /*
         * Party data
         */
        BOOST_CHECK_EQUAL(
            int(native_party_data->level),
            pokemon->get_level()
        );
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
            int(pksav_bigendian16(native_party_data->spcl)),
            pokemon->get_stats().at("Special")
        );
    }
}
