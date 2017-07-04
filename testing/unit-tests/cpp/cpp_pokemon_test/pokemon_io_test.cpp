/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include <pkmn/pokemon.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));

TEST(pokemon_io_test, test_3gpkm) {
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
    EXPECT_EQ(128734, mightyena->get_experience());
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
