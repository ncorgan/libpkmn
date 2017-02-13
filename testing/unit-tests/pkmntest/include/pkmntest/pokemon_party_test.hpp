/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_PARTY_TEST_HPP
#define POKEMON_PARTY_TEST_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon_party.hpp>

#include <gtest/gtest.h>

#include <string>

class pokemon_party_test: public ::testing::TestWithParam<std::string> {
    public:
        PKMNTEST_INLINE pkmn::pokemon_party::sptr get_party() {
            return _party;
        }

        PKMNTEST_INLINE const std::string& get_game() {
            return _game;
        }

        PKMNTEST_INLINE void reset_party() {
            _party = pkmn::pokemon_party::make(_game);

            ASSERT_NE(nullptr, _party.get());
            ASSERT_EQ(_game, _party->get_game());
            ASSERT_EQ(0, _party->get_num_pokemon());
            ASSERT_EQ(6, _party->as_vector().size());
        }

    protected:
        void SetUp() {
            _game = GetParam();
            reset_party();
        }

    private:

        std::string _game;
        pkmn::pokemon_party::sptr _party;
};

#endif /* POKEMON_PARTY_TEST_HPP */
