/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

#include <gtest/gtest.h>

class gen1_friendship_test: public ::testing::TestWithParam<std::string> {};

TEST_P(gen1_friendship_test, gen1_friendship_test) {
    std::string game = GetParam();

    pkmn::pokemon::sptr pikachu = pkmn::pokemon::make(
                                      "Pikachu",
                                      game,
                                      "",
                                      5
                                  );
    if(game == "Yellow") {
        pikachu->set_friendship(123);
        EXPECT_EQ(123, pikachu->get_friendship());

        // Also check a non-Pikachu
        pkmn::pokemon::sptr mewtwo = pkmn::pokemon::make(
                                         "Mewtwo",
                                         game,
                                         "",
                                         70
                                     );
        EXPECT_THROW(
            mewtwo->set_friendship(123);
        , pkmn::feature_not_in_game_error);
        EXPECT_THROW(
            (void)mewtwo->get_friendship();
        , pkmn::feature_not_in_game_error);
    } else {
        EXPECT_THROW(
            pikachu->set_friendship(123);
        , pkmn::feature_not_in_game_error);
        EXPECT_THROW(
            (void)pikachu->get_friendship();
        , pkmn::feature_not_in_game_error);
    }
}

static const std::string games[] = {
    "Red",
    "Blue",
    "Yellow"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gen1_friendship_test,
    gen1_friendship_test,
    ::testing::ValuesIn(games)
);
