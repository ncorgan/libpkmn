/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/game_save_tests.hpp>

#include <pkmn/exception.hpp>

#include <boost/config.hpp>

// Don't create the main in a library
#undef BOOST_TEST_MAIN
#include "pkmn_boost_unit_test.hpp"

#include <iostream>
#include <string>

BOOST_STATIC_CONSTEXPR const char* TOO_LONG_OT_NAME = "LibPKMNLibPKMN";
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_PID = 1351;
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_SID = 32123;

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

static const std::string GB_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold",  "Silver", "Crystal"
};

static const std::string RIVAL_NAME_SET_GAMES[] = {
    "Ruby", "Sapphire", "Emerald",
    "Black", "White",
    "X", "Y"
};

static const std::string MALE_ONLY_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver"
};

namespace pkmntest {

    static void test_trainer_name(
        pkmn::game_save::sptr save
    ) {
        BOOST_CHECK_THROW(
            save->set_trainer_name("");
        , std::invalid_argument);
        BOOST_CHECK_THROW(
            save->set_trainer_name(TOO_LONG_OT_NAME);
        , std::invalid_argument);

        save->set_trainer_name(pkmn::pokemon::LIBPKMN_OT_NAME);
        BOOST_CHECK_EQUAL(
            save->get_trainer_name(),
            std::string(pkmn::pokemon::LIBPKMN_OT_NAME)
        );
    }

