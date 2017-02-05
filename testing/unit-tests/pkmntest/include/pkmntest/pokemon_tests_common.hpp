/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_TESTS_COMMON_HPP
#define POKEMON_TESTS_COMMON_HPP

#include <pkmntest/config.hpp>

#include <string>

namespace pkmntest {

    PKMNTEST_API void invalid_pokemon_test(
        const std::string &game
    );

}

#endif /* POKEMON_TESTS_COMMON_HPP */
