/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GAME_SAVE_TESTS_COMMON_HPP
#define GAME_SAVE_TESTS_COMMON_HPP

#include <pkmn/game_save.hpp>

#include <string>

namespace pkmntest {

    void test_game_save_common_fields(
        pkmn::game_save::sptr save
    );

}

#endif /* GAME_SAVE_TESTS_COMMON_HPP */
