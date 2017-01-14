/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest-c/pokemon_pc_tests.h>

#include <unity.h>

#include <stdio.h>
#include <string.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static pkmn_error_t error = PKMN_ERROR_NONE;

static int game_to_generation(
    const char* game
) {
    if(!strcmp(game, "Red") || !strcmp(game, "Blue") || !strcmp(game, "Yellow")) {
        return 1;
    } else if(!strcmp(game, "Gold") || !strcmp(game, "Silver") || !strcmp(game, "Crystal")) {
        return 2;
    } else if(!strcmp(game, "Ruby") || !strcmp(game, "Sapphire") || !strcmp(game, "Emerald") ||
              !strcmp(game, "FireRed") || !strcmp(game, "LeafGreen") ||
              !strcmp(game, "Colosseum") || !strcmp(game, "XD"))
    {
        return 3;
    } else if(!strcmp(game, "Diamond") || !strcmp(game, "Pearl") || !strcmp(game, "Platinum") ||
              !strcmp(game, "HeartGold") || !strcmp(game, "SoulSilver"))
    {
        return 4;
    } else if(!strcmp(game, "Black") || !strcmp(game, "White") ||
              !strcmp(game, "Black 2") || !strcmp(game, "White 2"))
    {
        return 5;
    } else if(!strcmp(game, "X") || !strcmp(game, "Y") ||
              !strcmp(game, "Omega Ruby") || !strcmp(game, "Alpha Sapphire"))
    {
        return 6;
    } else {
        return 0;
    }
}

