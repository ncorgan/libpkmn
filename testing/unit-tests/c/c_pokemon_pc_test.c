/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static const struct pkmn_pokemon empty_pokemon =
{
    .species = PKMN_SPECIES_NONE,
    .game = PKMN_GAME_NONE,
    .p_internal = NULL
};
static const struct pkmn_pokemon_box empty_pokemon_box =
{
    .game = PKMN_GAME_NONE,
    .capacity = 0,
    .p_internal = NULL
};
static const struct pkmn_pokemon_list empty_pokemon_list =
{
    .p_pokemon = NULL,
    .length = 0
};
static const struct pkmn_pokemon_box_list empty_pokemon_box_list =
{
    .p_boxes = NULL,
    .length = 0
};
static const struct pkmn_string_list empty_string_list =
{
    .pp_strings = NULL,
    .length = 0
};

static void test_empty_pokemon_box(
    struct pkmn_pokemon_box* p_box
)
{
    TEST_ASSERT_NOT_NULL(p_box);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_box->game);

    error = pkmn_pokemon_box_get_name(
                p_box,
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
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    struct pkmn_pokemon_list pokemon_list = empty_pokemon_list;

    size_t num_pokemon = 0;
    error = pkmn_pokemon_box_get_num_pokemon(
                p_box,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_pokemon);

    error = pkmn_pokemon_box_as_list(
                p_box,
                &pokemon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_TRUE(pokemon_list.length > 0);

    for(size_t pokemon_index = 0; pokemon_index < pokemon_list.length; ++pokemon_index)
    {
        TEST_ASSERT_EQUAL(
            PKMN_SPECIES_NONE,
            pokemon_list.p_pokemon[pokemon_index].species
        );
        TEST_ASSERT_EQUAL(
            p_box->game,
            pokemon_list.p_pokemon[pokemon_index].game
        );
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);
}

static void test_box_name(
    struct pkmn_pokemon_box* p_box
)
{
    TEST_ASSERT_NOT_NULL(p_box);

    enum pkmn_error error = PKMN_ERROR_NONE;

    int generation = game_to_generation(p_box->game);

    if(generation == 1)
    {
        error = pkmn_pokemon_box_set_name(
                    p_box,
                    "ABCDEFGH"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);

        error = pkmn_pokemon_box_get_name(
                    p_box,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        error = pkmn_pokemon_box_set_name(
                    p_box,
                    "ABCDEFGHI"
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_box_set_name(
                    p_box,
                    "ABCDEFGH"
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_box_get_name(
                    p_box,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL_STRING("ABCDEFGH", strbuffer);
    }
}

static void test_setting_pokemon_in_box(
    struct pkmn_pokemon_box* p_box
)
{
    TEST_ASSERT_NOT_NULL(p_box);

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t num_pokemon = 0;

    struct pkmn_pokemon original_first = empty_pokemon;
    struct pkmn_pokemon original_second = empty_pokemon;

    int generation = game_to_generation(p_box->game);

    error = pkmn_pokemon_box_get_pokemon(
                p_box,
                0,
                &original_first
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(original_first.p_internal);

    error = pkmn_pokemon_box_get_pokemon(
                p_box,
                1,
                &original_second
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(original_second.p_internal);

    // Make sure we can't get Pokémon at invalid indices.
    error = pkmn_pokemon_box_get_pokemon(
                p_box,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_box_get_pokemon(
                p_box,
                p_box->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // Make sure we can't set Pokémon at invalid indices.
    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                -1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                p_box->capacity + 1,
                &original_first
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    /*
     * Create new Pokémon and place in box. The original variables should
     * have the same underlying Pokémon.
     */

    struct pkmn_pokemon bulbasaur = empty_pokemon;
    struct pkmn_pokemon charmander = empty_pokemon;
    struct pkmn_pokemon squirtle = empty_pokemon;

    error = pkmn_pokemon_init(
                PKMN_SPECIES_BULBASAUR,
                p_box->game,
                "",
                5,
                &bulbasaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(bulbasaur.p_internal);

    error = pkmn_pokemon_init(
                PKMN_SPECIES_CHARMANDER,
                p_box->game,
                "",
                5,
                &charmander
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(charmander.p_internal);

    error = pkmn_pokemon_init(
                PKMN_SPECIES_SQUIRTLE,
                p_box->game,
                "",
                5,
                &squirtle
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(squirtle.p_internal);

    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                0,
                &bulbasaur
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
                p_box,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(1, num_pokemon);

    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                1,
                &charmander
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
                p_box,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Replace one of the new ones.
    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                0,
                &squirtle
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
                p_box,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    struct pkmn_pokemon second_in_box = empty_pokemon;
    error = pkmn_pokemon_box_get_pokemon(
                p_box,
                1,
                &second_in_box
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(second_in_box.p_internal);

    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                2,
                &second_in_box
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
                p_box,
                &num_pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    // We should be able to clear the last contiguous Pokémon.
    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                2,
                &original_first
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
        p_box,
        &num_pokemon
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(2, num_pokemon);

    // Put it back.
    error = pkmn_pokemon_box_set_pokemon(
                p_box,
                2,
                &second_in_box
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_box_get_num_pokemon(
        p_box,
        &num_pokemon
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(3, num_pokemon);

    error = pkmn_pokemon_free(&second_in_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(second_in_box.p_internal);

    // Check that Pokémon can be placed non-contiguously in the correct games.
    struct pkmn_pokemon fifth_in_box = empty_pokemon;

    if(generation <= 2)
    {
        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    1,
                    &original_first
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    p_box,
                    &num_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    p_box,
                    1,
                    &second_in_box
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_CHARMANDER, second_in_box.species);

        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    4,
                    &bulbasaur
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

        error = pkmn_pokemon_box_get_pokemon(
                    p_box,
                    4,
                    &fifth_in_box
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, fifth_in_box.species);

        error = pkmn_pokemon_free(&fifth_in_box);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(fifth_in_box.p_internal);
    }
    else
    {
        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    1,
                    &original_first
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    p_box,
                    &num_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(2, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    p_box,
                    1,
                    &second_in_box
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, second_in_box.species);

        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    4,
                    &bulbasaur
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    p_box,
                    &num_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(3, num_pokemon);

        error = pkmn_pokemon_box_get_pokemon(
                    p_box,
                    4,
                    &fifth_in_box
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_BULBASAUR, fifth_in_box.species);

        // Restore it to how it was.

        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    1,
                    &charmander
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_box_set_pokemon(
                    p_box,
                    4,
                    &original_first
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_box_get_num_pokemon(
                    p_box,
                    &num_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(3, num_pokemon);
    }

    error = pkmn_pokemon_free(&second_in_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(second_in_box.p_internal);

    error = pkmn_pokemon_free(&fifth_in_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(fifth_in_box.p_internal);

    /*
     * Now check everything we've created. Each variable should have the
     * same Pokémon underneath, even if the pointer has changed.
     */
    struct pkmn_pokemon_list pokemon_list = empty_pokemon_list;
    error = pkmn_pokemon_box_as_list(
                p_box,
                &pokemon_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_TRUE(pokemon_list.length > 0);

    TEST_ASSERT_EQUAL(
        PKMN_SPECIES_SQUIRTLE,
        pokemon_list.p_pokemon[0].species
    );
    TEST_ASSERT_EQUAL(
        PKMN_SPECIES_CHARMANDER,
        pokemon_list.p_pokemon[1].species
    );
    TEST_ASSERT_EQUAL(
        PKMN_SPECIES_CHARMANDER,
        pokemon_list.p_pokemon[2].species
    );
    TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, original_first.species);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_NONE, original_second.species);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_BULBASAUR, bulbasaur.species);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_CHARMANDER, charmander.species);
    TEST_ASSERT_EQUAL(PKMN_SPECIES_SQUIRTLE, squirtle.species);

    // Cleanup

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_NULL(pokemon_list.p_pokemon);
    TEST_ASSERT_EQUAL(0, pokemon_list.length);

    error = pkmn_pokemon_free(&original_first);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(original_first.p_internal);

    error = pkmn_pokemon_free(&original_second);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(original_second.p_internal);

    error = pkmn_pokemon_free(&bulbasaur);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(bulbasaur.p_internal);

    error = pkmn_pokemon_free(&charmander);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(charmander.p_internal);

    error = pkmn_pokemon_free(&squirtle);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(squirtle.p_internal);
}

static void test_empty_pokemon_pc(
    struct pkmn_pokemon_pc* p_pc
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;

    struct pkmn_pokemon_box_list pokemon_box_list = empty_pokemon_box_list;
    error = pkmn_pokemon_pc_as_list(
                p_pc,
                &pokemon_box_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(pokemon_box_list.p_boxes);
    TEST_ASSERT_TRUE(pokemon_box_list.length > 0);

    for(size_t box_index = 0; box_index < pokemon_box_list.length; ++box_index)
    {
        TEST_ASSERT_EQUAL(
            p_pc->game,
            pokemon_box_list.p_boxes[box_index].game
        );
        test_empty_pokemon_box(
            &pokemon_box_list.p_boxes[box_index]
        );
    }

    error = pkmn_pokemon_box_list_free(&pokemon_box_list);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_box_list.p_boxes);
    TEST_ASSERT_EQUAL(0, pokemon_box_list.length);
}

static void test_pc_box_names(
    struct pkmn_pokemon_pc* p_pc
)
{
    TEST_ASSERT_NOT_NULL(p_pc);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_string_list box_names = empty_string_list;

    int generation = game_to_generation(p_pc->game);

    if(generation == 1)
    {
        error = pkmn_pokemon_pc_get_box_names(
                    p_pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        for(size_t box_index = 0; box_index < p_pc->capacity; ++box_index)
        {
            struct pkmn_pokemon_box pokemon_box = empty_pokemon_box;

            error = pkmn_pokemon_pc_get_box(
                        p_pc,
                        box_index,
                        &pokemon_box
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            char box_name[10] = {0};
            snprintf(box_name, sizeof(box_name), "BOX%zu", (box_index+1));

            error = pkmn_pokemon_box_set_name(
                        &pokemon_box,
                        box_name
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);

            error = pkmn_pokemon_box_free(&pokemon_box);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon_box.p_internal);
        }

        error = pkmn_pokemon_pc_get_box_names(
                    p_pc,
                    &box_names
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        for(size_t box_index = 0; box_index < p_pc->capacity; ++box_index)
        {
            char expected_box_name[10] = {0};
            snprintf(expected_box_name, sizeof(expected_box_name), "BOX%zu", (box_index+1));
            TEST_ASSERT_EQUAL_STRING(expected_box_name, box_names.pp_strings[box_index]);
        }

        error = pkmn_string_list_free(&box_names);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(box_names.pp_strings);
        TEST_ASSERT_EQUAL(0, box_names.length);
    }
}

static void test_setting_pokemon_in_boxes(
    struct pkmn_pokemon_pc* p_pc
)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon_box_list pokemon_p_boxes = empty_pokemon_box_list;

    error = pkmn_pokemon_pc_as_list(
                p_pc,
                &pokemon_p_boxes
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    for(size_t box_index = 0; box_index < pokemon_p_boxes.length; ++box_index)
    {
        test_setting_pokemon_in_box(
            &pokemon_p_boxes.p_boxes[box_index]
        );
    }

    for(size_t box_index = 0; box_index < pokemon_p_boxes.length; ++box_index)
    {
        struct pkmn_pokemon first_pokemon = empty_pokemon;
        struct pkmn_pokemon second_pokemon = empty_pokemon;
        struct pkmn_pokemon third_pokemon = empty_pokemon;

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_p_boxes.p_boxes[box_index],
                    0,
                    &first_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(first_pokemon.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_SQUIRTLE, first_pokemon.species);

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_p_boxes.p_boxes[box_index],
                    1,
                    &second_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(second_pokemon.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_CHARMANDER, second_pokemon.species);

        error = pkmn_pokemon_box_get_pokemon(
                    &pokemon_p_boxes.p_boxes[box_index],
                    2,
                    &third_pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(third_pokemon.p_internal);
        TEST_ASSERT_EQUAL(PKMN_SPECIES_CHARMANDER, third_pokemon.species);

        error = pkmn_pokemon_free(&third_pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(third_pokemon.p_internal);

        error = pkmn_pokemon_free(&second_pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(second_pokemon.p_internal);

        error = pkmn_pokemon_free(&first_pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(first_pokemon.p_internal);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_p_boxes);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_p_boxes.p_boxes);
    TEST_ASSERT_EQUAL(0, pokemon_p_boxes.length);
}

static void pokemon_box_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon_box pokemon_box = empty_pokemon_box;

    error = pkmn_pokemon_box_init(
                game,
                &pokemon_box
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(game, pokemon_box.game);
    TEST_ASSERT_NOT_NULL(pokemon_box.p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_box_strerror(&pokemon_box)
    );

    test_empty_pokemon_box(&pokemon_box);
    test_box_name(&pokemon_box);
    test_setting_pokemon_in_box(&pokemon_box);

    error = pkmn_pokemon_box_free(&pokemon_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_box.p_internal);
}

static void pokemon_pc_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon_pc pokemon_pc =
    {
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    error = pkmn_pokemon_pc_init(
                game,
                &pokemon_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(game, pokemon_pc.game);
    TEST_ASSERT_NOT_NULL(pokemon_pc.p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokemon_pc_strerror(&pokemon_pc)
    );

    test_empty_pokemon_pc(&pokemon_pc);
    test_pc_box_names(&pokemon_pc);
    test_setting_pokemon_in_boxes(&pokemon_pc);

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokemon_pc.p_internal);
}

#define BOX_AND_PC_TEST_FCNS(test_game_enum, test_game, game_name) \
    static void test_game ## _pokemon_box_test() \
    { \
        pokemon_box_test(test_game_enum); \
    } \
    static void test_game ## _pokemon_pc_test() \
    { \
        pokemon_pc_test(test_game_enum); \
    }

BOX_AND_PC_TEST_FCNS(PKMN_GAME_RED, red, "Red")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_BLUE, blue, "Blue")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_YELLOW, yellow, "Yellow")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_GOLD, gold, "Gold")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_SILVER, silver, "Silver")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_CRYSTAL, crystal, "Crystal")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_RUBY, ruby, "Ruby")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_SAPPHIRE, sapphire, "Sapphire")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_EMERALD, emerald, "Emerald")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_FIRERED, firered, "FireRed")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_LEAFGREEN, leafgreen, "LeafGreen")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_COLOSSEUM, colosseum, "Colosseum")
BOX_AND_PC_TEST_FCNS(PKMN_GAME_XD, xd, "XD")

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
