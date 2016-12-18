/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_POKEMON_TESTS_HPP
#define GEN1_POKEMON_TESTS_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmntest {

    PKMN_API void gen1_invalid_pokemon_test(
        const std::string &game
    );

    PKMN_API void gen1_friendship_test(
        const std::string &game
    );

    PKMN_API void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    );
}

#endif /* GEN1_POKEMON_TESTS_HPP */
