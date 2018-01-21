/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

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
static bool dummy_bool = 0;
static int dummy_int = 0;
static size_t dummy_size_t = 0;
static uint32_t dummy_uint32_t = 0;
static float dummy_float = 0;
static pkmn_gender_t dummy_pkmn_gender_t = PKMN_MALE;
static pkmn_string_list_t dummy_pkmn_string_list_t = { NULL, 0 };
static pkmn_pokemon_handle_t dummy_pokemon = NULL;

static const char* null_pointer_error_format = "Null pointer passed into parameter \"%s\"";

static void populate_pksav_saves() {
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

#define TEST_GAME_SAVE_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, STRBUFFER_LEN, null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_game_save_strerror(handle)); \
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

#define TEST_POKEMON_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_strerror(handle)); \
}

#define TEST_POKEMON_BOX_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_box_strerror(handle)); \
}

#define TEST_POKEMON_PARTY_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_party_strerror(handle)); \
}

#define TEST_POKEMON_PC_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(PKMN_ERROR_NULL_POINTER, error); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_strerror()); \
    TEST_ASSERT_EQUAL_STRING(strbuffer, pkmn_pokemon_pc_strerror(handle)); \
}

/*
 * <pkmn-c/game_save.h>
 */
static void game_save_error_test() {

    char save_filepath[STRBUFFER_LEN] = {0};
    snprintf(
        save_filepath,
        sizeof(save_filepath),
        "%s%sred_blue%spokemon_red.sav",
        PKSAV_TEST_SAVES, FS_SEPARATOR, FS_SEPARATOR
    );
    pkmn_game_save_handle_t game_save = NULL;
    error = pkmn_game_save_from_file(
                &game_save,
                save_filepath
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_game_save_handle_t null_game_save = NULL;

    pkmn_item_bag_t item_bag;
    pkmn_item_list_t item_pc;
    pkmn_pokemon_party_handle_t pokemon_party = NULL;
    pkmn_pokemon_pc_handle_t pokemon_pc = NULL;

    pkmn_game_save_type_t dummy_pkmn_game_save_type_t = PKMN_GAME_SAVE_TYPE_NONE;

    pkmn_trainer_info_t trainer_info;

    /*
     * pkmn_game_save_detect_type
     */

    error = pkmn_game_save_detect_type(
                NULL, // filepath
                &dummy_pkmn_game_save_type_t
            );
    TEST_NULL_POINTER_RETURN("filepath");

    error = pkmn_game_save_detect_type(
                strbuffer,
                NULL // game_save_type_out
            );
    TEST_NULL_POINTER_RETURN("game_save_type_out");

    /*
     * pkmn_game_save_from_file
     */

    error = pkmn_game_save_from_file(
                NULL, // handle_ptr
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_game_save_from_file(
                &null_game_save,
                NULL // filepath
            );
    TEST_NULL_POINTER_RETURN("filepath");

    /*
     * pkmn_game_save_free
     */

    error = pkmn_game_save_free(
                NULL // handle_ptr
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_game_save_free(
                &null_game_save
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_game_save_strerror
     */

    const char* game_save_strerror = pkmn_game_save_strerror(
                                         NULL // handle
                                     );
    TEST_ASSERT_NULL(game_save_strerror);

    /*
     * pkmn_game_save_get_filepath
     */

    error = pkmn_game_save_get_filepath(
                NULL, // handle
                strbuffer,
                STRBUFFER_LEN,
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_filepath(
                game_save,
                NULL, // filepath_out
                STRBUFFER_LEN,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "filepath_out");

    /*
     * pkmn_game_save_save
     */

    error = pkmn_game_save_save(
                NULL // handle
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_game_save_save_as
     */

    error = pkmn_game_save_save_as(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_save_as(
                game_save,
                NULL // filepath
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "filepath");

    /*
     * pkmn_game_save_get_game
     */

    error = pkmn_game_save_get_game(
                NULL, // handle
                strbuffer,
                STRBUFFER_LEN,
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_game(
                game_save,
                NULL, // game_out
                STRBUFFER_LEN,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "game_out");

    /*
     * pkmn_game_save_get_original_trainer_info
     */

    error = pkmn_game_save_get_trainer_info(
                NULL, // handle
                &trainer_info
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_trainer_info(
                game_save,
                NULL // trainer_info_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "trainer_info_out");

    /*
     * pkmn_game_save_set_original_trainer_info
     */

    error = pkmn_game_save_set_trainer_info(
                NULL, // handle
                &trainer_info
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_set_trainer_info(
                game_save,
                NULL // trainer_info
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "trainer_info");

    /*
     * pkmn_game_save_get_rival_name
     */

    error = pkmn_game_save_get_rival_name(
                NULL, // handle
                strbuffer,
                STRBUFFER_LEN,
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_rival_name(
                game_save,
                NULL, // rival_name_out
                STRBUFFER_LEN,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "rival_name_out");

    /*
     * pkmn_game_save_set_rival_name
     */

    error = pkmn_game_save_set_rival_name(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_set_rival_name(
                game_save,
                NULL // rival_name
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "rival_name");

    /*
     * pkmn_game_save_get_money
     */

    error = pkmn_game_save_get_money(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_money(
                game_save,
                NULL // money_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "money_out");

    /*
     * pkmn_game_save_set_money
     */

    error = pkmn_game_save_set_money(
                NULL, // handle
                dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_game_save_get_pokemon_party
     */

    error = pkmn_game_save_get_pokemon_party(
                NULL, // handle
                &pokemon_party
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_pokemon_party(
                game_save,
                NULL // pokemon_party_handle_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "pokemon_party_handle_out");

    /*
     * pkmn_game_save_get_pokemon_pc
     */

    error = pkmn_game_save_get_pokemon_pc(
                NULL, // handle
                &pokemon_pc
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_pokemon_pc(
                game_save,
                NULL // pokemon_pc_handle_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "pokemon_pc_handle_out");

    /*
     * pkmn_game_save_get_item_bag
     */

    error = pkmn_game_save_get_item_bag(
                NULL, // handle
                &item_bag
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_item_bag(
                game_save,
                NULL // item_bag_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "item_bag_out");

    /*
     * pkmn_game_save_get_item_pc
     */

    error = pkmn_game_save_get_item_pc(
                NULL, // handle
                &item_pc
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_game_save_get_item_pc(
                game_save,
                NULL // item_pc_out
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "item_pc_out");

    error = pkmn_game_save_free(&game_save);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}


/*
 * <pkmn-c/item_bag.h>
 */
static void item_bag_error_test()
{
    pkmn_item_list_t item_list;

    pkmn_item_bag_t item_bag;
    error = pkmn_item_bag_init(
                "Red",
                &item_bag
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
}

/*
 * <pkmn-c/item_list.h>
 */
static void item_list_error_test()
{
    pkmn_item_list_t item_list;
    error = pkmn_item_list_init(
                "Items",
                "Red",
                &item_list
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
                &dummy_pkmn_string_list_t
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
                &dummy_pkmn_string_list_t
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
                &dummy_pkmn_string_list_t
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
static void pokemon_error_test() {
    pkmn_pokemon_handle_t pokemon = NULL;
    error = pkmn_pokemon_make(
                &pokemon,
                "Vaporeon",
                "Ruby",
                "",
                70
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_move_slots_t dummy_pkmn_move_slots_t = {
        .move_slots = NULL,
        .length = 0
    };
    pkmn_database_pokemon_entry_t pokemon_entry;
    pkmn_trainer_info_t trainer_info;

    pkmn_pokemon_handle_t null_pokemon = NULL;

    /*
     * pkmn_pokemon_make
     */

    error = pkmn_pokemon_make(
                NULL, // handle_ptr
                "Vaporeon",
                "Ruby",
                "",
                70
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_make(
                &pokemon,
                NULL, // species
                "Ruby",
                "",
                70
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_pokemon_make(
                &pokemon,
                "Vaporeon",
                NULL, // game
                "",
                70
            );
    TEST_NULL_POINTER_RETURN("game");

    error = pkmn_pokemon_make(
                &pokemon,
                "Vaporeon",
                "Ruby",
                NULL, // form
                70
            );
    TEST_NULL_POINTER_RETURN("form");

    /*
     * pkmn_pokemon_from_file
     */

    error = pkmn_pokemon_from_file(
                NULL, // handle_ptr
                "pokemon.3gpkm"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_from_file(
                &pokemon,
                NULL // filepath
            );
    TEST_NULL_POINTER_RETURN("filepath");

    /*
     * pkmn_pokemon_free
     */

    error = pkmn_pokemon_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_free(
                &null_pokemon
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_pokemon_strerror
     */

    const char* pokemon_strerror = pkmn_pokemon_strerror(
                                       NULL // handle
                                   );
    TEST_ASSERT_NULL(pokemon_strerror);

    /*
     * pkmn_pokemon_get_species
     */

    error = pkmn_pokemon_get_species(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_species(
                pokemon,
                NULL, // species_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "species_out");

    /*
     * pkmn_pokemon_get_game
     */

    error = pkmn_pokemon_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_game(
                pokemon,
                NULL, // game_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "game_out");

    /*
     * pkmn_pokemon_get_form
     */

    error = pkmn_pokemon_get_form(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_form(
                pokemon,
                NULL, // form_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "form_out");

    /*
     * pkmn_pokemon_set_form
     */

    error = pkmn_pokemon_set_form(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_form(
                pokemon,
                NULL // form
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "form");

    /*
     * pkmn_pokemon_get_database_entry
     */

    error = pkmn_pokemon_get_database_entry(
                NULL, // handle
                &pokemon_entry
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_database_entry(
                pokemon,
                NULL // database_entry_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "database_entry_out");

    /*
     * pkmn_pokemon_get_nickname
     */

    error = pkmn_pokemon_get_nickname(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_nickname(
                pokemon,
                NULL, // nickname_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "nickname_out");

    /*
     * pkmn_pokemon_set_nickname
     */

    error = pkmn_pokemon_set_nickname(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_nickname(
                pokemon,
                NULL // nickname
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "nickname");

    /*
     * pkmn_pokemon_get_original_trainer_info
     */

    error = pkmn_pokemon_get_original_trainer_info(
                NULL, // handle
                &trainer_info
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_original_trainer_info(
                pokemon,
                NULL // trainer_info_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_info_out");

    /*
     * pkmn_pokemon_set_original_trainer_info
     */

    error = pkmn_pokemon_set_original_trainer_info(
                NULL, // handle
                &trainer_info
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_original_trainer_info(
                pokemon,
                NULL // trainer_info
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_info");

    /*
     * pkmn_pokemon_get_gender
     */

    error = pkmn_pokemon_get_gender(
                NULL, // handle
                &dummy_pkmn_gender_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_gender(
                pokemon,
                NULL // gender_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "gender_out");

    /*
     * pkmn_pokemon_is_shiny
     */

    error = pkmn_pokemon_is_shiny(
                NULL, // handle
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_is_shiny(
                pokemon,
                NULL // is_shiny_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "is_shiny_out");

    /*
     * pkmn_pokemon_set_shininess
     */

    error = pkmn_pokemon_set_shininess(
                NULL, // handle
                true
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_held_item
     */

    error = pkmn_pokemon_get_held_item(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_held_item(
                pokemon,
                NULL, // held_item_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "held_item_out");

    /*
     * pkmn_pokemon_set_held_item
     */

    error = pkmn_pokemon_set_held_item(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_held_item(
                pokemon,
                NULL // held_item
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "held_item");

    /*
     * pkmn_pokemon_get_current_trainer_friendship
     */

    error = pkmn_pokemon_get_current_trainer_friendship(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_current_trainer_friendship(
                pokemon,
                NULL // friendship_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "friendship_out");

    /*
     * pkmn_pokemon_set_current_trainer_friendship
     */

    error = pkmn_pokemon_set_current_trainer_friendship(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_ability
     */

    error = pkmn_pokemon_get_ability(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_ability(
                pokemon,
                NULL, // ability_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ability_out");

    /*
     * pkmn_pokemon_set_ability
     */

    error = pkmn_pokemon_set_ability(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_ability(
                pokemon,
                NULL // ability
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ability");

    /*
     * pkmn_pokemon_get_ball
     */

    error = pkmn_pokemon_get_ball(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_ball(
                pokemon,
                NULL, // ball_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ball_out");

    /*
     * pkmn_pokemon_set_ball
     */

    error = pkmn_pokemon_set_ball(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_ball(
                pokemon,
                NULL // ball
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ball");

    /*
     * pkmn_pokemon_get_level_met
     */

    error = pkmn_pokemon_get_level_met(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_level_met(
                pokemon,
                NULL // level_met_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "level_met_out");

    /*
     * pkmn_pokemon_set_level_met
     */

    error = pkmn_pokemon_set_level_met(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_location_met
     */

    error = pkmn_pokemon_get_location_met(
                NULL, // handle
                true,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_location_met(
                pokemon,
                true,
                NULL, // location_met_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "location_met_out");

    /*
     * pkmn_pokemon_set_location_met
     */

    error = pkmn_pokemon_set_location_met(
                NULL, // handle
                strbuffer,
                true
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_location_met(
                pokemon,
                NULL, // location_met
                true
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "location_met");

    /*
     * pkmn_pokemon_get_original_game
     */

    error = pkmn_pokemon_get_original_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                NULL, // original_game_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "original_game_out");

    /*
     * pkmn_pokemon_set_original_game
     */

    error = pkmn_pokemon_set_original_game(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_original_game(
                pokemon,
                NULL // original_game
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "original_game");

    /*
     * pkmn_pokemon_get_personality
     */

    error = pkmn_pokemon_get_personality(
                NULL, // handle
                &dummy_uint32_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_personality(
                pokemon,
                NULL // personality_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "personality_out");

    /*
     * pkmn_pokemon_set_personality
     */

    error = pkmn_pokemon_set_personality(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_experience
     */

    error = pkmn_pokemon_get_experience(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_experience(
                pokemon,
                NULL // experience_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "experience_out");

    /*
     * pkmn_pokemon_set_experience
     */

    error = pkmn_pokemon_set_experience(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_level
     */

    error = pkmn_pokemon_get_level(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_level(
                pokemon,
                NULL // level_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "level_out");

    /*
     * pkmn_pokemon_set_level
     */

    error = pkmn_pokemon_set_level(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_marking_names
     */

    error = pkmn_pokemon_get_marking_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_marking_names(
                pokemon,
                NULL // marking_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "marking_names_out");

    /*
     * pkmn_pokemon_has_marking
     */

    error = pkmn_pokemon_has_marking(
                NULL, // handle
                "Circle",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_has_marking(
                pokemon,
                NULL, // marking
                &dummy_bool
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "marking");

    error = pkmn_pokemon_has_marking(
                pokemon,
                strbuffer,
                NULL // has_marking_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "has_marking_out");

    /*
     * pkmn_pokemon_set_marking
     */

    error = pkmn_pokemon_set_marking(
                NULL, // handle
                strbuffer,
                true
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_marking(
                pokemon,
                NULL, // marking
                true
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "marking");

    /*
     * pkmn_pokemon_get_ribbon_names
     */

    error = pkmn_pokemon_get_ribbon_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_ribbon_names(
                pokemon,
                NULL // ribbon_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon_names_out");

    /*
     * pkmn_pokemon_has_ribbon
     */

    error = pkmn_pokemon_has_ribbon(
                NULL, // handle
                "Circle",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_has_ribbon(
                pokemon,
                NULL, // ribbon
                &dummy_bool
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon");

    error = pkmn_pokemon_has_ribbon(
                pokemon,
                strbuffer,
                NULL // has_ribbon_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "has_ribbon_out");

    /*
     * pkmn_pokemon_set_ribbon
     */

    error = pkmn_pokemon_set_ribbon(
                NULL, // handle
                strbuffer,
                true
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_ribbon(
                pokemon,
                NULL, // ribbon
                true
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ribbon");

    /*
     * pkmn_pokemon_get_moves
     */

    error = pkmn_pokemon_get_moves(
                NULL, // handle
                &dummy_pkmn_move_slots_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_moves(
                pokemon,
                NULL // move_slots_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "move_slots_out");

    /*
     * pkmn_pokemon_set_move
     */

    error = pkmn_pokemon_set_move(
                NULL, // handle
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_move(
                pokemon,
                NULL, // move
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "move");

    /*
     * pkmn_pokemon_get_contest_stat_names
     */

    error = pkmn_pokemon_get_contest_stat_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_contest_stat_names(
                pokemon,
                NULL // contest_stat_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "contest_stat_names_out");

    /*
     * pkmn_pokemon_get_contest_stat
     */

    error = pkmn_pokemon_get_contest_stat(
                NULL, // handle
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_contest_stat(
                pokemon,
                NULL, // stat
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    error = pkmn_pokemon_get_contest_stat(
                pokemon,
                strbuffer,
                NULL // contest_stat_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "contest_stat_out");

    /*
     * pkmn_pokemon_set_contest_stat
     */

    error = pkmn_pokemon_set_contest_stat(
                NULL, // handle
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_contest_stat(
                pokemon,
                NULL, // stat
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    /*
     * pkmn_pokemon_get_EV_names
     */

    error = pkmn_pokemon_get_EV_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_EV_names(
                pokemon,
                NULL // EV_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "EV_names_out");

    /*
     * pkmn_pokemon_get_EV
     */

    error = pkmn_pokemon_get_EV(
                NULL, // handle
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_EV(
                pokemon,
                NULL, // stat
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    error = pkmn_pokemon_get_EV(
                pokemon,
                strbuffer,
                NULL // EV_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "EV_out");

    /*
     * pkmn_pokemon_set_EV
     */

    error = pkmn_pokemon_set_EV(
                NULL, // handle
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_EV(
                pokemon,
                NULL, // stat
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    /*
     * pkmn_pokemon_get_IV_names
     */

    error = pkmn_pokemon_get_IV_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_IV_names(
                pokemon,
                NULL // IV_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "IV_names_out");

    /*
     * pkmn_pokemon_get_IV
     */

    error = pkmn_pokemon_get_IV(
                NULL, // handle
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_IV(
                pokemon,
                NULL, // stat
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    error = pkmn_pokemon_get_IV(
                pokemon,
                strbuffer,
                NULL // IV_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "IV_out");

    /*
     * pkmn_pokemon_set_IV
     */

    error = pkmn_pokemon_set_IV(
                NULL, // handle
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_IV(
                pokemon,
                NULL, // stat
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat");

    /*
     * pkmn_pokemon_get_stat_names
     */

    error = pkmn_pokemon_get_stat_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_stat_names(
                pokemon,
                NULL // stat_names_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat_names_out");

    /*
     * pkmn_pokemon_get_stat
     */

    error = pkmn_pokemon_get_stat(
                NULL, // handle
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_stat(
                pokemon,
                NULL, // stat_name
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat_name");

    error = pkmn_pokemon_get_stat(
                pokemon,
                strbuffer,
                NULL // stat_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "stat_out");

    /*
     * pkmn_pokemon_get_icon_filepath
     */

    error = pkmn_pokemon_get_icon_filepath(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_icon_filepath(
                pokemon,
                NULL, // icon_filepath_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "icon_filepath_out");

    /*
     * pkmn_pokemon_get_sprite_filepath
     */

    error = pkmn_pokemon_get_sprite_filepath(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_sprite_filepath(
                pokemon,
                NULL, // sprite_filepath_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "sprite_filepath_out");

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon);
}

/*
 * <pkmn-c/pokemon_box.h>
 */
static void pokemon_box_error_test() {
    pkmn_pokemon_box_handle_t pokemon_box = NULL;
    error = pkmn_pokemon_box_make(
                &pokemon_box,
                "Gold"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_list_t dummy_pkmn_pokemon_list_t = {
        .pokemon_list = NULL,
        .length = 0
    };

    pkmn_pokemon_box_handle_t null_pokemon_box = NULL;

    /*
     * pkmn_pokemon_box_make
     */

    error = pkmn_pokemon_box_make(
                NULL, // handle_ptr
                "Gold"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_box_make(
                &pokemon_box,
                NULL // game
            );
    TEST_NULL_POINTER_RETURN("game");

    /*
     * pkmn_pokemon_box_free
     */

    error = pkmn_pokemon_box_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_box_free(
                &null_pokemon_box
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_pokemon_box_strerror
     */

    const char* pokemon_box_strerror = pkmn_pokemon_box_strerror(
                                           NULL // handle
                                       );
    TEST_ASSERT_NULL(pokemon_box_strerror);

    /*
     * pkmn_pokemon_box_get_name
     */

    error = pkmn_pokemon_box_get_name(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_name(
                pokemon_box,
                NULL, // name_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "name_out");

    /*
     * pkmn_pokemon_box_set_name
     */

    error = pkmn_pokemon_box_set_name(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_set_name(
                pokemon_box,
                NULL // name
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "name");

    /*
     * pkmn_pokemon_box_get_game
     */

    error = pkmn_pokemon_box_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_game(
                pokemon_box,
                NULL, // game_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "game_out");

    /*
     * pkmn_pokemon_box_get_num_pokemon
     */

    error = pkmn_pokemon_box_get_num_pokemon(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_num_pokemon(
                pokemon_box,
                NULL // num_pokemon_out
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "num_pokemon_out");

    /*
     * pkmn_pokemon_box_get_capacity
     */

    error = pkmn_pokemon_box_get_capacity(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_capacity(
                pokemon_box,
                NULL // capacity_out
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "capacity_out");

    /*
     * pkmn_pokemon_box_get_pokemon
     */

    error = pkmn_pokemon_box_get_pokemon(
                NULL, // handle
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_pokemon(
                pokemon_box,
                0,
                NULL // pokemon_handle_out
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_handle_out");

    /*
     * pkmn_pokemon_box_set_pokemon
     */

    error = pkmn_pokemon_box_set_pokemon(
                NULL, // handle
                0,
                dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_set_pokemon(
                pokemon_box,
                0,
                NULL // pokemon_handle
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_handle");

    /*
     * pkmn_pokemon_box_as_array
     */

    error = pkmn_pokemon_box_as_array(
                NULL, // handle
                &dummy_pkmn_pokemon_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_as_array(
                pokemon_box,
                NULL // pokemon_list_out
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "pokemon_list_out");

    error = pkmn_pokemon_box_free(&pokemon_box);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_box);
}

/*
 * <pkmn-c/pokemon_party.h>
 */
static void pokemon_party_error_test() {
    pkmn_pokemon_party_handle_t pokemon_party = NULL;
    error = pkmn_pokemon_party_make(
                &pokemon_party,
                "Gold"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_list_t dummy_pkmn_pokemon_list_t = {
        .pokemon_list = NULL,
        .length = 0
    };

    pkmn_pokemon_party_handle_t null_pokemon_party = NULL;

    /*
     * pkmn_pokemon_party_make
     */

    error = pkmn_pokemon_party_make(
                NULL, // handle_ptr
                "Gold"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_party_make(
                &pokemon_party,
                NULL // game
            );
    TEST_NULL_POINTER_RETURN("game");

    /*
     * pkmn_pokemon_party_free
     */

    error = pkmn_pokemon_party_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_party_free(
                &null_pokemon_party
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_pokemon_party_strerror
     */

    const char* pokemon_party_strerror = pkmn_pokemon_party_strerror(
                                           NULL // handle
                                       );
    TEST_ASSERT_NULL(pokemon_party_strerror);

    /*
     * pkmn_pokemon_party_get_game
     */

    error = pkmn_pokemon_party_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_get_game(
                pokemon_party,
                NULL, // game_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "game_out");

    /*
     * pkmn_pokemon_party_get_num_pokemon
     */

    error = pkmn_pokemon_party_get_num_pokemon(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_get_num_pokemon(
                pokemon_party,
                NULL // num_pokemon_out
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "num_pokemon_out");

    /*
     * pkmn_pokemon_party_get_pokemon
     */

    error = pkmn_pokemon_party_get_pokemon(
                NULL, // handle
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_get_pokemon(
                pokemon_party,
                0,
                NULL // pokemon_handle_out
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_handle_out");

    /*
     * pkmn_pokemon_party_set_pokemon
     */

    error = pkmn_pokemon_party_set_pokemon(
                NULL, // handle
                0,
                dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_set_pokemon(
                pokemon_party,
                0,
                NULL // pokemon_handle
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_handle");

    /*
     * pkmn_pokemon_party_as_array
     */

    error = pkmn_pokemon_party_as_array(
                NULL, // handle
                &dummy_pkmn_pokemon_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_as_array(
                pokemon_party,
                NULL // pokemon_list_out
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "pokemon_list_out");

    error = pkmn_pokemon_party_free(&pokemon_party);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_party);
}

/*
 * <pkmn-c/pokemon_pc.h>
 */
static void pokemon_pc_error_test() {
    pkmn_pokemon_pc_handle_t pokemon_pc = NULL;
    error = pkmn_pokemon_pc_make(
                &pokemon_pc,
                "Gold"
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    pkmn_pokemon_box_handle_t dummy_pokemon_box = NULL;

    pkmn_pokemon_box_list_t dummy_pkmn_pokemon_box_list_t = {
        .pokemon_boxes = NULL,
        .length = 0
    };

    pkmn_pokemon_pc_handle_t null_pokemon_pc = NULL;

    /*
     * pkmn_pokemon_pc_make
     */

    error = pkmn_pokemon_pc_make(
                NULL, // handle_ptr
                "Gold"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_pc_make(
                &pokemon_pc,
                NULL // game
            );
    TEST_NULL_POINTER_RETURN("game");

    /*
     * pkmn_pokemon_pc_free
     */

    error = pkmn_pokemon_pc_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_pokemon_pc_free(
                &null_pokemon_pc
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_pokemon_pc_strerror
     */

    const char* pokemon_pc_strerror = pkmn_pokemon_pc_strerror(
                                           NULL // handle
                                       );
    TEST_ASSERT_NULL(pokemon_pc_strerror);

    /*
     * pkmn_pokemon_pc_get_game
     */

    error = pkmn_pokemon_pc_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_get_game(
                pokemon_pc,
                NULL, // game_out
                sizeof(strbuffer),
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "game_out");

    /*
     * pkmn_pokemon_pc_get_num_boxes
     */

    error = pkmn_pokemon_pc_get_num_boxes(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_get_num_boxes(
                pokemon_pc,
                NULL // num_boxes_out
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "num_boxes_out");

    /*
     * pkmn_pokemon_pc_get_pokemon
     */

    error = pkmn_pokemon_pc_get_box(
                NULL, // handle
                0,
                &dummy_pokemon_box
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_get_box(
                pokemon_pc,
                0,
                NULL // pokemon_handle_out
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "pokemon_box_handle_out");

    /*
     * pkmn_pokemon_pc_as_array
     */

    error = pkmn_pokemon_pc_as_array(
                NULL, // handle
                &dummy_pkmn_pokemon_box_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_as_array(
                pokemon_pc,
                NULL // pokemon_box_list_out
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "pokemon_box_list_out");

    /*
     * pkmn_pokemon_pc_get_box_names
     */

    error = pkmn_pokemon_pc_get_box_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_get_box_names(
                pokemon_pc,
                NULL // box_names_out
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "box_names_out");

    error = pkmn_pokemon_pc_free(&pokemon_pc);
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
    TEST_ASSERT_NULL(pokemon_pc);
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
    pkmn_gender_t dummy_pkmn_gender_t = PKMN_MALE;

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
                PKMN_GENDERLESS,
                strbuffer,
                &dummy_uint32_t
            );
    TEST_NULL_POINTER_RETURN("species");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                NULL, // ability
                PKMN_GENDERLESS,
                strbuffer,
                &dummy_uint32_t
            );
    TEST_NULL_POINTER_RETURN("ability");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDERLESS,
                NULL, // nature
                &dummy_uint32_t
            );
    TEST_NULL_POINTER_RETURN("nature");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDERLESS,
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
static void calculations_spinda_spots_error_test() {
    pkmn_spinda_coords_t dummy_coords;
    pkmn_spinda_spots_t dummy_spots;

    /*
     * pkmn_calculations_add_spinda_coords
     */

    error = pkmn_calculations_add_spinda_coords(
                NULL, // coords1
                &dummy_coords,
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("coords1");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                NULL, // coords2
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("coords2");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                &dummy_coords,
                NULL // result_out
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_add_spinda_spots
     */

    error = pkmn_calculations_add_spinda_spots(
                NULL, // spots1
                &dummy_spots,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots1");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                NULL, // spots2
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots2");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                &dummy_spots,
                NULL // result_out
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_add_spinda_coords_to_spots
     */

    error = pkmn_calculations_add_spinda_coords_to_spots(
                NULL, // spots
                &dummy_coords,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("spots");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                NULL, // coords
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("coords");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                &dummy_coords,
                NULL // result_out
            );
    TEST_NULL_POINTER_RETURN("result_out");

    /*
     * pkmn_calculations_spinda_spot_offset
     */

    error = pkmn_calculations_spinda_spot_offset(
                0,
                NULL // spot_offset_out
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
                &dummy_pkmn_string_list_t
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
                &dummy_pkmn_string_list_t
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
                &dummy_pkmn_string_list_t
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
                &dummy_pkmn_string_list_t
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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

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
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
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
 * <pkmn-c/types/string_types.h>
 */
static void types_string_types_test()
{
    /*
     * pkmn_string_list_free
     */
    error = pkmn_string_list_free(NULL);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
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
    populate_pksav_saves();

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
    PKMN_C_TEST(types_string_types_test)
    PKMN_C_TEST(utils_paths_error_test)
)
