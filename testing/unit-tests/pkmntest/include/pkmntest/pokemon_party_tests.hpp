/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_PARTY_TESTS_HPP
#define POKEMON_PARTY_TESTS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon_party.hpp>

#include <string>

namespace pkmntest {

    PKMNTEST_API void test_empty_pokemon_party(
        pkmn::pokemon_party::sptr party,
        const std::string &game
    );

    PKMNTEST_API void test_setting_pokemon(
        pkmn::pokemon_party::sptr party
    );

}

#endif /* POKEMON_PARTY_TESTS_HPP */
