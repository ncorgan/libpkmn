/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
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

class gender_test: public ::testing::TestWithParam<pkmn::e_game> {};

TEST_P(gender_test, gender_test)
{
    pkmn::e_game game = GetParam();

    // Single-gender
    pkmn::pokemon::sptr nidorina = pkmn::pokemon::make(
                                       "Nidorina",
                                       game,
                                       "",
                                       50
                                   );
    EXPECT_EQ(pkmn::e_gender::FEMALE, nidorina->get_gender());
    nidorina->set_gender(pkmn::e_gender::FEMALE);
    EXPECT_THROW(
        nidorina->set_gender(pkmn::e_gender::MALE);
    , std::invalid_argument);
    EXPECT_THROW(
        nidorina->set_gender(pkmn::e_gender::GENDERLESS);
    , std::invalid_argument);

    pkmn::pokemon::sptr nidorino = pkmn::pokemon::make(
                                       "Nidorino",
                                       game,
                                       "",
                                       50
                                   );
    EXPECT_EQ(pkmn::e_gender::MALE, nidorino->get_gender());
    nidorino->set_gender(pkmn::e_gender::MALE);
    EXPECT_THROW(
        nidorino->set_gender(pkmn::e_gender::FEMALE);
    , std::invalid_argument);
    EXPECT_THROW(
        nidorino->set_gender(pkmn::e_gender::GENDERLESS);
    , std::invalid_argument);

    pkmn::pokemon::sptr magnemite = pkmn::pokemon::make(
                                        "Magnemite",
                                        game,
                                        "",
                                        50
                                    );
    EXPECT_EQ(pkmn::e_gender::GENDERLESS, magnemite->get_gender());
    magnemite->set_gender(pkmn::e_gender::GENDERLESS);
    EXPECT_THROW(
        magnemite->set_gender(pkmn::e_gender::MALE);
    , std::invalid_argument);
    EXPECT_THROW(
        magnemite->set_gender(pkmn::e_gender::FEMALE);
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
        EXPECT_NE(pkmn::e_gender::GENDERLESS, pokemon->get_gender());

        pokemon->set_gender(pkmn::e_gender::MALE);
        EXPECT_EQ(pkmn::e_gender::MALE, pokemon->get_gender());
        pokemon->set_gender(pkmn::e_gender::FEMALE);
        EXPECT_EQ(pkmn::e_gender::FEMALE, pokemon->get_gender());
        EXPECT_THROW(
            pokemon->set_gender(pkmn::e_gender::GENDERLESS);
        , std::invalid_argument);
    }
}

static const std::vector<pkmn::e_game> games =
{
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL,
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD
};

INSTANTIATE_TEST_CASE_P(
    cpp_gender_test,
    gender_test,
    ::testing::ValuesIn(games)
);
