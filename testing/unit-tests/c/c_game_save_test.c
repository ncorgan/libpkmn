/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/pokemon_comparison.h>
#include <pkmntest-c/util.h>

#include <pkmn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef PKMN_C_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

#ifdef PKMN_C_PLATFORM_WIN32
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

#define TOO_LONG_OT_NAME "LibPKMNLibPKMN"
#define PKMN_DEFAULT_TRAINER_PID 1351
#define PKMN_DEFAULT_TRAINER_SID 32123
#define MONEY_MAX_VALUE 999999

#define STRBUFFER_LEN 1024
static char PKMN_TMP_DIR[STRBUFFER_LEN] = {0};
static char PKSAV_TEST_SAVES[STRBUFFER_LEN] = {0};
static char LIBPKMN_TEST_FILES[STRBUFFER_LEN] = {0};

static const char* RIVAL_NAME_SET_GAMES[] =
{
    "Ruby", "Sapphire", "Emerald",
    "Colosseum", "XD",
    "Black", "White",
    "X", "Y"
};

static const char* MALE_ONLY_GAMES[] =
{
    "Red", "Blue", "Yellow",
    "Gold", "Silver",
    "Colosseum", "XD"
};

static const pkmn_game_save2_t empty_game_save =
{
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon2_t empty_pokemon =
{
    .species = NULL,
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_party2_t empty_pokemon_party =
{
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_party2_t empty_pokemon_box =
{
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_pc2_t empty_pokemon_pc =
{
    .game = NULL,
    ._internal = NULL
};
static const pkmn_pokemon_list2_t empty_pokemon_list =
{
    .pokemon = NULL,
    .length = 0
};
static const pkmn_pokemon_box_list2_t empty_pokemon_box_list =
{
    .boxes = NULL,
    .length = 0
};
static const pkmn_trainer_info2_t empty_trainer_info =
{
    .name = NULL,
    .id = {0},
    .gender = PKMN_GENDER_GENDERLESS
};
static const pkmn_string_list_t empty_string_list =
{
    .strings = NULL,
    .length = 0
};
static const pkmn_item_list_t empty_item_list =
{
    .name = NULL,
    .game = NULL,
    ._internal = NULL
};
static const pkmn_item_bag_t empty_item_bag =
{
    .game = NULL,
    .pocket_names =
    {
        .strings = NULL,
        .length = 0
    },
    ._internal = NULL
};
static const pkmn_item_slots_t empty_item_slots =
{
    .item_slots = NULL,
    .length = 0
};

// Helper functions

static bool is_rival_name_set(
    const char* game
)
{
    size_t length = sizeof(RIVAL_NAME_SET_GAMES)/sizeof(RIVAL_NAME_SET_GAMES[0]);
    for(size_t i = 0; i < length; ++i)
    {
        if(!strcmp(game, RIVAL_NAME_SET_GAMES[i]))
        {
            return true;
        }
    }

    return false;
}

static bool is_male_only(
    const char* game
)
{
    size_t length = sizeof(MALE_ONLY_GAMES)/sizeof(MALE_ONLY_GAMES[0]);
    for(size_t i = 0; i < length; ++i)
    {
        if(!strcmp(game, MALE_ONLY_GAMES[i]))
        {
            return true;
        }
    }

    return false;
}

static void populate_path_vars()
{
    pkmn_get_tmp_dir(
        PKMN_TMP_DIR,
        sizeof(PKMN_TMP_DIR),
        NULL
    );

    char* value = getenv("PKSAV_TEST_SAVES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(PKSAV_TEST_SAVES, sizeof(PKSAV_TEST_SAVES), "%s", value);

    value = getenv("LIBPKMN_TEST_FILES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(LIBPKMN_TEST_FILES, sizeof(LIBPKMN_TEST_FILES), "%s", value);
}

static void compare_string_lists(
    pkmn_string_list_t* string_list1_ptr,
    pkmn_string_list_t* string_list2_ptr
)
{
    TEST_ASSERT_NOT_NULL(string_list1_ptr);
    TEST_ASSERT_NOT_NULL(string_list2_ptr);

    TEST_ASSERT_EQUAL(string_list1_ptr->length, string_list2_ptr->length);

    for(size_t string_index = 0; string_index < string_list1_ptr->length; ++string_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            string_list1_ptr->strings[string_index],
            string_list2_ptr->strings[string_index]
        );
    }
}

/*
 * Actual test functions
 */

static void game_save_test_trainer_info(
    pkmn_game_save2_t* game_save_ptr
)
{
    TEST_ASSERT_NOT_NULL(game_save_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(game_save_ptr->game);

    error = pkmn_game_save2_set_trainer_id(
                game_save_ptr,
                (generation >= 3) ? pkmn_pokemon2_default_trainer_id().id
                                  : pkmn_pokemon2_default_trainer_id().public_id
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_game_save2_set_trainer_name(
                game_save_ptr,
                pkmn_pokemon2_default_trainer_name()
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_game_save2_set_trainer_gender(
                game_save_ptr,
                PKMN_GENDER_FEMALE
            );
    if(is_male_only(game_save_ptr->game))
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    pkmn_trainer_info2_t trainer_info = empty_trainer_info;
    error = pkmn_game_save2_get_trainer_info(
                game_save_ptr,
                &trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL_STRING(
        pkmn_pokemon2_default_trainer_name(),
        trainer_info.name
    );
    TEST_ASSERT_EQUAL(
        (generation >= 3) ? pkmn_pokemon2_default_trainer_id().id
                          : pkmn_pokemon2_default_trainer_id().public_id,
        trainer_info.id.id
    );
    if(!is_male_only(game_save_ptr->game))
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, trainer_info.gender);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, trainer_info.gender);
    }

    error = pkmn_trainer_info_free(&trainer_info);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void game_save_test_rival_name(
    pkmn_game_save2_t* game_save_ptr
)
{
    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    if(is_rival_name_set(game_save_ptr->game))
    {
        error = pkmn_game_save2_set_rival_name(
                    game_save_ptr,
                    pkmn_pokemon2_default_trainer_name()
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    }
    else
    {
        error = pkmn_game_save2_set_rival_name(
                    game_save_ptr,
                    pkmn_pokemon2_default_trainer_name()
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_game_save2_get_rival_name(
                    game_save_ptr,
                    strbuffer,
                    sizeof(strbuffer),
                    NULL
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(
            strbuffer,
            pkmn_pokemon2_default_trainer_name()
        );
    }
}

static void game_save_test_common_fields(
    pkmn_game_save2_t* game_save_ptr
)
{
    TEST_ASSERT_NOT_NULL(game_save_ptr);

    int generation = game_to_generation(game_save_ptr->game);

    pkmn_error_t error = PKMN_ERROR_NONE;

    bool is_game_gamecube = !strcmp(game_save_ptr->game, "Colosseum") ||
                            !strcmp(game_save_ptr->game, "XD");

    pkmn_pokemon_list2_t pokemon_list = empty_pokemon_list;

    game_save_test_trainer_info(game_save_ptr);
    game_save_test_rival_name(game_save_ptr);

    error = pkmn_game_save2_set_money(
                game_save_ptr,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_game_save2_set_money(
                game_save_ptr,
                MONEY_MAX_VALUE+1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    const int money = 123456;
    int money_from_game_save = 0;

    error = pkmn_game_save2_set_money(
                game_save_ptr,
                money
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save2_get_money(
                game_save_ptr,
                &money_from_game_save
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(money, money_from_game_save);

    pkmn_pokedex_t pokedex =
    {
        .game = NULL,
        ._internal = NULL
    };
    bool has_seen = false;
    bool has_caught = false;

    // TODO: check if it's an egg before checking if it's in the Pokédex
    if(!is_game_gamecube)
    {
        error = pkmn_game_save2_get_pokedex(
                    game_save_ptr,
                    &pokedex
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    // Test the party.
    pkmn_pokemon_party2_t pokemon_party = empty_pokemon_party;
    error = pkmn_game_save2_get_pokemon_party(
                game_save_ptr,
                &pokemon_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game_save_ptr->game, pokemon_party.game);
    TEST_ASSERT_EQUAL(6, pokemon_party.capacity);
    TEST_ASSERT_NOT_NULL(pokemon_party._internal);

    error = pkmn_pokemon_party2_as_list(
                &pokemon_party,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    size_t num_pokemon = 0;
    error = pkmn_pokemon_party2_get_num_pokemon(
                &pokemon_party,
                &num_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(num_pokemon > 0);
    TEST_ASSERT_TRUE(num_pokemon <= 6);

    for(size_t pokemon_index = 0; pokemon_index < pokemon_party.capacity; ++pokemon_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            pokemon_party.game,
            pokemon_list.pokemon[pokemon_index].game
        );

        if(pokemon_index < num_pokemon)
        {
            TEST_ASSERT_NOT_EQUAL(
                strcmp(
                    "None",
                    pokemon_list.pokemon[pokemon_index].species
                ), 0
            );

            if(!is_game_gamecube)
            {
                bool is_egg = false;
                bool is_none = strcmp(pokemon_list.pokemon[pokemon_index].species, "None") == 0;
                bool is_invalid = (bool)strstr(pokemon_list.pokemon[pokemon_index].species, "Invalid");

                if(generation >= 2)
                {
                    error = pkmn_pokemon2_is_egg(
                                &pokemon_list.pokemon[pokemon_index],
                                &is_egg
                            );
                }
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                if(!is_egg && !is_none && !is_invalid)
                {
                    error = pkmn_pokedex_has_seen(
                                &pokedex,
                                pokemon_list.pokemon[pokemon_index].species,
                                &has_seen
                            );
                    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                    TEST_ASSERT_TRUE(has_seen);

                    error = pkmn_pokedex_has_caught(
                                &pokedex,
                                pokemon_list.pokemon[pokemon_index].species,
                                &has_caught
                            );
                    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                    TEST_ASSERT_TRUE(has_caught);
                }
            }
        }
        else
        {
            TEST_ASSERT_EQUAL_STRING(
                "None",
                pokemon_list.pokemon[pokemon_index].species
            );
        }
    }

    error = pkmn_pokemon_list2_free(&pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_party2_free(&pokemon_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Test PC.
    pkmn_pokemon_pc2_t pokemon_pc = empty_pokemon_pc;
    error = pkmn_game_save2_get_pokemon_pc(
                game_save_ptr,
                &pokemon_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game_save_ptr->game, pokemon_pc.game);
    TEST_ASSERT_TRUE(pokemon_pc.capacity > 0);
    TEST_ASSERT_NOT_NULL(pokemon_pc._internal);

    pkmn_pokemon_box_list2_t pokemon_box_list = empty_pokemon_box_list;
    error = pkmn_pokemon_pc2_as_list(
                &pokemon_pc,
                &pokemon_box_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_box_list.boxes);
    TEST_ASSERT_TRUE(pokemon_box_list.length > 0);

    for(size_t box_index = 0; box_index < pokemon_box_list.length; ++box_index)
    {
        error = pkmn_pokemon_box2_as_list(
                    &pokemon_box_list.boxes[box_index],
                    &pokemon_list
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(pokemon_list.pokemon);
        TEST_ASSERT_TRUE(pokemon_list.length > 0);

        for(size_t pokemon_index = 0; pokemon_index < pokemon_list.length; ++pokemon_index)
        {
            TEST_ASSERT_EQUAL_STRING(
                game_save_ptr->game,
                pokemon_list.pokemon[pokemon_index].game
            );

            if(!is_game_gamecube)
            {
                bool is_egg = false;
                bool is_none = strcmp(pokemon_list.pokemon[pokemon_index].species, "None") == 0;
                bool is_invalid = (bool)strstr(pokemon_list.pokemon[pokemon_index].species, "Invalid");

                if(generation >= 2)
                {
                    error = pkmn_pokemon2_is_egg(
                                &pokemon_list.pokemon[pokemon_index],
                                &is_egg
                            );
                }
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                if(!is_egg && !is_none && !is_invalid)
                {
                    error = pkmn_pokedex_has_seen(
                                &pokedex,
                                pokemon_list.pokemon[pokemon_index].species,
                                &has_seen
                            );
                    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                    TEST_ASSERT_TRUE(has_seen);

                    error = pkmn_pokedex_has_caught(
                                &pokedex,
                                pokemon_list.pokemon[pokemon_index].species,
                                &has_caught
                            );
                    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
                    TEST_ASSERT_TRUE(has_caught);
                }
            }
        }

        error = pkmn_pokemon_list2_free(&pokemon_list);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    if(!is_game_gamecube)
    {
        error = pkmn_pokedex_free(&pokedex);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_box_list2_free(&pokemon_box_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc2_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void randomize_pokemon(
    pkmn_game_save2_t* game_save_ptr
)
{
    TEST_ASSERT_NOT_NULL(game_save_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_string_list_t item_list = empty_string_list;
    error = pkmn_database_item_list(game_save_ptr->game, &item_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_party2_t pokemon_party = empty_pokemon_party;
    error = pkmn_game_save2_get_pokemon_party(
                game_save_ptr,
                &pokemon_party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t pokemon_index = 0; pokemon_index < pokemon_party.capacity; ++pokemon_index)
    {
        pkmn_pokemon2_t pokemon = empty_pokemon;
        get_random_pokemon(
            &pokemon,
            &item_list,
            NULL, // species
            game_save_ptr->game
        );

        error = pkmn_pokemon_party2_set_pokemon(
                    &pokemon_party,
                    pokemon_index,
                    &pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon2_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_party2_free(&pokemon_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_pc2_t pokemon_pc = empty_pokemon_pc;
    error = pkmn_game_save2_get_pokemon_pc(
                game_save_ptr,
                &pokemon_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_box_list2_t pokemon_box_list = empty_pokemon_box_list;
    error = pkmn_pokemon_pc2_as_list(
                &pokemon_pc,
                &pokemon_box_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    size_t box_capacity = pokemon_box_list.boxes[0].capacity;

    for(size_t box_index = 0; box_index < pokemon_pc.capacity; ++box_index)
    {
        for(size_t pokemon_index = 0; pokemon_index < box_capacity; ++pokemon_index)
        {
            pkmn_pokemon2_t pokemon = empty_pokemon;
            get_random_pokemon(
                &pokemon,
                &item_list,
                NULL, // species
                game_save_ptr->game
            );

            error = pkmn_pokemon_box2_set_pokemon(
                        &pokemon_box_list.boxes[box_index],
                        pokemon_index,
                        &pokemon
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon2_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        }
    }

    error = pkmn_pokemon_box_list2_free(&pokemon_box_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc2_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_string_list_free(&item_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void compare_item_lists(
    pkmn_item_list_t* item_list1_ptr,
    pkmn_item_list_t* item_list2_ptr
)
{
    TEST_ASSERT_NOT_NULL(item_list1_ptr);
    TEST_ASSERT_NOT_NULL(item_list2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    TEST_ASSERT_EQUAL_STRING(item_list1_ptr->name, item_list2_ptr->name);
    TEST_ASSERT_EQUAL_STRING(item_list1_ptr->game, item_list2_ptr->game);
    TEST_ASSERT_EQUAL(item_list1_ptr->capacity, item_list2_ptr->capacity);

    size_t num_items1 = 0;
    size_t num_items2 = 0;

    error = pkmn_item_list_get_num_items(
                item_list1_ptr,
                &num_items1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_get_num_items(
                item_list2_ptr,
                &num_items2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num_items1, num_items2);

    pkmn_item_slots_t item_slots1 = empty_item_slots;
    pkmn_item_slots_t item_slots2 = empty_item_slots;

    error = pkmn_item_list_as_list(
                item_list1_ptr,
                &item_slots1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_as_list(
                item_list2_ptr,
                &item_slots2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    TEST_ASSERT_EQUAL(item_slots1.length, item_slots2.length);
    for(size_t item_index = 0; item_index < item_slots1.length; ++item_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            item_slots1.item_slots[item_index].item,
            item_slots2.item_slots[item_index].item
        );
        TEST_ASSERT_EQUAL(
            item_slots1.item_slots[item_index].amount,
            item_slots2.item_slots[item_index].amount
        );
    }

    error = pkmn_item_slots_free(&item_slots2);
    error = pkmn_item_slots_free(&item_slots1);
}

static void compare_item_bags(
    pkmn_item_bag_t* item_bag1_ptr,
    pkmn_item_bag_t* item_bag2_ptr
)
{
    TEST_ASSERT_NOT_NULL(item_bag1_ptr);
    TEST_ASSERT_NOT_NULL(item_bag2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    compare_string_lists(
        &item_bag1_ptr->pocket_names,
        &item_bag2_ptr->pocket_names
    );

    for(size_t pocket_index = 0; pocket_index < item_bag1_ptr->pocket_names.length; ++pocket_index)
    {
        pkmn_item_list_t pocket1 = empty_item_list;
        pkmn_item_list_t pocket2 = empty_item_list;

        error = pkmn_item_bag_get_pocket(
                    item_bag1_ptr,
                    item_bag1_ptr->pocket_names.strings[pocket_index],
                    &pocket1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_item_bag_get_pocket(
                    item_bag2_ptr,
                    item_bag1_ptr->pocket_names.strings[pocket_index],
                    &pocket2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        compare_item_lists(&pocket1, &pocket2);

        error = pkmn_item_list_free(&pocket2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_item_list_free(&pocket1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
}

static void compare_pokemon_lists(
    pkmn_pokemon_list2_t* pokemon_list1_ptr,
    pkmn_pokemon_list2_t* pokemon_list2_ptr
)
{
    TEST_ASSERT_NOT_NULL(pokemon_list1_ptr);
    TEST_ASSERT_NOT_NULL(pokemon_list2_ptr);

    TEST_ASSERT_EQUAL(pokemon_list1_ptr->length, pokemon_list2_ptr->length);
    for(size_t pokemon_index = 0; pokemon_index < pokemon_list1_ptr->length; ++pokemon_index)
    {
        compare_pokemon(
            &pokemon_list1_ptr->pokemon[pokemon_index],
            &pokemon_list2_ptr->pokemon[pokemon_index]
        );
    }
}

static void compare_game_saves(
    pkmn_game_save2_t* game_save1_ptr,
    pkmn_game_save2_t* game_save2_ptr
)
{
    TEST_ASSERT_NOT_NULL(game_save1_ptr);
    TEST_ASSERT_NOT_NULL(game_save2_ptr);

    pkmn_error_t error = PKMN_ERROR_NONE;

    pkmn_pokemon_list2_t pokemon_list1 = empty_pokemon_list;
    pkmn_pokemon_list2_t pokemon_list2 = empty_pokemon_list;

    TEST_ASSERT_EQUAL_STRING(
        game_save1_ptr->game,
        game_save2_ptr->game
    );

    int generation = game_to_generation(game_save1_ptr->game);
    bool is_game_gamecube = !strcmp(game_save1_ptr->game, "Colosseum") ||
                            !strcmp(game_save1_ptr->game, "XD");

    // Compare bags.

    pkmn_item_bag_t item_bag1 = empty_item_bag;
    pkmn_item_bag_t item_bag2 = empty_item_bag;

    error = pkmn_game_save2_get_item_bag(
                game_save1_ptr,
                &item_bag1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save2_get_item_bag(
                game_save2_ptr,
                &item_bag2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    compare_item_bags(&item_bag1, &item_bag2);

    // Compare item PCs (if applicable).
    if((generation <= 3) || !is_game_gamecube)
    {
        pkmn_item_list_t item_pc1 = empty_item_list;
        pkmn_item_list_t item_pc2 = empty_item_list;

        error = pkmn_game_save2_get_item_pc(
                    game_save1_ptr,
                    &item_pc1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_game_save2_get_item_pc(
                    game_save2_ptr,
                    &item_pc2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        compare_item_lists(&item_pc1, &item_pc2);

        error = pkmn_item_list_free(&item_pc2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_item_list_free(&item_pc1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    // Compare parties.

    pkmn_pokemon_party2_t pokemon_party1 = empty_pokemon_party;
    pkmn_pokemon_party2_t pokemon_party2 = empty_pokemon_party;

    error = pkmn_game_save2_get_pokemon_party(
                game_save1_ptr,
                &pokemon_party1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save2_get_pokemon_party(
                game_save2_ptr,
                &pokemon_party2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_party2_as_list(
                &pokemon_party1,
                &pokemon_list1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party2_as_list(
                &pokemon_party2,
                &pokemon_list2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    compare_pokemon_lists(&pokemon_list1, &pokemon_list2);

    error = pkmn_pokemon_list2_free(&pokemon_list2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_list2_free(&pokemon_list1);

    error = pkmn_pokemon_party2_free(&pokemon_party2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_party2_free(&pokemon_party1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    // Compare Pokémon PCs.

    pkmn_pokemon_pc2_t pokemon_pc1 = empty_pokemon_pc;
    pkmn_pokemon_pc2_t pokemon_pc2 = empty_pokemon_pc;

    error = pkmn_game_save2_get_pokemon_pc(
                game_save1_ptr,
                &pokemon_pc1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save2_get_pokemon_pc(
                game_save1_ptr,
                &pokemon_pc2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_box_list2_t pokemon_box_list1 = empty_pokemon_box_list;
    pkmn_pokemon_box_list2_t pokemon_box_list2 = empty_pokemon_box_list;

    error = pkmn_pokemon_pc2_as_list(
                &pokemon_pc1,
                &pokemon_box_list1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_pc2_as_list(
                &pokemon_pc2,
                &pokemon_box_list2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(size_t box_index = 0; box_index < pokemon_box_list1.length; ++box_index)
    {
        error = pkmn_pokemon_box2_as_list(
                    &pokemon_box_list1.boxes[box_index],
                    &pokemon_list1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_box2_as_list(
                    &pokemon_box_list2.boxes[box_index],
                    &pokemon_list2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        compare_pokemon_lists(&pokemon_list1, &pokemon_list2);

        error = pkmn_pokemon_list2_free(&pokemon_list2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_list2_free(&pokemon_list1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_box_list2_free(&pokemon_box_list2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_list2_free(&pokemon_box_list1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc2_free(&pokemon_pc2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_pc2_free(&pokemon_pc1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void test_game_save(
    pkmn_game_save_type2_t save_type,
    const char* game,
    const char* subdir,
    const char* filename
)
{
    TEST_ASSERT_NOT_NULL(game);
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(filename);

    pkmn_error_t error = PKMN_ERROR_NONE;
    char strbuffer[STRBUFFER_LEN] = {0};

    pkmn_game_save_type2_t save_type_from_file = PKMN_GAME_SAVE_TYPE2_NONE;

    pkmn_game_save2_t game_save = empty_game_save;
    bool is_game_gamecube = !strcmp(game, "Colosseum") || !strcmp(game, "XD");

    char save_filepath[STRBUFFER_LEN] = {0};
    snprintf(
        save_filepath,
        sizeof(save_filepath),
        "%s%s%s%s%s",
        is_game_gamecube ? LIBPKMN_TEST_FILES : PKSAV_TEST_SAVES,
        FS_SEPARATOR, subdir, FS_SEPARATOR, filename
    );

    error = pkmn_game_save2_detect_type(
                save_filepath,
                &save_type_from_file
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(save_type, save_type_from_file);

    error = pkmn_game_save2_init_from_file(
                save_filepath,
                &game_save
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(game_save._internal);
    TEST_ASSERT_EQUAL_STRING(game, game_save.game);

    error = pkmn_game_save2_get_filepath(
                &game_save,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(save_filepath, strbuffer);

    game_save_test_common_fields(&game_save);
    randomize_pokemon(&game_save);

    char tmp_save_filepath[STRBUFFER_LEN] = {0};
    snprintf(
        tmp_save_filepath,
        sizeof(tmp_save_filepath),
        "%s%s%s_%d.sav",
        PKMN_TMP_DIR,
        FS_SEPARATOR,
        game,
        rand()
    );

    error = pkmn_game_save2_save_as(
                &game_save,
                tmp_save_filepath
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_game_save2_t game_save2 = empty_game_save;
    error = pkmn_game_save2_init_from_file(
                tmp_save_filepath,
                &game_save2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(game_save2._internal);

    error = pkmn_game_save2_get_filepath(
                &game_save2,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(tmp_save_filepath, strbuffer);
    compare_game_saves(
        &game_save,
        &game_save2
    );

#ifdef PKMN_C_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(DeleteFile(tmp_save_filepath), 0);
#else
    TEST_ASSERT_EQUAL(remove(tmp_save_filepath), 0);
#endif

    error = pkmn_game_save2_free(&game_save2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(game_save2._internal);

    error = pkmn_game_save2_free(&game_save);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(game_save._internal);
}

#define PKMN_C_GAME_SAVE_TEST(save_type, game, subdir, filename) \
{ \
    Unity.CurrentTestName = "c_game_save_test_" game; \
    ++Unity.NumberOfTests; \
    if(TEST_PROTECT()) { \
        test_game_save(save_type, game, subdir, filename); \
    } \
    UnityConcludeTest(); \
}

PKMN_C_TEST_MAIN(
    srand((unsigned int)time(NULL));
    populate_path_vars();
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW, "Red", "red_blue", "pokemon_red.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW, "Yellow", "yellow", "pokemon_yellow.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_GOLD_SILVER, "Gold", "gold_silver", "pokemon_gold.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_CRYSTAL, "Crystal", "crystal", "pokemon_crystal.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE, "Ruby", "ruby_sapphire", "pokemon_ruby.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_EMERALD, "Emerald", "emerald", "pokemon_emerald.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN, "FireRed", "firered_leafgreen", "pokemon_firered.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD, "Colosseum", "gamecube_saves", "pokemon_colosseum.gci");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_COLOSSEUM_XD, "XD", "gamecube_saves", "pokemon_xd.gci");
)
