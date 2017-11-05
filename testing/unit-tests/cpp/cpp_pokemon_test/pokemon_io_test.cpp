/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"
#include "types/rng.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));
static const fs::path TMP_DIR(pkmn::get_tmp_dir());

static bool get_random_bool()
{
    return (rand() % 100) > 50;
}

static pkmn::pokemon::sptr get_random_pokemon(
    const std::string &game
) {
    int generation = game_generations.at(game);
    pkmn::rng<uint32_t> rng;

    std::vector<std::string> item_list = pkmn::database::get_item_list(game);
    std::vector<std::string> move_list = pkmn::database::get_move_list(game);
    std::vector<std::string> pokemon_list = pkmn::database::get_pokemon_list(generation, true);

    // Don't deal with Deoxys issues here.
    std::string species;
    if(generation == 3) {
        do {
            species = pokemon_list[rng.rand() % pokemon_list.size()];
        } while(species == "Deoxys");
    } else {
        species = pokemon_list[rng.rand() % pokemon_list.size()];
    }
    pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                  species,
                                  game,
                                  "",
                                  ((rng.rand() % 99) + 2)
                              );
    for(int i = 0; i < 4; ++i) {
        std::string move = "";
        do
        {
            move = move_list[rng.rand() % move_list.size()];
        } while(move.find("Shadow") == 0);
        ret->set_move(move, i);
    }

    const std::map<std::string, int>& EVs = ret->get_EVs();
    for(auto iter = EVs.begin(); iter != EVs.end(); ++iter)
    {
        ret->set_EV(iter->first, (rand() % 256));
    }

    const std::map<std::string, int>& IVs = ret->get_IVs();
    for(auto iter = IVs.begin(); iter != IVs.end(); ++iter)
    {
        ret->set_IV(iter->first, (rand() % 16));
    }

    if(generation >= 2) {
        // Keep going until one is holdable
        while(ret->get_held_item() == "None") {
            try {
                ret->set_held_item(
                    item_list[rng.rand() % item_list.size()]
                );
            } catch(std::invalid_argument&) {}
        }
    }

    if(generation >= 3)
    {
        const std::map<std::string, bool>& markings = ret->get_markings();
        for(auto iter = markings.begin(); iter != markings.end(); ++iter)
        {
            ret->set_marking(iter->first, get_random_bool());
        }

        const std::map<std::string, bool>& ribbons = ret->get_ribbons();
        for(auto iter = ribbons.begin(); iter != ribbons.end(); ++iter)
        {
            ret->set_ribbon(iter->first, get_random_bool());
        }

        const std::map<std::string, int>& contest_stats = ret->get_contest_stats();
        for(auto iter = contest_stats.begin(); iter != contest_stats.end(); ++iter)
        {
            ret->set_contest_stat(iter->first, (rand() % 256));
        }
    }

    return ret;
}

