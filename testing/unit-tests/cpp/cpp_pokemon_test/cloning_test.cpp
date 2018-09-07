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
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_cloning_test,
    pokemon_cloning_test,
    ::testing::ValuesIn(pkmntest::SUPPORTED_GAMES)
);
