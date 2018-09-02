/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_TEST_HPP
#define POKEMON_TEST_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon.hpp>

#include <pkmn/enums/ball.hpp>
#include <pkmn/enums/item.hpp>
#include <pkmn/enums/move.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <string>
#include <utility>

class pokemon_test: public ::testing::TestWithParam<std::pair<pkmn::e_game, pkmn::e_species> >
{
    public:
        inline pkmn::pokemon::sptr get_pokemon()
        {
            return _pokemon;
        }

        inline pkmn::e_game get_game()
        {
            return _game;
        }

        inline pkmn::e_species get_species()
        {
            return _species;
        }

        inline void reset()
        {
             _pokemon = pkmn::pokemon::make(GetParam().second, _game, "", 30);

            ASSERT_NE(nullptr, _pokemon.get());
            ASSERT_EQ(_species, _pokemon->get_species());
            ASSERT_EQ(_game, _pokemon->get_game());
        }

    protected:
        void SetUp()
        {
            _game = GetParam().first;
            _species = GetParam().second;
            reset();
        }

    private:

        pkmn::e_species _species;
        pkmn::e_game _game;
        pkmn::pokemon::sptr _pokemon;
};

typedef struct
{
    pkmn::e_ball valid_ball;
    std::vector<pkmn::e_ball> invalid_balls;

    pkmn::e_item valid_item;
    std::vector<pkmn::e_item> invalid_items;

    std::string expected_original_location;
    std::vector<std::string> valid_locations;
    std::vector<std::string> invalid_locations;

    std::vector<pkmn::e_move> moves;
    std::vector<pkmn::e_move> invalid_moves;

    std::vector<pkmn::e_game> valid_original_games;
    std::vector<pkmn::e_game> invalid_original_games;
} pkmn_test_values_t;

void pokemon_test_common(
    const pkmn::pokemon::sptr& pokemon,
    const pkmn_test_values_t& test_values
);

#endif /* POKEMON_TEST_HPP */