static void compare_pokemon(
    pkmn::pokemon::sptr pokemon1,
    pkmn::pokemon::sptr pokemon2
) {
    std::string game = pokemon1->get_game();
    ASSERT_EQ(game, pokemon2->get_game());
    int generation = game_generations.at(game);

    EXPECT_EQ(pokemon1->get_species(), pokemon2->get_species());
    EXPECT_EQ(pokemon1->get_form(), pokemon2->get_form());
    EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
    EXPECT_EQ(pokemon1->get_trainer_name(), pokemon2->get_trainer_name());
    EXPECT_EQ(pokemon1->get_trainer_id(), pokemon2->get_trainer_id());
    EXPECT_EQ(pokemon1->get_experience(), pokemon2->get_experience());
    EXPECT_EQ(pokemon1->get_level(), pokemon2->get_level());

    const std::map<std::string, int>& EVs1 = pokemon1->get_EVs();
    const std::map<std::string, int>& EVs2 = pokemon2->get_EVs();
    for(auto iter = EVs1.begin(); iter != EVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, EVs2.at(iter->first));
    }

    const std::map<std::string, int>& IVs1 = pokemon1->get_IVs();
    const std::map<std::string, int>& IVs2 = pokemon2->get_IVs();
    for(auto iter = IVs1.begin(); iter != IVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, IVs2.at(iter->first));
    }

    const std::map<std::string, int>& stats1 = pokemon1->get_stats();
    const std::map<std::string, int>& stats2 = pokemon2->get_stats();
    for(auto iter = stats1.begin(); iter != stats1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, stats2.at(iter->first));
    }

    EXPECT_EQ(pokemon1->get_icon_filepath(), pokemon2->get_icon_filepath());
    EXPECT_EQ(pokemon1->get_sprite_filepath(), pokemon2->get_sprite_filepath());

    if(generation >= 2)
    {
        EXPECT_EQ(pokemon1->get_trainer_gender(), pokemon2->get_trainer_gender());
        EXPECT_EQ(pokemon1->get_friendship(), pokemon2->get_friendship());
        EXPECT_EQ(pokemon1->get_gender(), pokemon2->get_gender());
        EXPECT_EQ(pokemon1->is_shiny(), pokemon2->is_shiny());
        EXPECT_EQ(pokemon1->get_held_item(), pokemon2->get_held_item());
        EXPECT_EQ(pokemon1->get_level_met(), pokemon2->get_level_met());
        EXPECT_EQ(pokemon1->get_location_met(false), pokemon2->get_location_met(false));
    }
    if(generation >= 3)
    {
        EXPECT_EQ(pokemon1->get_ability(), pokemon2->get_ability());
        EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());
        EXPECT_EQ(pokemon1->get_personality(), pokemon2->get_personality());
        EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());

        const std::map<std::string, bool>& markings1 = pokemon1->get_markings();
        const std::map<std::string, bool>& markings2 = pokemon2->get_markings();
        for(auto iter = markings1.begin(); iter != markings1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, markings2.at(iter->first));
        }

        const std::map<std::string, bool>& ribbons1 = pokemon1->get_ribbons();
        const std::map<std::string, bool>& ribbons2 = pokemon2->get_ribbons();
        for(auto iter = ribbons1.begin(); iter != ribbons1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, ribbons2.at(iter->first));
        }

        const std::map<std::string, int>& contest_stats1 = pokemon1->get_contest_stats();
        const std::map<std::string, int>& contest_stats2 = pokemon2->get_contest_stats();
        for(auto iter = contest_stats1.begin(); iter != contest_stats1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, contest_stats2.at(iter->first));
        }
    }
    if(generation >= 4)
    {
        EXPECT_EQ(pokemon1->get_location_met(true), pokemon2->get_location_met(true));
    }
}

class _3gpkm_test: public ::testing::TestWithParam<std::string> {};

TEST_P(_3gpkm_test, test_saving_and_loading_3gpkm)
{
    std::string game = GetParam();

    fs::path _3gpkm_path = TMP_DIR / str(boost::format("%s_ %u.3gpkm") % game % pkmn::rng<uint32_t>().rand());

    pkmn::pokemon::sptr random_pokemon = get_random_pokemon(game);
    random_pokemon->export_to_file(_3gpkm_path.string());

    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(_3gpkm_path.string());

    compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(_3gpkm_path.string().c_str());
}

static const std::vector<std::string> GBA_GAMES = {"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"};

INSTANTIATE_TEST_CASE_P(
    _3gpkm_test,
    _3gpkm_test,
    ::testing::ValuesIn(GBA_GAMES)
);

