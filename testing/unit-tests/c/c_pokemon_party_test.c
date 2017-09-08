/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static pkmn_error_t error = PKMN_ERROR_NONE;

static void test_empty_pokemon_party(
    pkmn_pokemon_party_handle_t party,
    const char* game
) {
    error = pkmn_pokemon_party_get_game(
                party,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    pkmn_pokemon_list_t pokemon_list = {
        .pokemon_list = NULL,
        .length = 0
    };
    error = pkmn_pokemon_party_as_array(
                party,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(6, pokemon_list.length);

    int num_pokemon = 0;
    error = pkmn_pokemon_party_get_num_pokemon(
                party,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_pokemon);

    // Make sure trying to get a Pokémon at an invalid index fails.
    pkmn_pokemon_handle_t pokemon = NULL;
    error = pkmn_pokemon_party_get_pokemon(
                party,
                -1,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_party_get_pokemon(
                party,
                6,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    for(int i = 0; i < 6; ++i) {
        error = pkmn_pokemon_get_species(
                    pokemon_list.pokemon_list[i],
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);

        error = pkmn_pokemon_get_game(
                    pokemon_list.pokemon_list[i],
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(game, strbuffer);

        pkmn_move_slots_t move_slots = {
            .move_slots = NULL,
            .length = 0
        };
        error = pkmn_pokemon_get_moves(
                    pokemon_list.pokemon_list[i],
                    &move_slots
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(4, move_slots.length);

        for(int j = 0; j < 4; ++j) {
            TEST_ASSERT_EQUAL_STRING("None", move_slots.move_slots[j].move);
            TEST_ASSERT_EQUAL(0, move_slots.move_slots[j].pp);
        }

        error = pkmn_move_slots_free(&move_slots);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void test_setting_pokemon_in_party(
    pkmn_pokemon_party_handle_t party
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    char game[STRBUFFER_LEN] = {0};
    int num_pokemon = 0;

    error = pkmn_pokemon_party_get_game(
                party,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_handle_t original_first = NULL;
    pkmn_pokemon_handle_t original_second = NULL;

    error = pkmn_pokemon_party_get_pokemon(
                party,
                0,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_first);

    error = pkmn_pokemon_party_get_pokemon(
                party,
                1,
                &original_second
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_second);

    // Make sure we can't set Pokémon at invalid indices.
    error = pkmn_pokemon_party_set_pokemon(
                party,
                -1,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_party_set_pokemon(
                party,
                6,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Create new Pokémon and place in party. The original variables should
     * have the same underlying Pokémon.
     */
    pkmn_pokemon_handle_t bulbasaur = NULL;
    error = pkmn_pokemon_make(
                &bulbasaur,
                "Bulbasaur",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(bulbasaur);

    pkmn_pokemon_handle_t charmander = NULL;
    error = pkmn_pokemon_make(
                &charmander,
                "Charmander",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(charmander);

    pkmn_pokemon_handle_t squirtle = NULL;
    error = pkmn_pokemon_make(
                &squirtle,
                "Squirtle",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(squirtle);

    error = pkmn_pokemon_party_set_pokemon(
                party,
                0,
                bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(1, num_pokemon);

    error = pkmn_pokemon_party_set_pokemon(
                party,
                1,
                charmander
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);
    
    // Replace one of the new ones.
    error = pkmn_pokemon_party_set_pokemon(
                party,
                0,
                squirtle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Copy a Pokémon whose memory is already part of the party. Make sure we
    // can't copy it to itself.
    pkmn_pokemon_handle_t second_in_party = NULL;
    error = pkmn_pokemon_party_get_pokemon(
                party,
                1,
                &second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_set_pokemon(
                party,
                1,
                second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    error = pkmn_pokemon_party_set_pokemon(
                party,
                2,
                second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    // We should be able to clear the last contiguous Pokémon.
    error = pkmn_pokemon_party_set_pokemon(
                party,
                2,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Put it back.
    error = pkmn_pokemon_party_set_pokemon(
                party,
                2,
                second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_free(&second_in_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(second_in_party);

    // Check that Pokémon cannot be placed non-contiguously.
    pkmn_pokemon_handle_t fifth_in_party = NULL;
    error = pkmn_pokemon_party_set_pokemon(
                party,
                1,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_party_get_pokemon(
                party,
                1,
                &second_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_get_species(
                second_in_party,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

    error = pkmn_pokemon_free(&second_in_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(second_in_party);

    error = pkmn_pokemon_party_set_pokemon(
                party,
                4,
                bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_party_get_num_pokemon(
        party,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_party_get_pokemon(
                party,
                4,
                &fifth_in_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_get_species(
                fifth_in_party,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("None", strbuffer);

    error = pkmn_pokemon_free(&fifth_in_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(fifth_in_party);

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    pkmn_pokemon_handle_t current_first = NULL;
    pkmn_pokemon_handle_t current_second = NULL;
    pkmn_pokemon_handle_t current_third = NULL;

    error = pkmn_pokemon_party_get_pokemon(
                party,
                0,
                &current_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_species(
                current_first,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Squirtle", strbuffer);

    error = pkmn_pokemon_party_get_pokemon(
                party,
                1,
                &current_second
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_species(
                current_second,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

    error = pkmn_pokemon_party_get_pokemon(
                party,
                2,
                &current_third
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_species(
                current_third,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

    error = pkmn_pokemon_get_species(
                original_first,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("None", strbuffer);
    error = pkmn_pokemon_get_species(
                original_second,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("None", strbuffer);

    error = pkmn_pokemon_get_species(
                bulbasaur,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Bulbasaur", strbuffer);
    error = pkmn_pokemon_get_species(
                charmander,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);
    error = pkmn_pokemon_get_species(
                squirtle,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING("Squirtle", strbuffer);

    // Cleanup

    error = pkmn_pokemon_free(&original_first);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(original_first);

    error = pkmn_pokemon_free(&original_second);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(original_second);

    error = pkmn_pokemon_free(&bulbasaur);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(bulbasaur);

    error = pkmn_pokemon_free(&charmander);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(charmander);

    error = pkmn_pokemon_free(&squirtle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(squirtle);

    error = pkmn_pokemon_free(&current_first);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(current_first);

    error = pkmn_pokemon_free(&current_second);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(current_second);

    error = pkmn_pokemon_free(&current_third);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(current_third);
}

static void pokemon_party_test_common(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_party_handle_t party = NULL;

    error = pkmn_pokemon_party_make(
                &party,
                game
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(party);

    test_empty_pokemon_party(party, game);
    test_setting_pokemon_in_party(party);

    error = pkmn_pokemon_party_free(&party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(party);
}

// Red

static void red_pokemon_party_test() {
    pokemon_party_test_common("Red");
}

// Blue

static void blue_pokemon_party_test() {
    pokemon_party_test_common("Blue");
}

// Yellow

static void yellow_pokemon_party_test() {
    pokemon_party_test_common("Yellow");
}

// Gold

static void gold_pokemon_party_test() {
    pokemon_party_test_common("Gold");
}

// Silver

static void silver_pokemon_party_test() {
    pokemon_party_test_common("Silver");
}

// Crystal

static void crystal_pokemon_party_test() {
    pokemon_party_test_common("Crystal");
}

// Ruby

static void ruby_pokemon_party_test() {
    pokemon_party_test_common("Ruby");
}

// Sapphire

static void sapphire_pokemon_party_test() {
    pokemon_party_test_common("Sapphire");
}

// Emerald

static void emerald_pokemon_party_test() {
    pokemon_party_test_common("Emerald");
}

// FireRed

static void firered_pokemon_party_test() {
    pokemon_party_test_common("FireRed");
}

// LeafGreen

static void leafgreen_pokemon_party_test() {
    pokemon_party_test_common("LeafGreen");
}

// Colosseum

static void colosseum_pokemon_party_test() {
    pokemon_party_test_common("Colosseum");
}

// XD

static void xd_pokemon_party_test() {
    pokemon_party_test_common("XD");
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(red_pokemon_party_test)
    PKMN_C_TEST(blue_pokemon_party_test)
    PKMN_C_TEST(yellow_pokemon_party_test)

    PKMN_C_TEST(gold_pokemon_party_test)
    PKMN_C_TEST(silver_pokemon_party_test)
    PKMN_C_TEST(crystal_pokemon_party_test)

    PKMN_C_TEST(ruby_pokemon_party_test)
    PKMN_C_TEST(sapphire_pokemon_party_test)
    PKMN_C_TEST(emerald_pokemon_party_test)
    PKMN_C_TEST(firered_pokemon_party_test)
    PKMN_C_TEST(leafgreen_pokemon_party_test)

    PKMN_C_TEST(colosseum_pokemon_party_test)
    PKMN_C_TEST(xd_pokemon_party_test)
)
