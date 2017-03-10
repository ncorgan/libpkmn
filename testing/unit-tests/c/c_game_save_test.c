/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#include <stdio.h>
#include <string.h>

#ifdef PKMN_PLATFORM_WIN32
#    define FS_SEPARATOR "\\"
#else
#    define FS_SEPARATOR "/"
#endif

#define TOO_LONG_OT_NAME "LibPKMNLibPKMN"
#define LIBPKMN_OT_PID 1351
#define LIBPKMN_OT_SID 32123
#define MONEY_MAX_VALUE 999999

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};

static pkmn_error_t error = PKMN_ERROR_NONE;
static char PKSAV_TEST_SAVES[STRBUFFER_LEN] = {0};

static const char* RIVAL_NAME_SET_GAMES[] = {
    "Ruby", "Sapphire", "Emerald",
    "Black", "White",
    "X", "Y"
};

static const char* MALE_ONLY_GAMES[] = {
    "Red", "Blue", "Yellow",
    "Gold", "Silver"
};

/*
 * Utility functions
 */

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

static bool is_rival_name_set(
    const char* game
) {
    for(size_t i = 0; i < 7; ++i) {
        if(!strcmp(game, RIVAL_NAME_SET_GAMES[i])) {
            return true;
        }
    }

    return false;
}

static bool is_male_only(
    const char* game
) {
    for(size_t i = 0; i < 5; ++i) {
        if(!strcmp(game, MALE_ONLY_GAMES[i])) {
            return true;
        }
    }

    return false;
}

static void populate_pksav_test_saves() {
    char* value = getenv("PKSAV_TEST_SAVES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(PKSAV_TEST_SAVES, sizeof(PKSAV_TEST_SAVES), "%s", value);
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
                LIBPKMN_OT_NAME
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    error = pkmn_game_save_get_trainer_name(
                game_save,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL_STRING(LIBPKMN_OT_NAME, strbuffer);
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
    TEST_ASSERT_EQUAL((is_gb_game ? LIBPKMN_OT_PID : LIBPKMN_OT_ID), trainer_id);

    error = pkmn_game_save_get_trainer_public_id(
                game_save,
                &trainer_id_part
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_EQUAL(LIBPKMN_OT_PID, trainer_id_part);

    error = pkmn_game_save_get_trainer_secret_id(
                game_save,
                &trainer_id_part
            );
    if(is_gb_game) {
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_GAME_ERROR, error);
    } else {
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL(LIBPKMN_OT_SID, trainer_id_part);
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
                    LIBPKMN_OT_NAME
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
                    LIBPKMN_OT_NAME
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        error = pkmn_game_save_get_rival_name(
                    game_save,
                    strbuffer,
                    sizeof(strbuffer)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_EQUAL_STRING(LIBPKMN_OT_NAME, strbuffer);
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
                is_gb_game ? LIBPKMN_OT_PID : LIBPKMN_OT_ID
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    game_save_test_trainer_id(
        game_save,
        is_gb_game
    );

    error = pkmn_game_save_set_trainer_public_id(
                game_save,
                LIBPKMN_OT_PID
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    game_save_test_trainer_id(
        game_save,
        is_gb_game
    );

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
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

    error = pkmn_game_save_set_money(
                game_save,
                MONEY_MAX_VALUE+1
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_RANGE_ERROR, error);

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

    char save_filepath[STRBUFFER_LEN] = {0};
    snprintf(
        save_filepath,
        sizeof(save_filepath),
        "%s%s%s%s%s",
        PKSAV_TEST_SAVES, FS_SEPARATOR, subdir, FS_SEPARATOR, filename
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

    game_save_test_common_fields(
        game_save,
        game
    );

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
    populate_pksav_test_saves();
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW, "Red", "red_blue", "pokemon_red.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RED_BLUE_YELLOW, "Yellow", "yellow", "pokemon_yellow.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_GOLD_SILVER, "Gold", "gold_silver", "pokemon_gold.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_CRYSTAL, "Crystal", "crystal", "pokemon_crystal.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_RUBY_SAPPHIRE, "Ruby", "ruby_sapphire", "pokemon_ruby.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_EMERALD, "Emerald", "emerald", "pokemon_emerald.sav");
    PKMN_C_GAME_SAVE_TEST(PKMN_GAME_SAVE_TYPE_FIRERED_LEAFGREEN, "FireRed", "firered_leafgreen", "pokemon_firered.sav");
)
