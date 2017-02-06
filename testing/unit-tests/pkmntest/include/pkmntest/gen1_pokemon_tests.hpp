/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_POKEMON_TESTS_HPP
#define GEN1_POKEMON_TESTS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon.hpp>

namespace pkmntest {

    PKMNTEST_API void gen1_friendship_test(
        const std::string &game
    );

    PKMNTEST_API void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    );
}

#endif /* GEN1_POKEMON_TESTS_HPP */