void pkmntest_test_empty_pokemon_box(
    pkmn_pokemon_box_handle_t box,
    const char* game
) {
    int generation = game_to_generation(game);

    error = pkmn_pokemon_box_get_game(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    error = pkmn_pokemon_box_get_name(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    if(generation == 1) {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, "");
    }

    pkmn_pokemon_list_t pokemon_list = {
        .pokemon_list = NULL,
        .length = 0
    };
    error = pkmn_pokemon_box_as_array(
                box,
                &pokemon_list
            );

    int capacity = 0;
    error = pkmn_pokemon_box_get_capacity(
                box,
                &capacity
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    TEST_ASSERT_EQUAL(capacity, ((int)pokemon_list.length));

    // Make sure trying to get a Pokémon at an invalid index fails.
    pkmn_pokemon_handle_t pokemon = NULL;
    error = pkmn_pokemon_box_get_pokemon(
                box,
                -1,
                &pokemon
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_box_get_pokemon(
                box,
                capacity,
                &pokemon
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    for(int i = 0; i < capacity; ++i) {
        error = pkmn_pokemon_get_species(
                    pokemon_list.pokemon_list[i],
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

        error = pkmn_pokemon_get_game(
                    pokemon_list.pokemon_list[i],
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, game);

        pkmn_move_slots_t move_slots = {
            .move_slots = NULL,
            .length = 0
        };
        error = pkmn_pokemon_get_moves(
                    pokemon_list.pokemon_list[i],
                    &move_slots
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL(move_slots.length, 4);

        for(int j = 0; j < 4; ++j) {
            TEST_ASSERT_EQUAL_STRING(move_slots.move_slots[j].move, "None");
            TEST_ASSERT_EQUAL(move_slots.move_slots[j].pp, 0);
        }

        error = pkmn_move_slots_free(&move_slots);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

void pkmntest_test_box_name(
    pkmn_pokemon_box_handle_t box
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_box_get_game(
                box,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int generation = game_to_generation(strbuffer);

    if(generation == 1) {
        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

        error = pkmn_pokemon_box_get_name(
                    box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);
    } else {
        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGHI"
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_INVALID_ARGUMENT);

        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_box_get_name(
                    box,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, "ABCDEFGH");
    }
}

void pkmntest_test_setting_pokemon_in_box(
    pkmn_pokemon_box_handle_t box
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    char game[STRBUFFER_LEN] = {0};

    error = pkmn_pokemon_box_get_game(
                box,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_pokemon_handle_t original_first = NULL;
    pkmn_pokemon_handle_t original_second = NULL;

    error = pkmn_pokemon_box_get_pokemon(
                box,
                0,
                &original_first
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(original_first);

    error = pkmn_pokemon_box_get_pokemon(
                box,
                1,
                &original_second
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(original_second);

    // Make sure we can't set Pokémon at invalid indices.
    int capacity = 0;
    error = pkmn_pokemon_box_get_capacity(
                box,
                &capacity
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                -1,
                original_first
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                capacity,
                original_first
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    // Make sure we can't move these.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                2,
                original_first
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                3,
                original_second
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(bulbasaur);

    pkmn_pokemon_handle_t charmander = NULL;
    error = pkmn_pokemon_make(
                &charmander,
                "Charmander",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(charmander);

    pkmn_pokemon_handle_t squirtle = NULL;
    error = pkmn_pokemon_make(
                &squirtle,
                "Squirtle",
                game,
                "",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(squirtle);

    error = pkmn_pokemon_box_set_pokemon(
                box,
                0,
                bulbasaur
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                1,
                charmander
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    // Make sure we can't do that again.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                2,
                bulbasaur
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    error = pkmn_pokemon_box_set_pokemon(
                box,
                3,
                charmander
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RUNTIME_ERROR);
    
    // Replace one of the new ones.
    error = pkmn_pokemon_box_set_pokemon(
                box,
                0,
                squirtle
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    pkmn_pokemon_handle_t current_first = NULL;
    pkmn_pokemon_handle_t current_second = NULL;

    error = pkmn_pokemon_box_get_pokemon(
                box,
                0,
                &current_first
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_species(
                current_first,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Squirtle");

    error = pkmn_pokemon_box_get_pokemon(
                box,
                1,
                &current_second
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    error = pkmn_pokemon_get_species(
                current_second,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Charmander");

    error = pkmn_pokemon_get_species(
                original_first,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");
    error = pkmn_pokemon_get_species(
                original_second,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "None");

    error = pkmn_pokemon_get_species(
                bulbasaur,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Bulbasaur");
    error = pkmn_pokemon_get_species(
                charmander,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Charmander");
    error = pkmn_pokemon_get_species(
                squirtle,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, "Squirtle");

    // Cleanup

    error = pkmn_pokemon_free(&original_first);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(original_first);

    error = pkmn_pokemon_free(&original_second);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(original_second);

    error = pkmn_pokemon_free(&bulbasaur);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(bulbasaur);

    error = pkmn_pokemon_free(&charmander);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(charmander);

    error = pkmn_pokemon_free(&squirtle);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(squirtle);

    error = pkmn_pokemon_free(&current_first);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(current_first);

    error = pkmn_pokemon_free(&current_second);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(current_second);
}

void pkmntest_test_empty_pokemon_pc(
    pkmn_pokemon_pc_handle_t pc,
    const char* game
) {
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_pokemon_pc_get_game(
                pc,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL_STRING(strbuffer, game);

    int num_boxes = 0;
    error = pkmn_pokemon_pc_get_num_boxes(
                pc,
                &num_boxes
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    // Make sure we can't get boxes at invalid indices.
    pkmn_pokemon_box_handle_t pokemon_box;
    error = pkmn_pokemon_pc_get_box(
                pc,
                -1,
                &pokemon_box
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);
    error = pkmn_pokemon_pc_get_box(
                pc,
                num_boxes,
                &pokemon_box
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_RANGE_ERROR);

    pkmn_pokemon_box_list_t pokemon_box_list;
    error = pkmn_pokemon_pc_as_array(
                pc,
                &pokemon_box_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_EQUAL(pokemon_box_list.length, num_boxes);

    for(int i = 0; i < num_boxes; ++i) {
        pkmntest_test_empty_pokemon_box(
            pokemon_box_list.pokemon_boxes[i],
            game
        );
    }

    error = pkmn_pokemon_box_list_free(&pokemon_box_list);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pokemon_box_list.pokemon_boxes);
    TEST_ASSERT_EQUAL(pokemon_box_list.length, 0);
}

void pkmntest_test_pc_box_names(
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    int generation = game_to_generation(game);

    if(generation == 1) {
        error = pkmn_pokemon_pc_get_box(
                    pc,
                    0,
                    &box
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        error = pkmn_pokemon_box_set_name(
                    box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

        error = pkmn_pokemon_pc_get_box_names(
                    pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR);

        error = pkmn_pokemon_box_free(&box);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NULL(box);
    } else {
        int num_boxes = 0;
        error = pkmn_pokemon_pc_get_num_boxes(
                    pc,
                    &num_boxes
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

        for(int i = 0; i < num_boxes; ++i) {
            error = pkmn_pokemon_pc_get_box(
                        pc,
                        i,
                        &box
                    );
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            char box_name[10] = {0};
            snprintf(box_name, sizeof(box_name), "BOX%d", (i+1));

            error = pkmn_pokemon_box_set_name(
                        box,
                        box_name
                    );
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

            error = pkmn_pokemon_box_free(&box);
            TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
            TEST_ASSERT_NULL(box);
        }

        error = pkmn_pokemon_pc_get_box_names(
                    pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        for(int i = 0; i < num_boxes; ++i) {
            char expected_box_name[10] = {0};
            snprintf(expected_box_name, sizeof(expected_box_name), "BOX%d", (i+1));
            TEST_ASSERT_EQUAL_STRING(box_names.strings[i], expected_box_name);
        }

        error = pkmn_string_list_free(&box_names);
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NULL(box_names.strings);
        TEST_ASSERT_EQUAL(box_names.length, 0);
    }
}

void pkmntest_test_setting_pokemon_in_boxes(
    pkmn_pokemon_pc_handle_t pc
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;
    pkmn_pokemon_box_list_t pokemon_boxes;

    error = pkmn_pokemon_pc_as_array(
                pc,
                &pokemon_boxes
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        pkmntest_test_setting_pokemon_in_box(
            pokemon_boxes.pokemon_boxes[i]
        );
    }

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        error = pkmn_pokemon_box_get_pokemon(
                    pokemon_boxes.pokemon_boxes[i],
                    0,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_get_species(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, "Squirtle");

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_NULL(pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    pokemon_boxes.pokemon_boxes[i],
                    1,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_get_species(
                    pokemon,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
        TEST_ASSERT_EQUAL_STRING(strbuffer, "Charmander");

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}
