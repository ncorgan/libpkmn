/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_POKEMON_PC_TESTS_HPP
#define GEN1_POKEMON_PC_TESTS_HPP

#include <pkmn/config.hpp>

#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_pc.hpp>

#include <string>

namespace pkmntest {

    PKMN_API void test_empty_pokemon_box(
        pkmn::pokemon_box::sptr box,
        const std::string &game
    );

    PKMN_API void test_empty_pokemon_pc(
        pkmn::pokemon_pc::sptr pc,
        const std::string &game
    );

}

#endif /* GEN1_POKEMON_PC_TESTS_HPP */
