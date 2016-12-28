/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GAME_SAVE_TESTS_HPP
#define GAME_SAVE_TESTS_HPP

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>

#include <string>

namespace pkmntest {

    PKMN_API void game_save_test_common_fields(
        pkmn::game_save::sptr save
    );

}

#endif /* GAME_SAVE_TESTS_HPP */
