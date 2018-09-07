/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_comparison.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>

#include <pkmn/enums/game.hpp>

#include <gtest/gtest.h>

class pokemon_cloning_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(pokemon_cloning_test, clone_and_compare)
{
    pkmn::e_game game = GetParam();

    pkmn::pokemon::sptr original_pokemon = pkmntest::get_random_pokemon(game);
    pkmn::pokemon::sptr cloned_pokemon = original_pokemon->clone();

    pkmntest::compare_pokemon(original_pokemon, cloned_pokemon);

    // Delete the first instance and make sure the attributes of the second
    // instance are properly pointing toward its own functions for callbacks.
    ASSERT_EQ(1, original_pokemon.use_count());
    ASSERT_EQ(1, cloned_pokemon.use_count());
    original_pokemon.reset();
    ASSERT_EQ(0, original_pokemon.use_count());
    ASSERT_EQ(1, cloned_pokemon.use_count());

    for(const std::string& attribute_name: cloned_pokemon->get_numeric_attribute_names())
    {
        (void)cloned_pokemon->get_numeric_attribute(attribute_name);
    }
    for(const std::string& attribute_name: cloned_pokemon->get_string_attribute_names())
    {
        (void)cloned_pokemon->get_string_attribute(attribute_name);
    }
    for(const std::string& attribute_name: cloned_pokemon->get_boolean_attribute_names())
    {
        (void)cloned_pokemon->get_boolean_attribute(attribute_name);
    }
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_cloning_test,
    pokemon_cloning_test,
    ::testing::ValuesIn(pkmntest::SUPPORTED_GAMES)
);
