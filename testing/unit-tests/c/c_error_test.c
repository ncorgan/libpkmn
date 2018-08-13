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

static enum pkmn_error error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static char PKSAV_TEST_SAVES[STRBUFFER_LEN] = {0};
static bool dummy_bool = false;
static int dummy_int = 0;
static size_t dummy_size_t = 0;
static uint32_t dummy_uint32 = 0;
static float dummy_float = 0;
static struct pkmn_string_list dummy_string_list = { NULL, 0 };
static struct pkmn_trainer_info dummy_trainer_info = { NULL, {0}, PKMN_GENDER_GENDERLESS};
static struct pkmn_attribute_names dummy_attribute_names = {{NULL, 0}, {NULL, 0}, {NULL, 0}};
static struct pkmn_pokemon dummy_pokemon = {NULL, NULL, NULL};
static struct pkmn_pokemon_list dummy_pokemon_list = { NULL, 0 };

static const char* null_pointer_error_format = "Null pointer passed into parameter \"%s\".";

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

    enum pkmn_game_save_type dummy_game_save_type = PKMN_GAME_SAVE_TYPE_NONE;

    struct pkmn_item_bag dummy_item_bag = { NULL, { NULL, 0 }, NULL };
    struct pkmn_item_list dummy_item_list = { NULL, NULL, 0, NULL };
    struct pkmn_pokedex dummy_pokedex = { NULL, NULL };
    struct pkmn_pokemon_party dummy_pokemon_party = { NULL, 0, NULL };
    struct pkmn_pokemon_pc dummy_pokemon_pc = { NULL, 0, NULL };

    struct pkmn_game_save game_save = { NULL, NULL };

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
    TEST_NULL_POINTER_RETURN("p_filepath");

    error = pkmn_game_save_detect_type(
                filepath,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_game_save_type_out");

    /*
     * pkmn_game_save_init_from_file
     */

    error = pkmn_game_save_init_from_file(
                NULL,
                &game_save
            );
    TEST_NULL_POINTER_RETURN("p_filepath");

    error = pkmn_game_save_init_from_file(
                filepath,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_game_save_out");

    /*
     * pkmn_game_save_free
     */

    error = pkmn_game_save_free(NULL);
    TEST_NULL_POINTER_RETURN("p_game_save");

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
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_filepath(
                &game_save,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_filepath_buffer");

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
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_save_as
     */

    error = pkmn_game_save_save_as(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_save_as(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_filepath");

    /*
     * pkmn_game_save_get_trainer_info
     */

    error = pkmn_game_save_get_trainer_info(
                NULL,
                &dummy_trainer_info
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_trainer_info(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_trainer_info_out");

    /*
     * pkmn_game_save_set_trainer_name
     */

    error = pkmn_game_save_set_trainer_name(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_set_trainer_name(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_trainer_name");

    /*
     * pkmn_game_save_set_trainer_public_id
     */

    error = pkmn_game_save_set_trainer_public_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_set_trainer_secret_id
     */

    error = pkmn_game_save_set_trainer_secret_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_set_trainer_id
     */

    error = pkmn_game_save_set_trainer_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_set_trainer_gender
     */

    error = pkmn_game_save_set_trainer_gender(
                NULL,
                PKMN_GENDER_GENDERLESS
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_get_rival_name
     */

    error = pkmn_game_save_get_rival_name(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_rival_name(
                &game_save,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_rival_name_buffer");

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
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_set_rival_name(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_rival_name");

    /*
     * pkmn_game_save_get_money
     */

    error = pkmn_game_save_get_money(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_money(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_money_out");

    /*
     * pkmn_game_save_set_money
     */

    error = pkmn_game_save_set_money(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    /*
     * pkmn_game_save_get_pokedex
     */

    error = pkmn_game_save_get_pokedex(
                NULL,
                &dummy_pokedex
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_pokedex(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_pokedex_out");

    /*
     * pkmn_game_save_get_pokemon_party
     */

    error = pkmn_game_save_get_pokemon_party(
                NULL,
                &dummy_pokemon_party
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_pokemon_party(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_pokemon_party_out");

    /*
     * pkmn_game_save_get_pokemon_pc
     */

    error = pkmn_game_save_get_pokemon_pc(
                NULL,
                &dummy_pokemon_pc
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_pokemon_pc(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_pokemon_pc_out");

    /*
     * pkmn_game_save_get_item_bag
     */

    error = pkmn_game_save_get_item_bag(
                NULL,
                &dummy_item_bag
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_item_bag(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_item_bag_out");

    /*
     * pkmn_game_save_get_item_pc
     */

    error = pkmn_game_save_get_item_pc(
                NULL,
                &dummy_item_list
            );
    TEST_NULL_POINTER_RETURN("p_game_save");

    error = pkmn_game_save_get_item_pc(
                &game_save,
                NULL
            );
    TEST_GAME_SAVE_NULL_POINTER_RETURN(game_save, "p_item_pc_out");

    // Cleanup
    error = pkmn_game_save_free(&game_save);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/item_bag.h>
 */
static void item_bag_error_test()
{
    struct pkmn_item_list item_list = { NULL, NULL, 0, NULL };
    struct pkmn_item_bag item_bag = { NULL, { NULL, 0 }, NULL };

    error = pkmn_item_bag_init(
                "Red",
                &item_bag
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_item_bag_init
     */

    error = pkmn_item_bag_init(
                NULL,
                &item_bag
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_item_bag_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_bag_out");

    /*
     * pkmn_item_bag_free
     */

    error = pkmn_item_bag_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_bag");

    /*
     * pkmn_item_bag_strerror
     */

    const char* item_bag_strerror = pkmn_item_bag_strerror(
                                        NULL
                                    );
    TEST_ASSERT_NULL(item_bag_strerror);

    /*
     * pkmn_item_bag_get_pocket
     */

    error = pkmn_item_bag_get_pocket(
                NULL,
                "Items",
                &item_list
            );
    TEST_NULL_POINTER_RETURN("p_item_bag");

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                NULL,
                &item_list
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "p_pocket_name");

    error = pkmn_item_bag_get_pocket(
                &item_bag,
                "Items",
                NULL
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "p_item_list_out");

    /*
     * pkmn_item_bag_add
     */

    error = pkmn_item_bag_add(
                NULL,
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("p_item_bag");

    error = pkmn_item_bag_add(
                &item_bag,
                NULL,
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "p_item");

    /*
     * pkmn_item_bag_remove
     */

    error = pkmn_item_bag_remove(
                NULL,
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("p_item_bag");

    error = pkmn_item_bag_remove(
                &item_bag,
                NULL,
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "p_item");

    error = pkmn_item_bag_free(&item_bag);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/item_list.h>
 */
static void item_list_error_test()
{
    struct pkmn_item_list item_list = { NULL, NULL, 0, NULL };

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_list_out");

    error = pkmn_item_list_init(
                NULL,
                "Red",
                &item_list
            );
    TEST_NULL_POINTER_RETURN("p_name");

    error = pkmn_item_list_init(
                "Items",
                NULL,
                &item_list
            );
    TEST_NULL_POINTER_RETURN("p_game");

    /*
     * pkmn_item_list_free
     */

    error = pkmn_item_list_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    /*
     * pkmn_item_list_strerror
     */

    const char* item_list_strerror = pkmn_item_list_strerror(
                                         NULL
                                     );
    TEST_ASSERT_NULL(item_list_strerror);

    /*
     * pkmn_item_list_add
     */

    error = pkmn_item_list_add(
                NULL,
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    error = pkmn_item_list_add(
                &item_list,
                NULL,
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "p_item");

    /*
     * pkmn_item_list_remove
     */

    error = pkmn_item_list_remove(
                NULL,
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    error = pkmn_item_list_remove(
                &item_list,
                NULL,
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "p_item");

    /*
     * pkmn_item_list_move
     */

    error = pkmn_item_list_move(
                NULL,
                0,
                1
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    /*
     * pkmn_item_list_set_item
     */

    error = pkmn_item_list_set_item(
                NULL,
                0,
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    error = pkmn_item_list_set_item(
                &item_list,
                0,
                NULL,
                5
            );
    TEST_NULL_POINTER_RETURN("p_item");

    /*
     * pkmn_item_list_get_valid_items
     */

    error = pkmn_item_list_get_valid_items(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_item_list");

    error = pkmn_item_list_get_valid_items(
                &item_list,
                NULL
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "p_valid_items_out");
}

/*
 * <pkmn-c/pokedex.h>
 */
static void pokedex_error_test()
{
    struct pkmn_pokedex pokedex =
    {
        .p_game = NULL,
        .p_internal = NULL
    };
    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_pokedex_init("Red", &pokedex);
    PKMN_TEST_ASSERT_SUCCESS(error);

    /*
     * pkmn_pokedex_init
     */

    error = pkmn_pokedex_init(
                NULL,
                &pokedex
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_pokedex_init(
                "",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokedex_out");

    /*
     * pkmn_pokedex_free
     */

    error = pkmn_pokedex_free(NULL);
    TEST_NULL_POINTER_RETURN("p_pokedex");

    /*
     * pkmn_pokedex_strerror
     */

    const char* pokedex_strerror = pkmn_pokedex_strerror(NULL);
    TEST_ASSERT_NULL(pokedex_strerror);

    /*
     * pkmn_pokedex_has_seen
     */

    error = pkmn_pokedex_has_seen(
                NULL,
                "",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_has_seen(
                &pokedex,
                NULL,
                &dummy_bool
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_species");

    error = pkmn_pokedex_has_seen(
                &pokedex,
                "",
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_has_seen_out");

    /*
     * pkmn_pokedex_set_has_seen_species
     */

    error = pkmn_pokedex_set_has_seen_species(
                NULL,
                "",
                true
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_set_has_seen_species(
                &pokedex,
                NULL,
                true
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_species");

    /*
     * pkmn_pokedex_get_all_seen
     */

    error = pkmn_pokedex_get_all_seen(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_get_all_seen(
                &pokedex,
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_all_seen_out");

    /*
     * pkmn_pokedex_get_num_seen
     */

    error = pkmn_pokedex_get_num_seen(
                NULL,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_num_seen_out");

    /*
     * pkmn_pokedex_has_caught
     */

    error = pkmn_pokedex_has_caught(
                NULL,
                "",
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_has_caught(
                &pokedex,
                NULL,
                &dummy_bool
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_species");

    error = pkmn_pokedex_has_caught(
                &pokedex,
                "",
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_has_caught_out");

    /*
     * pkmn_pokedex_set_has_caught_species
     */

    error = pkmn_pokedex_set_has_caught_species(
                NULL,
                "",
                true
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_set_has_caught_species(
                &pokedex,
                NULL,
                true
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_species");

    /*
     * pkmn_pokedex_get_all_caught
     */

    error = pkmn_pokedex_get_all_caught(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_get_all_caught(
                &pokedex,
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_all_caught_out");

    /*
     * pkmn_pokedex_get_num_caught
     */

    error = pkmn_pokedex_get_num_caught(
                NULL,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokedex");

    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                NULL
            );
    TEST_POKEDEX_NULL_POINTER_RETURN(pokedex, "p_num_caught_out");
}

/*
 * <pkmn-c/pokemon.h>
 */
static void pokemon_error_test()
{
    struct pkmn_pokemon pokemon =
    {
        .p_species = NULL,
        .p_game = NULL,
        .p_internal = NULL
    };
    struct pkmn_database_pokemon_entry dummy_pokemon_entry;
    enum pkmn_condition dummy_condition = PKMN_CONDITION_NONE;
    enum pkmn_gender dummy_gender = PKMN_GENDER_GENDERLESS;
    struct pkmn_move_slots dummy_move_slots = { NULL, 0 };

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
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_pokemon_init(
                strbuffer,
                NULL,
                strbuffer,
                0,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_pokemon_init(
                strbuffer,
                strbuffer,
                NULL,
                0,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("p_form");

    error = pkmn_pokemon_init(
                strbuffer,
                strbuffer,
                strbuffer,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_out");

    /*
     * pkmn_pokemon_init_from_file
     */

    error = pkmn_pokemon_init_from_file(
                NULL,
                &pokemon
            );
    TEST_NULL_POINTER_RETURN("p_filepath");

    error = pkmn_pokemon_init_from_file(
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_out");

    /*
     * pkmn_pokemon_free
     */

    error = pkmn_pokemon_free(NULL);
    TEST_NULL_POINTER_RETURN("p_pokemon");

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
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_to_game(
                &pokemon,
                NULL,
                &pokemon
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_game");

    error = pkmn_pokemon_to_game(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_new_pokemon_out");

    /*
     * pkmn_pokemon_export_to_file
     */

    error = pkmn_pokemon_export_to_file(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_export_to_file(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_filepath");

    /*
     * pkmn_pokemon_get_form
     */

    error = pkmn_pokemon_get_form(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

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
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_form(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_form");

    /*
     * pkmn_pokemon_is_egg
     */

    error = pkmn_pokemon_is_egg(
                NULL,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_is_egg(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_is_egg_out");

    /*
     * pkmn_pokemon_set_is_egg
     */

    error = pkmn_pokemon_set_is_egg(
                NULL,
                dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_database_entry
     */

    error = pkmn_pokemon_get_database_entry(
                NULL,
                &dummy_pokemon_entry
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_database_entry(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_database_entry_out");

    /*
     * pkmn_pokemon_get_condition
     */

    error = pkmn_pokemon_get_condition(
                NULL,
                &dummy_condition
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_condition(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_condition_out");

    /*
     * pkmn_pokemon_set_condition
     */

    error = pkmn_pokemon_set_condition(
                NULL,
                dummy_condition
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_nickname
     */

    error = pkmn_pokemon_get_nickname(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_nickname(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_nickname_out");

    /*
     * pkmn_pokemon_set_nickname
     */

    error = pkmn_pokemon_set_nickname(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_nickname(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_nickname");

    /*
     * pkmn_pokemon_get_gender
     */

    error = pkmn_pokemon_get_gender(
                NULL,
                &dummy_gender
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_gender(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_gender_out");

    /*
     * pkmn_pokemon_set_gender
     */

    error = pkmn_pokemon_set_gender(
                NULL,
                dummy_gender
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_is_shiny
     */

    error = pkmn_pokemon_is_shiny(
                NULL,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_is_shiny(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_is_shiny_out");

    /*
     * pkmn_pokemon_set_is_shiny
     */

    error = pkmn_pokemon_set_is_shiny(
                NULL,
                dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_held_item
     */

    error = pkmn_pokemon_get_held_item(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_held_item(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_held_item_out");

    /*
     * pkmn_pokemon_set_held_item
     */

    error = pkmn_pokemon_set_held_item(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_held_item(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_held_item");

    /*
     * pkmn_pokemon_get_pokerus_duration
     */

    error = pkmn_pokemon_get_pokerus_duration(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_pokerus_duration(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_pokerus_duration_out");

    /*
     * pkmn_pokemon_set_pokerus_duration
     */

    error = pkmn_pokemon_set_pokerus_duration(
                NULL,
                dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_original_trainer_info
     */

    error = pkmn_pokemon_get_original_trainer_info(
                NULL,
                &dummy_trainer_info
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_original_trainer_info(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_original_trainer_info_out");

    /*
     * pkmn_pokemon_set_original_trainer_public_id
     */

    error = pkmn_pokemon_set_original_trainer_public_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_set_original_trainer_secret_id
     */

    error = pkmn_pokemon_set_original_trainer_secret_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_set_original_trainer_id
     */

    error = pkmn_pokemon_set_original_trainer_id(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_set_original_trainer_gender
     */

    error = pkmn_pokemon_set_original_trainer_gender(
                NULL,
                dummy_gender
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_current_trainer_friendship
     */

    error = pkmn_pokemon_get_current_trainer_friendship(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_current_trainer_friendship(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_current_trainer_friendship_out");

    /*
     * pkmn_pokemon_set_current_trainer_friendship
     */

    error = pkmn_pokemon_set_current_trainer_friendship(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_ability
     */

    error = pkmn_pokemon_get_ability(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_ability(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ability_out");

    /*
     * pkmn_pokemon_set_ability
     */

    error = pkmn_pokemon_set_ability(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_ability(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ability");

    /*
     * pkmn_pokemon_get_ball
     */

    error = pkmn_pokemon_get_ball(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_ball(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ball_out");

    /*
     * pkmn_pokemon_set_ball
     */

    error = pkmn_pokemon_set_ball(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_ball(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ball");

    /*
     * pkmn_pokemon_get_level_met
     */

    error = pkmn_pokemon_get_level_met(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_level_met(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_level_met_out");

    /*
     * pkmn_pokemon_set_level_met
     */

    error = pkmn_pokemon_set_level_met(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

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
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_location_met(
                &pokemon,
                false,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_location_met_out");

    /*
     * pkmn_pokemon_set_location_met
     */

    error = pkmn_pokemon_set_location_met(
                NULL,
                strbuffer,
                false
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_location_met(
                &pokemon,
                NULL,
                false
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_location_met");

    /*
     * pkmn_pokemon_get_original_game
     */

    error = pkmn_pokemon_get_original_game(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_original_game(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_original_game_out");

    /*
     * pkmn_pokemon_set_original_game
     */

    error = pkmn_pokemon_set_original_game(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_original_game(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_game");

    /*
     * pkmn_pokemon_get_personality
     */

    error = pkmn_pokemon_get_personality(
                NULL,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_personality(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_personality_out");

    /*
     * pkmn_pokemon_set_personality
     */

    error = pkmn_pokemon_set_personality(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_experience
     */

    error = pkmn_pokemon_get_experience(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_experience(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_experience_out");

    /*
     * pkmn_pokemon_set_experience
     */

    error = pkmn_pokemon_set_experience(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_level
     */

    error = pkmn_pokemon_get_level(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_level(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_level_out");

    /*
     * pkmn_pokemon_set_level
     */

    error = pkmn_pokemon_set_level(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_markings
     */

    error = pkmn_pokemon_get_markings(
                NULL,
                &dummy_bool,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_markings(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_has_markings_buffer_out");

    /*
     * pkmn_pokemon_set_has_marking
     */

    error = pkmn_pokemon_set_has_marking(
                NULL,
                PKMN_MARKING_CIRCLE,
                true
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_has_ribbon
     */

    error = pkmn_pokemon_has_ribbon(
                NULL,
                strbuffer,
                &dummy_bool
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_has_ribbon(
                &pokemon,
                NULL,
                &dummy_bool
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ribbon_name");

    error = pkmn_pokemon_has_ribbon(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_has_ribbon_out");

    /*
     * pkmn_pokemon_set_has_ribbon
     */

    error = pkmn_pokemon_set_has_ribbon(
                NULL,
                strbuffer,
                true
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_has_ribbon(
                &pokemon,
                NULL,
                true
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ribbon_name");

    /*
     * pkmn_pokemon_get_ribbon_names
     */

    error = pkmn_pokemon_get_ribbon_names(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_ribbon_names(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_ribbon_names_out");

    /*
     * pkmn_pokemon_get_contest_stats
     */

    error = pkmn_pokemon_get_contest_stats(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_contest_stats(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_contest_stats_buffer_out");

    /*
     * pkmn_pokemon_set_contest_stat
     */

    error = pkmn_pokemon_set_contest_stat(
                NULL,
                PKMN_CONTEST_STAT_COOL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_moves
     */

    error = pkmn_pokemon_get_moves(
                NULL,
                &dummy_move_slots
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_moves(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_move_slots_out");

    /*
     * pkmn_pokemon_set_move
     */

    error = pkmn_pokemon_set_move(
                NULL,
                0,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_move(
                &pokemon,
                0,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_move");

    /*
     * pkmn_pokemon_set_move_pp
     */

    error = pkmn_pokemon_set_move_pp(
                NULL,
                0,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_EVs
     */

    error = pkmn_pokemon_get_EVs(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_EVs(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_EVs_buffer_out");

    /*
     * pkmn_pokemon_set_EV
     */

    error = pkmn_pokemon_set_EV(
                NULL,
                PKMN_STAT_HP,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_IVs
     */

    error = pkmn_pokemon_get_IVs(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_IVs(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_IVs_buffer_out");

    /*
     * pkmn_pokemon_set_IV
     */

    error = pkmn_pokemon_set_IV(
                NULL,
                PKMN_STAT_HP,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_stats
     */

    error = pkmn_pokemon_get_stats(
                NULL,
                &dummy_int,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_stats(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_stats_buffer_out");

    /*
     * pkmn_pokemon_get_current_hp
     */

    error = pkmn_pokemon_get_current_hp(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_current_hp(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_current_hp_out");

    /*
     * pkmn_pokemon_set_current_hp
     */

    error = pkmn_pokemon_set_current_hp(
                NULL,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    /*
     * pkmn_pokemon_get_icon_filepath
     */

    error = pkmn_pokemon_get_icon_filepath(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_icon_filepath(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_icon_filepath_out");

    /*
     * pkmn_pokemon_get_sprite_filepath
     */

    error = pkmn_pokemon_get_sprite_filepath(
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_sprite_filepath(
                &pokemon,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_sprite_filepath_out");

    /*
     * pkmn_pokemon_get_numeric_attribute
     */

    error = pkmn_pokemon_get_numeric_attribute(
                NULL,
                strbuffer,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_numeric_attribute(
                &pokemon,
                NULL,
                &dummy_int
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_attribute_name");

    error = pkmn_pokemon_get_numeric_attribute(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_value_out");

    /*
     * pkmn_pokemon_set_numeric_attribute
     */

    error = pkmn_pokemon_set_numeric_attribute(
                NULL,
                strbuffer,
                0
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_numeric_attribute(
                &pokemon,
                NULL,
                0
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_attribute_name");

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
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_string_attribute(
                &pokemon,
                NULL,
                strbuffer,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_attribute_name");

    error = pkmn_pokemon_get_string_attribute(
                &pokemon,
                strbuffer,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_value_out");

    /*
     * pkmn_pokemon_set_string_attribute
     */

    error = pkmn_pokemon_set_string_attribute(
                NULL,
                strbuffer,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_set_string_attribute(
                &pokemon,
                NULL,
                strbuffer
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_attribute_name");

    error = pkmn_pokemon_set_string_attribute(
                &pokemon,
                strbuffer,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_value");

    /*
     * pkmn_pokemon_get_attribute_names
     */

    error = pkmn_pokemon_get_attribute_names(
                NULL,
                &dummy_attribute_names
            );
    TEST_NULL_POINTER_RETURN("p_pokemon");

    error = pkmn_pokemon_get_attribute_names(
                &pokemon,
                NULL
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "p_attribute_names_out");

    // Cleanup
    error = pkmn_pokemon_free(&pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_box.h>
 */
static void pokemon_box_error_test()
{
    struct pkmn_pokemon_box pokemon_box =
    {
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
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
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_pokemon_box_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box_out");

    /*
     * pkmn_pokemon_box_free
     */

    error = pkmn_pokemon_box_free(NULL);
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

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
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_get_name(
                &pokemon_box,
                NULL,
                0,
                &dummy_size_t
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_name_buffer_out");

    /*
     * pkmn_pokemon_box_set_name
     */

    error = pkmn_pokemon_box_set_name(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_set_name(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_name");

    /*
     * pkmn_pokemon_box_get_num_pokemon
     */

    error = pkmn_pokemon_box_get_num_pokemon(
                NULL,
                &dummy_size_t
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_get_num_pokemon(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_num_pokemon_out");

    /*
     * pkmn_pokemon_box_get_pokemon
     */

    error = pkmn_pokemon_box_get_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_get_pokemon(
                &pokemon_box,
                0,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_pokemon_out");

    /*
     * pkmn_pokemon_box_set_pokemon
     */

    error = pkmn_pokemon_box_set_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_set_pokemon(
                &pokemon_box,
                0,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_pokemon");

    /*
     * pkmn_pokemon_box_as_list
     */

    error = pkmn_pokemon_box_as_list(
                NULL,
                &dummy_pokemon_list
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_box");

    error = pkmn_pokemon_box_as_list(
                &pokemon_box,
                NULL
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "p_pokemon_list_out");

    // Cleanup
    error = pkmn_pokemon_box_free(&pokemon_box);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_party.h>
 */
static void pokemon_party_error_test()
{
    struct pkmn_pokemon_party pokemon_party =
    {
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
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
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_pokemon_party_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_party_out");

    /*
     * pkmn_pokemon_party_free
     */

    error = pkmn_pokemon_party_free(NULL);
    TEST_NULL_POINTER_RETURN("p_pokemon_party");

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
    TEST_NULL_POINTER_RETURN("p_pokemon_party");

    error = pkmn_pokemon_party_get_num_pokemon(
                &pokemon_party,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "p_num_pokemon_out");

    /*
     * pkmn_pokemon_party_get_pokemon
     */

    error = pkmn_pokemon_party_get_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_party");

    error = pkmn_pokemon_party_get_pokemon(
                &pokemon_party,
                0,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "p_pokemon_out");

    /*
     * pkmn_pokemon_party_set_pokemon
     */

    error = pkmn_pokemon_party_set_pokemon(
                NULL,
                0,
                &dummy_pokemon
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_party");

    error = pkmn_pokemon_party_set_pokemon(
                &pokemon_party,
                0,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "p_pokemon");

    /*
     * pkmn_pokemon_party_as_list
     */

    error = pkmn_pokemon_party_as_list(
                NULL,
                &dummy_pokemon_list
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_party");

    error = pkmn_pokemon_party_as_list(
                &pokemon_party,
                NULL
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "p_pokemon_list_out");

    // Cleanup
    error = pkmn_pokemon_party_free(&pokemon_party);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

/*
 * <pkmn-c/pokemon_pc.h>
 */
static void pokemon_pc_error_test()
{
    struct pkmn_pokemon_pc pokemon_pc =
    {
        .p_game = NULL,
        .capacity = 0,
        .p_internal = NULL
    };
    struct pkmn_pokemon_box dummy_pokemon_box = {NULL, 0, NULL};
    struct pkmn_pokemon_box_list dummy_pokemon_box_list = {NULL, 0};

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
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_pokemon_pc_init(
                "Red",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_pc_out");

    /*
     * pkmn_pokemon_pc_free
     */

    error = pkmn_pokemon_pc_free(NULL);
    TEST_NULL_POINTER_RETURN("p_pokemon_pc");

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
    TEST_NULL_POINTER_RETURN("p_pokemon_pc");

    error = pkmn_pokemon_pc_get_box(
                &pokemon_pc,
                0,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "p_pokemon_box_out");

    /*
     * pkmn_pokemon_pc_get_box_names
     */

    error = pkmn_pokemon_pc_get_box_names(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_pc");

    error = pkmn_pokemon_pc_get_box_names(
                &pokemon_pc,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "p_box_names_out");

    /*
     * pkmn_pokemon_pc_as_list
     */

    error = pkmn_pokemon_pc_as_list(
                NULL,
                &dummy_pokemon_box_list
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_pc");

    error = pkmn_pokemon_pc_as_list(
                &pokemon_pc,
                NULL
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "p_pokemon_box_list_out");

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
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_form_out");

    /*
     * pkmn_calculations_gen3_unown_form
     */

    error = pkmn_calculations_gen3_unown_form(
                0,
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_form_out");

    /*
     * pkmn_calculations_wurmple_becomes_silcoon
     */

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0,
                false,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_evolves_out");
}

/*
 * <pkmn-c/calculations/gender.h>
 */
static void calculations_gender_error_test() {
    enum pkmn_gender dummy_pkmn_gender = PKMN_GENDER_MALE;

    /*
     * pkmn_calculations_gen2_pokemon_gender
     */

    error = pkmn_calculations_gen2_pokemon_gender(
                NULL,
                0,
                &dummy_pkmn_gender
            );
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_calculations_gen2_pokemon_gender(
                strbuffer,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_gender_out");

    /*
     * pkmn_calculations_modern_pokemon_gender
     */

    error = pkmn_calculations_modern_pokemon_gender(
                NULL,
                0,
                &dummy_pkmn_gender
            );
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_calculations_modern_pokemon_gender(
                strbuffer,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_gender_out");
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
                NULL,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_nature_out");
}

/*
 * <pkmn-c/calculations/personality.h>
 */
static void calculations_personality_error_test() {
    /*
     * pkmn_calculations_generate_personality
     */

    error = pkmn_calculations_generate_personality(
                NULL,
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                NULL,
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("p_ability");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                NULL,
                &dummy_uint32
            );
    TEST_NULL_POINTER_RETURN("p_nature");

    error = pkmn_calculations_generate_personality(
                strbuffer,
                0,
                false,
                strbuffer,
                PKMN_GENDER_GENDERLESS,
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_personality_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_is_shiny_out");

    /*
     * pkmn_calculations_modern_shiny
     */

    error = pkmn_calculations_modern_shiny(
                0,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_is_shiny_out");
}

/*
 * <pkmn-c/calculations/size.h>
 */
static void calculations_size_error_test() {
    /*
     * pkmn_calculations_pokemon_size
     */

    error = pkmn_calculations_pokemon_size(
                NULL,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_calculations_pokemon_size(
                strbuffer,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_size_out");
}

/*
 * <pkmn-c/calculations/spinda_spots.h>
 */
static void calculations_spinda_spots_error_test() {
    struct pkmn_spinda_coords dummy_coords;
    struct pkmn_spinda_spots dummy_spots;

    /*
     * pkmn_calculations_add_spinda_coords
     */

    error = pkmn_calculations_add_spinda_coords(
                NULL,
                &dummy_coords,
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("p_coords1");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                NULL,
                &dummy_coords
            );
    TEST_NULL_POINTER_RETURN("p_coords2");

    error = pkmn_calculations_add_spinda_coords(
                &dummy_coords,
                &dummy_coords,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_result_out");

    /*
     * pkmn_calculations_add_spinda_spots
     */

    error = pkmn_calculations_add_spinda_spots(
                NULL,
                &dummy_spots,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("p_spots1");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                NULL,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("p_spots2");

    error = pkmn_calculations_add_spinda_spots(
                &dummy_spots,
                &dummy_spots,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_result_out");

    /*
     * pkmn_calculations_add_spinda_coords_to_spots
     */

    error = pkmn_calculations_add_spinda_coords_to_spots(
                NULL,
                &dummy_coords,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("p_spots");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                NULL,
                &dummy_spots
            );
    TEST_NULL_POINTER_RETURN("p_coords");

    error = pkmn_calculations_add_spinda_coords_to_spots(
                &dummy_spots,
                &dummy_coords,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_result_out");

    /*
     * pkmn_calculations_spinda_spot_offset
     */

    error = pkmn_calculations_spinda_spot_offset(
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_spot_offset_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_stat_out");

    /*
     * pkmn_calculations_gb_stat_range
     */

    error = pkmn_calculations_gb_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_stat_range_out");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_stat_out");

    /*
     * pkmn_calculations_modern_stat_range
     */

    error = pkmn_calculations_modern_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_stat_range_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_critical_hit_chance_out");

    /*
     * pkmn_calculations_critical_hit_chance
     */

    error = pkmn_calculations_critical_hit_chance(
                2,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_critical_hit_chance_out");

    /*
     * pkmn_calculations_gen1_critical_hit_modifier
     */

    error = pkmn_calculations_gen1_critical_hit_modifier(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_critical_hit_modifier_out");

    /*
     * pkmn_calculations_critical_hit_modifier
     */

    error = pkmn_calculations_critical_hit_modifier(
                2,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_critical_hit_modifier_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_damage_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_hidden_power_out");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_hidden_power_out");
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
                NULL,
                "",
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("p_attacking_type");

    error = pkmn_calculations_type_damage_modifier(
                1,
                "",
                NULL,
                &dummy_float
            );
    TEST_NULL_POINTER_RETURN("p_defending_type");

    error = pkmn_calculations_type_damage_modifier(
                1,
                "",
                "",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_type_damage_modifier_out");
}

/*
 * <pkmn-c/calculations/moves/natural_gift.h>
 */
static void calculations_moves_natural_gift_error_test()
{
    struct pkmn_natural_gift dummy_natural_gift = { NULL, 0 };

    /*
     * pkmn_calculations_natural_gift_stats
     */

    error = pkmn_calculations_natural_gift_stats(
                NULL,
                5,
                &dummy_natural_gift
            );
    TEST_NULL_POINTER_RETURN("p_item_name");

    error = pkmn_calculations_natural_gift_stats(
                "Oran Berry",
                5,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_natural_gift_stats_out");
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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_brine_power_out");

    /*
     * pkmn_calculations_crush_grip_power
     */

    error = pkmn_calculations_crush_grip_power(
                1,
                1,
                5,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_crush_grip_power_out");

    /*
     * pkmn_calculations_echoed_voice_powers
     */

    error = pkmn_calculations_echoed_voice_powers(
                NULL,
                0ULL,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_powers_buffer");

    /*
     * pkmn_calculations_electro_ball_power
     */

    error = pkmn_calculations_electro_ball_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_electro_ball_power_out");

    /*
     * pkmn_calculations_eruption_power
     */

    error = pkmn_calculations_eruption_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_eruption_power_out");

    /*
     * pkmn_calculations_flail_power
     */

    error = pkmn_calculations_flail_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_flail_power_out");

    /*
     * pkmn_calculations_fling_power
     */

    error = pkmn_calculations_fling_power(
                NULL,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_held_item");

    error = pkmn_calculations_fling_power(
                "",
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_fling_power_out");

    /*
     * pkmn_calculations_frustration_power
     */

    error = pkmn_calculations_frustration_power(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_frustration_power_out");

    /*
     * pkmn_calculations_fury_cutter_powers
     */

    error = pkmn_calculations_fury_cutter_powers(
                2,
                NULL,
                0ULL,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_powers_buffer");

    /*
     * pkmn_calculations_grass_knot_power
     */

    error = pkmn_calculations_grass_knot_power(
                1.0f,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_grass_knot_power_out");

    /*
     * pkmn_calculations_gyro_ball_power
     */

    error = pkmn_calculations_gyro_ball_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_gyro_ball_power_out");

    /*
     * pkmn_calculations_heat_crash_power
     */

    error = pkmn_calculations_heat_crash_power(
                1.0f,
                1.0f,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_heat_crash_power_out");

    /*
     * pkmn_calculations_heavy_slam_power
     */

    error = pkmn_calculations_heavy_slam_power(
                1.0f,
                1.0f,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_heavy_slam_power_out");

    /*
     * pkmn_calculations_ice_ball_powers
     */

    error = pkmn_calculations_ice_ball_powers(
                NULL,
                0ULL,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_powers_buffer");

    /*
     * pkmn_calculations_low_kick_power
     */

    error = pkmn_calculations_low_kick_power(
                1.0f,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_low_kick_power_out");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_power_trip_power_out");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_punishment_power_out");

    /*
     * pkmn_calculations_return_power
     */

    error = pkmn_calculations_return_power(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_return_power_out");

    /*
     * pkmn_calculations_reversal_power
     */

    error = pkmn_calculations_reversal_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_reversal_power_out");

    /*
     * pkmn_calculations_rollout_powers
     */

    error = pkmn_calculations_rollout_powers(
                NULL,
                0ULL,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_powers_buffer");

    /*
     * pkmn_calculations_spit_up_power
     */

    error = pkmn_calculations_spit_up_power(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_spit_up_power_out");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_stored_power_power_out");

    /*
     * pkmn_calculations_triple_kick_powers
     */

    error = pkmn_calculations_triple_kick_powers(
                NULL,
                0ULL,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_powers_buffer");

    /*
     * pkmn_calculations_trump_card_power
     */

    error = pkmn_calculations_trump_card_power(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_trump_card_power_out");

    /*
     * pkmn_calculations_water_spout_power
     */

    error = pkmn_calculations_water_spout_power(
                1,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_water_spout_power_out");

    /*
     * pkmn_calculations_wring_out_power
     */

    error = pkmn_calculations_wring_out_power(
                5,
                5,
                5,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_wring_out_power_out");
}

/*
 * <pkmn-c/database/item_entry.h>
 */
static void database_item_entry_error_test()
{
    struct pkmn_database_item_entry dummy_pkmn_database_item_entry;

    /*
     * pkmn_database_get_item_entry
     */

    error = pkmn_database_get_item_entry(
                NULL,
                strbuffer,
                &dummy_pkmn_database_item_entry
            );
    TEST_NULL_POINTER_RETURN("p_item_name");

    error = pkmn_database_get_item_entry(
                strbuffer,
                NULL,
                &dummy_pkmn_database_item_entry
            );
    TEST_NULL_POINTER_RETURN("p_item_game");

    error = pkmn_database_get_item_entry(
                strbuffer,
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_entry_out");

    /*
     * pkmn_database_item_entry_free
     */

    error = pkmn_database_item_entry_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_entry");
}

/*
 * <pkmn-c/database/lists.h>
 */
static void database_lists_error_test()
{
    /*
     * pkmn_database_ability_list
     */

    error = pkmn_database_ability_list(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_ability_list_out");

    /*
     * pkmn_database_game_list
     */

    error = pkmn_database_game_list(
                1,
                false,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_game_list_out");

    /*
     * pkmn_database_gamecube_shadow_pokemon_list
     */

    error = pkmn_database_gamecube_shadow_pokemon_list(
                false,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_gamecube_shadow_pokemon_list_out");

    /*
     * pkmn_database_item_list
     */

    error = pkmn_database_item_list(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_database_item_list(
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_item_list_out");

    /*
     * pkmn_database_location_list
     */

    error = pkmn_database_location_list(
                NULL,
                false,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_database_location_list(
                strbuffer,
                false,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_location_list_out");

    /*
     * pkmn_database_move_list
     */

    error = pkmn_database_move_list(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_database_move_list(
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_move_list_out");

    /*
     * pkmn_database_nature_list
     */

    error = pkmn_database_nature_list(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_nature_list_out");

    /*
     * pkmn_database_pokemon_list
     */

    error = pkmn_database_pokemon_list(
                1,
                false,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_list_out");

    /*
     * pkmn_database_region_list
     */

    error = pkmn_database_region_list(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_region_list_out");

    /*
     * pkmn_database_ribbon_list
     */

    error = pkmn_database_ribbon_list(
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_ribbon_list_out");

    /*
     * pkmn_database_super_training_medal_list
     */

    error = pkmn_database_super_training_medal_list(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_super_training_medal_list_out");

    /*
     * pkmn_database_type_list
     */

    error = pkmn_database_type_list(
                NULL,
                &dummy_string_list
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_database_type_list(
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_type_list_out");
}

/*
 * <pkmn-c/database/move_entry.h>
 */
static void database_move_entry_error_test()
{
    struct pkmn_database_move_entry dummy_pkmn_database_move_entry;

    /*
     * pkmn_database_get_move_entry
     */

    error = pkmn_database_get_move_entry(
                NULL,
                strbuffer,
                &dummy_pkmn_database_move_entry
            );
    TEST_NULL_POINTER_RETURN("p_move_name");

    error = pkmn_database_get_move_entry(
                strbuffer,
                NULL,
                &dummy_pkmn_database_move_entry
            );
    TEST_NULL_POINTER_RETURN("p_move_game");

    error = pkmn_database_get_move_entry(
                strbuffer,
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_move_entry_out");

    /*
     * pkmn_database_move_entry_free
     */

    error = pkmn_database_move_entry_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_move_entry");
}

/*
 * <pkmn-c/database/pokemon_entry.h>
 */
static void database_pokemon_entry_error_test()
{
    struct pkmn_database_pokemon_entry entry;
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
                NULL,
                strbuffer,
                strbuffer,
                &entry
            );
    TEST_NULL_POINTER_RETURN("p_species");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                NULL,
                strbuffer,
                &entry
            );
    TEST_NULL_POINTER_RETURN("p_game");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                NULL,
                &entry
            );
    TEST_NULL_POINTER_RETURN("p_form");

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                strbuffer,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry_out");

    /*
     * pkmn_database_pokemon_entry_set_form
     */

    error = pkmn_database_pokemon_entry_set_form(
                NULL,
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

    error = pkmn_database_pokemon_entry_set_form(
                &entry,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_form");

    /*
     * pkmn_database_pokemon_entry_experience_at_level
     */

    error = pkmn_database_pokemon_entry_experience_at_level(
                NULL,
                1,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

    error = pkmn_database_pokemon_entry_experience_at_level(
                &entry,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_experience_out");

    /*
     * pkmn_database_pokemon_entry_level_at_experience
     */

    error = pkmn_database_pokemon_entry_level_at_experience(
                NULL,
                1,
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

    error = pkmn_database_pokemon_entry_level_at_experience(
                &entry,
                1,
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_level_out");

    /*
     * pkmn_database_pokemon_entry_icon_filepath
     */

    error = pkmn_database_pokemon_entry_icon_filepath(
                NULL,
                false,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

    error = pkmn_database_pokemon_entry_icon_filepath(
                &entry,
                false,
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_icon_filepath_out");

    /*
     * pkmn_database_pokemon_entry_sprite_filepath
     */

    error = pkmn_database_pokemon_entry_sprite_filepath(
                NULL,
                false,
                false,
                strbuffer,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

    error = pkmn_database_pokemon_entry_sprite_filepath(
                &entry,
                false,
                false,
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_sprite_filepath_out");

    /*
     * pkmn_database_pokemon_entry_free
     */

    error = pkmn_database_pokemon_entry_free(
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_pokemon_entry");

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
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_filepath");
}

/*
 * <pkmn-c/types/attribute_names.h>
 */
static void types_attribute_names_test()
{
    /*
     * pkmn_attribute_names_free
     */
    error = pkmn_attribute_names_free(NULL);
    TEST_NULL_POINTER_RETURN("p_attribute_names");
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
    TEST_NULL_POINTER_RETURN("p_hidden_power");
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
    TEST_NULL_POINTER_RETURN("p_item_slot");

    /*
     * pkmn_item_slots_free
     */
    error = pkmn_item_slots_free(NULL);
    TEST_NULL_POINTER_RETURN("p_item_slots");
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
    TEST_NULL_POINTER_RETURN("p_levelup_move");

    /*
     * pkmn_levelup_moves_free
     */
    error = pkmn_levelup_moves_free(NULL);
    TEST_NULL_POINTER_RETURN("p_levelup_moves");
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
    TEST_NULL_POINTER_RETURN("p_move_slot");

    /*
     * pkmn_move_slots_free
     */
    error = pkmn_move_slots_free(NULL);
    TEST_NULL_POINTER_RETURN("p_move_slots");
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
    TEST_NULL_POINTER_RETURN("p_natural_gift");
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
    TEST_NULL_POINTER_RETURN("p_pokemon_box_list");
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
    TEST_NULL_POINTER_RETURN("p_pokemon_list");
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
    TEST_NULL_POINTER_RETURN("p_string_list");

    /*
     * pkmn_string_pair_free
     */
    error = pkmn_string_pair_free(NULL);
    TEST_NULL_POINTER_RETURN("p_string_pair");
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
    TEST_NULL_POINTER_RETURN("p_trainer_info");
}

/*
 * <pkmn-c/utils/paths.h>
 */
static void utils_paths_error_test() {
    /*
     * pkmn_get_appdata_dir
     */

    error = pkmn_get_appdata_dir(
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_appdata_dir_out");

    /*
     * pkmn_get_database_path
     */

    error = pkmn_get_database_path(
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_database_path_out");

    /*
     * pkmn_get_images_dir
     */

    error = pkmn_get_images_dir(
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_images_dir_out");

    /*
     * pkmn_get_tmp_dir
     */

    error = pkmn_get_tmp_dir(
                NULL,
                sizeof(strbuffer),
                NULL
            );
    TEST_NULL_POINTER_RETURN("p_tmp_dir_out");
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
    PKMN_C_TEST(types_attribute_names_test)
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
