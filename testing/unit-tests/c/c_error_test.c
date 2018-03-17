/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

// This test checks error cases specific to the C API, mostly null pointer
// checking.

#include "c_test_common.h"

#include <pkmn-c.h>

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

static pkmn_error_t error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static char PKSAV_TEST_SAVES[STRBUFFER_LEN] = {0};
static bool dummy_bool = false;
static int dummy_int = 0;
static size_t dummy_size_t = 0;
static uint32_t dummy_uint32 = 0;
static float dummy_float = 0;
static pkmn_string_list_t dummy_string_list = { NULL, 0 };
static pkmn_trainer_info_t dummy_trainer_info = { NULL, {0}, PKMN_GENDER_GENDERLESS};
static pkmn_attribute_names_t dummy_attribute_names = {{NULL, 0}, {NULL, 0}, {NULL, 0}};
static pkmn_pokemon_t dummy_pokemon = {NULL, NULL, NULL};
static pkmn_pokemon_list_t dummy_pokemon_list = { NULL, 0 };

static const char* null_pointer_error_format = "Null pointer passed into parameter \"%s\"";

static void populate_pksav_saves()
{
    char* value = getenv("PKSAV_TEST_SAVES");
    TEST_ASSERT_NOT_NULL(value);
    snprintf(PKSAV_TEST_SAVES, sizeof(PKSAV_TEST_SAVES), "%s", value);
}

#define TEST_NULL_POINTER_RETURN(param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
}

#define TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, param_name) \
{ \
    snprintf(strbuffer, STRBUFFER_LEN, null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_game_save_strerror(&game_save)); \
}

#define TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_item_bag_strerror(&item_bag)); \
}

#define TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_item_list_strerror(&item_list)); \
}

#define TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokedex_strerror(&pokedex)); \
}

#define TEST_POKEMON_NULL_POINTER_RETURN(pokemon, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_strerror(&pokemon)); \
}

#define TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_box_strerror(&pokemon_box)); \
}

#define TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_party_strerror(&pokemon_party)); \
}

#define TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_pc_strerror(&pokemon_pc)); \
}

static void test_pkmn_strerror_as_first_call()
{
    const char* strerror_output = pkmn_strerror();
    TEST_ASSERT_EQUAL_STRING("None", strerror_output);
}

/*
 * <pkmn-c/game_save.h>
 */
