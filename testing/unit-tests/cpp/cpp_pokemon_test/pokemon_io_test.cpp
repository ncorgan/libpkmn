/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"
#include "private_exports.hpp"
#include "types/rng.hpp"

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

static bool get_random_bool()
{
    return (rand() % 100) > 50;
}

static pkmn::pokemon::sptr get_random_pokemon(
    pkmn::e_game game
)
{
    int generation = pkmn::priv::game_enum_to_generation(game);
    pkmn::rng<uint32_t> rng;

    std::vector<std::string> item_list = pkmn::database::get_item_list(game);
    std::vector<std::string> move_list = pkmn::database::get_move_list(game);
    std::vector<std::string> pokemon_list = pkmn::database::get_pokemon_list(generation, true);

    // Don't deal with Deoxys or Unown issues here.
    std::string species;
    if(generation == 3) {
        do {
            species = pokemon_list[rng.rand() % pokemon_list.size()];
        } while(species == "Deoxys" || species == "Unown");
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

    const std::map<pkmn::e_stat, int>& EVs = ret->get_EVs();
    for(auto iter = EVs.begin(); iter != EVs.end(); ++iter)
    {
        ret->set_EV(iter->first, (rand() % 256));
    }

    const std::map<pkmn::e_stat, int>& IVs = ret->get_IVs();
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

        ret->set_shininess(get_random_bool());
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
)
{
    pkmn::e_game game = pokemon1->get_game();
    int generation = pkmn::priv::game_enum_to_generation(game);

    // There is no way to determine what game an imported Generation I-II
    // Pokémon comes from, so LibPKMN defaults to a default valid game.
    if(generation >= 3)
    {
        ASSERT_EQ(game, pokemon2->get_game());
    }

    EXPECT_EQ(pokemon1->get_species(), pokemon2->get_species());
    EXPECT_EQ(pokemon1->get_form(), pokemon2->get_form());
    EXPECT_EQ(pokemon1->get_original_trainer_id(), pokemon2->get_original_trainer_id());
    EXPECT_EQ(pokemon1->get_experience(), pokemon2->get_experience());
    EXPECT_EQ(pokemon1->get_level(), pokemon2->get_level());
    EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
    EXPECT_EQ(pokemon1->get_original_trainer_name(), pokemon2->get_original_trainer_name());

    const std::map<pkmn::e_stat, int>& EVs1 = pokemon1->get_EVs();
    const std::map<pkmn::e_stat, int>& EVs2 = pokemon2->get_EVs();
    for(auto iter = EVs1.begin(); iter != EVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, EVs2.at(iter->first));
    }

    const std::map<pkmn::e_stat, int>& IVs1 = pokemon1->get_IVs();
    const std::map<pkmn::e_stat, int>& IVs2 = pokemon2->get_IVs();
    for(auto iter = IVs1.begin(); iter != IVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, IVs2.at(iter->first));
    }

    const std::map<pkmn::e_stat, int>& stats1 = pokemon1->get_stats();
    const std::map<pkmn::e_stat, int>& stats2 = pokemon2->get_stats();
    for(auto iter = stats1.begin(); iter != stats1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, stats2.at(iter->first));
    }

    if(pokemon2->get_game() == game)
    {
        EXPECT_EQ(pokemon1->get_icon_filepath(), pokemon2->get_icon_filepath());
        EXPECT_EQ(pokemon1->get_sprite_filepath(), pokemon2->get_sprite_filepath());
    }

    if(generation >= 2)
    {
        EXPECT_EQ(pokemon1->get_original_trainer_gender(), pokemon2->get_original_trainer_gender());
        EXPECT_EQ(pokemon1->get_current_trainer_friendship(), pokemon2->get_current_trainer_friendship());
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

// Actual tests

class pk1_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pk1_test, test_saving_and_loading_pk1)
{
    pkmn::e_game game = GetParam();

    pkmn::pokemon::sptr random_pokemon = get_random_pokemon(game);
    std::string tmp_path = export_pokemon_to_tmp_file(random_pokemon, "pk1");
    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(tmp_path);

    compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(tmp_path.c_str());
}

static const std::vector<pkmn::e_game> GEN1_GAMES =
{
    pkmn::e_game::RED,
    pkmn::e_game::BLUE,
    pkmn::e_game::YELLOW
};

INSTANTIATE_TEST_CASE_P(
    pk1_test,
    pk1_test,
    ::testing::ValuesIn(GEN1_GAMES)
);

class pk2_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pk2_test, test_saving_and_loading_pk2)
{
    pkmn::e_game game = GetParam();

    pkmn::pokemon::sptr random_pokemon = get_random_pokemon(game);
    std::string tmp_path = export_pokemon_to_tmp_file(random_pokemon, "pk2");
    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(tmp_path);

    compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(tmp_path.c_str());
}

static const std::vector<pkmn::e_game> GEN2_GAMES =
{
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL
};

INSTANTIATE_TEST_CASE_P(
    pk2_test,
    pk2_test,
    ::testing::ValuesIn(GEN2_GAMES)
);

class _3gpkm_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(_3gpkm_test, test_saving_and_loading_3gpkm)
{
    pkmn::e_game game = GetParam();

    pkmn::pokemon::sptr random_pokemon = get_random_pokemon(game);
    std::string tmp_path = export_pokemon_to_tmp_file(random_pokemon, "3gpkm");
    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(tmp_path);

    compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(tmp_path.c_str());
}

static const std::vector<pkmn::e_game> GBA_GAMES =
{
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN
};

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
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_game());
    EXPECT_EQ("Standard", mightyena->get_form());
    EXPECT_EQ("MIGHTYENA", mightyena->get_nickname());
    EXPECT_FALSE(mightyena->is_shiny());
    EXPECT_EQ("Heart Scale", mightyena->get_held_item());
    EXPECT_EQ("A", mightyena->get_original_trainer_name());
    EXPECT_EQ(61415, mightyena->get_original_trainer_public_id());
    EXPECT_EQ(3417, mightyena->get_original_trainer_secret_id());
    EXPECT_EQ(223997927, mightyena->get_original_trainer_id());
    EXPECT_EQ(pkmn::e_gender::FEMALE, mightyena->get_original_trainer_gender());
    EXPECT_EQ(254, mightyena->get_current_trainer_friendship());
    EXPECT_EQ("Intimidate" , mightyena->get_ability());
    EXPECT_EQ("Great Ball", mightyena->get_ball());
    EXPECT_EQ(25, mightyena->get_level_met());
    EXPECT_EQ("Route 120", mightyena->get_location_met(false));
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_original_game());
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
    std::string species;
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
        "Unown",
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

// Generation III

static const std::vector<io_form_test_params_t> GEN3_IO_FORM_TEST_PARAMS =
{
    {
        "Unown",
        "3gpkm",
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN
        }
    }
};

INSTANTIATE_TEST_CASE_P(
    gen3_io_form_test,
    io_form_test,
    ::testing::ValuesIn(GEN3_IO_FORM_TEST_PARAMS)
);
