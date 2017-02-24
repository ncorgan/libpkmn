/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMNTEST_GAME_SAVE_TEST_HPP
#define PKMNTEST_GAME_SAVE_TEST_HPP

#include <pkmntest/config.hpp>

#include <pkmn/game_save.hpp>

#include <gtest/gtest.h>

#include <ctime>
#include <random>
#include <string>
#include <utility>

namespace pkmntest {

    typedef std::tuple<std::string, std::string, std::string> game_save_test_params_t;

    class PKMNTEST_API game_save_test: public ::testing::TestWithParam<game_save_test_params_t> {
        public:
            PKMNTEST_INLINE pkmn::game_save::sptr get_game_save() {
                return _game_save;
            }

            PKMNTEST_INLINE const game_save_test_params_t& get_params() {
                return _params;
            }

            void load_save();

        protected:
            // This needs to be in the same object file as the test.
            void SetUp() {
                _params = GetParam();
                load_save();
            }

        private:

            game_save_test_params_t _params;
            pkmn::game_save::sptr _game_save;
    };

    PKMNTEST_API void game_save_test_common_fields(
        pkmn::game_save::sptr save
    );
}

#endif /* PKMNTEST_GAME_SAVE_TEST_HPP */
