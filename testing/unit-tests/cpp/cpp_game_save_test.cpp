/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmntest/config.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/utils/paths.hpp>

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <map>

static BOOST_CONSTEXPR const char* TOO_LONG_OT_NAME = "LibPKMNLibPKMN";
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_PID = 1351;
BOOST_STATIC_CONSTEXPR uint16_t LIBPKMN_OT_SID = 32123;

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

BOOST_STATIC_CONSTEXPR int RUBY      = 7;
BOOST_STATIC_CONSTEXPR int SAPPHIRE  = 8;
BOOST_STATIC_CONSTEXPR int EMERALD   = 9;

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

namespace fs = boost::filesystem;

namespace pkmntest {

    static const fs::path PKSAV_TEST_SAVES(pkmn_getenv("PKSAV_TEST_SAVES"));

    typedef std::tuple<std::string, std::string, std::string> game_save_test_params_t;

    class game_save_test: public ::testing::TestWithParam<game_save_test_params_t> {
        public:
            PKMNTEST_INLINE pkmn::game_save::sptr get_game_save() {
                return _game_save;
            }

            PKMNTEST_INLINE const game_save_test_params_t& get_params() {
                return _params;
            }

        protected:
            void SetUp() {
                _params = GetParam();
                std::string save_path = fs::path(PKSAV_TEST_SAVES / std::get<2>(_params)).string();
                ASSERT_EQ(std::get<0>(_params), pkmn::game_save::detect_type(save_path));
                _game_save = pkmn::game_save::from_file(save_path);
                ASSERT_EQ(std::get<1>(_params), _game_save->get_game());
            }

        private:

            game_save_test_params_t _params;
            pkmn::game_save::sptr _game_save;
    };

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

    static void game_save_test_common_fields(
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
        if(is_gb_game(game)) {
            EXPECT_THROW(
                save->set_trainer_secret_id(LIBPKMN_OT_SID);
            , pkmn::feature_not_in_game_error);
        } else {
            save->set_trainer_secret_id(LIBPKMN_OT_SID);
            test_trainer_id(save, is_gb_game(game));
        }

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
        , std::out_of_range);
        EXPECT_THROW(
            save->set_money(MONEY_MAX_VALUE+1);
        , std::out_of_range);

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

    static pkmn::pokemon::sptr get_random_pokemon(
        const std::string &game,
        const std::vector<std::string> &pokemon_list,
        const std::vector<std::string> &move_list,
        const std::vector<std::string> &item_list
    ) {
        int generation = game_generations.at(game);

        // Don't deal with Deoxys issues here.
        std::string species;
        if(generation == 3) {
            do {
                species = pokemon_list[std::rand() % pokemon_list.size()];
            } while(species == "Deoxys");
        } else {
            species = pokemon_list[std::rand() % pokemon_list.size()];
        }
        pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                      species,
                                      game,
                                      "",
                                      ((std::rand() % 99) + 2)
                                  );
        for(int i = 0; i < 4; ++i) {
            ret->set_move(
                move_list[std::rand() % move_list.size()],
                i
            );
        }

        if(generation >= 2) {

            // Keep going until one is holdable
            while(ret->get_held_item().get_item_id() == 0) {
                try {
                    ret->set_held_item(
                        item_list[std::rand() % item_list.size()]
                    );
                } catch(std::invalid_argument&) {}
            }
        }

