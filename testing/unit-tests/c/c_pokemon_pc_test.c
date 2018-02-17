/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/util.h>

#include <pkmn.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static const pkmn_pokemon_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_box_t empty_pokemon_box =
{
    .game = NULL,
    .capacity = 0,
    ._internal = NULL
};
static const pkmn_pokemon_list_t empty_pokemon_list =
{
    .pokemon = NULL,
    .length = 0
};
static const pkmn_pokemon_box_list_t empty_pokemon_box_list =
{
    .boxes = NULL,
    .length = 0
};
static const pkmn_string_list_t empty_string_list =
{
    .strings = NULL,
    .length = 0
};

static void test_empty_pokemon_box(
    pkmn_pokemon_box_t* box_ptr
)
{
    TEST_ASSERT_NOT_NULL(box_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(box_ptr->game);

    error = pkmn_pokemon_box_get_name(
                box_ptr,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    if(generation == 1)
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    pkmn_pokemon_list_t pokemon_list = empty_pokemon_list;

    size_t num_pokemon = 0;
    error = pkmn_pokemon_box_get_num_pokemon(
                box_ptr,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(0, num_pokemon);

    error = pkmn_pokemon_box_as_list(
                box_ptr,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_TRUE(pokemon_list.length > 0);

    for(size_t pokemon_index = 0; pokemon_index < pokemon_list.length; ++pokemon_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            pokemon_list.pokemon[pokemon_index].species
        );
        TEST_ASSERT_EQUAL_STRING(
            box_ptr->game,
            pokemon_list.pokemon[pokemon_index].game
        );
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);
}

static void test_box_name(
    pkmn_pokemon_box_t* box_ptr
)
{
    TEST_ASSERT_NOT_NULL(box_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(box_ptr->game);

    if(generation == 1)
    {
        error = pkmn_pokemon_box_set_name(
                    box_ptr,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_box_get_name(
                    box_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        error = pkmn_pokemon_box_set_name(
                    box_ptr,
                    "ABCDEFGHI"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_box_set_name(
                    box_ptr,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_box_get_name(
                    box_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("ABCDEFGH", strbuffer);
    }
}

static void test_setting_pokemon_in_box(
    pkmn_pokemon_box_t* box_ptr
)
{
    TEST_ASSERT_NOT_NULL(box_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    size_t num_pokemon = 0;

    pkmn_pokemon_t original_first = empty_pokemon;
    pkmn_pokemon_t original_second = empty_pokemon;

    int generation = game_to_generation(box_ptr->game);

    error = pkmn_pokemon_box_get_pokemon(
                box_ptr,
                0,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_first._internal);

    error = pkmn_pokemon_box_get_pokemon(
                box_ptr,
                1,
                &original_second
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(original_second._internal);

    // Make sure we can't get Pokémon at invalid indices.
    error = pkmn_pokemon_box_get_pokemon(
                box_ptr,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_box_get_pokemon(
                box_ptr,
                box_ptr->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Make sure we can't set Pokémon at invalid indices.
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                box_ptr->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Create new Pokémon and place in box. The original variables should
     * have the same underlying Pokémon.
     */

    pkmn_pokemon_t bulbasaur = empty_pokemon;
    pkmn_pokemon_t charmander = empty_pokemon;
    pkmn_pokemon_t squirtle = empty_pokemon;

    error = pkmn_pokemon_init(
                "Bulbasaur",
                box_ptr->game,
                "",
                5,
                &bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(bulbasaur._internal);

    error = pkmn_pokemon_init(
                "Charmander",
                box_ptr->game,
                "",
                5,
                &charmander
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(charmander._internal);

    error = pkmn_pokemon_init(
                "Squirtle",
                box_ptr->game,
                "",
                5,
                &squirtle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(squirtle._internal);

    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                0,
                &bulbasaur
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
                box_ptr,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(1, num_pokemon);

    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                1,
                &charmander
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
                box_ptr,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Replace one of the new ones.
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                0,
                &squirtle
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
                box_ptr,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Copy a Pokémon whose memory is already part of the box. Make sure we
    // can't copy it to itself.

    pkmn_pokemon_t second_in_box = empty_pokemon;
    error = pkmn_pokemon_box_get_pokemon(
                box_ptr,
                1,
                &second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(second_in_box._internal);

    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                1,
                &second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                2,
                &second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
                box_ptr,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    // We should be able to clear the last contiguous Pokémon.
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                2,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box_ptr,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Put it back.
    error = pkmn_pokemon_box_set_pokemon(
                box_ptr,
                2,
                &second_in_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_get_num_pokemon(
        box_ptr,
        &num_pokemon
    );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_free(&second_in_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(second_in_box._internal);

    // Check that Pokémon can be placed non-contiguously in the correct games.
    pkmn_pokemon_t fifth_in_box = empty_pokemon;

    if(generation <= 2)
    {
        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    1,
                    &original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    box_ptr,
                    &num_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box_ptr,
                    1,
                    &second_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Charmander", second_in_box.species);

        error = pkmn_pokemon_free(&second_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_in_box._internal);

        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    4,
                    &bulbasaur
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

        error = pkmn_pokemon_box_get_pokemon(
                    box_ptr,
                    4,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", fifth_in_box.species);

        error = pkmn_pokemon_free(&fifth_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(fifth_in_box._internal);
    }
    else
    {
        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    1,
                    &original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    box_ptr,
                    &num_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(2, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box_ptr,
                    1,
                    &second_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("None", second_in_box.species);

        error = pkmn_pokemon_free(&second_in_box);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_in_box._internal);

        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    4,
                    &bulbasaur
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    box_ptr,
                    &num_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    box_ptr,
                    4,
                    &fifth_in_box
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING("Bulbasaur", fifth_in_box.species);

        // Restore it to how it was.

        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    1,
                    &charmander
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_set_pokemon(
                    box_ptr,
                    4,
                    &original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    box_ptr,
                    &num_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(3, num_pokemon);
    }

    error = pkmn_pokemon_free(&second_in_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(second_in_box._internal);

    error = pkmn_pokemon_free(&fifth_in_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(fifth_in_box._internal);

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    pkmn_pokemon_list_t pokemon_list = empty_pokemon_list;
    error = pkmn_pokemon_box_as_list(
                box_ptr,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_TRUE(pokemon_list.length > 0);

    TEST_ASSERT_EQUAL_STRING(
        "Squirtle",
        pokemon_list.pokemon[0].species
    );
    TEST_ASSERT_EQUAL_STRING(
        "Charmander",
        pokemon_list.pokemon[1].species
    );
    TEST_ASSERT_EQUAL_STRING(
        "Charmander",
        pokemon_list.pokemon[2].species
    );
    TEST_ASSERT_EQUAL_STRING("None", original_first.species);
    TEST_ASSERT_EQUAL_STRING("None", original_second.species);
    TEST_ASSERT_EQUAL_STRING("Bulbasaur", bulbasaur.species);
    TEST_ASSERT_EQUAL_STRING("Charmander", charmander.species);
    TEST_ASSERT_EQUAL_STRING("Squirtle", squirtle.species);

    // Cleanup

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_NULL(pokemon_list.pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);

    error = pkmn_pokemon_free(&original_first);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(original_first._internal);

    error = pkmn_pokemon_free(&original_second);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(original_second._internal);

    error = pkmn_pokemon_free(&bulbasaur);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(bulbasaur._internal);

    error = pkmn_pokemon_free(&charmander);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(charmander._internal);

    error = pkmn_pokemon_free(&squirtle);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(squirtle._internal);
}

static void test_empty_pokemon_pc(
    pkmn_pokemon_pc_t* pc_ptr
)
{
    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_box_list_t pokemon_box_list = empty_pokemon_box_list;
    error = pkmn_pokemon_pc_as_list(
                pc_ptr,
                &pokemon_box_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_box_list.boxes);
    TEST_ASSERT_TRUE(pokemon_box_list.length > 0);

    for(size_t box_index = 0; box_index < pokemon_box_list.length; ++box_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            pc_ptr->game,
            pokemon_box_list.boxes[box_index].game
        );
        test_empty_pokemon_box(
            &pokemon_box_list.boxes[box_index]
        );
    }

    error = pkmn_pokemon_box_list_free(&pokemon_box_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_box_list.boxes);
    TEST_ASSERT_EQUAL(0, pokemon_box_list.length);
}

static void test_pc_box_names(
    pkmn_pokemon_pc_t* pc_ptr
)
{
    TEST_ASSERT_NOT_NULL(pc_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_string_list_t box_names = empty_string_list;

    int generation = game_to_generation(pc_ptr->game);

    if(generation == 1)
    {
        error = pkmn_pokemon_pc_get_box_names(
                    pc_ptr,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        for(size_t box_index = 0; box_index < pc_ptr->capacity; ++box_index)
        {
            pkmn_pokemon_box_t pokemon_box = empty_pokemon_box;

            error = pkmn_pokemon_pc_get_box(
                        pc_ptr,
                        box_index,
                        &pokemon_box
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            char box_name[10] = {0};
            snprintf(box_name, sizeof(box_name), "BOX%zu", (box_index+1));

            error = pkmn_pokemon_box_set_name(
                        &pokemon_box,
                        box_name
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon_box_free(&pokemon_box);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon_box._internal);
        }

        error = pkmn_pokemon_pc_get_box_names(
                    pc_ptr,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        for(size_t box_index = 0; box_index < pc_ptr->capacity; ++box_index)
        {
            char expected_box_name[10] = {0};
            snprintf(expected_box_name, sizeof(expected_box_name), "BOX%zu", (box_index+1));
            TEST_ASSERT_EQUAL_STRING(expected_box_name, box_names.strings[box_index]);
        }

        error = pkmn_string_list_free(&box_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(box_names.strings);
        TEST_ASSERT_EQUAL(0, box_names.length);
    }
}

static void test_setting_pokemon_in_boxes(
    pkmn_pokemon_pc_t* pc_ptr
)
{
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_list_t pokemon_boxes = empty_pokemon_box_list;

    error = pkmn_pokemon_pc_as_list(
                pc_ptr,
                &pokemon_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t box_index = 0; box_index < pokemon_boxes.length; ++box_index)
    {
        test_setting_pokemon_in_box(
            &pokemon_boxes.boxes[box_index]
        );
    }

    for(size_t box_index = 0; box_index < pokemon_boxes.length; ++box_index)
    {
        pkmn_pokemon_t first_pokemon = empty_pokemon;
        pkmn_pokemon_t second_pokemon = empty_pokemon;
        pkmn_pokemon_t third_pokemon = empty_pokemon;

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_boxes.boxes[box_index],
                    0,
                    &first_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(first_pokemon._internal);
        TEST_ASSERT_EQUAL_STRING("Squirtle", first_pokemon.species);

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_boxes.boxes[box_index],
                    1,
                    &second_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(second_pokemon._internal);
        TEST_ASSERT_EQUAL_STRING("Charmander", second_pokemon.species);

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_boxes.boxes[box_index],
                    2,
                    &third_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(third_pokemon._internal);
        TEST_ASSERT_EQUAL_STRING("Charmander", third_pokemon.species);

        error = pkmn_pokemon_free(&third_pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(third_pokemon._internal);

        error = pkmn_pokemon_free(&second_pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(second_pokemon._internal);

        error = pkmn_pokemon_free(&first_pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(first_pokemon._internal);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_boxes.boxes);
    TEST_ASSERT_EQUAL(0, pokemon_boxes.length);
}

static void pokemon_box_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_box_t pokemon_box = empty_pokemon_box;

    error = pkmn_pokemon_box_init(
                game,
                &pokemon_box
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_box._internal);

    test_empty_pokemon_box(&pokemon_box);
    test_box_name(&pokemon_box);
    test_setting_pokemon_in_box(&pokemon_box);

    error = pkmn_pokemon_box_free(&pokemon_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_box._internal);
}

static void pokemon_pc_test(
    const char* game
)
{
    TEST_ASSERT_NOT_NULL(game);

    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_pc_t pokemon_pc =
    {
        .game = NULL,
        ._internal = NULL
    };

    error = pkmn_pokemon_pc_init(
                game,
                &pokemon_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_pc._internal);

    test_empty_pokemon_pc(&pokemon_pc);
    test_pc_box_names(&pokemon_pc);
    test_setting_pokemon_in_boxes(&pokemon_pc);

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_pc._internal);
}

#define BOX_AND_PC_TEST_FCNS(test_game, game_name) \
    static void test_game ## _pokemon_box_test() \
    { \
        pokemon_box_test(game_name); \
    } \
    static void test_game ## _pokemon_pc_test() \
    { \
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
