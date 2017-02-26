/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMNTEST_POKEMON_PC_TEST_HPP
#define PKMNTEST_POKEMON_PC_TEST_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_pc.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <string>
#include <utility>

namespace pkmntest {

    class pokemon_box_test: public ::testing::TestWithParam<std::string> {
        public:
            PKMNTEST_INLINE pkmn::pokemon_box::sptr get_pokemon_box() {
                return _pokemon_box;
            }

            PKMNTEST_INLINE const std::string& get_game() {
                return _game;
            }

            PKMNTEST_INLINE void reset() {
                _pokemon_box = pkmn::pokemon_box::make(_game);

                ASSERT_NE(nullptr, _pokemon_box.get());
                ASSERT_EQ(_game, _pokemon_box->get_game());
            }

        protected:
            void SetUp() {
                _game = GetParam();
                reset();
            }

        private:

            std::string _game;
            pkmn::pokemon_box::sptr _pokemon_box;
    };

    PKMNTEST_API void pokemon_box_test_common(
        pkmn::pokemon_box::sptr box 
    );

    class pokemon_pc_test: public ::testing::TestWithParam<std::string> {
        public:
            PKMNTEST_INLINE pkmn::pokemon_pc::sptr get_pokemon_pc() {
                return _pokemon_pc;
            }

            PKMNTEST_INLINE const std::string& get_game() {
                return _game;
            }

            PKMNTEST_INLINE void reset() {
                _pokemon_pc = pkmn::pokemon_pc::make(_game);

                ASSERT_NE(nullptr, _pokemon_pc.get());
                ASSERT_EQ(_game, _pokemon_pc->get_game());
            }

        protected:
            void SetUp() {
                _game = GetParam();
                reset();
            }

        private:

            std::string _game;
            pkmn::pokemon_pc::sptr _pokemon_pc;
    };

    PKMNTEST_API void pokemon_pc_test_common(
        pkmn::pokemon_pc::sptr pc
    );
}

#endif /* PKMNTEST_POKEMON_PC_TEST_HPP */
