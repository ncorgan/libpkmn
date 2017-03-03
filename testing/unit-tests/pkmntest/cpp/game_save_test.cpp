/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmntest/game_save_test.hpp>

#include <pkmn/exception.hpp>

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>

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

    static PKMN_INLINE bool is_gb_game(
        const std::string &game
    ) {
        return std::find(
                   GB_GAMES,
                   GB_GAMES+6,
                   game
               ) != GB_GAMES+6;
    }

    static PKMN_INLINE bool is_rival_name_set(
        const std::string &game
    ) {
        return std::find(
                   RIVAL_NAME_SET_GAMES,
                   RIVAL_NAME_SET_GAMES+7,
                   game
               ) != RIVAL_NAME_SET_GAMES+7;
    }

    static PKMN_INLINE bool is_male_only(
        const std::string &game
    ) {
        return std::find(
                   MALE_ONLY_GAMES,
                   MALE_ONLY_GAMES+5,
                   game
               ) != MALE_ONLY_GAMES+5;
    }

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
        std::string game = save->get_game();
        int generation = game_generations.at(game);

        test_trainer_name(save);

        save->set_trainer_id(
            is_gb_game(game) ? LIBPKMN_OT_PID : pkmn::pokemon::LIBPKMN_OT_ID
        );
        test_trainer_id(save, is_gb_game(game));
        save->set_trainer_public_id(LIBPKMN_OT_PID);
        test_trainer_id(save, is_gb_game(game));

        test_rival_name(save, is_rival_name_set(game));

        if(is_male_only(game)) {
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
        if(generation >= 2) {
            EXPECT_EQ(pc->as_vector().size(), pc->get_box_names().size());
        }
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
                if(is_gb_game(game)) {
                    if(j < num_box_pokemon) {
                        EXPECT_NE("None", box_pokemon->get_species());
                    } else {
                        EXPECT_EQ("None", box_pokemon->get_species());
                    }
                }
            }
        }
    }

    // TODO: specific case for Gamecube games
    BOOST_STATIC_CONSTEXPR size_t pksav_pc_pokemon_sizes[] = {
        0,
        sizeof(pksav_gen1_pc_pokemon_t),
        sizeof(pksav_gen2_pc_pokemon_t),
        sizeof(pksav_gba_pc_pokemon_t)
    };
    BOOST_STATIC_CONSTEXPR size_t pksav_pokemon_party_data_sizes[] = {
        0,
        sizeof(pksav_gen1_pokemon_party_data_t),
        sizeof(pksav_gen2_pokemon_party_data_t),
        sizeof(pksav_gba_pokemon_party_data_t)
    };

    static void compare_item_lists(
        pkmn::item_list::sptr list1,
        pkmn::item_list::sptr list2
    ) {
        EXPECT_EQ(list1->get_name(), list2->get_name());
        EXPECT_EQ(list1->get_capacity(), list2->get_capacity());
        EXPECT_EQ(list1->get_num_items(), list2->get_num_items());

        const pkmn::item_slots_t& item_slots1 = list1->as_vector();
        const pkmn::item_slots_t& item_slots2 = list2->as_vector();
        ASSERT_EQ(item_slots1.size(), item_slots2.size());
        for(size_t i = 0; i < item_slots1.size(); ++i) {
            EXPECT_EQ(item_slots1[i].item.get_item_id(), item_slots2[i].item.get_item_id());
            EXPECT_EQ(item_slots1[i].amount, item_slots2[i].amount);
        }
    }

    static void compare_pokemon(
        pkmn::pokemon::sptr pokemon1,
        pkmn::pokemon::sptr pokemon2
    ) {
        int generation = game_generations.at(pokemon1->get_game());

        // Names are stored separately in Generations I-II.
        if(generation < 3) {
            EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
            EXPECT_EQ(pokemon1->get_trainer_name(), pokemon2->get_trainer_name());
        }

        // On the C++ level, check the underlying memory.
        EXPECT_EQ(
            0,
            memcmp(
                pokemon1->get_native_pc_data(),
                pokemon2->get_native_pc_data(),
                pksav_pc_pokemon_sizes[generation]
            )
        );
        EXPECT_EQ(
            0,
            memcmp(
                pokemon1->get_native_party_data(),
                pokemon2->get_native_party_data(),
                pksav_pokemon_party_data_sizes[generation]
            )
        );
    }

    void check_two_game_saves_equal(
        pkmn::game_save::sptr save1,
        pkmn::game_save::sptr save2
    ) {
        ASSERT_EQ(save1->get_game(), save2->get_game());
        std::string game = save1->get_game();
        int generation = game_generations.at(game);

        EXPECT_EQ(save1->get_trainer_name(), save2->get_trainer_name());
        EXPECT_EQ(save1->get_trainer_id(), save2->get_trainer_id());
        EXPECT_EQ(save1->get_trainer_public_id(), save2->get_trainer_public_id());

        if(not is_gb_game(game)) {
            EXPECT_EQ(save1->get_trainer_secret_id(), save2->get_trainer_secret_id());
        }
        if(not is_male_only(game)) {
            EXPECT_EQ(save1->get_trainer_gender(), save2->get_trainer_gender());
        }
        if(not is_rival_name_set(game)) {
            EXPECT_EQ(save1->get_rival_name(), save2->get_rival_name());
        }

        EXPECT_EQ(save1->get_money(), save2->get_money());

        pkmn::item_bag::sptr item_bag1 = save1->get_item_bag();
        pkmn::item_bag::sptr item_bag2 = save2->get_item_bag();

        const pkmn::item_pockets_t& pockets1 = item_bag1->get_pockets();
        const pkmn::item_pockets_t& pockets2 = item_bag2->get_pockets();
        ASSERT_EQ(pockets1.size(), pockets2.size());

        const std::vector<std::string>& pocket_names1 = item_bag1->get_pocket_names();
        const std::vector<std::string>& pocket_names2 = item_bag2->get_pocket_names();
        ASSERT_EQ(pocket_names1.size(), pocket_names2.size());
        for(auto iter = pocket_names1.begin(); iter != pocket_names1.end(); ++iter) {
            compare_item_lists(
                pockets1.at(*iter),
                pockets2.at(*iter)
            );
        }

        if(generation <= 3) {
            compare_item_lists(
                save1->get_item_pc(),
                save2->get_item_pc()
            );
        }

        pkmn::pokemon_party::sptr party1 = save1->get_pokemon_party();
        pkmn::pokemon_party::sptr party2 = save2->get_pokemon_party();
        EXPECT_EQ(party1->get_num_pokemon(), party2->get_num_pokemon());
        for(int i = 0; i < 6; ++i) {
            compare_pokemon(
                party1->get_pokemon(i),
                party2->get_pokemon(i)
            );
        }

        if(generation >= 2) {
            const std::vector<std::string> box_names1 = save1->get_pokemon_pc()->get_box_names();
            const std::vector<std::string> box_names2 = save2->get_pokemon_pc()->get_box_names();
            ASSERT_GT(box_names1.size(), 0);
            EXPECT_EQ(box_names1.size(), box_names2.size());

            for(size_t i = 0; i < box_names1.size(); ++i) {
                EXPECT_EQ(box_names1[i], box_names2[i]);
            }
        }

        const pkmn::pokemon_box_list_t& boxes1 = save1->get_pokemon_pc()->as_vector();
        const pkmn::pokemon_box_list_t& boxes2 = save2->get_pokemon_pc()->as_vector();
        EXPECT_EQ(boxes1.size(), boxes2.size());
        for(size_t i = 0; i < boxes1.size(); ++i) {
            pkmn::pokemon_box::sptr box1 = boxes1[i];
            pkmn::pokemon_box::sptr box2 = boxes2[i];
            if(generation >= 2) {
                EXPECT_EQ(box1->get_name(), box2->get_name());
            }

            EXPECT_EQ(box1->get_capacity(), box2->get_capacity());
            EXPECT_EQ(box1->get_num_pokemon(), box2->get_num_pokemon());
            for(int i = 0; i < box1->get_capacity(); ++i) {
                compare_pokemon(
                    box1->get_pokemon(i),
                    box2->get_pokemon(i)
                );
            }
        }
    }
}
