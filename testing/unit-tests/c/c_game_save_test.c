/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/util.h>

#include <pkmn.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef PKMN_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <unistd.h>
#endif

#ifdef PKMN_PLATFORM_WIN32
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

#define TOO_LONG_OT_NAME "LibPKMNLibPKMN"
#define PKMN_DEFAULT_TRAINER_PID 1351
#define PKMN_DEFAULT_TRAINER_SID 32123
#define MONEY_MAX_VALUE 999999

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static pkmn_error_t error = PKMN_ERROR_NONE;
static char PKMN_TMP_DIR[STRBUFFER_LEN] = {0};
static char PKSAV_TEST_SAVES[STRBUFFER_LEN] = {0};
static char LIBPKMN_TEST_FILES[STRBUFFER_LEN] = {0};

static const char* RIVAL_NAME_SET_GAMES[] = {
    "Ruby", "Sapphire", "Emerald",
    "Colosseum", "XD",
    "Black", "White",
    "X", "Y"
};

static const char* MALE_ONLY_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver",
    "Colosseum", "XD"
};

static bool is_rival_name_set(
    const char* game
) {
    size_t length = sizeof(RIVAL_NAME_SET_GAMES)/sizeof(RIVAL_NAME_SET_GAMES[0]);
    for(size_t i = 0; i < length; ++i) {
        if(!strcmp(game, RIVAL_NAME_SET_GAMES[i])) {
            return true;
        }
    }

    return false;
}

static bool is_male_only(
    const char* game
) {
    size_t length = sizeof(MALE_ONLY_GAMES)/sizeof(MALE_ONLY_GAMES[0]);
    for(size_t i = 0; i < length; ++i) {
        if(!strcmp(game, MALE_ONLY_GAMES[i])) {
            return true;
        }
    }

    return false;
}

