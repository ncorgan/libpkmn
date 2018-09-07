/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"
#include "private_exports.hpp"
#include "types/rng.hpp"

#include <pkmntest/pokemon_comparison.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <cstdlib>

namespace fs = boost::filesystem;

static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));
static const fs::path TMP_DIR(pkmn::get_tmp_dir());

static std::string export_pokemon_to_tmp_file(
    pkmn::pokemon::sptr pokemon,
    const std::string& extension
)
{
    fs::path output_path = TMP_DIR / str(boost::format("%u.%s")
                                         % pkmn::rng<uint32_t>().rand()
                                         % extension);

    pokemon->export_to_file(output_path.string());

    return output_path.string();
}

// Actual tests

using pokemon_io_test_params_t = std::tuple<
    pkmn::e_game, // game
    std::string   // extension
>;

class pokemon_io_test: public ::testing::TestWithParam<pokemon_io_test_params_t> {};

TEST_P(pokemon_io_test, test_saving_and_loading)
{
    pokemon_io_test_params_t test_params = GetParam();
    pkmn::e_game game = std::get<0>(test_params);
    const std::string& extension = std::get<1>(test_params);

    pkmn::pokemon::sptr random_pokemon = pkmntest::get_random_pokemon(game);
    std::string tmp_path = export_pokemon_to_tmp_file(
                               random_pokemon,
                               extension
                           );
    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(tmp_path);

    pkmntest::compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(tmp_path.c_str());
}

static const std::vector<pokemon_io_test_params_t> TEST_PARAMS =
{
    // Generation I
    std::make_tuple(pkmn::e_game::RED,    "pk1"),
    std::make_tuple(pkmn::e_game::BLUE,   "pk1"),
    std::make_tuple(pkmn::e_game::YELLOW, "pk1"),

    // Generation II
    std::make_tuple(pkmn::e_game::GOLD,    "pk2"),
    std::make_tuple(pkmn::e_game::SILVER,  "pk2"),
    std::make_tuple(pkmn::e_game::CRYSTAL, "pk2"),

    // Game Boy Advance
    std::make_tuple(pkmn::e_game::RUBY,      "3gpkm"),
    std::make_tuple(pkmn::e_game::RUBY,      "pk3"),
    std::make_tuple(pkmn::e_game::SAPPHIRE,  "3gpkm"),
    std::make_tuple(pkmn::e_game::SAPPHIRE,  "pk3"),
    std::make_tuple(pkmn::e_game::EMERALD,   "3gpkm"),
    std::make_tuple(pkmn::e_game::EMERALD,   "pk3"),
    std::make_tuple(pkmn::e_game::FIRERED,   "3gpkm"),
    std::make_tuple(pkmn::e_game::FIRERED,   "pk3"),
    std::make_tuple(pkmn::e_game::LEAFGREEN, "3gpkm"),
    std::make_tuple(pkmn::e_game::LEAFGREEN, "pk3"),

    // Gamecube
    std::make_tuple(pkmn::e_game::COLOSSEUM, "ck3"),
    std::make_tuple(pkmn::e_game::XD,        "xk3"),
};

INSTANTIATE_TEST_CASE_P(
    pokemon_io_test,
    pokemon_io_test,
    ::testing::ValuesIn(TEST_PARAMS)
);

