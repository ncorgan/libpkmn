/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/util.hpp>

#include "private_exports.hpp"
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

class pokedex_test: public ::testing::TestWithParam<pkmn::e_game> {};

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
    const pksav_type* p_pksav_pokedex,
    const std::vector<int>& seen_numbers,
    const std::vector<int>& caught_numbers,
    bool expected_value
)
{
    ASSERT_NE(nullptr, p_pksav_pokedex);

    check_pksav_buffer(
        p_pksav_pokedex->p_seen,
        seen_numbers,
        expected_value
    );
    check_pksav_buffer(
        p_pksav_pokedex->p_owned,
        caught_numbers,
        expected_value
    );
}

static void check_gba_pksav_pokedex(
    const struct pksav_gba_pokedex* p_gba_pokedex,
    const std::vector<int>& seen_numbers,
    const std::vector<int>& caught_numbers,
    bool expected_value
)
{
    ASSERT_NE(nullptr, p_gba_pokedex);

    const size_t num_bytes = static_cast<size_t>(std::ceil(386.0f / 8.0f));

    check_pksav_buffer(
        p_gba_pokedex->p_seenA,
        seen_numbers,
        expected_value
    );
    check_pksav_buffer(
        p_gba_pokedex->p_owned,
        caught_numbers,
        expected_value
    );

    // All three seen buffers should be equal.
    EXPECT_EQ(
        0,
        std::memcmp(
            p_gba_pokedex->p_seenA,
            p_gba_pokedex->p_seenB,
            num_bytes
        )
    );
    EXPECT_EQ(
        0,
        std::memcmp(
            p_gba_pokedex->p_seenA,
            p_gba_pokedex->p_seenC,
            num_bytes
        )
    );
}

TEST_P(pokedex_test, pokedex_test)
{
    pkmn::e_game game = GetParam();
    int generation = pkmn::priv::game_enum_to_generation(game);

    pkmn::rng<int> int_rng;
    pkmn::rng<size_t> size_rng;

    pkmn::pokedex::sptr pokedex = pkmn::pokedex::make(game);
    ASSERT_EQ(game, pokedex->get_game());

    void* p_native = pokedex->get_native();
    ASSERT_NE(nullptr, p_native);

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
    EXPECT_EQ(num_pokemon_seen, pokedex->get_all_seen().size());

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
    EXPECT_EQ(num_pokemon_caught, pokedex->get_all_caught().size());

    // Check underlying native representations.

    switch(generation)
    {
        case 1:
            check_gb_pksav_pokedex<struct pksav_gen1_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen1_pokedex_lists*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        case 2:
            check_gb_pksav_pokedex<struct pksav_gen2_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen2_pokedex_lists*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        case 3:
            check_gba_pksav_pokedex(
                reinterpret_cast<const struct pksav_gba_pokedex*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                true
            );
            break;

        default:
            break;
    }

    // Remove all entries. Do this in reverse because adding a Pokémon as
    // having been caught also makes it having been seen.

    std::vector<std::string> all_seen = pokedex->get_all_seen();
    for(const std::string& species: all_seen)
    {
        pokedex->set_has_seen(species, false);
        EXPECT_FALSE(pokedex->has_seen(species));
    }
    EXPECT_EQ(0, pokedex->get_num_seen());
    EXPECT_EQ(0ULL, pokedex->get_all_seen().size());

    std::vector<std::string> all_caught = pokedex->get_all_caught();
    for(const std::string& species: all_caught)
    {
        pokedex->set_has_caught(species, false);
        EXPECT_FALSE(pokedex->has_caught(species));
    }
    EXPECT_EQ(0, pokedex->get_num_caught());
    EXPECT_EQ(0ULL, pokedex->get_all_caught().size());

    // Check underlying native representations again.

    switch(generation)
    {
        case 1:
            check_gb_pksav_pokedex<struct pksav_gen1_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen1_pokedex_lists*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        case 2:
            check_gb_pksav_pokedex<struct pksav_gen2_pokedex_lists>(
                reinterpret_cast<const struct pksav_gen2_pokedex_lists*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        case 3:
            check_gba_pksav_pokedex(
                reinterpret_cast<const struct pksav_gba_pokedex*>(p_native),
                seen_pokemon_nums,
                caught_pokemon_nums,
                false
            );
            break;

        default:
            break;
    }
}

static const std::vector<pkmn::e_game> PARAMS =
{
    pkmn::e_game::RED,
    pkmn::e_game::BLUE,
    pkmn::e_game::YELLOW,
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL,
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
};

INSTANTIATE_TEST_CASE_P(
    cpp_pokedex_test,
    pokedex_test,
    ::testing::ValuesIn(PARAMS)
);