static void populate_path_vars() {
    pkmn_get_tmp_dir(
        PKMN_TMP_DIR,
        sizeof(PKMN_TMP_DIR)
    );

    char* value = getenv("PKSAV_TEST_SAVES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(PKSAV_TEST_SAVES, sizeof(PKSAV_TEST_SAVES), "%s", value);

    value = getenv("LIBPKMN_TEST_FILES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(LIBPKMN_TEST_FILES, sizeof(LIBPKMN_TEST_FILES), "%s", value);
}

/*
 * Actual test functions
 */

static void game_save_test_trainer_name(
    pkmn_game_save_handle_t game_save
) {
    TEST_ASSERT_NOT_NULL(game_save);

    error = pkmn_game_save_set_trainer_name(
                game_save,
                ""
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_game_save_set_trainer_name(
                game_save,
                TOO_LONG_OT_NAME
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_game_save_set_trainer_name(
                game_save,
                PKMN_DEFAULT_TRAINER_NAME
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_game_save_get_trainer_name(
                game_save,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(PKMN_DEFAULT_TRAINER_NAME, strbuffer);
}

static void game_save_test_trainer_id(
    pkmn_game_save_handle_t game_save,
    bool is_gb_game
) {
    TEST_ASSERT_NOT_NULL(game_save);

    uint32_t trainer_id = 0;
    uint16_t trainer_id_part = 0;

    error = pkmn_game_save_get_trainer_id(
                game_save,
                &trainer_id
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL((is_gb_game ? PKMN_DEFAULT_TRAINER_PID : PKMN_DEFAULT_TRAINER_ID), trainer_id);

    error = pkmn_game_save_get_trainer_public_id(
                game_save,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(PKMN_DEFAULT_TRAINER_PID, trainer_id_part);

    error = pkmn_game_save_get_trainer_secret_id(
                game_save,
                &trainer_id_part
            );
    if(is_gb_game) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_DEFAULT_TRAINER_SID, trainer_id_part);
    }
}

static void game_save_test_rival_name(
    pkmn_game_save_handle_t game_save,
    bool is_rival_name_set
) {
    TEST_ASSERT_NOT_NULL(game_save);

    if(is_rival_name_set) {
        error = pkmn_game_save_set_rival_name(
                    game_save,
                    PKMN_DEFAULT_TRAINER_NAME
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        error = pkmn_game_save_set_rival_name(
                    game_save,
                    ""
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        error = pkmn_game_save_set_rival_name(
                    game_save,
                    TOO_LONG_OT_NAME
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_game_save_set_rival_name(
                    game_save,
                    PKMN_DEFAULT_TRAINER_NAME
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_game_save_get_rival_name(
                    game_save,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(PKMN_DEFAULT_TRAINER_NAME, strbuffer);
    }
}

static void game_save_test_common_fields(
    pkmn_game_save_handle_t game_save,
    const char* game
) {
    TEST_ASSERT_NOT_NULL(game_save);
    TEST_ASSERT_NOT_NULL(game);

    int generation = game_to_generation(game);

    game_save_test_trainer_name(game_save);

    bool is_gb_game = (generation <= 2);

    error = pkmn_game_save_set_trainer_id(
                game_save,
                is_gb_game ? PKMN_DEFAULT_TRAINER_PID : PKMN_DEFAULT_TRAINER_ID
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    game_save_test_trainer_id(
        game_save,
        is_gb_game
    );

    error = pkmn_game_save_set_trainer_public_id(
                game_save,
                PKMN_DEFAULT_TRAINER_PID
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    game_save_test_trainer_id(
        game_save,
        is_gb_game
    );

    error = pkmn_game_save_set_trainer_secret_id(
                game_save,
                PKMN_DEFAULT_TRAINER_SID
            );
    if(is_gb_game) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        game_save_test_trainer_id(
            game_save,
            is_gb_game
        );
    }

    game_save_test_rival_name(
        game_save,
        is_rival_name_set(game)
    );

    pkmn_gender_t trainer_gender = PKMN_GENDERLESS;
    if(is_male_only(game)) {
        error = pkmn_game_save_get_trainer_gender(
                    game_save,
                    &trainer_gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_MALE, trainer_gender);

        error = pkmn_game_save_set_trainer_gender(
                    game_save,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        error = pkmn_game_save_set_trainer_gender(
                    game_save,
                    PKMN_MALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_game_save_get_trainer_gender(
                    game_save,
                    &trainer_gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_MALE, trainer_gender);

        error = pkmn_game_save_set_trainer_gender(
                    game_save,
                    PKMN_FEMALE
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_game_save_get_trainer_gender(
                    game_save,
                    &trainer_gender
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(PKMN_FEMALE, trainer_gender);

        error = pkmn_game_save_set_trainer_gender(
                    game_save,
                    PKMN_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }

    error = pkmn_game_save_set_money(
                game_save,
                -1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_game_save_set_money(
                game_save,
                MONEY_MAX_VALUE+1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    int money = 0;
    error = pkmn_game_save_set_money(
                game_save,
                123456
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_game_save_get_money(
                game_save,
                &money
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(123456, money);

    // Test the party
    pkmn_pokemon_party_handle_t party = NULL;
    error = pkmn_game_save_get_pokemon_party(
                game_save,
                &party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(party);

    int num_party_pokemon = 0;
    error = pkmn_pokemon_party_get_num_pokemon(
                party,
                &num_party_pokemon
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(num_party_pokemon > 0);
    TEST_ASSERT_TRUE(num_party_pokemon <= 6);

    pkmn_pokemon_list_t pokemon_list;
    error = pkmn_pokemon_party_as_array(
                party,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(6, pokemon_list.length);
    TEST_ASSERT_NOT_NULL(pokemon_list.pokemon_list);

    for(int i = 0; i < 6; ++i) {
        TEST_ASSERT_NOT_NULL(pokemon_list.pokemon_list[i]);
        error = pkmn_pokemon_get_species(
                    pokemon_list.pokemon_list[i],
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        if(i < num_party_pokemon) {
            TEST_ASSERT_NOT_EQUAL(0, strcmp("None", strbuffer));
        } else {
            TEST_ASSERT_EQUAL_STRING("None", strbuffer);
        }
    }

    error = pkmn_pokemon_list_free(&pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_party_free(&party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(party);

    // Test the PC
    pkmn_pokemon_pc_handle_t pokemon_pc = NULL;
    error = pkmn_game_save_get_pokemon_pc(
                game_save,
                &pokemon_pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pokemon_pc);

    int num_boxes = 0;
    error = pkmn_pokemon_pc_get_num_boxes(
                pokemon_pc,
                &num_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_box_list_t pokemon_boxes;
    error = pkmn_pokemon_pc_as_array(
                pokemon_pc,
                &pokemon_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num_boxes, (int)pokemon_boxes.length);

    if(generation >= 2) {
        pkmn_string_list_t box_names;
        error = pkmn_pokemon_pc_get_box_names(
                    pokemon_pc,
                    &box_names
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL(pokemon_boxes.length, box_names.length);

        for(size_t i = 0; i < box_names.length; ++i) {
            TEST_ASSERT_NOT_NULL(box_names.strings[i]);
        }

        error = pkmn_string_list_free(&box_names);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        TEST_ASSERT_NOT_NULL(pokemon_boxes.pokemon_boxes[i]);

        int box_capacity = 0;
        int num_box_pokemon = 0;

        error = pkmn_pokemon_box_get_capacity(
                    pokemon_boxes.pokemon_boxes[i],
                    &box_capacity
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(box_capacity > 0);

        error = pkmn_pokemon_box_get_num_pokemon(
                    pokemon_boxes.pokemon_boxes[i],
                    &num_box_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(num_box_pokemon <= box_capacity);

        error = pkmn_pokemon_box_as_array(
                    pokemon_boxes.pokemon_boxes[i],
                    &pokemon_list
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(box_capacity, (int)pokemon_list.length);

        for(size_t j = 0; j < pokemon_list.length; ++j) {
            TEST_ASSERT_NOT_NULL(pokemon_list.pokemon_list[i]);

            // Boxes are only contiguous in Game Boy games.
            if(is_gb_game) {
                error = pkmn_pokemon_get_species(
                            pokemon_list.pokemon_list[j],
                            strbuffer,
                            sizeof(strbuffer)
                        );
                TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

                if((int)j < num_box_pokemon) {
                    TEST_ASSERT_NOT_EQUAL(0, strcmp("None", strbuffer));
                } else {
                    TEST_ASSERT_EQUAL_STRING("None", strbuffer);
                }
            }
        }

        error = pkmn_pokemon_list_free(&pokemon_list);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_pc);
}

static void get_random_pokemon(
    const char* game,
    const pkmn_string_list_t* pokemon_list,
    const pkmn_string_list_t* move_list,
    const pkmn_string_list_t* item_list,
    pkmn_pokemon_handle_t* pokemon_out
) {
    TEST_ASSERT_NOT_NULL(game);
    TEST_ASSERT_NOT_NULL(pokemon_list);
    TEST_ASSERT_NOT_NULL(move_list);
    TEST_ASSERT_NOT_NULL(item_list);
    TEST_ASSERT_NOT_NULL(pokemon_out);

    pkmn_error_t error = PKMN_ERROR_NONE;

    int generation = game_to_generation(game);
    const char* species = NULL;
    if(generation == 3) {
        do {
            // Don't deal with Deoxys issues here.
            species = pokemon_list->strings[rand() % pokemon_list->length];
        } while(!strcmp(species, "Deoxys"));
    } else {
        species = pokemon_list->strings[rand() % pokemon_list->length];
    }
    TEST_ASSERT_NOT_NULL(species);

    error = pkmn_pokemon_make(
                pokemon_out,
                species,
                game,
                "",
                ((rand() % 99) + 2)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(*pokemon_out);

    for(int i = 0; i < 4; ++i) {
        const char* move = NULL;
        size_t index = 0;
        do
        {
            index = rand() % move_list->length;
            move = move_list->strings[index];
        } while(strstr(move_list->strings[index], "Shadow"));
        error = pkmn_pokemon_set_move(
                    *pokemon_out,
                    move,
                    i
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    if(generation >= 2) {
        // Keep going until one is holdable
        do {
            error = pkmn_pokemon_set_held_item(
                        *pokemon_out,
                        item_list->strings[rand() % item_list->length]
                    );
        } while(error == PKMN_ERROR_INVALID_ARGUMENT);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }
}

static void randomize_pokemon(
    pkmn_game_save_handle_t game_save,
    const char* game,
    const pkmn_string_list_t* item_list
) {
    TEST_ASSERT_NOT_NULL(game_save);
    TEST_ASSERT_NOT_NULL(game);
    TEST_ASSERT_NOT_NULL(item_list);

    int generation = game_to_generation(game);
    TEST_ASSERT_TRUE(generation > 0);

    pkmn_string_list_t pokemon_list;
    pkmn_string_list_t move_list;

    error = pkmn_database_pokemon_list(
                generation,
                true,
                &pokemon_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_database_move_list(
                game,
                &move_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_party_handle_t party = NULL;
    error = pkmn_game_save_get_pokemon_party(
                game_save,
                &party
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(party);

    for(int i = 0; i < 6; ++i) {
        pkmn_pokemon_handle_t pokemon = NULL;
        get_random_pokemon(
            game,
            &pokemon_list,
            &move_list,
            item_list,
            &pokemon
        );
        TEST_ASSERT_NOT_NULL(pokemon);

        error = pkmn_pokemon_party_set_pokemon(
                    party,
                    i,
                    pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_pokemon_free(&pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(pokemon);
    }

    error = pkmn_pokemon_party_free(&party);
    TEST_ASSERT_NULL(party);

    pkmn_pokemon_pc_handle_t pc = NULL;
    error = pkmn_game_save_get_pokemon_pc(
                game_save,
                &pc
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pc);

    pkmn_pokemon_box_list_t pokemon_boxes;
    error = pkmn_pokemon_pc_as_array(
                pc,
                &pokemon_boxes
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(pokemon_boxes.length > 0);

    for(size_t i = 0; i < pokemon_boxes.length; ++i) {
        TEST_ASSERT_NOT_NULL(pokemon_boxes.pokemon_boxes[i]);

        pkmn_pokemon_list_t box_pokemon;
        error = pkmn_pokemon_box_as_array(
                    pokemon_boxes.pokemon_boxes[i],
                    &box_pokemon
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(box_pokemon.length > 0);

        for(size_t j = 0; j < box_pokemon.length; ++j) {
            pkmn_pokemon_handle_t pokemon = NULL;
            get_random_pokemon(
                game,
                &pokemon_list,
                &move_list,
                item_list,
                &pokemon
            );
            TEST_ASSERT_NOT_NULL(pokemon);

            error = pkmn_pokemon_box_set_pokemon(
                        pokemon_boxes.pokemon_boxes[i],
                        (int)j,
                        pokemon
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            error = pkmn_pokemon_free(&pokemon);
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_NULL(pokemon);
        }

        error = pkmn_pokemon_list_free(&box_pokemon);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc_free(&pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pc);

    error = pkmn_string_list_free(&move_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_string_list_free(&pokemon_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

void compare_item_lists(
    pkmn_item_list_handle_t item_list1,
    pkmn_item_list_handle_t item_list2
) {
    TEST_ASSERT_NOT_NULL(item_list1);
    TEST_ASSERT_NOT_NULL(item_list2);

    char item_list1_strbuffer[STRBUFFER_LEN] = {0};
    char item_list2_strbuffer[STRBUFFER_LEN] = {0};

    error = pkmn_item_list_get_name(
                item_list1,
                item_list1_strbuffer,
                sizeof(item_list1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_get_name(
                item_list2,
                item_list2_strbuffer,
                sizeof(item_list1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(item_list1_strbuffer, item_list2_strbuffer);

    error = pkmn_item_list_get_game(
                item_list1,
                item_list1_strbuffer,
                sizeof(item_list1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_get_game(
                item_list2,
                item_list2_strbuffer,
                sizeof(item_list1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(item_list1_strbuffer, item_list2_strbuffer);

    int capacity1 = 0;
    int capacity2 = 0;
    error = pkmn_item_list_get_capacity(
                item_list1,
                &capacity1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_get_capacity(
                item_list2,
                &capacity2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(capacity1, capacity2);

    int num_items1 = 0;
    int num_items2 = 0;
    error = pkmn_item_list_get_num_items(
                item_list1,
                &num_items1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_get_num_items(
                item_list2,
                &num_items2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(num_items1, num_items2);

    pkmn_item_slots_t item_slots1;
    pkmn_item_slots_t item_slots2;
    error = pkmn_item_list_as_array(
                item_list1,
                &item_slots1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_list_as_array(
                item_list2,
                &item_slots2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(item_slots1.length, item_slots2.length);
    TEST_ASSERT_NOT_NULL(item_slots1.item_slots);
    TEST_ASSERT_NOT_NULL(item_slots2.item_slots);

    for(size_t i = 0; i < item_slots1.length; ++i) {

        TEST_ASSERT_EQUAL_STRING(item_slots1.item_slots[i].item, item_slots2.item_slots[i].item);
        TEST_ASSERT_EQUAL(item_slots1.item_slots[i].amount, item_slots2.item_slots[i].amount);
    }

    error = pkmn_item_slots_free(&item_slots1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_item_slots_free(&item_slots2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

static void compare_pokemon(
    pkmn_pokemon_handle_t pokemon1,
    pkmn_pokemon_handle_t pokemon2
) {
    TEST_ASSERT_NOT_NULL(pokemon1);
    TEST_ASSERT_NOT_NULL(pokemon2);

    char pokemon1_strbuffer[STRBUFFER_LEN] = {0};
    char pokemon2_strbuffer[STRBUFFER_LEN] = {0};

    pkmn_trainer_info_t pokemon1_trainer_info =
    {
        .nickname = {0},
        .trainer_name = {0},
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_MALE
    };
    pkmn_trainer_info_t pokemon2_trainer_info =
    {
        .nickname = {0},
        .trainer_name = {0},
        .trainer_id =
        {
            .public_id = 0,
            .secret_id = 0
        },
        .trainer_gender = PKMN_MALE
    };

    error = pkmn_pokemon_get_species(
                pokemon1,
                pokemon1_strbuffer,
                sizeof(pokemon1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_species(
                pokemon2,
                pokemon2_strbuffer,
                sizeof(pokemon2_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(pokemon1_strbuffer, pokemon2_strbuffer);

    error = pkmn_pokemon_get_game(
                pokemon1,
                pokemon1_strbuffer,
                sizeof(pokemon1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_game(
                pokemon2,
                pokemon2_strbuffer,
                sizeof(pokemon2_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(pokemon1_strbuffer, pokemon2_strbuffer);

    error = pkmn_pokemon_get_trainer_info(
                pokemon1,
                &pokemon1_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_get_trainer_info(
                pokemon2,
                &pokemon2_trainer_info
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(
        pokemon1_trainer_info.nickname,
        pokemon2_trainer_info.nickname
    );
    TEST_ASSERT_EQUAL_STRING(
        pokemon1_trainer_info.trainer_name,
        pokemon2_trainer_info.trainer_name
    );
}

static void compare_game_saves(
    pkmn_game_save_handle_t save1,
    pkmn_game_save_handle_t save2
) {
    TEST_ASSERT_NOT_NULL(save1);
    TEST_ASSERT_NOT_NULL(save2);

    char save1_strbuffer[STRBUFFER_LEN] = {0};
    char save2_strbuffer[STRBUFFER_LEN] = {0};

    char game[STRBUFFER_LEN] = {0};

    error = pkmn_game_save_get_game(
                save1,
                game,
                sizeof(game)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save_get_game(
                save2,
                save2_strbuffer,
                sizeof(save2_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, save2_strbuffer);

    int generation = game_to_generation(save1_strbuffer);
    bool is_gb_game = (generation <= 2);

    error = pkmn_game_save_get_trainer_name(
                save1,
                save1_strbuffer,
                sizeof(save1_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save_get_trainer_name(
                save2,
                save2_strbuffer,
                sizeof(save2_strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(save1_strbuffer, save2_strbuffer);

    uint32_t trainer_id1 = 0;
    uint32_t trainer_id2 = 0;
    error = pkmn_game_save_get_trainer_id(
                save1,
                &trainer_id1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save_get_trainer_id(
                save2,
                &trainer_id2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(trainer_id1, trainer_id2);

    uint16_t trainer_public_id1 = 0;
    uint16_t trainer_public_id2 = 0;
    error = pkmn_game_save_get_trainer_public_id(
                save1,
                &trainer_public_id1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save_get_trainer_public_id(
                save2,
                &trainer_public_id2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(trainer_public_id1, trainer_public_id2);

    if(!is_gb_game) {
        uint16_t trainer_secret_id1 = 0;
        uint16_t trainer_secret_id2 = 0;
        error = pkmn_game_save_get_trainer_secret_id(
                    save1,
                    &trainer_secret_id1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_game_save_get_trainer_secret_id(
                    save2,
                    &trainer_secret_id2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(trainer_secret_id1, trainer_secret_id2);
    }
    if(!is_male_only(game)) {
        pkmn_gender_t gender1 = PKMN_GENDERLESS;
        pkmn_gender_t gender2 = PKMN_GENDERLESS;

        error = pkmn_game_save_get_trainer_gender(
                    save1,
                    &gender1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_game_save_get_trainer_gender(
                    save2,
                    &gender2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(gender1, gender2);
    }
    if(!is_rival_name_set(game)) {
        error = pkmn_game_save_get_rival_name(
                    save1,
                    save1_strbuffer,
                    sizeof(save1_strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_game_save_get_rival_name(
                    save2,
                    save2_strbuffer,
                    sizeof(save2_strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(save1_strbuffer, save2_strbuffer);
    }

    int money1 = 0;
    int money2 = 0;
    error = pkmn_game_save_get_money(
                save1,
                &money1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_game_save_get_money(
                save2,
                &money2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(money1, money2);

    pkmn_item_bag_handle_t item_bag1 = NULL;
    pkmn_item_bag_handle_t item_bag2 = NULL;

    error = pkmn_game_save_get_item_bag(
                save1,
                &item_bag1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag1);
    error = pkmn_game_save_get_item_bag(
                save1,
                &item_bag2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(item_bag2);

    pkmn_string_list_t pocket_names1;
    pkmn_string_list_t pocket_names2;

    error = pkmn_item_bag_get_pocket_names(
                item_bag1,
                &pocket_names1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(pocket_names1.length > 0);
    error = pkmn_item_bag_get_pocket_names(
                item_bag2,
                &pocket_names2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(pocket_names2.length > 0);
    TEST_ASSERT_EQUAL(pocket_names1.length, pocket_names2.length);

    for(size_t i = 0; i < pocket_names1.length; ++i) {
        TEST_ASSERT_NOT_NULL(pocket_names1.strings[i]);
        pkmn_item_list_handle_t item_list1 = NULL;
        pkmn_item_list_handle_t item_list2 = NULL;

        error = pkmn_item_bag_get_pocket(
                    item_bag1,
                    pocket_names1.strings[i],
                    &item_list1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_list1);
        error = pkmn_item_bag_get_pocket(
                    item_bag2,
                    pocket_names1.strings[i],
                    &item_list2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_list2);

        error = pkmn_item_list_free(&item_list2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_list2);
        error = pkmn_item_list_free(&item_list1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_list1);
    }

    error = pkmn_string_list_free(&pocket_names1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_string_list_free(&pocket_names2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_item_bag_free(&item_bag2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_bag2);
    error = pkmn_item_bag_free(&item_bag1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(item_bag1);

    if(generation <= 3) {
        pkmn_item_list_handle_t item_pc1 = NULL;
        pkmn_item_list_handle_t item_pc2 = NULL;

        error = pkmn_game_save_get_item_pc(
                    save1,
                    &item_pc1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_pc1);
        error = pkmn_game_save_get_item_pc(
                    save2,
                    &item_pc2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NOT_NULL(item_pc2);

        compare_item_lists(
            item_pc1,
            item_pc2
        );

        error = pkmn_item_list_free(&item_pc2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_pc2);
        error = pkmn_item_list_free(&item_pc1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_NULL(item_pc1);
    }

    pkmn_pokemon_party_handle_t party1 = NULL;
    pkmn_pokemon_party_handle_t party2 = NULL;

    error = pkmn_game_save_get_pokemon_party(
                save1,
                &party1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(party1);
    error = pkmn_game_save_get_pokemon_party(
                save2,
                &party2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(party2);

    pkmn_pokemon_list_t party_pokemon1;
    pkmn_pokemon_list_t party_pokemon2;

    error = pkmn_pokemon_party_as_array(
                party1,
                &party_pokemon1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(6, party_pokemon1.length);
    error = pkmn_pokemon_party_as_array(
                party2,
                &party_pokemon2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(6, party_pokemon2.length);

    for(size_t i = 0; i < 6; ++i) {
        TEST_ASSERT_NOT_NULL(party_pokemon1.pokemon_list[i]);
        TEST_ASSERT_NOT_NULL(party_pokemon2.pokemon_list[i]);

        compare_pokemon(
            party_pokemon1.pokemon_list[i],
            party_pokemon2.pokemon_list[i]
        );
    }

    error = pkmn_pokemon_list_free(&party_pokemon2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_list_free(&party_pokemon1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_party_free(&party2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(party2);
    error = pkmn_pokemon_party_free(&party1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(party1);

    pkmn_pokemon_pc_handle_t pc1 = NULL;
    pkmn_pokemon_pc_handle_t pc2 = NULL;

    error = pkmn_game_save_get_pokemon_pc(
                save1,
                &pc1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pc1);
    error = pkmn_game_save_get_pokemon_pc(
                save2,
                &pc2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(pc2);

    pkmn_pokemon_box_list_t pokemon_boxes1;
    pkmn_pokemon_box_list_t pokemon_boxes2;

    error = pkmn_pokemon_pc_as_array(
                pc1,
                &pokemon_boxes1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(pokemon_boxes1.length > 0);
    error = pkmn_pokemon_pc_as_array(
                pc2,
                &pokemon_boxes2
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(pokemon_boxes2.length > 0);
    TEST_ASSERT_EQUAL(pokemon_boxes1.length, pokemon_boxes2.length);

    if(generation >= 2) {
        pkmn_string_list_t box_names1;
        pkmn_string_list_t box_names2;

        error = pkmn_pokemon_pc_get_box_names(
                    pc1,
                    &box_names1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_pc_get_box_names(
                    pc2,
                    &box_names2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        TEST_ASSERT_EQUAL(box_names1.length, box_names2.length);
        for(size_t i = 0; i < box_names1.length; ++i) {
            TEST_ASSERT_EQUAL_STRING(box_names1.strings[i], box_names2.strings[i]);
        }

        error = pkmn_string_list_free(&box_names2);
        error = pkmn_string_list_free(&box_names1);
    }

    for(size_t i = 0; i < pokemon_boxes1.length; ++i) {
        TEST_ASSERT_NOT_NULL(pokemon_boxes1.pokemon_boxes[i]);
        TEST_ASSERT_NOT_NULL(pokemon_boxes2.pokemon_boxes[i]);

        if(generation >= 2) {
            error = pkmn_pokemon_box_get_name(
                        pokemon_boxes1.pokemon_boxes[i],
                        save1_strbuffer,
                        sizeof(save1_strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            error = pkmn_pokemon_box_get_name(
                        pokemon_boxes2.pokemon_boxes[i],
                        save2_strbuffer,
                        sizeof(save2_strbuffer)
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

            TEST_ASSERT_EQUAL_STRING(save1_strbuffer, save2_strbuffer);
        }

        pkmn_pokemon_list_t box_pokemon1;
        pkmn_pokemon_list_t box_pokemon2;

        error = pkmn_pokemon_box_as_array(
                    pokemon_boxes1.pokemon_boxes[i],
                    &box_pokemon1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(box_pokemon1.length > 0);
        error = pkmn_pokemon_box_as_array(
                    pokemon_boxes2.pokemon_boxes[i],
                    &box_pokemon2
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(box_pokemon2.length > 0);
        TEST_ASSERT_EQUAL(box_pokemon1.length, box_pokemon2.length);

        for(size_t j = 0; j < box_pokemon1.length; ++j) {
            TEST_ASSERT_NOT_NULL(box_pokemon1.pokemon_list[j]);
            TEST_ASSERT_NOT_NULL(box_pokemon2.pokemon_list[j]);
            compare_pokemon(
                box_pokemon1.pokemon_list[j],
                box_pokemon2.pokemon_list[j]
            );
        }

        error = pkmn_pokemon_list_free(&box_pokemon1);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        error = pkmn_pokemon_list_free(&box_pokemon2);
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    }

    error = pkmn_pokemon_box_list_free(&pokemon_boxes2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    error = pkmn_pokemon_box_list_free(&pokemon_boxes1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_pokemon_pc_free(&pc2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pc2);
    error = pkmn_pokemon_pc_free(&pc1);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pc1);
}

static void test_game_save(
    pkmn_game_save_type_t save_type,
    const char* game,
    const char* subdir,
    const char* filename
) {
    TEST_ASSERT_NOT_NULL(game);
    TEST_ASSERT_NOT_NULL(subdir);
    TEST_ASSERT_NOT_NULL(filename);

    pkmn_game_save_type_t save_type_from_file = PKMN_GAME_SAVE_TYPE_NONE;
    pkmn_game_save_handle_t game_save = NULL;
    pkmn_string_list_t item_list;
    bool gamecube = !strcmp(game, "Colosseum") || !strcmp(game, "XD");

    char save_filepath[STRBUFFER_LEN] = {0};
    snprintf(
        save_filepath,
        sizeof(save_filepath),
        "%s%s%s%s%s",
        gamecube ? LIBPKMN_TEST_FILES : PKSAV_TEST_SAVES,
        FS_SEPARATOR, subdir, FS_SEPARATOR, filename
    );

    error = pkmn_game_save_detect_type(
                save_filepath,
                &save_type_from_file
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(save_type, save_type_from_file);

    error = pkmn_game_save_from_file(
                &game_save,
                save_filepath
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(game_save);

    error = pkmn_game_save_get_game(
                game_save,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(game, strbuffer);

    error = pkmn_database_item_list(
                game,
                &item_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_TRUE(item_list.length > 0);

    game_save_test_common_fields(
        game_save,
        game
    );
    // TODO: randomize_items
    randomize_pokemon(
        game_save,
        game,
        &item_list
    );

    error = pkmn_string_list_free(&item_list);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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

    error = pkmn_game_save_save_as(
                game_save,
                tmp_save_filepath
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_game_save_handle_t game_save2 = NULL;
    error = pkmn_game_save_from_file(
                &game_save2,
                tmp_save_filepath
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NOT_NULL(game_save2);

    compare_game_saves(
        game_save,
        game_save2
    );

#ifdef PKMN_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(DeleteFile(tmp_save_filepath), 0);
#else
    TEST_ASSERT_EQUAL(remove(tmp_save_filepath), 0);
#endif

    error = pkmn_game_save_free(&game_save2);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(game_save2);

    error = pkmn_game_save_free(&game_save);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(game_save);
}

#define PKMN_C_GAME_SAVE_TEST(save_type, game, subdir, filename) { \
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
