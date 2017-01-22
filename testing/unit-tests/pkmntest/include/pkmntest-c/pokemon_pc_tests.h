/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_PC_TESTS_H
#define POKEMON_PC_TESTS_H

#include <pkmntest-c/config.h>

#include <pkmn-c/pokemon_box.h>
#include <pkmn-c/pokemon_pc.h>

PKMNTEST_API void pkmntest_test_empty_pokemon_box(
    pkmn_pokemon_box_handle_t box,
    const char* game
);

PKMNTEST_API void pkmntest_test_box_name(
    pkmn_pokemon_box_handle_t box
);

PKMNTEST_API void pkmntest_test_setting_pokemon_in_box(
    pkmn_pokemon_box_handle_t box
);

PKMNTEST_API void pkmntest_test_empty_pokemon_pc(
    pkmn_pokemon_pc_handle_t pc,
    const char* game
);

PKMNTEST_API void pkmntest_test_pc_box_names(
    pkmn_pokemon_pc_handle_t pc
);

PKMNTEST_API void pkmntest_test_setting_pokemon_in_boxes(
    pkmn_pokemon_pc_handle_t pc
);

#endif /* POKEMON_PC_TESTS_H */
