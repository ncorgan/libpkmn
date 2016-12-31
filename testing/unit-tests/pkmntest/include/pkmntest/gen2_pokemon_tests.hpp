/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN2_POKEMON_TESTS_HPP
#define GEN2_POKEMON_TESTS_HPP

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmntest {

    PKMN_API void gen2_invalid_pokemon_test(
        const std::string &game
    );

    PKMN_API void gen2_unown_form_test(
        const std::string &game
    );

    PKMN_API void gen2_pokemon_test(
        pkmn::pokemon::sptr pokemon,
        const std::string &species,
        const std::string &game
    );
}

#endif /* GEN2_POKEMON_TESTS_HPP */
