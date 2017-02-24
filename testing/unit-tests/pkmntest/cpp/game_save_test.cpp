/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmntest/game_save_test.hpp>

#include <pkmn/exception.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <map>

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

// No database access here
static const std::map<std::string, int> game_generations = boost::assign::map_list_of
    ("Red", 1)
    ("Blue", 1)
    ("Yellow", 1)
    ("Gold", 2)
    ("Silver", 2)
    ("Crystal", 2)
    ("Ruby", 3)
    ("Sapphire", 3)
    ("Emerald", 3)
    ("FireRed", 3)
    ("LeafGreen", 3)
    ("Colosseum", 3)
    ("XD", 3)
    ("Diamond", 4)
    ("Pearl", 4)
    ("Platinum", 4)
    ("HeartGold", 4)
    ("SoulSilver", 4)
    ("Black", 5)
    ("White", 5)
    ("Black 2", 5)
    ("White 2", 5)
    ("X", 6)
    ("Y", 6)
    ("Omega Ruby", 6)
    ("Alpha Sapphire", 6)
;

namespace fs = boost::filesystem;

namespace pkmntest {

    static const fs::path PKSAV_TEST_SAVES(pkmn_getenv("PKSAV_TEST_SAVES"));

    void game_save_test::load_save() {
        std::string save_path = fs::path(PKSAV_TEST_SAVES / std::get<2>(_params)).string();
        ASSERT_EQ(std::get<0>(_params), pkmn::game_save::detect_type(save_path));
        _game_save = pkmn::game_save::from_file(save_path);
        ASSERT_EQ(std::get<1>(_params), _game_save->get_game());
    }

    static void test_trainer_name(
        pkmn::game_save::sptr save
    ) {
        EXPECT_THROW(
            save->set_trainer_name("");
        , std::invalid_argument);
        EXPECT_THROW(
            save->set_trainer_name(TOO_LONG_OT_NAME);
        , std::invalid_argument);

        save->set_trainer_name(pkmn::pokemon::LIBPKMN_OT_NAME);
        EXPECT_EQ(std::string(pkmn::pokemon::LIBPKMN_OT_NAME), save->get_trainer_name());
    }

    static void test_trainer_id(
        pkmn::game_save::sptr save,
        bool is_gb_game
    ) {
        EXPECT_EQ((is_gb_game ? LIBPKMN_OT_PID : pkmn::pokemon::LIBPKMN_OT_ID), save->get_trainer_id());
        EXPECT_EQ(LIBPKMN_OT_PID, save->get_trainer_public_id());
        if(is_gb_game) {
            EXPECT_THROW(
                save->get_trainer_secret_id();
            , pkmn::feature_not_in_game_error);
        } else {
            EXPECT_EQ(LIBPKMN_OT_SID, save->get_trainer_secret_id());
        }
    }

    static void test_rival_name(
        pkmn::game_save::sptr save,
        bool is_rival_name_set
    ) {
        if(is_rival_name_set) {
            EXPECT_THROW(
                save->set_rival_name(pkmn::pokemon::LIBPKMN_OT_NAME);
            , pkmn::feature_not_in_game_error);
        } else {
            EXPECT_THROW(
                save->set_rival_name("");
            , std::invalid_argument);
            EXPECT_THROW(
                save->set_rival_name(TOO_LONG_OT_NAME);
            , std::invalid_argument);

            save->set_rival_name(pkmn::pokemon::LIBPKMN_OT_NAME);
            EXPECT_EQ(std::string(pkmn::pokemon::LIBPKMN_OT_NAME), save->get_rival_name());
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
        test_trainer_id(save, is_gb_game);
        save->set_trainer_public_id(LIBPKMN_OT_PID);
        test_trainer_id(save, is_gb_game);

        bool is_rival_name_set = (
            std::find(
                RIVAL_NAME_SET_GAMES,
                RIVAL_NAME_SET_GAMES+7,
                save->get_game()
            ) != RIVAL_NAME_SET_GAMES+7
        );
        test_rival_name(save, is_rival_name_set);

        bool is_male_only = (std::find(
            MALE_ONLY_GAMES,
            MALE_ONLY_GAMES+5,
            save->get_game()
        ) != MALE_ONLY_GAMES+5);
        if(is_male_only) {
            EXPECT_EQ("Male", save->get_trainer_gender());
            EXPECT_THROW(
                save->set_trainer_gender("Male");
            , pkmn::feature_not_in_game_error);
            EXPECT_THROW(
                save->set_trainer_gender("Female");
            , pkmn::feature_not_in_game_error);
        } else {
            save->set_trainer_gender("Male");
            EXPECT_EQ("Male", save->get_trainer_gender());
            save->set_trainer_gender("Female");
            EXPECT_EQ("Female", save->get_trainer_gender());
            EXPECT_THROW(
                save->set_trainer_gender("Genderless");
            , std::invalid_argument);
        }

        EXPECT_THROW(
            save->set_money(-1);
        , pkmn::range_error);
        EXPECT_THROW(
            save->set_money(MONEY_MAX_VALUE+1);
        , pkmn::range_error);

        save->set_money(123456);
        EXPECT_EQ(123456, save->get_money());

        // To avoid crashing
        ASSERT_NE(nullptr, save->get_item_bag().get());
        ASSERT_NE(nullptr, save->get_item_pc().get());
        ASSERT_NE(nullptr, save->get_pokemon_party().get());
        ASSERT_NE(nullptr, save->get_pokemon_pc().get());

        // Make sure get_num_pokemon() and the party matches
        pkmn::pokemon_party::sptr party = save->get_pokemon_party();
        int num_party_pokemon = party->get_num_pokemon();
        EXPECT_GT(num_party_pokemon, 0);
        ASSERT_LE(num_party_pokemon, 6);
        EXPECT_EQ(6, party->as_vector().size());
        for(int i = 0; i < 6; ++i) {
            pkmn::pokemon::sptr party_pokemon = party->get_pokemon(i);
            ASSERT_NE(nullptr, party_pokemon.get());
            if(i < num_party_pokemon) {
                EXPECT_NE("None", party_pokemon->get_species());
            } else {
                EXPECT_EQ("None", party_pokemon->get_species());
            }
        }

        pkmn::pokemon_pc::sptr pc = save->get_pokemon_pc();
        int num_boxes = pc->get_num_boxes();
        EXPECT_EQ(pc->as_vector().size(), num_boxes);
        for(int i = 0; i < num_boxes; ++i) {
            pkmn::pokemon_box::sptr box = pc->get_box(i);
            ASSERT_NE(nullptr, box.get());

            int capacity = box->get_capacity();
            int num_box_pokemon = box->get_num_pokemon();
            EXPECT_GT(capacity, 0);
            EXPECT_GE(num_box_pokemon, 0);
            ASSERT_LE(num_box_pokemon, capacity);
            EXPECT_EQ(box->as_vector().size(), capacity);

            for(int j = 0; j < capacity; ++j) {
                pkmn::pokemon::sptr box_pokemon = box->get_pokemon(j);
                ASSERT_NE(nullptr, box_pokemon.get());

                // Boxes are only contiguous in Game Boy games.
                if(is_gb_game) {
                    if(j < num_box_pokemon) {
                        EXPECT_NE("None", box_pokemon->get_species());
                    } else {
                        EXPECT_EQ("None", box_pokemon->get_species());
                    }
                }
            }
        }
    }
}