// Test loading a known .3gpkm file from outside LibPKMN.
TEST(pokemon_io_test, test_outside_3gpkm) {
    const fs::path _3GPKM_DIR = LIBPKMN_TEST_FILES / "3gpkm";

    pkmn::pokemon::sptr mightyena = pkmn::pokemon::from_file(
                                        (_3GPKM_DIR / "MIGHTYENA.3gpkm").string()
                                    );
    EXPECT_EQ(pkmn::e_species::MIGHTYENA, mightyena->get_species());
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_game());
    EXPECT_EQ("Standard", mightyena->get_form());
    EXPECT_EQ("MIGHTYENA", mightyena->get_nickname());
    EXPECT_FALSE(mightyena->is_shiny());
    EXPECT_EQ(pkmn::e_condition::NONE, mightyena->get_condition());
    EXPECT_EQ(pkmn::e_item::HEART_SCALE, mightyena->get_held_item());
    EXPECT_EQ("A", mightyena->get_original_trainer_name());
    EXPECT_EQ(61415, mightyena->get_original_trainer_public_id());
    EXPECT_EQ(3417, mightyena->get_original_trainer_secret_id());
    EXPECT_EQ(223997927, mightyena->get_original_trainer_id());
    EXPECT_EQ(pkmn::e_gender::FEMALE, mightyena->get_original_trainer_gender());
    EXPECT_EQ(254, mightyena->get_current_trainer_friendship());
    EXPECT_EQ(pkmn::e_ability::INTIMIDATE, mightyena->get_ability());
    EXPECT_EQ(pkmn::e_ball::GREAT_BALL, mightyena->get_ball());
    EXPECT_EQ(25, mightyena->get_level_met());
    EXPECT_EQ("Route 120", mightyena->get_location_met(false));
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_original_game());
    EXPECT_EQ(3557601241, mightyena->get_personality());
    EXPECT_EQ(128734, mightyena->get_experience());
    EXPECT_EQ(50, mightyena->get_level());

    const std::map<pkmn::e_marking, bool>& mightyena_markings = mightyena->get_markings();
    EXPECT_EQ(4, mightyena_markings.size());
    for(const auto& marking_iter: mightyena_markings)
    {
        EXPECT_FALSE(marking_iter.second);
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

    const std::map<pkmn::e_contest_stat, int>& mightyena_contest_stats = mightyena->get_contest_stats();
    EXPECT_EQ(6, mightyena_contest_stats.size());
    for(auto iter = mightyena_contest_stats.begin(); iter != mightyena_contest_stats.end(); ++iter) {
        EXPECT_EQ(0, iter->second);
    }

    static const std::vector<pkmn::e_move> expected_mightyena_moves =
    {
        pkmn::e_move::CRUNCH,
        pkmn::e_move::STRENGTH,
        pkmn::e_move::SHADOW_BALL,
        pkmn::e_move::DOUBLE_EDGE
    };

    const pkmn::move_slots_t mightyena_moves = mightyena->get_moves();
    EXPECT_EQ(4, mightyena_moves.size());
    for(int move_index = 0; move_index < 4; ++move_index)
    {
        EXPECT_EQ(
            expected_mightyena_moves[move_index],
            mightyena_moves[move_index].move
        );
    }

    const std::map<pkmn::e_stat, int>& mightyena_EVs = mightyena->get_EVs();
    EXPECT_EQ(6, mightyena_EVs.size());
    EXPECT_EQ(30, mightyena_EVs.at(pkmn::e_stat::HP));
    EXPECT_EQ(110, mightyena_EVs.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(32, mightyena_EVs.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(48, mightyena_EVs.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(17, mightyena_EVs.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(83, mightyena_EVs.at(pkmn::e_stat::SPECIAL_DEFENSE));

    const std::map<pkmn::e_stat, int>& mightyena_IVs = mightyena->get_IVs();
    EXPECT_EQ(6, mightyena_IVs.size());
    EXPECT_EQ(26, mightyena_IVs.at(pkmn::e_stat::HP));
    EXPECT_EQ(28, mightyena_IVs.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(4, mightyena_IVs.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(13, mightyena_IVs.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(25, mightyena_IVs.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(26, mightyena_IVs.at(pkmn::e_stat::SPECIAL_DEFENSE));

    const std::map<pkmn::e_stat, int>& mightyena_stats = mightyena->get_stats();
    EXPECT_EQ(6, mightyena_stats.size());
    EXPECT_EQ(146, mightyena_stats.at(pkmn::e_stat::HP));
    EXPECT_EQ(122, mightyena_stats.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(81, mightyena_stats.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(87, mightyena_stats.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(79, mightyena_stats.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(88, mightyena_stats.at(pkmn::e_stat::SPECIAL_DEFENSE));
}

// These tests makes sure that when a Pokémon is exported and re-imported, that
// its form is preserved.

typedef struct
{
    pkmn::e_species species;
    std::string file_extension;
    std::vector<pkmn::e_game> games;
} io_form_test_params_t;

class io_form_test: public ::testing::TestWithParam<io_form_test_params_t> {};

TEST_P(io_form_test, test_form_is_preserved_after_io)
{
    io_form_test_params_t test_params = GetParam();

    for(pkmn::e_game game: test_params.games)
    {
        std::vector<std::string> forms = pkmn::database::pokemon_entry(
                                             test_params.species,
                                             game,
                                             ""
                                         ).get_forms();

        // Instantiate a Pokémon in the given form, export and reimport it,
        // and make sure the form is preserved.
        for(const std::string& form: forms)
        {
            pkmn::pokemon::sptr pokemon_of_form = pkmn::pokemon::make(
                                                      test_params.species,
                                                      game,
                                                      form,
                                                      5
                                                  );
            ASSERT_EQ(test_params.species, pokemon_of_form->get_species());
            ASSERT_EQ(game, pokemon_of_form->get_game());
            ASSERT_EQ(form, pokemon_of_form->get_form());

            std::string tmp_path = export_pokemon_to_tmp_file(
                                       pokemon_of_form,
                                       test_params.file_extension
                                   );

            // Don't check game, as it's not guaranteed to match in all cases.
            pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(
                                                       tmp_path
                                                   );
            ASSERT_EQ(test_params.species, imported_pokemon->get_species());
            ASSERT_EQ(form, imported_pokemon->get_form());

            std::remove(tmp_path.c_str());
        }

        // Instantiate a Pokémon in the default form, set it to each form,
        // export and reimport it, and make sure the form is preserved.
        pkmn::pokemon::sptr test_pokemon = pkmn::pokemon::make(
                                               test_params.species,
                                               game,
                                               "",
                                               5
                                           );
        ASSERT_EQ(test_params.species, test_pokemon->get_species());
        ASSERT_EQ(game, test_pokemon->get_game());

        for(const std::string& form: forms)
        {
            test_pokemon->set_form(form);

            std::string tmp_path = export_pokemon_to_tmp_file(
                                       test_pokemon,
                                       test_params.file_extension
                                   );

            // Don't check game, as it's not guaranteed to match in all cases.
            pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(
                                                       tmp_path
                                                   );
            ASSERT_EQ(test_params.species, imported_pokemon->get_species());
            ASSERT_EQ(form, imported_pokemon->get_form());

            std::remove(tmp_path.c_str());
        }
    }
}

// Generation II

static const std::vector<io_form_test_params_t> GEN2_IO_FORM_TEST_PARAMS =
{
    {
        pkmn::e_species::UNOWN,
        "pk2",
        {
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        }
    }
};

INSTANTIATE_TEST_CASE_P(
    gen2_io_form_test,
    io_form_test,
    ::testing::ValuesIn(GEN2_IO_FORM_TEST_PARAMS)
);

static const std::vector<io_form_test_params_t> GEN3_IO_FORM_TEST_PARAMS =
{
    {
        pkmn::e_species::UNOWN,
        "3gpkm",
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN
        }
    },
    {
        pkmn::e_species::UNOWN,
        "pk3",
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN
        }
    },
    {
        pkmn::e_species::UNOWN,
        "ck3",
        {pkmn::e_game::COLOSSEUM}
    },
    {
        pkmn::e_species::UNOWN,
        "xk3",
        {pkmn::e_game::XD}
    },
};

INSTANTIATE_TEST_CASE_P(
    gen3_io_form_test,
    io_form_test,
    ::testing::ValuesIn(GEN3_IO_FORM_TEST_PARAMS)
);