        return ret;
    }

    void randomize_items(
        pkmn::game_save::sptr save,
        const std::vector<std::string> &item_list
    ) {
        // Clear out what items the save happens to have to put it in a known state.
        // TODO: when clear() added to item_list
        (void)save;
        (void)item_list;
    }

    void randomize_pokemon(
        pkmn::game_save::sptr save,
        const std::vector<std::string> &item_list
    ) {
        int generation = game_generations.at(save->get_game());
        std::vector<std::string> pokemon_list = pkmn::database::get_pokemon_list(generation, true);
        std::vector<std::string> move_list = pkmn::database::get_move_list(save->get_game());

        pkmn::pokemon_party::sptr party = save->get_pokemon_party();
        for(int i = 0; i < 6; ++i) {
            party->set_pokemon(
                i, 
                get_random_pokemon(
                    save->get_game(),
                    pokemon_list,
                    move_list,
                    item_list
                )
            );
        }
        pkmn::pokemon_pc::sptr pc = save->get_pokemon_pc();
        const pkmn::pokemon_box_list_t& boxes = pc->as_vector();
        int capacity = boxes[0]->get_capacity();
        for(size_t i = 0; i < boxes.size(); ++i) {
            for(int j = 0; j < capacity; ++j) {
                boxes[i]->set_pokemon(
                    j, 
                    get_random_pokemon(
                        save->get_game(),
                        pokemon_list,
                        move_list,
                        item_list
                    )
                );
            }
        }
    }

    // TODO: specific case for Gamecube games
    BOOST_STATIC_CONSTEXPR size_t pksav_item_pc_sizes[] = {
        0,
        sizeof(pksav_gen1_item_pc_t),
        sizeof(pksav_gen2_item_pc_t),
        (sizeof(pksav_item_t)*50)
    };

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

    static void compare_game_saves(
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
        void* native1 = item_bag1->get_native();
        void* native2 = item_bag2->get_native();
        size_t item_bag_size = 0;
        switch(generation) {
            case 1:
                item_bag_size = sizeof(pksav_gen1_item_bag_t);
                break;

            case 2:
                item_bag_size = sizeof(pksav_gen2_item_bag_t);
                break;

            case 3:
                switch(save1->get_item_pc()->as_vector().at(0).item.get_item_id()) {
                    case RUBY:
                    case SAPPHIRE:
                        item_bag_size = sizeof(pksav_rs_item_storage_t);
                        break;

                    case EMERALD:
                        item_bag_size = sizeof(pksav_emerald_item_storage_t);
                        break;

                    default:
                        item_bag_size = sizeof(pksav_frlg_item_storage_t);
                        break;
                }
                break;

            default:
                break;
        }

        // On the C++ level, check the underlying memory.
        EXPECT_EQ(0, memcmp(native1, native2, item_bag_size));

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

            // On the C++ level, check the underlying memory.
            EXPECT_EQ(
                0,
                memcmp(
                    save1->get_item_pc()->get_native(),
                    save2->get_item_pc()->get_native(),
                    pksav_item_pc_sizes[generation]
                )
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

    static const fs::path TMP_DIR(pkmn::get_tmp_dir());

    static const game_save_test_params_t params[] = {
        game_save_test_params_t("Red/Blue/Yellow", "Red", "red_blue/pokemon_red.sav"),
        game_save_test_params_t("Red/Blue/Yellow", "Yellow", "yellow/pokemon_yellow.sav"),
        game_save_test_params_t("Gold/Silver", "Gold", "gold_silver/pokemon_gold.sav"),
        game_save_test_params_t("Crystal", "Crystal", "crystal/pokemon_crystal.sav"),
        game_save_test_params_t("Ruby/Sapphire", "Ruby", "ruby_sapphire/pokemon_ruby.sav"),
        game_save_test_params_t("Emerald", "Emerald", "emerald/pokemon_emerald.sav"),
        game_save_test_params_t("FireRed/LeafGreen", "FireRed", "firered_leafgreen/pokemon_firered.sav")
    };

    TEST_P(game_save_test, game_save_test) {
        pkmn::game_save::sptr save = get_game_save();

        std::vector<std::string> item_list = pkmn::database::get_item_list(save->get_game());

        pkmntest::game_save_test_common_fields(save);
        pkmntest::randomize_items(
            save,
            item_list
        );
        pkmntest::randomize_pokemon(
            save,
            item_list
        );

        fs::path temp_save_path = TMP_DIR / str(boost::format("%s_%d.sav") % save->get_game().c_str() % std::rand());
        save->save_as(temp_save_path.string());

        pkmn::game_save::sptr save2 = pkmn::game_save::from_file(temp_save_path.string());
        pkmntest::compare_game_saves(save, save2);

        std::remove(temp_save_path.string().c_str());
    }

    INSTANTIATE_TEST_CASE_P(
        cpp_game_save_test,
        game_save_test,
        ::testing::ValuesIn(params)
    );
}
