/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_pc_test.hpp>

static const std::string games[] = {
    "Red",
    "Blue",
    "Yellow",
    "Gold",
    "Silver",
    "Crystal",
    "Ruby",
    "Sapphire",
    "Emerald",
    "FireRed",
    "LeafGreen"
};

namespace pkmntest {

TEST_P(pokemon_box_test, pokemon_box_test) {
    pkmntest::pokemon_box_test_common(get_pokemon_box());
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_box_test,
    pokemon_box_test,
    ::testing::ValuesIn(games)
);

TEST_P(pokemon_pc_test, pokemon_pc_test) {
    pkmntest::pokemon_pc_test_common(get_pokemon_pc());
}

INSTANTIATE_TEST_CASE_P(
    cpp_pokemon_pc_test,
    pokemon_pc_test,
    ::testing::ValuesIn(games)
);

}