    static void test_rival_name(
        pkmn::game_save::sptr save,
        bool is_rival_name_set
    ) {
        if(is_rival_name_set) {
            BOOST_CHECK_THROW(
                save->set_rival_name(pkmn::pokemon::LIBPKMN_OT_NAME);
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_THROW(
                save->set_rival_name("");
            , std::invalid_argument);
            BOOST_CHECK_THROW(
                save->set_rival_name(TOO_LONG_OT_NAME);
            , std::invalid_argument);

            save->set_rival_name(pkmn::pokemon::LIBPKMN_OT_NAME);
            BOOST_CHECK_EQUAL(
                save->get_rival_name(),
                std::string(pkmn::pokemon::LIBPKMN_OT_NAME)
            );
        }
    }

    static void test_trainer_id(
        pkmn::game_save::sptr save,
        bool is_gb_game
    ) {
        BOOST_CHECK_EQUAL(
            save->get_trainer_id(),
            (is_gb_game ? LIBPKMN_OT_PID : pkmn::pokemon::LIBPKMN_OT_ID)
        );
        BOOST_CHECK_EQUAL(
            save->get_trainer_public_id(),
            LIBPKMN_OT_PID
        );
        if(is_gb_game) {
            BOOST_CHECK_THROW(
                save->get_trainer_secret_id();
            , pkmn::feature_not_in_game_error);
        } else {
            BOOST_CHECK_EQUAL(
                save->get_trainer_secret_id(),
                LIBPKMN_OT_SID
            );
        }
    }

    void game_save_test_common_fields(
        pkmn::game_save::sptr save
    ) {
        test_trainer_name(save);

        bool is_gb_game = (
            std::find(
                GB_GAMES,
                GB_GAMES+6,
                save->get_game()
            ) != GB_GAMES+6
        );
        save->set_trainer_id(
            is_gb_game ? LIBPKMN_OT_PID : pkmn::pokemon::LIBPKMN_OT_ID
        );
        test_trainer_id(
            save,
            is_gb_game
        );

        save->set_trainer_public_id(LIBPKMN_OT_PID);
        test_trainer_id(
            save,
            is_gb_game
        );

        if(is_gb_game) {
            BOOST_CHECK_THROW(
                save->set_trainer_id(pkmn::pokemon::LIBPKMN_OT_ID);
            , pkmn::range_error);
            BOOST_CHECK_THROW(
                save->set_trainer_secret_id(LIBPKMN_OT_SID);
            , pkmn::feature_not_in_game_error);
        } else {
            save->set_trainer_secret_id(LIBPKMN_OT_SID);
            test_trainer_id(
                save,
                is_gb_game
            );
        }

        bool is_rival_name_set = (
            std::find(
                RIVAL_NAME_SET_GAMES,
                RIVAL_NAME_SET_GAMES+7,
                save->get_game()
            ) != RIVAL_NAME_SET_GAMES+7
        );
        test_rival_name(
            save,
            is_rival_name_set
        );

        bool is_male_only = (std::find(
            MALE_ONLY_GAMES,
            MALE_ONLY_GAMES+5,
            save->get_game()
        ) != MALE_ONLY_GAMES+5);
        if(is_male_only) {
            BOOST_CHECK_EQUAL(
                save->get_trainer_gender(),
                "Male"
            );
            BOOST_CHECK_THROW(
                save->set_trainer_gender("Male");
            , pkmn::feature_not_in_game_error);
            BOOST_CHECK_THROW(
                save->set_trainer_gender("Female");
            , pkmn::feature_not_in_game_error);
        } else {
            save->set_trainer_gender("Male");
            BOOST_CHECK_EQUAL(
                save->get_trainer_gender(),
                "Male"
            );
            save->set_trainer_gender("Female");
            BOOST_CHECK_EQUAL(
                save->get_trainer_gender(),
                "Female"
            );
            BOOST_CHECK_THROW(
                save->set_trainer_gender("Genderless");
            , std::invalid_argument);
        }

        BOOST_CHECK_THROW(
            save->set_money(-1);
        , pkmn::range_error);
        BOOST_CHECK_THROW(
            save->set_money(MONEY_MAX_VALUE+1);
        , pkmn::range_error);

        save->set_money(123456);
        BOOST_CHECK_EQUAL(
            save->get_money(),
            123456
        );

        // To avoid crashing
        BOOST_REQUIRE(save->get_item_bag().get() != nullptr);
        BOOST_REQUIRE(save->get_item_pc().get() != nullptr);
        BOOST_REQUIRE(save->get_pokemon_party().get() != nullptr);
        BOOST_REQUIRE(save->get_pokemon_pc().get() != nullptr);

        // Make sure get_num_pokemon() and the party matches
        pkmn::pokemon_party::sptr party = save->get_pokemon_party();
        int num_party_pokemon = party->get_num_pokemon();
        BOOST_CHECK_GT(num_party_pokemon, 0);
        BOOST_REQUIRE_LE(num_party_pokemon, 6);
        BOOST_CHECK_EQUAL(party->as_vector().size(), 6);
        for(int i = 0; i < 6; ++i) {
            pkmn::pokemon::sptr party_pokemon = party->get_pokemon(i);
            BOOST_REQUIRE(party_pokemon.get() != nullptr);
            if(i < num_party_pokemon) {
                BOOST_CHECK_NE(
                    party_pokemon->get_species(),
                    "None"
                );
            } else {
                BOOST_CHECK_EQUAL(
                    party_pokemon->get_species(),
                    "None"
                );
            }
        }

        pkmn::pokemon_pc::sptr pc = save->get_pokemon_pc();
        int num_boxes = pc->get_num_boxes();
        BOOST_CHECK_EQUAL(num_boxes, pc->as_vector().size());
        for(int i = 0; i < num_boxes; ++i) {
            pkmn::pokemon_box::sptr box = pc->get_box(i);
            BOOST_REQUIRE(box.get() != nullptr);

            int capacity = box->get_capacity();
            int num_box_pokemon = box->get_num_pokemon();
            BOOST_CHECK_GT(capacity, 0);
            BOOST_CHECK_GE(num_box_pokemon, 0);
            BOOST_REQUIRE_LE(num_box_pokemon, capacity);
            BOOST_CHECK_EQUAL(capacity, box->as_vector().size());

            for(int j = 0; j < capacity; ++j) {
                pkmn::pokemon::sptr box_pokemon = box->get_pokemon(j);
                BOOST_REQUIRE(box_pokemon.get() != nullptr);

                if(std::find(GB_GAMES, GB_GAMES+6, save->get_game()) != GB_GAMES+6) {
                    if(j < num_box_pokemon) {
                        BOOST_CHECK_NE(
                            box_pokemon->get_species(),
                            "None"
                        );
                    } else {
                        BOOST_CHECK_EQUAL(
                            box_pokemon->get_species(),
                            "None"
                        );
                    }
                }
            }
        }
    }
}
