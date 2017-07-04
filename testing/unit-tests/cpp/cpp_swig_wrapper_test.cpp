/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "swig/modules/swig_map_wrappers.hpp"

#include <gtest/gtest.h>

TEST(cpp_swig_wrapper_test, test_pokemon_maps)
{
    pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                      "Bulbasaur",
                                      "FireRed",
                                      "",
                                      5
                                  );
    pkmn::swig::pokemon_EV_map EV_map(pokemon);
    pkmn::swig::pokemon_IV_map IV_map(pokemon);

    // Set EV through the Pokémon, query through the wrapper class.
    pokemon->set_EV("Attack", 25);
    EXPECT_EQ(25, pokemon->get_EVs().at("Attack"));
    EXPECT_EQ(25, EV_map.get_EV("Attack"));

    // Set EV through the wrapper class, query through the Pokémon.
    EV_map.set_EV("Defense", 5);
    EXPECT_EQ(5, EV_map.get_EV("Defense"));
    EXPECT_EQ(5, pokemon->get_EVs().at("Defense"));

    // Set IV through the Pokémon, query through the wrapper class.
    pokemon->set_IV("Attack", 11);
    EXPECT_EQ(11, pokemon->get_IVs().at("Attack"));
    EXPECT_EQ(11, IV_map.get_IV("Attack"));

    // Set IV through the wrapper class, query through the Pokémon.
    IV_map.set_IV("Defense", 2);
    EXPECT_EQ(2, IV_map.get_IV("Defense"));
    EXPECT_EQ(2, pokemon->get_IVs().at("Defense"));
}
