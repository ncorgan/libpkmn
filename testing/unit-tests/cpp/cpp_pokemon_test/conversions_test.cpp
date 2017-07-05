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

    // TODO: account for dest_game generation
    int generation = game_generations.at(params.origin_game);

    // Set random values. TODO: EVs, IVs
    pkmn::rng<int> int_rng;
    pkmn::rng<uint8_t> uint8_rng;
    pkmn::rng<uint16_t> uint16_rng;
    pkmn::rng<uint32_t> uint32_rng;
    pkmn::rng<size_t> size_rng;

    std::vector<std::string> items = pkmn::database::get_item_list(params.origin_game);
    std::vector<std::string> moves = pkmn::database::get_move_list(params.origin_game);

    for(int i = 0; i < 4; ++i)
    {
        first_pokemon->set_move(
            moves[size_rng.rand(0, moves.size()-1)],
            i
        );
    }

    if(generation >= 3)
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

    if(generation >= 2)
    {
        // Make sure the item is holdable.
        std::string held_item = "";
        do
        {
            held_item = items[size_rng.rand(0, items.size()-1)];
        } while(not pkmn::database::item_entry(held_item, params.origin_game).holdable());

        first_pokemon->set_held_item(held_item);
        first_pokemon->set_gender(random_bool() ? "Male" : "Female");
        first_pokemon->set_shininess(random_bool());
        first_pokemon->set_friendship(uint8_rng.rand());

        if(params.origin_game != "Gold" and params.origin_game != "Crystal")
        {
            first_pokemon->set_trainer_gender(random_bool() ? "Male" : "Female");
        }

        first_pokemon->set_level_met(int_rng.rand(2, 100));
    }

    first_pokemon->set_nickname(random_string(10));
    first_pokemon->set_trainer_name(random_string(7));
    first_pokemon->set_level(int_rng.rand(2, 100));

    // Convert to the second game and compare.
    pkmn::pokemon::sptr second_pokemon = first_pokemon->to_game(params.dest_game);

    EXPECT_EQ(first_pokemon->get_species(), second_pokemon->get_species());
    EXPECT_EQ(params.dest_game, second_pokemon->get_game());
    EXPECT_EQ(first_pokemon->get_form(), second_pokemon->get_form());
    EXPECT_EQ(first_pokemon->get_nickname(), second_pokemon->get_nickname());
    EXPECT_EQ(first_pokemon->get_trainer_name(), second_pokemon->get_trainer_name());
    EXPECT_EQ(first_pokemon->get_trainer_id(), second_pokemon->get_trainer_id());
    EXPECT_EQ(first_pokemon->get_trainer_public_id(), second_pokemon->get_trainer_public_id());
    EXPECT_EQ(first_pokemon->get_trainer_gender(), second_pokemon->get_trainer_gender());
    EXPECT_EQ(first_pokemon->get_experience(), second_pokemon->get_experience());
    EXPECT_EQ(first_pokemon->get_level(), second_pokemon->get_level());

    const pkmn::move_slots_t& first_pokemon_move_slots = first_pokemon->get_moves();
    const pkmn::move_slots_t& second_pokemon_move_slots = second_pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ(first_pokemon_move_slots[i].move, second_pokemon_move_slots[i].move);
        EXPECT_EQ(first_pokemon_move_slots[i].pp, second_pokemon_move_slots[i].pp);
    }

    if(generation >= 3)
    {
        EXPECT_EQ(first_pokemon->get_trainer_secret_id(), second_pokemon->get_trainer_secret_id());
        EXPECT_EQ(first_pokemon->get_ability(), second_pokemon->get_ability());
        EXPECT_EQ(first_pokemon->get_ball(), second_pokemon->get_ball());
        EXPECT_EQ(first_pokemon->get_original_game(), second_pokemon->get_original_game());
        EXPECT_EQ(first_pokemon->get_personality(), second_pokemon->get_personality());

        // TODO: markings, ribbons, contest stats
    }

    if(generation >= 2)
    {
        EXPECT_EQ(first_pokemon->get_gender(), second_pokemon->get_gender());
        EXPECT_EQ(first_pokemon->is_shiny(), second_pokemon->is_shiny());
        EXPECT_EQ(first_pokemon->get_held_item(), second_pokemon->get_held_item());
        EXPECT_EQ(first_pokemon->get_friendship(), second_pokemon->get_friendship());
        EXPECT_EQ(first_pokemon->get_level_met(), second_pokemon->get_level_met());
    }
}

// TODO: Generation II -> I
static const conversions_test_params_t TEST_PARAMS[] =
{
    {"Bulbasaur", "", "Red", "Yellow"},
    {"Squirtle", "", "Blue", "Gold"},
    {"Cyndaquil", "", "Gold", "Crystal"},
};

INSTANTIATE_TEST_CASE_P(
    cpp_conversions_test,
    conversions_test,
    ::testing::ValuesIn(TEST_PARAMS)
);
