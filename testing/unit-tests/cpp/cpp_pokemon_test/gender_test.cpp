/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/pokemon.hpp>

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace fs = boost::filesystem;

class gender_test: public ::testing::TestWithParam<std::string> {};

TEST_P(gender_test, gender_test) {
    std::string game = GetParam();

    // Single-gender
    pkmn::pokemon::sptr nidorina = pkmn::pokemon::make(
                                       "Nidorina",
                                       game,
                                       "",
                                       50
                                   );
    EXPECT_EQ("Female", nidorina->get_gender());
    nidorina->set_gender("Female");
    EXPECT_THROW(
        nidorina->set_gender("Male");
    , std::invalid_argument);
    EXPECT_THROW(
        nidorina->set_gender("Genderless");
    , std::invalid_argument);

    pkmn::pokemon::sptr nidorino = pkmn::pokemon::make(
                                       "Nidorino",
                                       game,
                                       "",
                                       50
                                   );
    EXPECT_EQ("Male", nidorino->get_gender());
    nidorino->set_gender("Male");
    EXPECT_THROW(
        nidorino->set_gender("Female");
    , std::invalid_argument);
    EXPECT_THROW(
        nidorino->set_gender("Genderless");
    , std::invalid_argument);

    pkmn::pokemon::sptr magnemite = pkmn::pokemon::make(
                                        "Magnemite",
                                        game,
                                        "",
                                        50
                                    );
    EXPECT_EQ("Genderless", magnemite->get_gender());
    magnemite->set_gender("Genderless");
    EXPECT_THROW(
        magnemite->set_gender("Male");
    , std::invalid_argument);
    EXPECT_THROW(
        magnemite->set_gender("Female");
    , std::invalid_argument);

    static const std::vector<std::string> mixed_pokemon = boost::assign::list_of
        ("Charmander") // 87.5% male
        ("Growlithe")  // 75% male
        ("Pidgey")     // 50% male
        ("Vulpix")     // 25% male
    ;

    for(auto pokemon_iter = mixed_pokemon.begin(); pokemon_iter != mixed_pokemon.end(); ++pokemon_iter) {
        pkmn::pokemon::sptr pokemon = pkmn::pokemon::make(
                                          *pokemon_iter,
                                          game,
                                          "",
                                          50
                                      );
        EXPECT_NE("Genderless", pokemon->get_gender());

        pokemon->set_gender("Male");
        EXPECT_EQ("Male", pokemon->get_gender());
        pokemon->set_gender("Female");
        EXPECT_EQ("Female", pokemon->get_gender());
        EXPECT_THROW(
            pokemon->set_gender("Genderless");
        , std::invalid_argument);
    }
}

static const std::string games[] = {
    "Gold",
    "Silver",
    "Crystal",
    "Ruby",
    "Sapphire",
    "Emerald",
    "FireRed",
    "LeafGreen",
    "Colosseum",
    "XD",
    "Diamond",
    "Pearl",
    "Platinum",
    "HeartGold",
    "SoulSilver",
    "Black",
    "White",
    "Black 2",
    "White 2"
};

INSTANTIATE_TEST_CASE_P(
    cpp_gender_test,
    gender_test,
    ::testing::ValuesIn(games)
);
