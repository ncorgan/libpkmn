/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/util.hpp>

#include "pksav/pksav_call.hpp"
#include "types/rng.hpp"

#include <pkmn/pokedex.hpp>
#include <pkmn/database/lists.hpp>

#include <pksav/common/pokedex.h>

#include <gtest/gtest.h>

#include <string>

class pokedex_test: public ::testing::TestWithParam<std::string> {};

TEST_P(pokedex_test, pokedex_test)
{
    std::string game = GetParam();
    int generation = game_generations.at(game);

    pkmn::rng<int> int_rng;
    pkmn::rng<size_t> size_rng;

    pkmn::pokedex::sptr pokedex = pkmn::pokedex::make(game);
    //uint8_t* native_has_seen = reinterpret_cast<uint8_t*>(pokedex->get_native_has_seen());
    //uint8_t* native_has_caught = reinterpret_cast<uint8_t*>(pokedex->get_native_has_caught());

    // Check initial values.
    EXPECT_EQ(0, pokedex->get_num_seen());
    EXPECT_EQ(0, pokedex->get_num_caught());

    // Set random Pokémon to be seen and caught.
    std::vector<std::string> all_pokemon = pkmn::database::get_pokemon_list(generation, true);

    std::vector<int> seen_pokemon_nums;
    size_t num_pokemon_seen = size_rng.rand(1, all_pokemon.size());

    std::vector<int> caught_pokemon_nums;
    size_t num_pokemon_caught = size_rng.rand(1, all_pokemon.size());

    while(seen_pokemon_nums.size() < num_pokemon_seen)
    {
        int index = int_rng.rand(0, int(all_pokemon.size()-1));
        if(std::find(seen_pokemon_nums.begin(), seen_pokemon_nums.end(), (index+1)) == seen_pokemon_nums.end())
        {
            seen_pokemon_nums.emplace_back(index+1);

            pokedex->set_has_seen(all_pokemon[index], true);
            EXPECT_TRUE(pokedex->has_seen(all_pokemon[index]));
        }
    }
    EXPECT_EQ(num_pokemon_seen, size_t(pokedex->get_num_seen()));

    while(caught_pokemon_nums.size() < num_pokemon_caught)
    {
        int index = int_rng.rand(0, int(all_pokemon.size()-1));
        if(std::find(caught_pokemon_nums.begin(), caught_pokemon_nums.end(), (index+1)) == caught_pokemon_nums.end())
        {
            caught_pokemon_nums.emplace_back(index+1);

            pokedex->set_has_caught(all_pokemon[index], true);
            EXPECT_TRUE(pokedex->has_caught(all_pokemon[index]));
        }
    }
    EXPECT_EQ(num_pokemon_caught, size_t(pokedex->get_num_caught()));

    // Check underlying native representations.
/*
    for(int pokemon_num: seen_pokemon_nums)
    {
        bool result_from_pksav = false;
        PKSAV_CALL(
            pksav_get_pokedex_bit(
                native_has_seen,
                uint16_t(pokemon_num),
                &result_from_pksav
            )
        )

        EXPECT_TRUE(result_from_pksav);
    }

    for(int pokemon_num: caught_pokemon_nums)
    {
        bool result_from_pksav = false;
        PKSAV_CALL(
            pksav_get_pokedex_bit(
                native_has_caught,
                uint16_t(pokemon_num),
                &result_from_pksav
            )
        )

        EXPECT_TRUE(result_from_pksav);
    }
*/
    // Remove all entries.

    for(int pokemon_num: seen_pokemon_nums)
    {
        pokedex->set_has_seen(all_pokemon[pokemon_num-1], false);
        EXPECT_FALSE(pokedex->has_seen(all_pokemon[pokemon_num-1]));
    }
    EXPECT_EQ(0, pokedex->get_num_seen());

    for(int pokemon_num: caught_pokemon_nums)
    {
        pokedex->set_has_caught(all_pokemon[pokemon_num-1], false);
        EXPECT_FALSE(pokedex->has_caught(all_pokemon[pokemon_num-1]));
    }
    EXPECT_EQ(0, pokedex->get_num_caught());

    // Check underlying native representations again.
/*
    for(int pokemon_num: seen_pokemon_nums)
    {
        bool result_from_pksav = true;
        PKSAV_CALL(
            pksav_get_pokedex_bit(
                native_has_seen,
                uint16_t(pokemon_num),
                &result_from_pksav
            )
        )

        EXPECT_FALSE(result_from_pksav);
    }

    for(int pokemon_num: caught_pokemon_nums)
    {
        bool result_from_pksav = true;
        PKSAV_CALL(
            pksav_get_pokedex_bit(
                native_has_caught,
                uint16_t(pokemon_num),
                &result_from_pksav
            )
        )

        EXPECT_FALSE(result_from_pksav);
    }
    */
}

static const std::vector<std::string> PARAMS =
{
    "Red", "Blue", "Yellow",
    "Gold", "Silver", "Crystal",
    "Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"
};

INSTANTIATE_TEST_CASE_P(
    cpp_pokedex_test,
    pokedex_test,
    ::testing::ValuesIn(PARAMS)
);
