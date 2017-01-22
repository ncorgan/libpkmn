/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef GEN1_POKEMON_PC_TESTS_HPP
#define GEN1_POKEMON_PC_TESTS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon_box.hpp>
#include <pkmn/pokemon_pc.hpp>

#include <string>

namespace pkmntest {

    PKMNTEST_API void test_empty_pokemon_box(
        pkmn::pokemon_box::sptr box,
        const std::string &game
    );

    PKMNTEST_API void test_box_name(
        pkmn::pokemon_box::sptr box
    );

    PKMNTEST_API void test_setting_pokemon(
        pkmn::pokemon_box::sptr box
    );

    PKMNTEST_API void test_empty_pokemon_pc(
        pkmn::pokemon_pc::sptr pc,
        const std::string &game
    );

    PKMNTEST_API void test_pc_box_names(
        pkmn::pokemon_pc::sptr pc
    );

    PKMNTEST_API void test_setting_pokemon_in_boxes(
        pkmn::pokemon_pc::sptr pc
    );

}

#endif /* GEN1_POKEMON_PC_TESTS_HPP */
