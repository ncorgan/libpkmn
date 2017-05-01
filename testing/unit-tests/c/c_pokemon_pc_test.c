/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/util.h>

#include <pkmn.h>

#define STRBUFFER_LEN 1024

static pkmn_error_t error = PKMN_ERROR_NONE;
static char strbuffer[STRBUFFER_LEN] = {0};

static void test_empty_pokemon_box(
    pkmn_pokemon_box_handle_t box,
    const char* game
) {
    int generation = game_to_generation(game);

    error = pkmn_pokemon_box_get_game(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    error = pkmn_pokemon_box_get_name(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    if(generation == 1) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("", strbuffer);
    }

    pkmn_pokemon_list_t pokemon_list = {
        .pokemon_list = NULL,
        .length = 0
    };
    error = pkmn_pokemon_box_as_array(
                box,
                &pokemon_list
            );

    int num_pokemon = 0;
    error = pkmn_pokemon_box_get_num_pokemon(
                box,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_pokemon);

    int capacity = 0;
    error = pkmn_pokemon_box_get_capacity(
                box,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(((int)pokemon_list.length), capacity);

    // Make sure trying to get a Pokémon at an invalid index fails.
    pkmn_pokemon_handle_t pokemon = NULL;
    error = pkmn_pokemon_box_get_pokemon(
                box,
                -1,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_pokemon_box_get_pokemon(
                box,
                capacity,
                &pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    for(int i = 0; i < capacity; ++i) {
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

static void test_box_name(
    pkmn_pokemon_box_handle_t box
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_box_get_game(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(strbuffer);

    if(generation == 1) {
        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_box_get_name(
                    box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGHI"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_box_get_name(
                    box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("ABCDEFGH", strbuffer);
    }
}

static void test_setting_pokemon_in_box(
    pkmn_pokemon_box_handle_t box
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    char game[STRBUFFER_LEN] = {0};
    int num_pokemon = 0;

    error = pkmn_pokemon_box_get_game(
                box,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);

    pkmn_pokemon_handle_t original_first = NULL;
    pkmn_pokemon_handle_t original_second = NULL;

    error = pkmn_pokemon_box_get_pokemon(
                box,
                0,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_first);

    error = pkmn_pokemon_box_get_pokemon(
                box,
                1,
                &original_second
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_second);

    // Make sure we can't set Pokémon at invalid indices.
    int capacity = 0;
    error = pkmn_pokemon_box_get_capacity(
                box,
                &capacity
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                -1,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                capacity,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    /*
     * Create new Pokémon and place in box. The original variables should
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

    error = pkmn_pokemon_box_set_pokemon(
                box,
                0,
                bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(1, num_pokemon);

    error = pkmn_pokemon_box_set_pokemon(
                box,
                1,
                charmander
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);
    
    // Replace one of the new ones.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                0,
                squirtle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Copy a Pokémon whose memory is already part of the box. Make sure we
    // can't copy it to itself.
    pkmn_pokemon_handle_t second_in_box = NULL;
    error = pkmn_pokemon_box_get_pokemon(
                box,
                1,
                &second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                1,
                second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    error = pkmn_pokemon_box_set_pokemon(
                box,
                2,
                second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    // We should always be able to clear the last contiguous Pokémon.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                2,
                original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Put it back.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                2,
                second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_free(&second_in_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(second_in_box);

    // Check that Pokémon can be placed non-contiguously in the correct games.
    pkmn_pokemon_handle_t fifth_in_box = NULL;
    if(generation <= 2) {
        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    1,
                    original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_pokemon_box_get_num_pokemon(
            box,
            &num_pokemon
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    1,
                    &second_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    second_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

        error = pkmn_pokemon_free(&second_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_in_box);

        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    4,
                    bulbasaur
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
        error = pkmn_pokemon_box_get_num_pokemon(
            box,
            &num_pokemon
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    4,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    fifth_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);

        error = pkmn_pokemon_free(&fifth_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(fifth_in_box);
    } else {
        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    1,
                    original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_num_pokemon(
            box,
            &num_pokemon
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(2, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    1,
                    &second_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    second_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);

        error = pkmn_pokemon_free(&second_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_in_box);

        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    4,
                    bulbasaur
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_num_pokemon(
            box,
            &num_pokemon
        );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    4,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    fifth_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Bulbasaur", strbuffer);

        error = pkmn_pokemon_free(&fifth_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(fifth_in_box);

        // Restore it to how it was.
        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    1,
                    charmander
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_set_pokemon(
                    box,
                    4,
                    original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    3,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    1,
                    &second_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    second_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

        error = pkmn_pokemon_box_get_pokemon(
                    box,
                    4,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_get_species(
                    fifth_in_box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", strbuffer);

        error = pkmn_pokemon_free(&second_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_in_box);

        error = pkmn_pokemon_free(&fifth_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(fifth_in_box);
    }

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    pkmn_pokemon_handle_t current_first = NULL;
    pkmn_pokemon_handle_t current_second = NULL;
    pkmn_pokemon_handle_t current_third = NULL;

    error = pkmn_pokemon_box_get_pokemon(
                box,
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

    error = pkmn_pokemon_box_get_pokemon(
                box,
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

    error = pkmn_pokemon_box_get_pokemon(
                box,
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

static void test_empty_pokemon_pc(
    pkmn_pokemon_pc_handle_t pc,
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_pc_get_game(
                pc,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    int num_boxes = 0;
    error = pkmn_pokemon_pc_get_num_boxes(
                pc,
                &num_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Make sure we can't get boxes at invalid indices.
    pkmn_pokemon_box_handle_t pokemon_box;
    error = pkmn_pokemon_pc_get_box(
                pc,
                -1,
                &pokemon_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);
    error = pkmn_pokemon_pc_get_box(
                pc,
                num_boxes,
                &pokemon_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    pkmn_pokemon_box_list_t pokemon_box_list;
    error = pkmn_pokemon_pc_as_array(
                pc,
                &pokemon_box_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num_boxes, pokemon_box_list.length);

    for(int i = 0; i < num_boxes; ++i) {
        test_empty_pokemon_box(
            pokemon_box_list.pokemon_boxes[i],
            game
        );
    }

    error = pkmn_pokemon_box_list_free(&pokemon_box_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_box_list.pokemon_boxes);
    TEST_ASSERT_EQUAL(0, pokemon_box_list.length);
}

static void test_pc_box_names(
    pkmn_pokemon_pc_handle_t pc
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_handle_t box = NULL;
    pkmn_string_list_t box_names;
    char game[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon_pc_get_game(
                pc,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    int generation = game_to_generation(game);

    if(generation == 1) {
        error = pkmn_pokemon_pc_get_box(
                    pc,
                    0,
                    &box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_pc_get_box_names(
                    pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_box_free(&box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(box);
    } else {
        int num_boxes = 0;
        error = pkmn_pokemon_pc_get_num_boxes(
                    pc,
                    &num_boxes
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(int i = 0; i < num_boxes; ++i) {
            error = pkmn_pokemon_pc_get_box(
                        pc,
                        i,
                        &box
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            char box_name[10] = {0};
            snprintf(box_name, sizeof(box_name), "BOX%d", (i+1));

            error = pkmn_pokemon_box_set_name(
                        box,
                        box_name
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon_box_free(&box);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(box);
        }

        error = pkmn_pokemon_pc_get_box_names(
                    pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        for(int i = 0; i < num_boxes; ++i) {
            char expected_box_name[10] = {0};
            snprintf(expected_box_name, sizeof(expected_box_name), "BOX%d", (i+1));
            TEST_ASSERT_EQUAL_STRING(expected_box_name, box_names.strings[i]);
        }

        error = pkmn_string_list_free(&box_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(box_names.strings);
        TEST_ASSERT_EQUAL(0, box_names.length);
    }
}

static void test_setting_pokemon_in_boxes(
    pkmn_pokemon_pc_handle_t pc
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;
    pkmn_pokemon_box_list_t pokemon_boxes;

    error = pkmn_pokemon_pc_as_array(
                pc,
                &pokemon_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        test_setting_pokemon_in_box(
            pokemon_boxes.pokemon_boxes[i]
        );
    }

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        error = pkmn_pokemon_box_get_pokemon(
                    pokemon_boxes.pokemon_boxes[i],
                    0,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_get_species(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Squirtle", strbuffer);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    pokemon_boxes.pokemon_boxes[i],
                    1,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_get_species(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Charmander", strbuffer);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void pokemon_box_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_handle_t pokemon_box = NULL;

    error = pkmn_pokemon_box_make(
                &pokemon_box,
                game
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    test_empty_pokemon_box(
        pokemon_box,
        game
    );

    test_box_name(pokemon_box);

    test_setting_pokemon_in_box(pokemon_box);

    error = pkmn_pokemon_box_free(&pokemon_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_box);
}

static void pokemon_pc_test(
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_pc_handle_t pokemon_pc = NULL;

    error = pkmn_pokemon_pc_make(
                &pokemon_pc,
                game
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    test_empty_pokemon_pc(
        pokemon_pc,
        game
    );

    test_pc_box_names(pokemon_pc);

    test_setting_pokemon_in_boxes(pokemon_pc);

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_pc);
}

#define BOX_AND_PC_TEST_FCNS(test_game, game_name) \
    static void test_game ## _pokemon_box_test() { \
        pokemon_box_test(game_name); \
    } \
    static void test_game ## _pokemon_pc_test() { \
        pokemon_pc_test(game_name); \
    }

BOX_AND_PC_TEST_FCNS(red, "Red")
BOX_AND_PC_TEST_FCNS(blue, "Blue")
BOX_AND_PC_TEST_FCNS(yellow, "Yellow")
BOX_AND_PC_TEST_FCNS(gold, "Gold")
BOX_AND_PC_TEST_FCNS(silver, "Silver")
BOX_AND_PC_TEST_FCNS(crystal, "Crystal")
BOX_AND_PC_TEST_FCNS(ruby, "Ruby")
BOX_AND_PC_TEST_FCNS(sapphire, "Sapphire")
BOX_AND_PC_TEST_FCNS(emerald, "Emerald")
BOX_AND_PC_TEST_FCNS(firered, "FireRed")
BOX_AND_PC_TEST_FCNS(leafgreen, "LeafGreen")
BOX_AND_PC_TEST_FCNS(colosseum, "Colosseum")
BOX_AND_PC_TEST_FCNS(xd, "XD")

#define BOX_AND_PC_TESTS(test_game) \
    PKMN_C_TEST(test_game ## _pokemon_box_test) \
    PKMN_C_TEST(test_game ## _pokemon_pc_test)

PKMN_C_TEST_MAIN(
    BOX_AND_PC_TESTS(red)
    BOX_AND_PC_TESTS(blue)
    BOX_AND_PC_TESTS(yellow)
    BOX_AND_PC_TESTS(gold)
    BOX_AND_PC_TESTS(silver)
    BOX_AND_PC_TESTS(crystal)
    BOX_AND_PC_TESTS(ruby)
    BOX_AND_PC_TESTS(sapphire)
    BOX_AND_PC_TESTS(emerald)
    BOX_AND_PC_TESTS(firered)
    BOX_AND_PC_TESTS(leafgreen)
    BOX_AND_PC_TESTS(colosseum)
    BOX_AND_PC_TESTS(xd)
)
