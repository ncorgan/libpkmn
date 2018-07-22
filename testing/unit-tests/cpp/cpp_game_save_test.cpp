/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"
#include "private_exports.hpp"
#include "pksav/enum_maps.hpp"
#include "types/rng.hpp"
#include "utils/misc.hpp"

#include <pkmntest/config.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/database/lists.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/enums/enum_to_string.hpp>
#include <pkmn/utils/paths.hpp>

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include "libpkmgc_includes.hpp"

#include <gtest/gtest.h>

#include <boost/assign.hpp>
#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <map>

static BOOST_CONSTEXPR const char* TOO_LONG_OT_NAME = "LibPKMNLibPKMN";
BOOST_STATIC_CONSTEXPR uint16_t DEFAULT_TRAINER_PID = 1351;
BOOST_STATIC_CONSTEXPR uint16_t DEFAULT_TRAINER_SID = 32123;

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

static const std::vector<pkmn::e_game> RIVAL_NAME_SET_GAMES =
{
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD,
    pkmn::e_game::BLACK,
    pkmn::e_game::WHITE,
    pkmn::e_game::X,
    pkmn::e_game::Y
};

namespace fs = boost::filesystem;

namespace pkmntest {

    static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));
    static const fs::path PKSAV_TEST_SAVES(pkmn_getenv("PKSAV_TEST_SAVES"));

    typedef std::tuple<
                pkmn::e_game_save_type,
                pkmn::e_game,
                std::string>
            game_save_test_params_t;

    class game_save_test: public ::testing::TestWithParam<game_save_test_params_t>
    {
        public:
            inline pkmn::game_save::sptr get_game_save()
            {
                return _game_save;
            }

            inline const game_save_test_params_t& get_params()
            {
                return _params;
            }

            inline bool is_gamecube()
            {
                return _gamecube;
            }

        protected:
            void SetUp()
            {
                _params = GetParam();

                _gamecube = (std::get<1>(_params) == pkmn::e_game::COLOSSEUM) ||
                            (std::get<1>(_params) == pkmn::e_game::XD);

                std::string save_path = fs::path((_gamecube ? LIBPKMN_TEST_FILES : PKSAV_TEST_SAVES) / std::get<2>(_params)).string();
                ASSERT_EQ(std::get<0>(_params), pkmn::game_save::detect_type(save_path));
                _game_save = pkmn::game_save::from_file(save_path);
                ASSERT_EQ(std::get<1>(_params), _game_save->get_game());
            }

        private:

            game_save_test_params_t _params;
            bool _gamecube;
            pkmn::game_save::sptr _game_save;
    };

    static inline bool is_gb_game(pkmn::e_game game)
    {
        return (pkmn::priv::game_enum_to_generation(game) <= 2);
    }

    static inline bool is_rival_name_set(pkmn::e_game game)
    {
        return (std::find(
                    RIVAL_NAME_SET_GAMES.begin(),
                    RIVAL_NAME_SET_GAMES.end(),
                    game
                ) != RIVAL_NAME_SET_GAMES.end());
    }

    static inline bool is_male_only(pkmn::e_game game)
    {
        return (is_gb_game(game) && (game != pkmn::e_game::CRYSTAL)) ||
               (game == pkmn::e_game::COLOSSEUM) ||
               (game == pkmn::e_game::XD);
    }

    static void test_trainer_name(
        const pkmn::game_save::sptr& save
    )
    {
        EXPECT_THROW(
            save->set_trainer_name("");
        , std::invalid_argument);
        EXPECT_THROW(
            save->set_trainer_name(TOO_LONG_OT_NAME);
        , std::invalid_argument);

        save->set_trainer_name(pkmn::pokemon::DEFAULT_TRAINER_NAME);
        EXPECT_EQ(std::string(pkmn::pokemon::DEFAULT_TRAINER_NAME), save->get_trainer_name());
    }

    static void test_trainer_id(
        const pkmn::game_save::sptr& save,
        bool is_gb_game
    )
    {
        EXPECT_EQ((is_gb_game ? DEFAULT_TRAINER_PID : pkmn::pokemon::DEFAULT_TRAINER_ID), save->get_trainer_id());
        EXPECT_EQ(DEFAULT_TRAINER_PID, save->get_trainer_public_id());
        if(is_gb_game) {
            EXPECT_THROW(
                save->get_trainer_secret_id();
            , pkmn::feature_not_in_game_error);
        } else {
            EXPECT_EQ(DEFAULT_TRAINER_SID, save->get_trainer_secret_id());
        }
    }

    static void test_rival_name(
        const pkmn::game_save::sptr& save,
        bool is_rival_name_set
    )
    {
        if(is_rival_name_set) {
            EXPECT_THROW(
                save->set_rival_name(pkmn::pokemon::DEFAULT_TRAINER_NAME);
            , pkmn::feature_not_in_game_error);
        } else {
            EXPECT_THROW(
                save->set_rival_name("");
            , std::invalid_argument);
            EXPECT_THROW(
                save->set_rival_name(TOO_LONG_OT_NAME);
            , std::invalid_argument);

            save->set_rival_name(pkmn::pokemon::DEFAULT_TRAINER_NAME);
            EXPECT_EQ(std::string(pkmn::pokemon::DEFAULT_TRAINER_NAME), save->get_rival_name());
        }
    }

    static void test_time_played(
        const pkmn::game_save::sptr& save
    )
    {
        if((save->get_game() == pkmn::e_game::COLOSSEUM) ||
           (save->get_game() == pkmn::e_game::XD))
        {
            ASSERT_THROW(
                save->get_time_played();
            , pkmn::unimplemented_error);
            ASSERT_THROW(
                save->set_time_played(pkmn::time_duration());
            , pkmn::unimplemented_error);
        }
        else
        {
            // Generation I doesn't have frames.
            int generation = pkmn::priv::game_enum_to_generation(save->get_game());

            pkmn::rng<int> int_rng;
            pkmn::time_duration duration(
                                    int_rng.rand(0, 255),
                                    int_rng.rand(0, 59),
                                    int_rng.rand(0, 59),
                                    0
                                );
            if(generation != 1)
            {
                duration.frames = int_rng.rand(0, 59);
            }

            save->set_time_played(duration);
            ASSERT_EQ(duration, save->get_time_played());

            // Test invalid times.

            static const pkmn::time_duration too_low_hours_time_played(-1,0,0,0);
            static const pkmn::time_duration too_high_hours_time_played(999999,0,0,0);

            static const pkmn::time_duration too_low_minutes_time_played(0,-1,0,0);
            static const pkmn::time_duration too_high_minutes_time_played(0,999999,0,0);

            static const pkmn::time_duration too_low_seconds_time_played(0,0,-1,0);
            static const pkmn::time_duration too_high_seconds_time_played(0,0,999999,0);

            static const std::vector<pkmn::time_duration> invalid_time_durations =
            {
                too_low_hours_time_played,
                too_high_hours_time_played,
                too_low_minutes_time_played,
                too_high_minutes_time_played,
                too_low_seconds_time_played,
                too_high_seconds_time_played,
            };

            for(const pkmn::time_duration& invalid_time_duration: invalid_time_durations)
            {
                ASSERT_THROW(
                    save->set_time_played(invalid_time_duration);
                , std::out_of_range);
            }

            if(generation != 1)
            {
                static const pkmn::time_duration too_low_frames_time_played(0,0,0,-1);
                static const pkmn::time_duration too_high_frames_time_played(0,0,0,999999);

                ASSERT_THROW(
                    save->set_time_played(too_low_frames_time_played);
                , std::out_of_range);
                ASSERT_THROW(
                    save->set_time_played(too_high_frames_time_played);
                , std::out_of_range);
            }
        }
    }

    static void game_save_test_common_fields(
        const pkmn::game_save::sptr& save
    )
    {
        pkmn::e_game game = save->get_game();
        int generation = pkmn::priv::game_enum_to_generation(game);

        test_trainer_name(save);

        save->set_trainer_id(
            is_gb_game(game) ? DEFAULT_TRAINER_PID : pkmn::pokemon::DEFAULT_TRAINER_ID
        );
        test_trainer_id(save, is_gb_game(game));
        save->set_trainer_public_id(DEFAULT_TRAINER_PID);
        test_trainer_id(save, is_gb_game(game));
        if(is_gb_game(game))
        {
            EXPECT_THROW(
                save->set_trainer_secret_id(DEFAULT_TRAINER_SID);
            , pkmn::feature_not_in_game_error);
        }
        else
        {
            save->set_trainer_secret_id(DEFAULT_TRAINER_SID);
            test_trainer_id(save, is_gb_game(game));
        }

        test_rival_name(save, is_rival_name_set(game));

        if(is_male_only(game))
        {
            EXPECT_EQ(pkmn::e_gender::MALE, save->get_trainer_gender());
            EXPECT_THROW(
                save->set_trainer_gender(pkmn::e_gender::MALE);
            , pkmn::feature_not_in_game_error);
            EXPECT_THROW(
                save->set_trainer_gender(pkmn::e_gender::FEMALE);
            , pkmn::feature_not_in_game_error);
        }
        else
        {
            save->set_trainer_gender(pkmn::e_gender::MALE);
            EXPECT_EQ(pkmn::e_gender::MALE, save->get_trainer_gender());
            save->set_trainer_gender(pkmn::e_gender::FEMALE);
            EXPECT_EQ(pkmn::e_gender::FEMALE, save->get_trainer_gender());
            EXPECT_THROW(
                save->set_trainer_gender(pkmn::e_gender::GENDERLESS);
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
        for(int i = 0; i < 6; ++i)
        {
            pkmn::pokemon::sptr party_pokemon = party->get_pokemon(i);
            ASSERT_NE(nullptr, party_pokemon.get());
            if(i < num_party_pokemon)
            {
                EXPECT_NE(
                    pkmn::e_species::NONE,
                    party_pokemon->get_species()
                );
            }
            else
            {
                EXPECT_EQ(
                    pkmn::e_species::NONE,
                    party_pokemon->get_species()
                );
            }
        }

        pkmn::pokemon_pc::sptr pc = save->get_pokemon_pc();
        int num_boxes = pc->get_num_boxes();
        EXPECT_EQ(pc->as_vector().size(), num_boxes);
        if(generation >= 2)
        {
            EXPECT_EQ(pc->as_vector().size(), pc->get_box_names().size());
        }
        for(int i = 0; i < num_boxes; ++i)
        {
            pkmn::pokemon_box::sptr box = pc->get_box(i);
            ASSERT_NE(nullptr, box.get());

            int capacity = box->get_capacity();
            int num_box_pokemon = box->get_num_pokemon();
            EXPECT_GT(capacity, 0);
            EXPECT_GE(num_box_pokemon, 0);
            ASSERT_LE(num_box_pokemon, capacity);
            EXPECT_EQ(box->as_vector().size(), capacity);

            for(int j = 0; j < capacity; ++j)
            {
                pkmn::pokemon::sptr box_pokemon = box->get_pokemon(j);
                ASSERT_NE(nullptr, box_pokemon.get());

                // Boxes are only contiguous in Game Boy games.
                if(is_gb_game(game))
                {
                    if(j < num_box_pokemon)
                    {
                        EXPECT_NE(
                            pkmn::e_species::NONE,
                            box_pokemon->get_species()
                        );
                    }
                    else
                    {
                        EXPECT_EQ(
                            pkmn::e_species::NONE,
                            box_pokemon->get_species()
                        );
                    }
                }
            }
        }

        if((save->get_game() != pkmn::e_game::COLOSSEUM) &&
           (save->get_game() != pkmn::e_game::XD))
        {
            // Test Pokédex. Any Pokémon in the party and PC must be in the
            // Pokédex.
            pkmn::pokedex::sptr pokedex = save->get_pokedex();
            EXPECT_GE(pokedex->get_num_seen(), pokedex->get_num_caught());

            const pkmn::pokemon_list_t& party_vector = save->get_pokemon_party()->as_vector();
            for(const auto& pokemon: party_vector)
            {
                pkmn::e_species species = pokemon->get_species();
                if(species != pkmn::e_species::NONE)
                {
                    EXPECT_TRUE(pokedex->has_seen(species));
                    EXPECT_TRUE(pokedex->has_caught(species));
                }
            }

            const pkmn::pokemon_box_list_t& pc_vector = save->get_pokemon_pc()->as_vector();
            for(const auto& box: pc_vector)
            {
                const pkmn::pokemon_list_t& box_vector = box->as_vector();
                for(const auto& pokemon: box_vector)
                {
                    pkmn::e_species species = pokemon->get_species();
                    if((species != pkmn::e_species::NONE) and (not pokemon->is_egg()))
                    {
                        EXPECT_TRUE(pokedex->has_seen(species));
                        EXPECT_TRUE(pokedex->has_caught(species));
                    }
                }
            }

            // Make sure that when a Pokémon is added to the party or PC, it's
            // added to the Pokédex. Manually remove the test species from the
            // Pokédex to confirm this behavior.

            const pkmn::e_species test_species1 = pkmn::e_species::BULBASAUR;
            const pkmn::e_species test_species2 = pkmn::e_species::CHARMANDER;

            pokedex->set_has_seen(test_species1, false);
            ASSERT_FALSE(pokedex->has_seen(test_species1));
            ASSERT_FALSE(pokedex->has_caught(test_species1));

            pokedex->set_has_seen(test_species2, false);
            ASSERT_FALSE(pokedex->has_seen(test_species2));
            ASSERT_FALSE(pokedex->has_caught(test_species2));

            pkmn::pokemon::sptr test_pokemon1 = pkmn::pokemon::make(
                                                    test_species1,
                                                    game,
                                                    "",
                                                    5
                                                );
            pkmn::pokemon::sptr test_pokemon2 = pkmn::pokemon::make(
                                                    test_species2,
                                                    game,
                                                    "",
                                                    5
                                                );

            save->get_pokemon_party()->set_pokemon(0, test_pokemon1);
            EXPECT_TRUE(pokedex->has_seen(test_species1));
            EXPECT_TRUE(pokedex->has_caught(test_species1));

            save->get_pokemon_pc()->get_box(0)->set_pokemon(0, test_pokemon2);
            EXPECT_TRUE(pokedex->has_seen(test_species2));
            EXPECT_TRUE(pokedex->has_caught(test_species2));
        }

        test_time_played(save);
    }

    static void game_save_test_attributes(
        const pkmn::game_save::sptr& save
    )
    {
        pkmn::e_game game = save->get_game();
        int generation = pkmn::priv::game_enum_to_generation(game);
        pkmn::rng<int> int_rng;

        switch(generation)
        {
            case 1:
            {
                ASSERT_TRUE(
                    pkmn::does_vector_contain_value<std::string>(
                        save->get_numeric_attribute_names(),
                        "Casino coins"
                    )
                );
                EXPECT_GE(
                    save->get_numeric_attribute("Casino coins"),
                    0
                );
                EXPECT_LE(
                    save->get_numeric_attribute("Casino coins"),
                    9999
                );

                int num_casino_coins = int_rng.rand(0, 99);
                save->set_numeric_attribute("Casino coins", num_casino_coins);
                EXPECT_EQ(
                    num_casino_coins,
                    save->get_numeric_attribute("Casino coins")
                );

                if(game == pkmn::e_game::YELLOW)
                {
                    ASSERT_TRUE(
                        pkmn::does_vector_contain_value<std::string>(
                            save->get_numeric_attribute_names(),
                            "Pikachu friendship"
                        )
                    );
                    EXPECT_GE(
                        save->get_numeric_attribute("Pikachu friendship"),
                        0
                    );
                    EXPECT_LE(
                        save->get_numeric_attribute("Pikachu friendship"),
                        255
                    );

                    int pikachu_friendship = int_rng.rand(0, 255);
                    save->set_numeric_attribute("Pikachu friendship", pikachu_friendship);
                    EXPECT_EQ(
                        pikachu_friendship,
                        save->get_numeric_attribute("Pikachu friendship")
                    );
                }
                else
                {
                    ASSERT_FALSE(
                        pkmn::does_vector_contain_value<std::string>(
                            save->get_numeric_attribute_names(),
                            "Pikachu friendship"
                        )
                    );
                }
                break;
            }

            case 2:
            {
                // Check for the default palette. Boys are red, girls are blue.
                pkmn::e_gender gender = save->get_trainer_gender();
                if(gender == pkmn::e_gender::MALE)
                {
                    ASSERT_EQ("Red", save->get_string_attribute("Player palette"));
                }
                else
                {
                    ASSERT_EQ("Blue", save->get_string_attribute("Player palette"));
                }

                // Now make sure setting to each works.
                static const std::vector<std::string> GEN2_PALETTES =
                {
                    "Red", "Blue", "Green", "Brown",
                    "Orange", "Gray", "Dark Green", "Dark Red"
                };
                for(const std::string& palette: GEN2_PALETTES)
                {
                    save->set_string_attribute("Player palette", palette);
                    ASSERT_EQ(palette, save->get_string_attribute("Player palette"));
                }

                save->set_boolean_attribute("Daylight savings time?", false);
                ASSERT_FALSE(save->get_boolean_attribute("Daylight savings time?"));
                save->set_boolean_attribute("Daylight savings time?", true);
                ASSERT_TRUE(save->get_boolean_attribute("Daylight savings time?"));

                break;
            }

            case 3:
            {
                if((save->get_game() != pkmn::e_game::COLOSSEUM) &&
                   (save->get_game() != pkmn::e_game::XD))
                {
                    // Casino coins
                    ASSERT_TRUE(
                        pkmn::does_vector_contain_value<std::string>(
                            save->get_numeric_attribute_names(),
                            "Casino coins"
                        )
                    );
                    EXPECT_GE(
                        save->get_numeric_attribute("Casino coins"),
                        0
                    );
                    EXPECT_LE(
                        save->get_numeric_attribute("Casino coins"),
                        9999
                    );

                    int num_casino_coins = int_rng.rand(0, 9999);
                    save->set_numeric_attribute("Casino coins", num_casino_coins);
                    EXPECT_EQ(
                        num_casino_coins,
                        save->get_numeric_attribute("Casino coins")
                    );

                    // National Dex unlocked?
                    ASSERT_TRUE(
                        pkmn::does_vector_contain_value<std::string>(
                            save->get_boolean_attribute_names(),
                            "National Dex unlocked?"
                        )
                    );

                    save->set_boolean_attribute("National Dex unlocked?", false);
                    EXPECT_FALSE(save->get_boolean_attribute("National Dex unlocked?"));

                    save->set_boolean_attribute("National Dex unlocked?", true);
                    EXPECT_TRUE(save->get_boolean_attribute("National Dex unlocked?"));
                }
                break;
            }

            default:
                break;
        }
    }

    static pkmn::pokemon::sptr get_random_pokemon(
        pkmn::e_game game,
        const std::vector<pkmn::e_species>& pokemon_list,
        const std::vector<std::string>& move_list,
        const std::vector<pkmn::e_item>& item_list
    )
    {
        int generation = pkmn::priv::game_enum_to_generation(game);
        pkmn::rng<uint32_t> rng;
        (void)move_list;

        // Don't deal with Deoxys or Unown issues here.
        pkmn::e_species species = pkmn::e_species::NONE;
        if(generation == 3)
        {
            do
            {
                species = pokemon_list[rng.rand() % pokemon_list.size()];
            }
            while((species == pkmn::e_species::UNOWN) ||
                  (species == pkmn::e_species::DEOXYS));
        }
        else
        {
            species = pokemon_list[rng.rand() % pokemon_list.size()];
        }
        pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                      species,
                                      game,
                                      "",
                                      ((rng.rand() % 99) + 2)
                                  );
        for(int i = 0; i < 4; ++i)
        {
            std::string move = "";
            do
            {
                move = move_list[rng.rand() % move_list.size()];
            }
            while(move.find("Shadow") == 0);
            ret->set_move(move, i);
        }

        if(generation >= 2)
        {
            // Keep going until one is holdable
            while(ret->get_held_item() == pkmn::e_item::NONE)
            {
                try
                {
                    ret->set_held_item(
                        item_list[rng.rand() % item_list.size()]
                    );
                } catch(std::invalid_argument&) {}
            }
        }

        // Set condition (TODO: add enum_maps to pkmntest library)
        /*std::vector<std::string> conditions;
        if(generation <= 2)
        {
            for(const auto& condition: pksav::get_gb_condition_bimap().left)
            {
                conditions.emplace_back(condition.first);
            }
        }
        else
        {
            for(const auto& condition: pksav::get_condition_mask_bimap().left)
            {
                conditions.emplace_back(condition.first);
            }
        }

        ret->set_condition(conditions[rng.rand() % conditions.size()]);*/

        return ret;
    }

    void randomize_items(
        const pkmn::game_save::sptr& save,
        const std::vector<pkmn::e_item>& item_list
    ) {
        // Clear out what items the save happens to have to put it in a known state.
        // TODO: when clear() added to item_list
        (void)save;
        (void)item_list;
    }

    void randomize_pokemon(
        const pkmn::game_save::sptr& save,
        const std::vector<pkmn::e_item>& item_list
    )
    {
        int generation = pkmn::priv::game_enum_to_generation(save->get_game());
        std::vector<pkmn::e_species> pokemon_list = pkmn::database::get_pokemon_list(generation, true);
        std::vector<std::string> move_list = pkmn::database::get_move_name_list(save->get_game());

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
        sizeof(struct pksav_gen1_item_pc),
        sizeof(struct pksav_gen2_item_pc),
        (sizeof(struct pksav_item)*50)
    };

    // TODO: specific case for Gamecube games
    BOOST_STATIC_CONSTEXPR size_t pksav_pc_pokemon_sizes[] = {
        0,
        sizeof(struct pksav_gen1_pc_pokemon),
        sizeof(struct pksav_gen2_pc_pokemon),
        sizeof(struct pksav_gba_pc_pokemon)
    };
    BOOST_STATIC_CONSTEXPR size_t pksav_pokemon_party_data_sizes[] = {
        0,
        sizeof(struct pksav_gen1_pokemon_party_data),
        sizeof(struct pksav_gen2_pokemon_party_data),
        sizeof(struct pksav_gba_pokemon_party_data)
    };

    static void compare_item_lists(
        const pkmn::item_list::sptr& list1,
        const pkmn::item_list::sptr& list2
    )
    {
        EXPECT_EQ(list1->get_name(), list2->get_name());
        EXPECT_EQ(list1->get_capacity(), list2->get_capacity());
        EXPECT_EQ(list1->get_num_items(), list2->get_num_items());

        const pkmn::item_slots_t& item_slots1 = list1->as_vector();
        const pkmn::item_slots_t& item_slots2 = list2->as_vector();
        ASSERT_EQ(item_slots1.size(), item_slots2.size());
        for(size_t i = 0; i < item_slots1.size(); ++i)
        {
            EXPECT_EQ(item_slots1[i].item, item_slots2[i].item) << list1->get_name();
            EXPECT_EQ(item_slots1[i].amount, item_slots2[i].amount) << list1->get_name();
        }
    }

    static void compare_pokedexes(
        pkmn::pokedex::sptr pokedex1,
        pkmn::pokedex::sptr pokedex2
    )
    {
        EXPECT_EQ(pokedex1->get_all_seen(), pokedex2->get_all_seen());
        EXPECT_EQ(pokedex1->get_all_caught(), pokedex2->get_all_caught());
    }

    static void compare_pokemon(
        const pkmn::pokemon::sptr& pokemon1,
        const pkmn::pokemon::sptr& pokemon2
    )
    {
        pkmn::e_game game = pokemon1->get_game();
        int generation = pkmn::priv::game_enum_to_generation(game);

        // Names are stored separately in Generations I-II.
        if(generation < 3)
        {
            EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
            EXPECT_EQ(pokemon1->get_original_trainer_name(), pokemon2->get_original_trainer_name());
        }

        // On the C++ level, check the underlying representation.
        if((pokemon1->get_game() == pkmn::e_game::COLOSSEUM) ||
           (pokemon1->get_game() == pkmn::e_game::XD))
        {
            const LibPkmGC::GC::Pokemon* native1 = reinterpret_cast<const LibPkmGC::GC::Pokemon*>(pokemon1->get_native_pc_data());
            const LibPkmGC::GC::Pokemon* native2 = reinterpret_cast<const LibPkmGC::GC::Pokemon*>(pokemon2->get_native_pc_data());

            EXPECT_EQ(native1->species, native2->species);
            EXPECT_EQ(native1->SID, native2->SID);
            EXPECT_EQ(native1->TID, native2->TID);
            EXPECT_EQ(native1->PID, native2->PID);
            EXPECT_EQ(native1->shadowPkmID, native2->shadowPkmID);
            EXPECT_STREQ(native1->name->toUTF8(), native2->name->toUTF8());
            EXPECT_STREQ(native1->OTName->toUTF8(), native2->OTName->toUTF8());

            pkmn::ability_pair_t abilities = pokemon1->get_database_entry().get_abilities();

            EXPECT_EQ(
                native1->getAbility(),
                native2->getAbility()
            ) << pkmn::species_to_string(pokemon1->get_species()) << " "
              << LibPkmGC::Localization::getPokemonAbilityName(LibPkmGC::English, native1->getAbility()) << " "
              << LibPkmGC::Localization::getPokemonAbilityName(LibPkmGC::English, native2->getAbility()) << " "
              << "(" << pkmn::ability_to_string(abilities.first) << ", "
              << pkmn::ability_to_string(abilities.second) << ")";

            const pkmn::move_slots_t& moves1 = pokemon1->get_moves();
            const pkmn::move_slots_t& moves2 = pokemon2->get_moves();
            for(size_t i = 0; i < 4; ++i)
            {
                // These may be different if Shadow moves were mistakenly allowed to be set.
                EXPECT_EQ(moves1.at(i).move, moves2.at(i).move);
                EXPECT_EQ(native1->moves[i].move, native2->moves[i].move);
                EXPECT_EQ(native1->moves[i].currentPPs, native2->moves[i].currentPPs);
            }
            for(size_t i = 0; i < 6; ++i)
            {
                EXPECT_EQ(native1->EVs[i], native2->EVs[i]);
                EXPECT_EQ(native1->IVs[i], native2->IVs[i]);
                EXPECT_EQ(native1->partyData.stats[i], native2->partyData.stats[i]);
            }
        }
        else
        {
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
    }

    static void compare_libpkmgc_items(
        const LibPkmGC::Item* items1,
        const LibPkmGC::Item* items2,
        size_t length
    )
    {
        for(size_t i = 0; i < length; ++i)
        {
            EXPECT_EQ(items1[i].index, items2[i].index) << i;
            EXPECT_EQ(items1[i].quantity, items2[i].quantity) << i;
        }
    }

    static void compare_game_saves(
        const pkmn::game_save::sptr& save1,
        const pkmn::game_save::sptr& save2
    )
    {
        ASSERT_EQ(save1->get_game(), save2->get_game());
        pkmn::e_game game = save1->get_game();
        int generation = pkmn::priv::game_enum_to_generation(game);

        EXPECT_EQ(save1->get_trainer_name(), save2->get_trainer_name());
        EXPECT_EQ(save1->get_trainer_id(), save2->get_trainer_id());
        EXPECT_EQ(save1->get_trainer_public_id(), save2->get_trainer_public_id());

        if(not is_gb_game(game))
        {
            EXPECT_EQ(save1->get_trainer_secret_id(), save2->get_trainer_secret_id());
        }
        if(not is_male_only(game))
        {
            EXPECT_EQ(save1->get_trainer_gender(), save2->get_trainer_gender());
        }
        if(not is_rival_name_set(game))
        {
            EXPECT_EQ(save1->get_rival_name(), save2->get_rival_name());
        }

        EXPECT_EQ(save1->get_money(), save2->get_money());

        pkmn::item_bag::sptr item_bag1 = save1->get_item_bag();
        pkmn::item_bag::sptr item_bag2 = save2->get_item_bag();
        void* native1 = item_bag1->get_native();
        void* native2 = item_bag2->get_native();

        // On the C++ level, check the underlying memory.
        if((save1->get_game() == pkmn::e_game::COLOSSEUM) ||
           (save1->get_game() == pkmn::e_game::XD))
        {
            const LibPkmGC::GC::BagData* native1 = reinterpret_cast<LibPkmGC::GC::BagData*>(item_bag1->get_native());
            const LibPkmGC::GC::BagData* native2 = reinterpret_cast<LibPkmGC::GC::BagData*>(item_bag2->get_native());

            compare_libpkmgc_items(
                native1->regularItems,
                native2->regularItems,
                native1->nbRegularItems
            );
            compare_libpkmgc_items(
                native1->keyItems,
                native2->keyItems,
                43
            );
            compare_libpkmgc_items(
                native1->pokeballs,
                native2->pokeballs,
                16
            );
            compare_libpkmgc_items(
                native1->TMs,
                native2->TMs,
                64
            );
            compare_libpkmgc_items(
                native1->berries,
                native2->berries,
                46
            );
            compare_libpkmgc_items(
                native1->colognes,
                native2->colognes,
                3
            );
            if(game == pkmn::e_game::XD)
            {
                compare_libpkmgc_items(
                    dynamic_cast<const LibPkmGC::XD::BagData*>(native1)->battleCDs,
                    dynamic_cast<const LibPkmGC::XD::BagData*>(native2)->battleCDs,
                    60
                );
            }
        }
        else
        {
            size_t item_bag_size = 0;
            switch(generation)
            {
                case 1:
                    item_bag_size = sizeof(struct pksav_gen1_item_bag);
                    break;

                case 2:
                    item_bag_size = sizeof(struct pksav_gen2_item_bag);
                    break;

                case 3:
                    switch(save1->get_game())
                    {
                        case pkmn::e_game::RUBY:
                        case pkmn::e_game::SAPPHIRE:
                            item_bag_size = sizeof(struct pksav_gba_rs_item_bag);
                            break;

                        case pkmn::e_game::EMERALD:
                            item_bag_size = sizeof(struct pksav_gba_emerald_item_bag);
                            break;

                        default:
                            item_bag_size = sizeof(struct pksav_gba_frlg_item_bag);
                            break;
                    }
                    break;

                default:
                    break;
            }
            EXPECT_EQ(0, memcmp(native1, native2, item_bag_size));
        }

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
            if((save1->get_game() == pkmn::e_game::COLOSSEUM) ||
               (save1->get_game() == pkmn::e_game::XD))
            {
                compare_libpkmgc_items(
                    reinterpret_cast<const LibPkmGC::Item*>(save1->get_item_pc()->get_native()),
                    reinterpret_cast<const LibPkmGC::Item*>(save2->get_item_pc()->get_native()),
                    235
                );
            }
            else
            {
                EXPECT_EQ(
                    0,
                    memcmp(
                        save1->get_item_pc()->get_native(),
                        save2->get_item_pc()->get_native(),
                        pksav_item_pc_sizes[generation]
                    )
                );
            }
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
        for(size_t box_index = 0; box_index < boxes1.size(); ++box_index)
        {
            const pkmn::pokemon_box::sptr& box1 = boxes1[box_index];
            const pkmn::pokemon_box::sptr& box2 = boxes2[box_index];
            if(generation >= 2)
            {
                EXPECT_EQ(box1->get_name(), box2->get_name());
            }

            EXPECT_EQ(box1->get_capacity(), box2->get_capacity());
            EXPECT_EQ(box1->get_num_pokemon(), box2->get_num_pokemon());
            for(int pokemon_index = 0;
                pokemon_index < box1->get_capacity();
                ++pokemon_index)
            {
                compare_pokemon(
                    box1->get_pokemon(pokemon_index),
                    box2->get_pokemon(pokemon_index)
                );
            }
        }

        if((save1->get_game() != pkmn::e_game::COLOSSEUM) &&
           (save1->get_game() != pkmn::e_game::XD))
        {
            compare_pokedexes(
                save1->get_pokedex(),
                save2->get_pokedex()
            );
        }
    }

    static const fs::path TMP_DIR(pkmn::get_tmp_dir());

    static const game_save_test_params_t params[] = {
        game_save_test_params_t(
            pkmn::e_game_save_type::RED_BLUE,
            pkmn::e_game::RED,
            "red_blue/pokemon_red.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::YELLOW,
            pkmn::e_game::YELLOW,
            "yellow/pokemon_yellow.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::GOLD_SILVER,
            pkmn::e_game::GOLD,
            "gold_silver/pokemon_gold.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::CRYSTAL,
            pkmn::e_game::CRYSTAL,
            "crystal/pokemon_crystal.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::RUBY_SAPPHIRE,
            pkmn::e_game::RUBY,
            "ruby_sapphire/pokemon_ruby.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::EMERALD,
            pkmn::e_game::EMERALD,
            "emerald/pokemon_emerald.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::FIRERED_LEAFGREEN,
            pkmn::e_game::FIRERED,
            "firered_leafgreen/pokemon_firered.sav"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::COLOSSEUM_XD,
            pkmn::e_game::COLOSSEUM,
            "gamecube_saves/pokemon_colosseum.gci"
        ),
        game_save_test_params_t(
            pkmn::e_game_save_type::COLOSSEUM_XD,
            pkmn::e_game::XD,
            "gamecube_saves/pokemon_xd.gci"
        )
    };

    TEST_P(game_save_test, game_save_test) {
        const pkmn::game_save::sptr& save = get_game_save();

        std::vector<pkmn::e_item> item_list = pkmn::database::get_item_list(save->get_game());

        pkmntest::game_save_test_common_fields(save);
        pkmntest::game_save_test_attributes(save);
        pkmntest::randomize_items(
            save,
            item_list
        );
        pkmntest::randomize_pokemon(
            save,
            item_list
        );

        fs::path temp_save_path = TMP_DIR / str(boost::format("%d_%u.sav")
                                                % int(save->get_game())
                                                % pkmn::rng<uint32_t>().rand());
        save->save_as(temp_save_path.string());

        const pkmn::game_save::sptr& save2 = pkmn::game_save::from_file(temp_save_path.string());
        pkmntest::compare_game_saves(save, save2);

        std::remove(temp_save_path.string().c_str());
    }

    INSTANTIATE_TEST_CASE_P(
        cpp_game_save_test,
        game_save_test,
        ::testing::ValuesIn(params)
    );
}