static void game_save_error_test()
{
    populate_pksav_saves();

    pkmn_game_save_type_t dummy_game_save_type = PKMN_GAME_SAVE_TYPE_NONE;

    pkmn_item_bag_t dummy_item_bag = { NULL, { NULL, 0 }, NULL };
    pkmn_item_list_t dummy_item_list = { NULL, NULL, 0, NULL };
    pkmn_pokedex_t dummy_pokedex = { NULL, NULL };
    pkmn_pokemon_party_t dummy_pokemon_party = { NULL, 0, NULL };
    pkmn_pokemon_pc_t dummy_pokemon_pc = { NULL, 0, NULL };

    pkmn_game_save_t game_save = { NULL, NULL };

    char filepath[STRBUFFER_LEN] = {0};
    snprintf(
        filepath,
        sizeof(filepath),
        "%s%sred_blue%spokemon_red.sav",
        PKSAV_TEST_SAVES,
        FS_SEPARATOR,
        FS_SEPARATOR
    );

    error = pkmn_game_save_init_from_file(
                filepath,
                &game_save
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_game_save_detect_type
     */

    error = pkmn_game_save_detect_type(
                NULL,
                &dummy_game_save_type
            );
    TEST_NULL_POINTER_RETURN("filepath");

    error = pkmn_game_save_detect_type(
                filepath,
                NULL
            );
    TEST_NULL_POINTER_RETURN("game_save_type_out");

    /*
     * pkmn_game_save_init_from_file
     */

    error = pkmn_game_save_init_from_file(
                NULL,
                &game_save
            );
    TEST_NULL_POINTER_RETURN("filepath");

    error = pkmn_game_save_init_from_file(
                filepath,
                NULL
            );
    TEST_NULL_POINTER_RETURN("game_save_out");

    /*
     * pkmn_game_save_free
     */

    error = pkmn_game_save_free(NULL);
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_strerror
     */

    const char* game_save_strerror = pkmn_game_save_strerror(NULL);
    TEST_ASSERT_NULL(game_save_strerror);

    /*
     * pkmn_game_save_get_filepath
     */

    error = pkmn_game_save_get_filepath(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_filepath(
                &game_save,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "filepath_buffer");

    // actual_filepath_length_out can be NULL.
    error = pkmn_game_save_get_filepath(
                &game_save,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_game_save_save
     */

    error = pkmn_game_save_save(NULL);
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_save_as
     */

    error = pkmn_game_save_save_as(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_save_as(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "filepath");

    /*
     * pkmn_game_save_get_trainer_info
     */

    error = pkmn_game_save_get_trainer_info(
                NULL,
                &dummy_trainer_info
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_trainer_info(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "trainer_info_out");

    /*
     * pkmn_game_save_set_trainer_name
     */

    error = pkmn_game_save_set_trainer_name(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_set_trainer_name(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "trainer_name");

    /*
     * pkmn_game_save_set_trainer_public_id
     */

    error = pkmn_game_save_set_trainer_public_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_set_trainer_secret_id
     */

    error = pkmn_game_save_set_trainer_secret_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_set_trainer_id
     */

    error = pkmn_game_save_set_trainer_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_set_trainer_gender
     */

    error = pkmn_game_save_set_trainer_gender(
                NULL,
                PKMN_GENDER_GENDERLESS
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_get_rival_name
     */

    error = pkmn_game_save_get_rival_name(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_rival_name(
                &game_save,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "rival_name_buffer");

    // actual_rival_name_length_out can be NULL.
    error = pkmn_game_save_get_rival_name(
                &game_save,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_game_save_set_rival_name
     */

    error = pkmn_game_save_set_rival_name(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_set_rival_name(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "rival_name");

    /*
     * pkmn_game_save_get_money
     */

    error = pkmn_game_save_get_money(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_money(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "money_out");

    /*
     * pkmn_game_save_set_money
     */

    error = pkmn_game_save_set_money(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    /*
     * pkmn_game_save_get_pokedex
     */

    error = pkmn_game_save_get_pokedex(
                NULL,
                &dummy_pokedex
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_pokedex(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "pokedex_out");

    /*
     * pkmn_game_save_get_pokemon_party
     */

    error = pkmn_game_save_get_pokemon_party(
                NULL,
                &dummy_pokemon_party
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_pokemon_party(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "pokemon_party_out");

    /*
     * pkmn_game_save_get_pokemon_pc
     */

    error = pkmn_game_save_get_pokemon_pc(
                NULL,
                &dummy_pokemon_pc
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_pokemon_pc(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "pokemon_pc_out");

    /*
     * pkmn_game_save_get_item_bag
     */

    error = pkmn_game_save_get_item_bag(
                NULL,
                &dummy_item_bag
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_item_bag(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "item_bag_out");

    /*
     * pkmn_game_save_get_item_pc
     */

    error = pkmn_game_save_get_item_pc(
                NULL,
                &dummy_item_list
            );
    TEST_NULL_POINTER_RETURN("game_save_ptr");

    error = pkmn_game_save_get_item_pc(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "item_pc_out");

    // Cleanup
    error = pkmn_game_save_free(&game_save);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/item_bag.h>
 */
static void item_bag_error_test()
{
    pkmn_item_list_t item_list = { NULL, NULL, 0, NULL };
    pkmn_item_bag_t item_bag = { NULL, { NULL, 0 }, NULL };

    error = pkmn_item_bag_init(
                "Red",
                &item_bag
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_item_bag_init
     */

    error = pkmn_item_bag_init(
                NULL, // game
                &item_bag
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_item_bag_init(
                "Red",
                NULL // item_bag_out
            );
    TEST_NULL_POINTER_RETURN("item_bag_out");

    /*
     * pkmn_item_bag_free
     */

    error = pkmn_item_bag_free(
                NULL // item_bag_ptr
            );
    TEST_NULL_POINTER_RETURN("item_bag_ptr");

    /*
     * pkmn_item_bag_strerror
     */

    const char* item_bag_strerror = pkmn_item_bag_strerror(
                                        NULL // item_bag_ptr
                                    );
    TEST_ASSERT_NULL(item_bag_strerror);

    /*
     * pkmn_item_bag_get_pocket
     */

    error = pkmn_item_bag_get_pocket(
                NULL, // item_bag_ptr
                "Items",
                &item_list
            );
    TEST_NULL_POINTER_RETURN("item_bag_ptr");

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                NULL, // pocket_name
                &item_list
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "pocket_name");

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                "Items",
                NULL // item_list_out
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "item_list_out");

    /*
     * pkmn_item_bag_add
     */

    error = pkmn_item_bag_add(
                NULL, // item_bag_ptr
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("item_bag_ptr");

    error = pkmn_item_bag_add(
                &item_bag,
                NULL, // item
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "item");

    /*
     * pkmn_item_bag_remove
     */

    error = pkmn_item_bag_remove(
                NULL, // item_bag_ptr
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("item_bag_ptr");

    error = pkmn_item_bag_remove(
                &item_bag,
                NULL, // item
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "item");

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/item_list.h>
 */
static void item_list_error_test()
{
    pkmn_item_list_t item_list = { NULL, NULL, 0, NULL };

    error = pkmn_item_list_init(
                "Items",
                "Red",
                &item_list
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_item_list_init
     */

    error = pkmn_item_list_init(
                "Items",
                "Red",
                NULL // item_list_out
            );
    TEST_NULL_POINTER_RETURN("item_list_out");

    error = pkmn_item_list_init(
                NULL, // name
                "Red",
                &item_list
            );
    TEST_NULL_POINTER_RETURN("name");

    error = pkmn_item_list_init(
                "Items",
                NULL, // game
                &item_list
            );
    TEST_NULL_POINTER_RETURN("game");

    /*
     * pkmn_item_list_free
     */

    error = pkmn_item_list_free(
                NULL // item_list_ptr
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    /*
     * pkmn_item_list_strerror
     */

    const char* item_list_strerror = pkmn_item_list_strerror(
                                         NULL // item_list_ptr
                                     );
    TEST_ASSERT_NULL(item_list_strerror);

    /*
     * pkmn_item_list_add
     */

    error = pkmn_item_list_add(
                NULL, // item_list_ptr
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    error = pkmn_item_list_add(
                &item_list,
                NULL, // item
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "item");

    /*
     * pkmn_item_list_remove
     */

    error = pkmn_item_list_remove(
                NULL, // item_list_ptr
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    error = pkmn_item_list_remove(
                &item_list,
                NULL, // item
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "item");

    /*
     * pkmn_item_list_move
     */

    error = pkmn_item_list_move(
                NULL, // item_list_ptr
                0,
                1
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    /*
     * pkmn_item_list_set_item
     */

    error = pkmn_item_list_set_item(
                NULL, // item_list_ptr
                0,
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    error = pkmn_item_list_set_item(
                &item_list, // item_list_ptr
                0,
                NULL, // item
                5
            );
    TEST_NULL_POINTER_RETURN("item");

    /*
     * pkmn_item_list_get_valid_items
     */

    error = pkmn_item_list_get_valid_items(
                NULL, // item_list_ptr
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("item_list_ptr");

    error = pkmn_item_list_get_valid_items(
                &item_list,
                NULL // valid_items_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "valid_items_out");
}

/*
 * <pkmn-c/pokedex.h>
 */
static void pokedex_error_test()
{
    pkmn_pokedex_t pokedex =
    {
        .game = NULL,
        ._internal = NULL
    };
    pkmn_error_t error = PKMN_ERROR_NONE;

    error = pkmn_pokedex_init("Red", &pokedex);
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokedex_init
     */

    error = pkmn_pokedex_init(
                NULL, // game
                &pokedex
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokedex_init(
                "",
                NULL // pokedex_out
            );
    TEST_NULL_POINTER_RETURN("pokedex_out");

    /*
     * pkmn_pokedex_free
     */

    error = pkmn_pokedex_free(NULL); // pokedex_ptr
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    /*
     * pkmn_pokedex_strerror
     */

    const char* pokedex_strerror = pkmn_pokedex_strerror(NULL); // pokedex_ptr
    TEST_ASSERT_NULL(pokedex_strerror);

    /*
     * pkmn_pokedex_has_seen
     */

    error = pkmn_pokedex_has_seen(
                NULL, // pokedex_ptr
                "",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_has_seen(
                &pokedex,
                NULL, // species
                &dummy_bool
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "species");

    error = pkmn_pokedex_has_seen(
                &pokedex,
                "",
                NULL // has_seen_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "has_seen_out");

    /*
     * pkmn_pokedex_set_has_seen
     */

    error = pkmn_pokedex_set_has_seen(
                NULL, // pokedex_ptr
                "",
                true
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_set_has_seen(
                &pokedex,
                NULL, // species
                true
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "species");

    /*
     * pkmn_pokedex_get_all_seen
     */

    error = pkmn_pokedex_get_all_seen(
                NULL, // pokedex_ptr
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_get_all_seen(
                &pokedex,
                NULL // all_seen_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "all_seen_out");

    /*
     * pkmn_pokedex_get_num_seen
     */

    error = pkmn_pokedex_get_num_seen(
                NULL, // pokedex_ptr
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                NULL // num_seen_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "num_seen_out");

    /*
     * pkmn_pokedex_has_caught
     */

    error = pkmn_pokedex_has_caught(
                NULL, // pokedex_ptr
                "",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_has_caught(
                &pokedex,
                NULL, // species
                &dummy_bool
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "species");

    error = pkmn_pokedex_has_caught(
                &pokedex,
                "",
                NULL // has_caught_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "has_caught_out");

    /*
     * pkmn_pokedex_set_has_caught
     */

    error = pkmn_pokedex_set_has_caught(
                NULL, // pokedex_ptr
                "",
                true
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_set_has_caught(
                &pokedex,
                NULL, // species
                true
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "species");

    /*
     * pkmn_pokedex_get_all_caught
     */

    error = pkmn_pokedex_get_all_caught(
                NULL, // pokedex_ptr
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_get_all_caught(
                &pokedex,
                NULL // all_caught_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "all_caught_out");

    /*
     * pkmn_pokedex_get_num_caught
     */

    error = pkmn_pokedex_get_num_caught(
                NULL, // pokedex_ptr
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokedex_ptr");

    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                NULL // num_caught_out
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "num_caught_out");
}

/*
 * <pkmn-c/pokemon.h>
 */
static void pokemon_error_test()
{
    pkmn_pokemon_t pokemon =
    {
        .species = NULL,
        .game = NULL,
        ._internal = NULL
    };
    pkmn_database_pokemon_entry_t dummy_pokemon_entry;
    pkmn_condition_t dummy_condition = PKMN_CONDITION_NONE;
    pkmn_gender_t dummy_gender = PKMN_GENDER_GENDERLESS;
    pkmn_move_slots_t dummy_move_slots = { NULL, 0 };

    error = pkmn_pokemon_init(
                "Bulbasaur",
                "Red",
                "",
                5,
                &pokemon
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokemon_init
     */

    error = pkmn_pokemon_init(
                NULL,
                strbuffer,
                strbuffer,
                0,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_pokemon_init(
                strbuffer,
                NULL,
                strbuffer,
                0,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokemon_init(
                strbuffer,
                strbuffer,
                NULL,
                0,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("form");

    error = pkmn_pokemon_init(
                strbuffer,
                strbuffer,
                strbuffer,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_out");

    /*
     * pkmn_pokemon_init_from_file
     */

    error = pkmn_pokemon_init_from_file(
                NULL,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("filepath");

    error = pkmn_pokemon_init_from_file(
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_out");

    /*
     * pkmn_pokemon_free
     */

    error = pkmn_pokemon_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_strerror
     */

    const char* pokemon_strerror = pkmn_pokemon_strerror(NULL);
    TEST_ASSERT_NULL(pokemon_strerror);

    /*
     * pkmn_pokemon_to_game
     */

    error = pkmn_pokemon_to_game(
                NULL,
                strbuffer,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_to_game(
                &pokemon,
                NULL,
                &pokemon
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "game");

    error = pkmn_pokemon_to_game(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "new_pokemon_out");

    /*
     * pkmn_pokemon_export_to_file
     */

    error = pkmn_pokemon_export_to_file(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_export_to_file(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "filepath");

    /*
     * pkmn_pokemon_get_form
     */

    error = pkmn_pokemon_get_form(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_form(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "form_buffer");

    /*
     * pkmn_pokemon_set_form
     */

    error = pkmn_pokemon_set_form(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_form(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "form");

    /*
     * pkmn_pokemon_is_egg
     */

    error = pkmn_pokemon_is_egg(
                NULL,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_is_egg(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "is_egg_out");

    /*
     * pkmn_pokemon_set_is_egg
     */

    error = pkmn_pokemon_set_is_egg(
                NULL,
                dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_database_entry
     */

    error = pkmn_pokemon_get_database_entry(
                NULL,
                &dummy_pokemon_entry
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_database_entry(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "database_entry_ptr");

    /*
     * pkmn_pokemon_get_condition
     */

    error = pkmn_pokemon_get_condition(
                NULL,
                &dummy_condition
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_condition(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "condition_out");

    /*
     * pkmn_pokemon_set_condition
     */

    error = pkmn_pokemon_set_condition(
                NULL,
                dummy_condition
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_nickname
     */

    error = pkmn_pokemon_get_nickname(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_nickname(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "nickname_out");

    /*
     * pkmn_pokemon_set_nickname
     */

    error = pkmn_pokemon_set_nickname(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_nickname(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "nickname");

    /*
     * pkmn_pokemon_get_gender
     */

    error = pkmn_pokemon_get_gender(
                NULL,
                &dummy_gender
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_gender(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "gender_out");

    /*
     * pkmn_pokemon_set_gender
     */

    error = pkmn_pokemon_set_gender(
                NULL,
                dummy_gender
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_is_shiny
     */

    error = pkmn_pokemon_is_shiny(
                NULL,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_is_shiny(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "is_shiny_out");

    /*
     * pkmn_pokemon_set_is_shiny
     */

    error = pkmn_pokemon_set_is_shiny(
                NULL,
                dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_held_item
     */

    error = pkmn_pokemon_get_held_item(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_held_item(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "held_item_out");

    /*
     * pkmn_pokemon_set_held_item
     */

    error = pkmn_pokemon_set_held_item(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_held_item(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "held_item");

    /*
     * pkmn_pokemon_get_pokerus_duration
     */

    error = pkmn_pokemon_get_pokerus_duration(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_pokerus_duration(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "pokerus_duration_out");

    /*
     * pkmn_pokemon_set_pokerus_duration
     */

    error = pkmn_pokemon_set_pokerus_duration(
                NULL,
                dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_original_trainer_info
     */

    error = pkmn_pokemon_get_original_trainer_info(
                NULL,
                &dummy_trainer_info
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_original_trainer_info(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "original_trainer_info_out");

    /*
     * pkmn_pokemon_set_original_trainer_public_id
     */

    error = pkmn_pokemon_set_original_trainer_public_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_set_original_trainer_secret_id
     */

    error = pkmn_pokemon_set_original_trainer_secret_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_set_original_trainer_id
     */

    error = pkmn_pokemon_set_original_trainer_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_set_original_trainer_gender
     */

    error = pkmn_pokemon_set_original_trainer_gender(
                NULL,
                dummy_gender
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_current_trainer_friendship
     */

    error = pkmn_pokemon_get_current_trainer_friendship(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_current_trainer_friendship(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "current_trainer_friendship_out");

    /*
     * pkmn_pokemon_set_current_trainer_friendship
     */

    error = pkmn_pokemon_set_current_trainer_friendship(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_ability
     */

    error = pkmn_pokemon_get_ability(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_ability(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ability_out");

    /*
     * pkmn_pokemon_set_ability
     */

    error = pkmn_pokemon_set_ability(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_ability(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ability");

    /*
     * pkmn_pokemon_get_ball
     */

    error = pkmn_pokemon_get_ball(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_ball(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ball_out");

    /*
     * pkmn_pokemon_set_ball
     */

    error = pkmn_pokemon_set_ball(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_ball(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ball");

    /*
     * pkmn_pokemon_get_level_met
     */

    error = pkmn_pokemon_get_level_met(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_level_met(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "level_met_out");

    /*
     * pkmn_pokemon_set_level_met
     */

    error = pkmn_pokemon_set_level_met(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_location_met
     */

    error = pkmn_pokemon_get_location_met(
                NULL,
                false,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_location_met(
                &pokemon,
                false,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "location_met_out");

    /*
     * pkmn_pokemon_set_location_met
     */

    error = pkmn_pokemon_set_location_met(
                NULL,
                strbuffer,
                false
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_location_met(
                &pokemon,
                NULL,
                false
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "location_met");

    /*
     * pkmn_pokemon_get_original_game
     */

    error = pkmn_pokemon_get_original_game(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_original_game(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "original_game_out");

    /*
     * pkmn_pokemon_set_original_game
     */

    error = pkmn_pokemon_set_original_game(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_original_game(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "game");

    /*
     * pkmn_pokemon_get_personality
     */

    error = pkmn_pokemon_get_personality(
                NULL,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_personality(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "personality_out");

    /*
     * pkmn_pokemon_set_personality
     */

    error = pkmn_pokemon_set_personality(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_experience
     */

    error = pkmn_pokemon_get_experience(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_experience(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "experience_out");

    /*
     * pkmn_pokemon_set_experience
     */

    error = pkmn_pokemon_set_experience(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_level
     */

    error = pkmn_pokemon_get_level(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_level(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "level_out");

    /*
     * pkmn_pokemon_set_level
     */

    error = pkmn_pokemon_set_level(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_markings
     */

    error = pkmn_pokemon_get_markings(
                NULL,
                &dummy_bool,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_markings(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "has_markings_buffer_out");

    /*
     * pkmn_pokemon_set_has_marking
     */

    error = pkmn_pokemon_set_has_marking(
                NULL,
                PKMN_MARKING_CIRCLE,
                true
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_has_ribbon
     */

    error = pkmn_pokemon_has_ribbon(
                NULL,
                strbuffer,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_has_ribbon(
                &pokemon,
                NULL,
                &dummy_bool
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon_name");

    error = pkmn_pokemon_has_ribbon(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "has_ribbon_out");

    /*
     * pkmn_pokemon_set_has_ribbon
     */

    error = pkmn_pokemon_set_has_ribbon(
                NULL,
                strbuffer,
                true
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_has_ribbon(
                &pokemon,
                NULL,
                true
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon_name");

    /*
     * pkmn_pokemon_get_ribbon_names
     */

    error = pkmn_pokemon_get_ribbon_names(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_ribbon_names(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon_names_out");

    /*
     * pkmn_pokemon_get_contest_stats
     */

    error = pkmn_pokemon_get_contest_stats(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_contest_stats(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "contest_stats_buffer_out");

    /*
     * pkmn_pokemon_set_contest_stat
     */

    error = pkmn_pokemon_set_contest_stat(
                NULL,
                PKMN_CONTEST_STAT_COOL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_moves
     */

    error = pkmn_pokemon_get_moves(
                NULL,
                &dummy_move_slots
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_moves(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "move_slots_out");

    /*
     * pkmn_pokemon_set_move
     */

    error = pkmn_pokemon_set_move(
                NULL,
                0,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_move(
                &pokemon,
                0,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "move");

    /*
     * pkmn_pokemon_set_move_pp
     */

    error = pkmn_pokemon_set_move_pp(
                NULL,
                0,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_EVs
     */

    error = pkmn_pokemon_get_EVs(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_EVs(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "EVs_buffer_out");

    /*
     * pkmn_pokemon_set_EV
     */

    error = pkmn_pokemon_set_EV(
                NULL,
                PKMN_STAT_HP,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_IVs
     */

    error = pkmn_pokemon_get_IVs(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_IVs(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "IVs_buffer_out");

    /*
     * pkmn_pokemon_set_IV
     */

    error = pkmn_pokemon_set_IV(
                NULL,
                PKMN_STAT_HP,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_stats
     */

    error = pkmn_pokemon_get_stats(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_stats(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stats_buffer_out");

    /*
     * pkmn_pokemon_get_current_hp
     */

    error = pkmn_pokemon_get_current_hp(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_current_hp(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "current_hp_out");

    /*
     * pkmn_pokemon_set_current_hp
     */

    error = pkmn_pokemon_set_current_hp(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    /*
     * pkmn_pokemon_get_icon_filepath
     */

    error = pkmn_pokemon_get_icon_filepath(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_icon_filepath(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "icon_filepath_out");

    /*
     * pkmn_pokemon_get_sprite_filepath
     */

    error = pkmn_pokemon_get_sprite_filepath(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_sprite_filepath(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "sprite_filepath_out");

    /*
     * pkmn_pokemon_get_numeric_attribute
     */

    error = pkmn_pokemon_get_numeric_attribute(
                NULL,
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_numeric_attribute(
                &pokemon,
                NULL,
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "attribute_name");

    error = pkmn_pokemon_get_numeric_attribute(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "value_out");

    /*
     * pkmn_pokemon_set_numeric_attribute
     */

    error = pkmn_pokemon_set_numeric_attribute(
                NULL,
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_numeric_attribute(
                &pokemon,
                NULL,
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "attribute_name");

    /*
     * pkmn_pokemon_get_string_attribute
     */

    error = pkmn_pokemon_get_string_attribute(
                NULL,
                strbuffer,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_string_attribute(
                &pokemon,
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "attribute_name");

    error = pkmn_pokemon_get_string_attribute(
                &pokemon,
                strbuffer,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "value_out");

    /*
     * pkmn_pokemon_set_string_attribute
     */

    error = pkmn_pokemon_set_string_attribute(
                NULL,
                strbuffer,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_set_string_attribute(
                &pokemon,
                NULL,
                strbuffer
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "attribute_name");

    error = pkmn_pokemon_set_string_attribute(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "value");

    /*
     * pkmn_pokemon_get_attribute_names
     */

    error = pkmn_pokemon_get_attribute_names(
                NULL,
                &dummy_attribute_names
            );
    TEST_NULL_POINTER_RETURN("pokemon_ptr");

    error = pkmn_pokemon_get_attribute_names(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "attribute_names_out");

    // Cleanup
    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_box.h>
 */
static void pokemon_box_error_test()
{
    pkmn_pokemon_box_t pokemon_box =
    {
        .game = NULL,
        .capacity = 0,
        ._internal = NULL
    };

    error = pkmn_pokemon_box_init(
                "Red",
                &pokemon_box
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokemon_box_init
     */

    error = pkmn_pokemon_box_init(
                NULL,
                &pokemon_box
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokemon_box_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_out");

    /*
     * pkmn_pokemon_box_free
     */

    error = pkmn_pokemon_box_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    /*
     * pkmn_pokemon_box_strerror
     */

    const char* pokemon_box_strerror = pkmn_pokemon_box_strerror(NULL);
    TEST_ASSERT_NULL(pokemon_box_strerror);

    /*
     * pkmn_pokemon_box_get_name
     */

    error = pkmn_pokemon_box_get_name(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_get_name(
                &pokemon_box,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "name_buffer_out");

    /*
     * pkmn_pokemon_box_set_name
     */

    error = pkmn_pokemon_box_set_name(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_set_name(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "name");

    /*
     * pkmn_pokemon_box_get_num_pokemon
     */

    error = pkmn_pokemon_box_get_num_pokemon(
                NULL,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_get_num_pokemon(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "num_pokemon_out");

    /*
     * pkmn_pokemon_box_get_pokemon
     */

    error = pkmn_pokemon_box_get_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_get_pokemon(
                &pokemon_box,
                0,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_out");

    /*
     * pkmn_pokemon_box_set_pokemon
     */

    error = pkmn_pokemon_box_set_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_set_pokemon(
                &pokemon_box,
                0,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_ptr");

    /*
     * pkmn_pokemon_box_as_list
     */

    error = pkmn_pokemon_box_as_list(
                NULL,
                &dummy_pokemon_list
            );
    TEST_NULL_POINTER_RETURN("pokemon_box_ptr");

    error = pkmn_pokemon_box_as_list(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_list_out");

    // Cleanup
    error = pkmn_pokemon_box_free(&pokemon_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_party.h>
 */
static void pokemon_party_error_test()
{
    pkmn_pokemon_party_t pokemon_party =
    {
        .game = NULL,
        .capacity = 0,
        ._internal = NULL
    };

    error = pkmn_pokemon_party_init(
                "Red",
                &pokemon_party
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokemon_party_init
     */

    error = pkmn_pokemon_party_init(
                NULL,
                &pokemon_party
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokemon_party_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_party_out");

    /*
     * pkmn_pokemon_party_free
     */

    error = pkmn_pokemon_party_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_party_ptr");

    /*
     * pkmn_pokemon_party_strerror
     */

    const char* pokemon_party_strerror = pkmn_pokemon_party_strerror(NULL);
    TEST_ASSERT_NULL(pokemon_party_strerror);

    /*
     * pkmn_pokemon_party_get_num_pokemon
     */

    error = pkmn_pokemon_party_get_num_pokemon(
                NULL,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("pokemon_party_ptr");

    error = pkmn_pokemon_party_get_num_pokemon(
                &pokemon_party,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "num_pokemon_out");

    /*
     * pkmn_pokemon_party_get_pokemon
     */

    error = pkmn_pokemon_party_get_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("pokemon_party_ptr");

    error = pkmn_pokemon_party_get_pokemon(
                &pokemon_party,
                0,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_out");

    /*
     * pkmn_pokemon_party_set_pokemon
     */

    error = pkmn_pokemon_party_set_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("pokemon_party_ptr");

    error = pkmn_pokemon_party_set_pokemon(
                &pokemon_party,
                0,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_ptr");

    /*
     * pkmn_pokemon_party_as_list
     */

    error = pkmn_pokemon_party_as_list(
                NULL,
                &dummy_pokemon_list
            );
    TEST_NULL_POINTER_RETURN("pokemon_party_ptr");

    error = pkmn_pokemon_party_as_list(
                &pokemon_party,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_list_out");

    // Cleanup
    error = pkmn_pokemon_party_free(&pokemon_party);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_pc.h>
 */
static void pokemon_pc_error_test()
{
    pkmn_pokemon_pc_t pokemon_pc =
    {
        .game = NULL,
        .capacity = 0,
        ._internal = NULL
    };
    pkmn_pokemon_box_t dummy_pokemon_box = {NULL, 0, NULL};
    pkmn_pokemon_box_list_t dummy_pokemon_box_list = {NULL, 0};

    error = pkmn_pokemon_pc_init(
                "Red",
                &pokemon_pc
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokemon_pc_init
     */

    error = pkmn_pokemon_pc_init(
                NULL,
                &pokemon_pc
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokemon_pc_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_pc_out");

    /*
     * pkmn_pokemon_pc_free
     */

    error = pkmn_pokemon_pc_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_pc_ptr");

    /*
     * pkmn_pokemon_pc_strerror
     */

    const char* pokemon_pc_strerror = pkmn_pokemon_pc_strerror(NULL);
    TEST_ASSERT_NULL(pokemon_pc_strerror);

    /*
     * pkmn_pokemon_pc_get_box
     */

    error = pkmn_pokemon_pc_get_box(
                NULL,
                0,
                &dummy_pokemon_box
            );
    TEST_NULL_POINTER_RETURN("pokemon_pc_ptr");

    error = pkmn_pokemon_pc_get_box(
                &pokemon_pc,
                0,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "pokemon_box_out");

    /*
     * pkmn_pokemon_pc_get_box_names
     */

    error = pkmn_pokemon_pc_get_box_names(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("pokemon_pc_ptr");

    error = pkmn_pokemon_pc_get_box_names(
                &pokemon_pc,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "box_names_out");

    /*
     * pkmn_pokemon_pc_as_list
     */

    error = pkmn_pokemon_pc_as_list(
                NULL,
                &dummy_pokemon_box_list
            );
    TEST_NULL_POINTER_RETURN("pokemon_pc_ptr");

    error = pkmn_pokemon_pc_as_list(
                &pokemon_pc,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "pokemon_box_list_out");

    // Cleanup
    error = pkmn_pokemon_pc_free(&pokemon_pc);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/calculations/form.h>
 */
static void calculations_form_error_test() {
    /*
     * pkmn_calculations_gen2_unown_form
     */

    error = pkmn_calculations_gen2_unown_form(
                0,
                0,
                0,
                0,
                NULL, // form_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("form_out");

    /*
     * pkmn_calculations_gen3_unown_form
     */

    error = pkmn_calculations_gen3_unown_form(
                0,
                NULL, // form_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("form_out");

    /*
     * pkmn_calculations_wurmple_becomes_silcoon
     */

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0,
                false,
                NULL // evolves_out
            );
    TEST_NULL_POINTER_RETURN("evolves_out");
}

/*
 * <pkmn-c/calculations/gender.h>
 */
static void calculations_gender_error_test() {
    pkmn_gender_t dummy_pkmn_gender_t = PKMN_GENDER_MALE;

    /*
     * pkmn_calculations_gen2_pokemon_gender
     */

    error = pkmn_calculations_gen2_pokemon_gender(
                NULL, // species
                0,
                &dummy_pkmn_gender_t
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_calculations_gen2_pokemon_gender(
                strbuffer,
                0,
                NULL // gender_out
            );
    TEST_NULL_POINTER_RETURN("gender_out");

    /*
     * pkmn_calculations_modern_pokemon_gender
     */

    error = pkmn_calculations_modern_pokemon_gender(
                NULL, // species
                0,
                &dummy_pkmn_gender_t
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_calculations_modern_pokemon_gender(
                strbuffer,
                0,
                NULL // gender_out
            );
    TEST_NULL_POINTER_RETURN("gender_out");
}

/*
 * <pkmn-c/calculations/nature.h>
 */
static void calculations_nature_error_test() {
    /*
     * pkmn_calculations__nature
     */
    error = pkmn_calculations_nature(
                0,
                NULL, // nature_out
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("nature_out");
}

/*
 * <pkmn-c/calculations/personality.h>
 */
static void calculations_personality_error_test() {
    /*
     * pkmn_calculations_generate_personality
     */

    error = pkmn_calculations_generate_personality(
                NULL, // species
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                NULL, // ability
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("ability");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                NULL, // nature
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("nature");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                NULL // personality_out
            );
    TEST_NULL_POINTER_RETURN("personality_out");
}

/*
 * <pkmn-c/calculations/shininess.h>
 */
static void calculations_shininess_error_test() {
    /*
     * pkmn_calculations_gen2_shiny
     */

    error = pkmn_calculations_gen2_shiny(
                0,
                0,
                0,
                0,
                NULL // shiny_out
            );
    TEST_NULL_POINTER_RETURN("shiny_out");

    /*
     * pkmn_calculations_modern_shiny
     */

    error = pkmn_calculations_modern_shiny(
                0,
                0,
                NULL // shiny_out
            );
    TEST_NULL_POINTER_RETURN("shiny_out");
}

/*
 * <pkmn-c/calculations/size.h>
 */
static void calculations_size_error_test() {
    /*
     * pkmn_calculations_pokemon_size
     */

    error = pkmn_calculations_pokemon_size(
                NULL, // species
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_calculations_pokemon_size(
                strbuffer,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                NULL // size_out
            );
    TEST_NULL_POINTER_RETURN("size_out");
}

/*
 * <pkmn-c/calculations/spinda_spots.h>
 */
static void calculations_spinda_spots_error_test()
{
    pkmn_spinda_coords_t dummy_coords;
    pkmn_spinda_spots_t dummy_spots;

    /*
     * pkmn_calculations_add_spinda_coords
     */

    error = pkmn_calculations_add_spinda_coords(
                NULL,
                &dummy_coords,
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("coords1_ptr");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                NULL,
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("coords2_ptr");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                &dummy_coords,
                NULL
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_add_spinda_spots
     */

    error = pkmn_calculations_add_spinda_spots(
                NULL,
                &dummy_spots,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots1_ptr");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                NULL,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots2_ptr");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                &dummy_spots,
                NULL
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_add_spinda_coords_to_spots
     */

    error = pkmn_calculations_add_spinda_coords_to_spots(
                NULL,
                &dummy_coords,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots_ptr");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                NULL,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("coords_ptr");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                &dummy_coords,
                NULL
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_spinda_spot_offset
     */

    error = pkmn_calculations_spinda_spot_offset(
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("spot_offset_out");
}

/*
 * <pkmn-c/calculations/stats.h>
 */
static void calculations_stats_error_test() {
    /*
     * pkmn_calculations_gb_stat
     */

    error = pkmn_calculations_gb_stat(
                PKMN_STAT_HP,
                0,
                0,
                0,
                0,
                NULL // stat_out
            );
    TEST_NULL_POINTER_RETURN("stat_out");

    /*
     * pkmn_calculations_gb_stat_range
     */

    error = pkmn_calculations_gb_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL // stat_range_out
            );
    TEST_NULL_POINTER_RETURN("stat_range_out");

    /*
     * pkmn_calculations_modern_stat
     */

    error = pkmn_calculations_modern_stat(
                PKMN_STAT_HP,
                0,
                0.0f,
                0,
                0,
                0,
                NULL // stat_out
            );
    TEST_NULL_POINTER_RETURN("stat_out");

    /*
     * pkmn_calculations_modern_stat_range
     */

    error = pkmn_calculations_modern_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL // stat_range_out
            );
    TEST_NULL_POINTER_RETURN("stat_range_out");
}

/*
 * <pkmn-c/calculations/moves/critical_hit.h>
 */

static void calculations_moves_critical_hit_error_test()
{
    /*
     * pkmn_calculations_gen1_critical_hit_chance
     */

    error = pkmn_calculations_gen1_critical_hit_chance(
                1,
                false,
                false,
                NULL // critical_hit_chance_out
            );
    TEST_NULL_POINTER_RETURN("critical_hit_chance_out");

    /*
     * pkmn_calculations_critical_hit_chance
     */

    error = pkmn_calculations_critical_hit_chance(
                2,
                1,
                NULL // critical_hit_chance_out
            );
    TEST_NULL_POINTER_RETURN("critical_hit_chance_out");

    /*
     * pkmn_calculations_gen1_critical_hit_modifier
     */

    error = pkmn_calculations_gen1_critical_hit_modifier(
                1,
                NULL // critical_hit_modifier_out
            );
    TEST_NULL_POINTER_RETURN("critical_hit_modifier_out");

    /*
     * pkmn_calculations_critical_hit_modifier
     */

    error = pkmn_calculations_critical_hit_modifier(
                2,
                NULL // critical_hit_modifier_out
            );
    TEST_NULL_POINTER_RETURN("critical_hit_modifier_out");
}

/*
 * <pkmn-c/calculations/moves/damage.h>
 */
static void calculations_moves_damage_error_test()
{
    /*
     * pkmn_calculations_damage
     */

    error = pkmn_calculations_damage(
                1,
                1,
                1,
                1,
                1.0f,
                NULL // damage_out
            );
    TEST_NULL_POINTER_RETURN("damage_out");
}

/*
 * <pkmn-c/calculations/moves/hidden_power.h>
 */
static void calculations_moves_hidden_power_error_test() {
    /*
     * pkmn_calculations_gen2_hidden_power
     */

    error = pkmn_calculations_gen2_hidden_power(
                0,
                0,
                0,
                0,
                NULL // hidden_power_out
            );
    TEST_NULL_POINTER_RETURN("hidden_power_out");

    /*
     * pkmn_calculations_modern_hidden_power
     */

    error = pkmn_calculations_modern_hidden_power(
                0,
                0,
                0,
                0,
                0,
                0,
                NULL // hidden_power_out
            );
    TEST_NULL_POINTER_RETURN("hidden_power_out");
}

/*
 * <pkmn-c/calculations/moves/modifiers.h>
 */
static void calculations_moves_modifiers_error_test()
{
    /*
     * pkmn_calculations_type_damage_modifier
     */

    error = pkmn_calculations_type_damage_modifier(
                1,
                NULL, // attacking_type
                "",
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("attacking_type");

    error = pkmn_calculations_type_damage_modifier(
                1,
                "",
                NULL, // defending_type
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("defending_type");

    error = pkmn_calculations_type_damage_modifier(
                1,
                "",
                "",
                NULL // type_damage_modifier_out
            );
    TEST_NULL_POINTER_RETURN("type_damage_modifier_out");
}

/*
 * <pkmn-c/calculations/moves/natural_gift.h>
 */
static void calculations_moves_natural_gift_error_test()
{
    pkmn_natural_gift_t dummy_pkmn_natural_gift_t = { NULL, 0 };

    /*
     * pkmn_calculations_natural_gift_stats
     */

    error = pkmn_calculations_natural_gift_stats(
                NULL, // item_name
                5,
                &dummy_pkmn_natural_gift_t
            );
    TEST_NULL_POINTER_RETURN("item_name");

    error = pkmn_calculations_natural_gift_stats(
                "Oran Berry",
                5,
                NULL // natural_gift_stats_out
            );
    TEST_NULL_POINTER_RETURN("natural_gift_stats_out");
}

/*
 * <pkmn-c/calculations/moves/power.h>
 */
static void calculations_moves_power_error_test()
{
    /*
     * pkmn_calculations_brine_power
     */

    error = pkmn_calculations_brine_power(
                1,
                1,
                NULL // brine_power_out
            );
    TEST_NULL_POINTER_RETURN("brine_power_out");

    /*
     * pkmn_calculations_crush_grip_power
     */

    error = pkmn_calculations_crush_grip_power(
                1,
                1,
                5,
                NULL // crush_grip_power_out
            );
    TEST_NULL_POINTER_RETURN("crush_grip_power_out");

    /*
     * pkmn_calculations_echoed_voice_powers
     */

    error = pkmn_calculations_echoed_voice_powers(
                NULL, // powers_buffer
                0ULL,
                NULL // num_powers_out (can be NULL)
            );
    TEST_NULL_POINTER_RETURN("powers_buffer");

    /*
     * pkmn_calculations_electro_ball_power
     */

    error = pkmn_calculations_electro_ball_power(
                1,
                1,
                NULL // electro_ball_power_out
            );
    TEST_NULL_POINTER_RETURN("electro_ball_power_out");

    /*
     * pkmn_calculations_eruption_power
     */

    error = pkmn_calculations_eruption_power(
                1,
                1,
                NULL // eruption_power_out
            );
    TEST_NULL_POINTER_RETURN("eruption_power_out");

    /*
     * pkmn_calculations_flail_power
     */

    error = pkmn_calculations_flail_power(
                1,
                1,
                NULL // flail_power_out
            );
    TEST_NULL_POINTER_RETURN("flail_power_out");

    /*
     * pkmn_calculations_fling_power
     */

    error = pkmn_calculations_fling_power(
                NULL, // held_item
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("held_item");

    error = pkmn_calculations_fling_power(
                "",
                NULL // fling_power_out
            );
    TEST_NULL_POINTER_RETURN("fling_power_out");

    /*
     * pkmn_calculations_frustration_power
     */

    error = pkmn_calculations_frustration_power(
                1,
                NULL // frustration_power_out
            );
    TEST_NULL_POINTER_RETURN("frustration_power_out");

    /*
     * pkmn_calculations_fury_cutter_powers
     */

    error = pkmn_calculations_fury_cutter_powers(
                2,
                NULL, // powers_buffer
                0ULL,
                NULL // num_powers_out (can be NULL)
            );
    TEST_NULL_POINTER_RETURN("powers_buffer");

    /*
     * pkmn_calculations_grass_knot_power
     */

    error = pkmn_calculations_grass_knot_power(
                1.0f,
                NULL // grass_knot_power_out
            );
    TEST_NULL_POINTER_RETURN("grass_knot_power_out");

    /*
     * pkmn_calculations_gyro_ball_power
     */

    error = pkmn_calculations_gyro_ball_power(
                1,
                1,
                NULL // gyro_ball_power_out
            );
    TEST_NULL_POINTER_RETURN("gyro_ball_power_out");

    /*
     * pkmn_calculations_heat_crash_power
     */

    error = pkmn_calculations_heat_crash_power(
                1.0f,
                1.0f,
                NULL // heat_crash_power_out
            );
    TEST_NULL_POINTER_RETURN("heat_crash_power_out");

    /*
     * pkmn_calculations_heavy_slam_power
     */

    error = pkmn_calculations_heavy_slam_power(
                1.0f,
                1.0f,
                NULL // heavy_slam_power_out
            );
    TEST_NULL_POINTER_RETURN("heavy_slam_power_out");

    /*
     * pkmn_calculations_ice_ball_powers
     */

    error = pkmn_calculations_ice_ball_powers(
                NULL, // powers_buffer
                0ULL,
                NULL // num_powers_out (can be NULL)
            );
    TEST_NULL_POINTER_RETURN("powers_buffer");

    /*
     * pkmn_calculations_low_kick_power
     */

    error = pkmn_calculations_low_kick_power(
                1.0f,
                1,
                NULL // low_kick_power_out
            );
    TEST_NULL_POINTER_RETURN("low_kick_power_out");

    /*
     * pkmn_calculations_power_trip_power
     */

    error = pkmn_calculations_power_trip_power(
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                NULL // power_trip_power_out
            );
    TEST_NULL_POINTER_RETURN("power_trip_power_out");

    /*
     * pkmn_calculations_punishment_power
     */

    error = pkmn_calculations_punishment_power(
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                NULL // punishment_power_out
            );
    TEST_NULL_POINTER_RETURN("punishment_power_out");

    /*
     * pkmn_calculations_return_power
     */

    error = pkmn_calculations_return_power(
                1,
                NULL // return_power_out
            );
    TEST_NULL_POINTER_RETURN("return_power_out");

    /*
     * pkmn_calculations_reversal_power
     */

    error = pkmn_calculations_reversal_power(
                1,
                1,
                NULL // reversal_power_out
            );
    TEST_NULL_POINTER_RETURN("reversal_power_out");

    /*
     * pkmn_calculations_rollout_powers
     */

    error = pkmn_calculations_rollout_powers(
                NULL, // powers_buffer
                0ULL,
                NULL // num_powers_out (can be NULL)
            );
    TEST_NULL_POINTER_RETURN("powers_buffer");

    /*
     * pkmn_calculations_spit_up_power
     */

    error = pkmn_calculations_spit_up_power(
                1,
                NULL // spit_up_power_out
            );
    TEST_NULL_POINTER_RETURN("spit_up_power_out");

    /*
     * pkmn_calculations_stored_power_power
     */

    error = pkmn_calculations_stored_power_power(
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                NULL // stored_power_power_out
            );
    TEST_NULL_POINTER_RETURN("stored_power_power_out");

    /*
     * pkmn_calculations_triple_kick_powers
     */

    error = pkmn_calculations_triple_kick_powers(
                NULL, // powers_buffer
                0ULL,
                NULL // num_powers_out (can be NULL)
            );
    TEST_NULL_POINTER_RETURN("powers_buffer");

    /*
     * pkmn_calculations_trump_card_power
     */

    error = pkmn_calculations_trump_card_power(
                1,
                NULL // trump_card_power_out
            );
    TEST_NULL_POINTER_RETURN("trump_card_power_out");

    /*
     * pkmn_calculations_water_spout_power
     */

    error = pkmn_calculations_water_spout_power(
                1,
                1,
                NULL // water_spout_power_out
            );
    TEST_NULL_POINTER_RETURN("water_spout_power_out");

    /*
     * pkmn_calculations_wring_out_power
     */

    error = pkmn_calculations_wring_out_power(
                5,
                5,
                5,
                NULL // wring_out_power_out
            );
    TEST_NULL_POINTER_RETURN("wring_out_power_out");
}

/*
 * <pkmn-c/database/item_entry.h>
 */
static void database_item_entry_error_test() {
    pkmn_database_item_entry_t dummy_pkmn_database_item_entry_t;

    /*
     * pkmn_database_get_item_entry
     */

    error = pkmn_database_get_item_entry(
                NULL, // item_name
                strbuffer,
                &dummy_pkmn_database_item_entry_t
            );
    TEST_NULL_POINTER_RETURN("item_name");

    error = pkmn_database_get_item_entry(
                strbuffer,
                NULL, // item_game
                &dummy_pkmn_database_item_entry_t
            );
    TEST_NULL_POINTER_RETURN("item_game");

    error = pkmn_database_get_item_entry(
                strbuffer,
                strbuffer,
                NULL // item_entry_out
            );
    TEST_NULL_POINTER_RETURN("item_entry_out");

    /*
     * pkmn_database_item_entry_free
     */

    error = pkmn_database_item_entry_free(
                NULL // item_entry
            );
    TEST_NULL_POINTER_RETURN("item_entry");
}

/*
 * <pkmn-c/database/lists.h>
 */
static void database_lists_error_test() {
    /*
     * pkmn_database_ability_list
     */

    error = pkmn_database_ability_list(
                1,
                NULL // ability_list_out
            );
    TEST_NULL_POINTER_RETURN("ability_list_out");

    /*
     * pkmn_database_game_list
     */

    error = pkmn_database_game_list(
                1,
                false,
                NULL // game_list_out
            );
    TEST_NULL_POINTER_RETURN("game_list_out");

    /*
     * pkmn_database_gamecube_shadow_pokemon_list
     */

    error = pkmn_database_gamecube_shadow_pokemon_list(
                false,
                NULL // gamecube_shadow_pokemon_list_out
            );
    TEST_NULL_POINTER_RETURN("gamecube_shadow_pokemon_list_out");

    /*
     * pkmn_database_item_list
     */

    error = pkmn_database_item_list(
                NULL, // game
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_database_item_list(
                strbuffer,
                NULL // item_list_out
            );
    TEST_NULL_POINTER_RETURN("item_list_out");

    /*
     * pkmn_database_location_list
     */

    error = pkmn_database_location_list(
                NULL, // game
                false,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_database_location_list(
                strbuffer,
                false,
                NULL // location_list_out
            );
    TEST_NULL_POINTER_RETURN("location_list_out");

    /*
     * pkmn_database_move_list
     */

    error = pkmn_database_move_list(
                NULL, // game
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_database_move_list(
                strbuffer,
                NULL // move_list_out
            );
    TEST_NULL_POINTER_RETURN("move_list_out");

    /*
     * pkmn_database_nature_list
     */

    error = pkmn_database_nature_list(
                NULL // nature_list_out
            );
    TEST_NULL_POINTER_RETURN("nature_list_out");

    /*
     * pkmn_database_pokemon_list
     */

    error = pkmn_database_pokemon_list(
                1,
                false,
                NULL // pokemon_list_out
            );
    TEST_NULL_POINTER_RETURN("pokemon_list_out");

    /*
     * pkmn_database_region_list
     */

    error = pkmn_database_region_list(
                NULL // region_list_out
            );
    TEST_NULL_POINTER_RETURN("region_list_out");

    /*
     * pkmn_database_ribbon_list
     */

    error = pkmn_database_ribbon_list(
                1,
                NULL // ribbon_list_out
            );
    TEST_NULL_POINTER_RETURN("ribbon_list_out");

    /*
     * pkmn_database_super_training_medal_list
     */

    error = pkmn_database_super_training_medal_list(
                NULL // super_training_medal_list_out
            );
    TEST_NULL_POINTER_RETURN("super_training_medal_list_out");

    /*
     * pkmn_database_type_list
     */

    error = pkmn_database_type_list(
                NULL, // game
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_database_type_list(
                strbuffer,
                NULL // type_list_out
            );
    TEST_NULL_POINTER_RETURN("type_list_out");
}

/*
 * <pkmn-c/database/move_entry.h>
 */
static void database_move_entry_error_test() {
    pkmn_database_move_entry_t dummy_pkmn_database_move_entry_t;

    /*
     * pkmn_database_get_move_entry
     */

    error = pkmn_database_get_move_entry(
                NULL, // move_name
                strbuffer,
                &dummy_pkmn_database_move_entry_t
            );
    TEST_NULL_POINTER_RETURN("move_name");

    error = pkmn_database_get_move_entry(
                strbuffer,
                NULL, // move_game
                &dummy_pkmn_database_move_entry_t
            );
    TEST_NULL_POINTER_RETURN("move_game");

    error = pkmn_database_get_move_entry(
                strbuffer,
                strbuffer,
                NULL // move_entry_out
            );
    TEST_NULL_POINTER_RETURN("move_entry_out");

    /*
     * pkmn_database_move_entry_free
     */

    error = pkmn_database_move_entry_free(
                NULL // move_entry
            );
    TEST_NULL_POINTER_RETURN("move_entry");
}

/*
 * <pkmn-c/database/pokemon_entry.h>
 */
static void database_pokemon_entry_error_test() {
    pkmn_database_pokemon_entry_t entry;
    error = pkmn_database_get_pokemon_entry(
                "Charmander",
                "Diamond",
                "",
                &entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_database_get_pokemon_entry
     */

    error = pkmn_database_get_pokemon_entry(
                NULL, // species
                strbuffer,
                strbuffer,
                &entry
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                NULL, // game
                strbuffer,
                &entry
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                NULL, // form
                &entry
            );
    TEST_NULL_POINTER_RETURN("form");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                strbuffer,
                NULL // pokemon_entry_out
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry_out");

    /*
     * pkmn_database_pokemon_entry_set_form
     */

    error = pkmn_database_pokemon_entry_set_form(
                NULL, // pokemon_entry
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_set_form(
                &entry,
                NULL // form
            );
    TEST_NULL_POINTER_RETURN("form");

    /*
     * pkmn_database_pokemon_entry_experience_at_level
     */

    error = pkmn_database_pokemon_entry_experience_at_level(
                NULL, // pokemon_entry
                1,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_experience_at_level(
                &entry,
                1,
                NULL // experience_out
            );
    TEST_NULL_POINTER_RETURN("experience_out");

    /*
     * pkmn_database_pokemon_entry_level_at_experience
     */

    error = pkmn_database_pokemon_entry_level_at_experience(
                NULL, // pokemon_entry
                1,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_level_at_experience(
                &entry,
                1,
                NULL // level_out
            );
    TEST_NULL_POINTER_RETURN("level_out");

    /*
     * pkmn_database_pokemon_entry_icon_filepath
     */

    error = pkmn_database_pokemon_entry_icon_filepath(
                NULL, // pokemon_entry
                false,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_icon_filepath(
                &entry,
                false,
                NULL, // icon_filepath_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("icon_filepath_out");

    /*
     * pkmn_database_pokemon_entry_sprite_filepath
     */

    error = pkmn_database_pokemon_entry_sprite_filepath(
                NULL, // pokemon_entry
                false,
                false,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_sprite_filepath(
                &entry,
                false,
                false,
                NULL, // sprite_filepath_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("sprite_filepath_out");

    /*
     * pkmn_database_pokemon_entry_free
     */

    error = pkmn_database_pokemon_entry_free(
                NULL // pokemon_entry
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_free(
                &entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/gui/spinda.h>
 */
static void gui_spinda_test()
{
    /*
     * pkmn_gui_generate_spinda_sprite_at_filepath
     */
    error = pkmn_gui_generate_spinda_sprite_at_filepath(
                0,
                0,
                false,
                NULL // filepath
            );
    TEST_NULL_POINTER_RETURN("filepath");
}

/*
 * <pkmn-c/types/hidden_power.h>
 */
static void types_hidden_power_test()
{
    /*
     * pkmn_hidden_power_free
     */
    error = pkmn_hidden_power_free(NULL);
    TEST_NULL_POINTER_RETURN("hidden_power_ptr");
}

/*
 * <pkmn-c/types/item_slot.h>
 */
static void types_item_slot_test()
{
    /*
     * pkmn_item_slot_free
     */
    error = pkmn_item_slot_free(NULL);
    TEST_NULL_POINTER_RETURN("item_slot_ptr");

    /*
     * pkmn_item_slots_free
     */
    error = pkmn_item_slots_free(NULL);
    TEST_NULL_POINTER_RETURN("item_slots_ptr");
}

/*
 * <pkmn-c/types/levelup_move.h>
 */
static void types_levelup_move_test()
{
    /*
     * pkmn_levelup_move_free
     */
    error = pkmn_levelup_move_free(NULL);
    TEST_NULL_POINTER_RETURN("levelup_move_ptr");

    /*
     * pkmn_levelup_moves_free
     */
    error = pkmn_levelup_moves_free(NULL);
    TEST_NULL_POINTER_RETURN("levelup_moves_ptr");
}

/*
 * <pkmn-c/types/move_slot.h>
 */
static void types_move_slot_test()
{
    /*
     * pkmn_move_slot_free
     */
    error = pkmn_move_slot_free(NULL);
    TEST_NULL_POINTER_RETURN("move_slot_ptr");

    /*
     * pkmn_move_slots_free
     */
    error = pkmn_move_slots_free(NULL);
    TEST_NULL_POINTER_RETURN("move_slots_ptr");
}

/*
 * <pkmn-c/types/natural_gift.h>
 */
static void types_natural_gift_test()
{
    /*
     * pkmn_natural_gift_free
     */
    error = pkmn_natural_gift_free(NULL);
    TEST_NULL_POINTER_RETURN("natural_gift_ptr");
}

/*
 * <pkmn-c/types/pokemon_box_list.h>
 */
static void types_pokemon_box_list_test()
{
    /*
     * pkmn_pokemon_box_list_free
     */
    error = pkmn_pokemon_box_list_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_box_list_ptr");
}

/*
 * <pkmn-c/types/pokemon_list.h>
 */
static void types_pokemon_list_test()
{
    /*
     * pkmn_pokemon_list_free
     */
    error = pkmn_pokemon_list_free(NULL);
    TEST_NULL_POINTER_RETURN("pokemon_list_ptr");
}

/*
 * <pkmn-c/types/string_types.h>
 */
static void types_string_types_test()
{
    /*
     * pkmn_string_list_free
     */
    error = pkmn_string_list_free(NULL);
    TEST_NULL_POINTER_RETURN("string_list_ptr");

    /*
     * pkmn_string_pair_free
     */
    error = pkmn_string_pair_free(NULL);
    TEST_NULL_POINTER_RETURN("string_pair_ptr");
}

/*
 * <pkmn-c/types/trainer_info.h>
 */
static void types_trainer_info_test()
{
    /*
     * pkmn_trainer_info_free
     */
    error = pkmn_trainer_info_free(NULL);
    TEST_NULL_POINTER_RETURN("trainer_info_ptr");
}

/*
 * <pkmn-c/utils/paths.h>
 */
static void utils_paths_error_test() {
    /*
     * pkmn_get_appdata_dir
     */

    error = pkmn_get_appdata_dir(
                NULL, // appdata_dir_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("appdata_dir_out");

    /*
     * pkmn_get_database_path
     */

    error = pkmn_get_database_path(
                NULL, // database_path_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("database_path_out");

    /*
     * pkmn_get_images_dir
     */

    error = pkmn_get_images_dir(
                NULL, // images_dir_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("images_dir_out");

    /*
     * pkmn_get_tmp_dir
     */

    error = pkmn_get_tmp_dir(
                NULL, // tmp_dir_out
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("tmp_dir_out");
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(test_pkmn_strerror_as_first_call)
    PKMN_C_TEST(game_save_error_test)
    PKMN_C_TEST(item_bag_error_test)
    PKMN_C_TEST(item_list_error_test)
    PKMN_C_TEST(pokedex_error_test)
    PKMN_C_TEST(pokemon_error_test)
    PKMN_C_TEST(pokemon_box_error_test)
    PKMN_C_TEST(pokemon_party_error_test)
    PKMN_C_TEST(pokemon_pc_error_test)
    PKMN_C_TEST(calculations_form_error_test)
    PKMN_C_TEST(calculations_gender_error_test)
    PKMN_C_TEST(calculations_nature_error_test)
    PKMN_C_TEST(calculations_personality_error_test)
    PKMN_C_TEST(calculations_shininess_error_test)
    PKMN_C_TEST(calculations_size_error_test)
    PKMN_C_TEST(calculations_spinda_spots_error_test)
    PKMN_C_TEST(calculations_stats_error_test)
    PKMN_C_TEST(calculations_moves_critical_hit_error_test)
    PKMN_C_TEST(calculations_moves_damage_error_test)
    PKMN_C_TEST(calculations_moves_hidden_power_error_test)
    PKMN_C_TEST(calculations_moves_modifiers_error_test)
    PKMN_C_TEST(calculations_moves_natural_gift_error_test)
    PKMN_C_TEST(calculations_moves_power_error_test)
    PKMN_C_TEST(database_item_entry_error_test)
    PKMN_C_TEST(database_lists_error_test)
    PKMN_C_TEST(database_move_entry_error_test)
    PKMN_C_TEST(database_pokemon_entry_error_test)
    PKMN_C_TEST(gui_spinda_test)
    PKMN_C_TEST(types_hidden_power_test)
    PKMN_C_TEST(types_item_slot_test)
    PKMN_C_TEST(types_levelup_move_test)
    PKMN_C_TEST(types_move_slot_test)
    PKMN_C_TEST(types_natural_gift_test)
    PKMN_C_TEST(types_pokemon_box_list_test)
    PKMN_C_TEST(types_pokemon_list_test)
    PKMN_C_TEST(types_trainer_info_test)
    PKMN_C_TEST(types_string_types_test)
    PKMN_C_TEST(utils_paths_error_test)
)
