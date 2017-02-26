/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMNTEST_POKEMON_TEST_HPP
#define PKMNTEST_POKEMON_TEST_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <string>
#include <utility>

namespace pkmntest {

    class pokemon_test: public ::testing::TestWithParam<std::pair<std::string, std::string> > {
        public:
            static PKMNTEST_INLINE void SetUpTestCase() {
                std::srand((unsigned int)std::time(NULL));
            }

            PKMNTEST_INLINE pkmn::pokemon::sptr get_pokemon() {
                return _pokemon;
            }

            PKMNTEST_INLINE const std::string& get_game() {
                return _game;
            }

            PKMNTEST_INLINE const std::string& get_species() {
                return _species;
            }

            PKMNTEST_INLINE void reset() {
                 _pokemon = pkmn::pokemon::make(GetParam().second, _game, "", 30);

                ASSERT_NE(nullptr, _pokemon.get());
                ASSERT_EQ(GetParam().second, _pokemon->get_species());
                ASSERT_EQ(_game, _pokemon->get_game());
            }

        protected:
            void SetUp() {
                _game = GetParam().first;
                _species = GetParam().second;
                reset();
            }

        private:

            std::string _species, _game;
            pkmn::pokemon::sptr _pokemon;
    };

    typedef struct {
        std::string valid_ball;
        std::vector<std::string> invalid_balls;

        std::string valid_item;
        std::vector<std::string> invalid_items;

        std::string expected_original_location;
        std::vector<std::string> valid_locations;
        std::vector<std::string> invalid_locations;

        std::vector<std::string> moves;
        std::vector<std::string> invalid_moves;

        std::vector<std::string> valid_original_games;
        std::vector<std::string> invalid_original_games;
    } pkmn_test_values_t;

    PKMNTEST_API void pokemon_test_common(
        pkmn::pokemon::sptr pokemon,
        const pkmn_test_values_t &test_values
    );
}

#endif /* PKMNTEST_POKEMON_TEST_HPP */