// Test loading a known .3gpkm file from outside LibPKMN.
TEST(pokemon_io_test, test_outside_3gpkm) {
    const fs::path _3GPKM_DIR = LIBPKMN_TEST_FILES / "3gpkm";

    pkmn::pokemon::sptr mightyena = pkmn::pokemon::from_file(
                                        (_3GPKM_DIR / "MIGHTYENA.3gpkm").string()
                                    );
    EXPECT_EQ("Mightyena", mightyena->get_species());
    EXPECT_EQ("Emerald", mightyena->get_game());
    EXPECT_EQ("Standard", mightyena->get_form());
    EXPECT_EQ("MIGHTYENA", mightyena->get_nickname());
    EXPECT_FALSE(mightyena->is_shiny());
    EXPECT_EQ("Heart Scale", mightyena->get_held_item());
    EXPECT_EQ("A", mightyena->get_trainer_name());
    EXPECT_EQ(61415, mightyena->get_trainer_public_id());
    EXPECT_EQ(3417, mightyena->get_trainer_secret_id());
    EXPECT_EQ(223997927, mightyena->get_trainer_id());
    EXPECT_EQ("Female", mightyena->get_trainer_gender());
    EXPECT_EQ(254, mightyena->get_friendship());
    EXPECT_EQ("Intimidate" , mightyena->get_ability());
    EXPECT_EQ("Great Ball", mightyena->get_ball());
    EXPECT_EQ(25, mightyena->get_level_met());
    EXPECT_EQ("Route 120", mightyena->get_location_met(false));
    EXPECT_EQ("Emerald", mightyena->get_original_game());
    EXPECT_EQ(3557601241, mightyena->get_personality());
    EXPECT_EQ(128734 , mightyena->get_experience());
    EXPECT_EQ(50, mightyena->get_level());

    const std::map<std::string, bool>& mightyena_markings = mightyena->get_markings();
    EXPECT_EQ(4, mightyena_markings.size());
    for(auto iter = mightyena_markings.begin(); iter != mightyena_markings.end(); ++iter) {
        EXPECT_FALSE(iter->second);
    }

    const std::map<std::string, bool>& mightyena_ribbons = mightyena->get_ribbons();
    EXPECT_EQ(32, mightyena_ribbons.size());
    for(auto iter = mightyena_ribbons.begin(); iter != mightyena_ribbons.end(); ++iter) {
        if(iter->first == "Champion") {
            EXPECT_TRUE(iter->second);
        } else {
            EXPECT_FALSE(iter->second);
        }
    }

    const std::map<std::string, int>& mightyena_contest_stats = mightyena->get_contest_stats();
    EXPECT_EQ(6, mightyena_contest_stats.size());
    for(auto iter = mightyena_contest_stats.begin(); iter != mightyena_contest_stats.end(); ++iter) {
        EXPECT_EQ(0, iter->second);
    }

    static const std::string expected_mightyena_moves[] = {
        "Crunch", "Strength", "Shadow Ball", "Double-Edge"
    };
    const pkmn::move_slots_t mightyena_moves = mightyena->get_moves();
    EXPECT_EQ(4, mightyena_moves.size());
    for(int i = 0; i < 4; ++i) {
        EXPECT_EQ(expected_mightyena_moves[i], mightyena_moves[i].move);
    }

    const std::map<std::string, int>& mightyena_EVs = mightyena->get_EVs();
    EXPECT_EQ(6, mightyena_EVs.size());
    EXPECT_EQ(30, mightyena_EVs.at("HP"));
    EXPECT_EQ(110, mightyena_EVs.at("Attack"));
    EXPECT_EQ(32, mightyena_EVs.at("Defense"));
    EXPECT_EQ(48, mightyena_EVs.at("Speed"));
    EXPECT_EQ(17, mightyena_EVs.at("Special Attack"));
    EXPECT_EQ(83, mightyena_EVs.at("Special Defense"));

    const std::map<std::string, int>& mightyena_IVs = mightyena->get_IVs();
    EXPECT_EQ(6, mightyena_IVs.size());
    EXPECT_EQ(26, mightyena_IVs.at("HP"));
    EXPECT_EQ(28, mightyena_IVs.at("Attack"));
    EXPECT_EQ(4, mightyena_IVs.at("Defense"));
    EXPECT_EQ(13, mightyena_IVs.at("Speed"));
    EXPECT_EQ(25, mightyena_IVs.at("Special Attack"));
    EXPECT_EQ(26, mightyena_IVs.at("Special Defense"));

    const std::map<std::string, int>& mightyena_stats = mightyena->get_stats();
    EXPECT_EQ(6, mightyena_stats.size());
    EXPECT_EQ(146, mightyena_stats.at("HP"));
    EXPECT_EQ(122, mightyena_stats.at("Attack"));
    EXPECT_EQ(81, mightyena_stats.at("Defense"));
    EXPECT_EQ(87, mightyena_stats.at("Speed"));
    EXPECT_EQ(79, mightyena_stats.at("Special Attack"));
    EXPECT_EQ(88, mightyena_stats.at("Special Defense"));
}
