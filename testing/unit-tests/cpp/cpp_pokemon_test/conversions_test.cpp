/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "types/rng.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>

#include <gtest/gtest.h>

static std::string random_string(
    size_t len
)
{
    std::string ret = "";
    pkmn::rng<char> char_rng;

    for(size_t i = 0; i < len; ++i)
    {
        ret += char_rng.rand('a', 'z');
    }

    return ret;
}

bool random_bool()
{
    return (pkmn::rng<int>().rand(0, 99) >= 50);
}

typedef struct
{
    std::string species;
    std::string form;

    std::string origin_game;
    std::string dest_game;
} conversions_test_params_t;

class conversions_test: public ::testing::TestWithParam<conversions_test_params_t> {};

// TODO: specific met locations
TEST_P(conversions_test, conversions_test)
{
    conversions_test_params_t params = GetParam();

    pkmn::pokemon::sptr first_pokemon = pkmn::pokemon::make(
                                            params.species,
                                            params.origin_game,
                                            params.form,
                                            50
                                        );

    int origin_generation = game_generations.at(params.origin_game);
    int dest_generation = game_generations.at(params.dest_game);
    int min_generation = std::min<int>(origin_generation, dest_generation);
    std::string game_for_lists = (min_generation == origin_generation) ? params.origin_game : params.dest_game;

    // Set random values. TODO: EVs, IVs
    pkmn::rng<int> int_rng;
    pkmn::rng<uint8_t> uint8_rng;
    pkmn::rng<uint16_t> uint16_rng;
    pkmn::rng<uint32_t> uint32_rng;
    pkmn::rng<size_t> size_rng;

    std::vector<std::string> items = pkmn::database::get_item_list(game_for_lists);
    std::vector<std::string> moves = pkmn::database::get_move_list(game_for_lists);

    for(int i = 0; i < 4; ++i)
    {
        /*
         * This will get rid of some legitimate moves, like Shadow Ball, but not
         * enough to cause an issue.
         */
        std::string move_name;
        do
        {
            move_name = moves[size_rng.rand(0, moves.size()-1)];
        }
        while(move_name.find("Shadow") == 0);

        first_pokemon->set_move(
            move_name,
            i
        );
    }

    if(origin_generation >= 3)
    {
        first_pokemon->set_trainer_id(uint32_rng.rand());

        std::pair<std::string, std::string> abilities = first_pokemon->get_database_entry().get_abilities();
        if(abilities.second != "None")
        {
            first_pokemon->set_ability(random_bool() ? abilities.first : abilities.second);
        }

        // TODO: random ball

        first_pokemon->set_personality(uint32_rng.rand());
    }
    else
    {
        first_pokemon->set_trainer_id(uint16_rng.rand());
    }

    if(min_generation >= 2)
    {
        /*
         * Make sure the item is holdable. For Generation III, no GCN_exclusive items appear
         * to be holdable.
         */
        std::string held_item = "";
        do
        {
            held_item = items[size_rng.rand(0, items.size()-1)];
        } while(not pkmn::database::item_entry(held_item, params.origin_game).holdable());

        first_pokemon->set_held_item(held_item);
    }
    if(origin_generation >= 2)
    {
        first_pokemon->set_gender(random_bool() ? "Male" : "Female");
        first_pokemon->set_shininess(random_bool());
        first_pokemon->set_friendship(uint8_rng.rand());

        if(params.origin_game != "Gold" and params.origin_game != "Crystal")
        {
            first_pokemon->set_trainer_gender(random_bool() ? "Male" : "Female");
        }

        // The max level met value in Generation II is 63.
        first_pokemon->set_level_met(int_rng.rand(2, (origin_generation == 2) ? 63 : 100));
    }
    if(origin_generation >= 3)
    {
        // Randomize markings, ribbons, and contest stats.
        const std::map<std::string, bool>& markings = first_pokemon->get_markings();
        for(auto iter = markings.begin(); iter != markings.end(); ++iter)
        {
            first_pokemon->set_marking(iter->first, random_bool());
        }
        const std::map<std::string, bool>& ribbons = first_pokemon->get_ribbons();
        for(auto iter = ribbons.begin(); iter != ribbons.end(); ++iter)
        {
            first_pokemon->set_ribbon(iter->first, random_bool());
        }
        const std::map<std::string, int>& contest_stats = first_pokemon->get_contest_stats();
        for(auto iter = contest_stats.begin(); iter != contest_stats.end(); ++iter)
        {
            first_pokemon->set_contest_stat(iter->first, int_rng.rand(0, 255));
        }
    }

    first_pokemon->set_nickname(random_string(10));
    first_pokemon->set_trainer_name(random_string(7));

    // The max level met value in Generation II is 63, which restricts this as well.
    first_pokemon->set_level(int_rng.rand(2, (dest_generation == 2) ? 63 : 100));

    // Convert to the second game and compare.
    pkmn::pokemon::sptr second_pokemon = first_pokemon->to_game(params.dest_game);

    EXPECT_EQ(first_pokemon->get_species(), second_pokemon->get_species());
    EXPECT_EQ(params.dest_game, second_pokemon->get_game());
    EXPECT_EQ(first_pokemon->get_form(), second_pokemon->get_form());
    EXPECT_EQ(first_pokemon->get_nickname(), second_pokemon->get_nickname());
    EXPECT_EQ(first_pokemon->get_trainer_name(), second_pokemon->get_trainer_name());
    EXPECT_EQ(first_pokemon->get_trainer_id(), second_pokemon->get_trainer_id());
    EXPECT_EQ(first_pokemon->get_trainer_public_id(), second_pokemon->get_trainer_public_id());
    EXPECT_EQ(first_pokemon->get_experience(), second_pokemon->get_experience());
    EXPECT_EQ(first_pokemon->get_level(), second_pokemon->get_level());

    const pkmn::move_slots_t& first_pokemon_move_slots = first_pokemon->get_moves();
    const pkmn::move_slots_t& second_pokemon_move_slots = second_pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ(first_pokemon_move_slots[i].move, second_pokemon_move_slots[i].move);
        EXPECT_EQ(first_pokemon_move_slots[i].pp, second_pokemon_move_slots[i].pp);
    }

    if(min_generation >= 3)
    {
        EXPECT_EQ(first_pokemon->get_trainer_secret_id(), second_pokemon->get_trainer_secret_id());
        EXPECT_EQ(first_pokemon->get_ability(), second_pokemon->get_ability());
        EXPECT_EQ(first_pokemon->get_ball(), second_pokemon->get_ball());
        EXPECT_EQ(first_pokemon->get_original_game(), second_pokemon->get_original_game());
        EXPECT_EQ(first_pokemon->get_personality(), second_pokemon->get_personality());

        if(origin_generation == dest_generation)
        {
            EXPECT_TRUE(first_pokemon->get_markings() == second_pokemon->get_markings());
            EXPECT_TRUE(first_pokemon->get_ribbons() == second_pokemon->get_ribbons());
            EXPECT_TRUE(first_pokemon->get_contest_stats() == second_pokemon->get_contest_stats());
        }
        // TODO: else specific functions to check
    }
    if(min_generation >= 2)
    {
        EXPECT_EQ(first_pokemon->get_trainer_gender(), second_pokemon->get_trainer_gender());
        EXPECT_EQ(first_pokemon->get_gender(), second_pokemon->get_gender());
        EXPECT_EQ(first_pokemon->is_shiny(), second_pokemon->is_shiny());
        EXPECT_EQ(first_pokemon->get_held_item(), second_pokemon->get_held_item());
        EXPECT_EQ(first_pokemon->get_friendship(), second_pokemon->get_friendship());
        EXPECT_EQ(first_pokemon->get_level(), second_pokemon->get_level_met());
    }
}

static const conversions_test_params_t TEST_PARAMS[] =
{
    {"Bulbasaur", "", "Red", "Yellow"},
    {"Squirtle", "", "Blue", "Gold"},
    {"Cyndaquil", "", "Gold", "Crystal"},
    {"Charmander", "", "Silver", "Blue"},
    {"Eevee", "", "Ruby", "Colosseum"},
    {"Espeon", "", "Emerald", "Colosseum"},
    {"Umbreon", "", "FireRed", "Colosseum"},
    {"Eevee", "", "Ruby", "XD"},
    {"Espeon", "", "Emerald", "XD"},
    {"Umbreon", "", "FireRed", "XD"},
    {"Eevee", "", "Colosseum", "Sapphire"}
    /*{"Espeon", "", "Colosseum", "Emerald"},
    {"Umbreon", "", "Colosseum", "LeafGreen"},
    {"Eevee", "", "XD", "Sapphire"},
    {"Espeon", "", "XD", "Emerald"},
    {"Umbreon", "", "XD", "LeafGreen"}*/
};

INSTANTIATE_TEST_CASE_P(
    cpp_conversions_test,
    conversions_test,
    ::testing::ValuesIn(TEST_PARAMS)
);
