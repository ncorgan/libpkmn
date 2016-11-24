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

    PKMN_API void gen1_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    );

    // TODO: box and PC tests when implemented
}

#endif /* GEN1_POKEMON_TESTS_HPP */
