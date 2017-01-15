/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_tests_common.h"

#include <pkmntest-c/gba_pokemon_tests.h>

#include <pkmn-c/pokemon.h>
#include <pkmn-c/calculations/form.h>
#include <pkmn-c/database/move_entry.h>

#include <unity.h>

#include <ctype.h>
#include <string.h>

//#define STRBUFFER_LEN 1024
//static char strbuffer[STRBUFFER_LEN] = {0};

void pkmntest_gba_invalid_pokemon_test(
    const char* game
) {
    test_invalid_pokemon(game);
}

/*typedef pkmn_error_t (*pokemon_stat_fcn_t)(pkmn_pokemon_handle_t, pkmn_string_list_t*);

static void gba_pokemon_check_stat_map(
    pkmn_pokemon_handle_t pokemon,
    bool is_stats,
    pokemon_stat_fcn_t stat_fcn
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_string_list_t stat_names;

    error = stat_fcn(
                pokemon,
                &stat_names
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(stat_names.length, (is_stats ? 6 : 5));
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[0], "Attack");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[1], "Defense");
    TEST_ASSERT_EQUAL_STRING(stat_names.strings[2], "HP");
    if(is_stats) {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special Attack");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Special Defense");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[5], "Speed");
    } else {
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[3], "Special");
        TEST_ASSERT_EQUAL_STRING(stat_names.strings[4], "Speed");
    }

    error = pkmn_string_list_free(&stat_names);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}*/

void pkmntest_gba_pokemon_test(
    pkmn_pokemon_handle_t pokemon,
    const char* species,
    const char* game
) {
    (void)pokemon;
    (void)species;
    (void)game;
}
