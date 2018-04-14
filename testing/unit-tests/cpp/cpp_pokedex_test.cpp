/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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
#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/pokedex.h>

#include <gtest/gtest.h>

#include <string>

class pokedex_test: public ::testing::TestWithParam<std::string> {};

static void check_pksav_buffer(
    const uint8_t* pksav_buffer,
    const std::vector<int>& numbers_to_check,
    bool expected_value
)
{
    ASSERT_NE(nullptr, pksav_buffer);

    for(int pokemon_num: numbers_to_check)
    {
        bool result_from_pksav = false;
        PKSAV_CALL(
            pksav_get_pokedex_bit(
                pksav_buffer,
                uint16_t(pokemon_num),
                &result_from_pksav
            )
        )
        EXPECT_EQ(expected_value, result_from_pksav);
    }
}

template <typename pksav_type>
static void check_gb_pksav_pokedex(
    const pksav_type* pksav_pokedex_ptr,
    const std::vector<int>& seen_numbers,
    const std::vector<int>& caught_numbers,
    bool expected_value
)
{
    ASSERT_NE(nullptr, pksav_pokedex_ptr);

    check_pksav_buffer(
        pksav_pokedex_ptr->seen_ptr,
        seen_numbers,
        expected_value
    );
    check_pksav_buffer(
        pksav_pokedex_ptr->owned_ptr,
        caught_numbers,
        expected_value
    );
}

static void check_gba_pksav_pokedex(
    const struct pksav_gba_pokedex* gba_pokedex_ptr,
    const std::vector<int>& seen_numbers,
    const std::vector<int>& caught_numbers,
    bool expected_value
)
{
    ASSERT_NE(nullptr, gba_pokedex_ptr);

    const size_t num_bytes = std::ceil(386.0f / 8.0f);

    check_pksav_buffer(
        gba_pokedex_ptr->seen_ptrA,
        seen_numbers,
        expected_value
    );
    check_pksav_buffer(
        gba_pokedex_ptr->owned_ptr,
        caught_numbers,
        expected_value
    );

    // All three seen buffers should be equal.
    EXPECT_EQ(
        0,
        std::memcmp(
            gba_pokedex_ptr->seen_ptrA,
            gba_pokedex_ptr->seen_ptrB,
            num_bytes
        )
    );
    EXPECT_EQ(
        0,
        std::memcmp(
            gba_pokedex_ptr->seen_ptrA,
            gba_pokedex_ptr->seen_ptrC,
            num_bytes
        )
    );
}

TEST_P(pokedex_test, pokedex_test)
{
    std::string game = GetParam();
    int generation = game_generations.at(game);

    pkmn::rng<int> int_rng;
    pkmn::rng<size_t> size_rng;

    pkmn::pokedex::sptr pokedex = pkmn::pokedex::make(game);
    ASSERT_EQ(game, pokedex->get_game());

    void* native_ptr = pokedex->get_native();
    ASSERT_NE(nullptr, native_ptr);

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

    switch(generation)
    {
        case 1:
            check_gb_pksav_pokedex<struct pksav_gen1_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen1_pokedex_lists*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        case 2:
            check_gb_pksav_pokedex<struct pksav_gen2_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen2_pokedex_lists*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        case 3:
            check_gba_pksav_pokedex(
                reinterpret_cast<const struct pksav_gba_pokedex*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        default:
            break;
    }

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

    switch(generation)
    {
        case 1:
            check_gb_pksav_pokedex<struct pksav_gen1_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen1_pokedex_lists*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        case 2:
            check_gb_pksav_pokedex<struct pksav_gen2_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen2_pokedex_lists*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        case 3:
            check_gba_pksav_pokedex(
                reinterpret_cast<const struct pksav_gba_pokedex*>(native_ptr),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        default:
            break;
    }
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
