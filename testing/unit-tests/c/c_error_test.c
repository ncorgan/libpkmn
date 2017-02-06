/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static pkmn_error_t error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static bool dummy_bool = 0;
static int dummy_int = 0;
static uint16_t dummy_uint16_t = 0;
static uint32_t dummy_uint32_t = 0;
static float dummy_float = 0;
static pkmn_gender_t dummy_pkmn_gender_t = PKMN_MALE;
static pkmn_string_list_t dummy_pkmn_string_list_t = {
    .strings = NULL,
    .length = 0
};
static pkmn_pokemon_handle_t dummy_pokemon = NULL;

static const char* null_pointer_error_format = "Null pointer passed into parameter \"%s\"";

#define TEST_NULL_POINTER_RETURN(param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
}

#define TEST_ITEM_BAG_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_item_bag_strerror(handle), strbuffer); \
}

#define TEST_ITEM_LIST_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_item_list_strerror(handle), strbuffer); \
}

#define TEST_POKEMON_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_pokemon_strerror(handle), strbuffer); \
}

#define TEST_POKEMON_BOX_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_pokemon_box_strerror(handle), strbuffer); \
}

#define TEST_POKEMON_PARTY_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_pokemon_party_strerror(handle), strbuffer); \
}

#define TEST_POKEMON_PC_NULL_POINTER_RETURN(handle, param_name) \
{ \
    snprintf(strbuffer, sizeof(strbuffer), null_pointer_error_format, param_name); \
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER); \
    TEST_ASSERT_EQUAL_STRING(pkmn_strerror(), strbuffer); \
    TEST_ASSERT_EQUAL_STRING(pkmn_pokemon_pc_strerror(handle), strbuffer); \
}

/*
 * <pkmn-c/build_info.h>
 */
static void build_info_error_test() {
    /*
     * pkmn_get_boost_version
     */

    error = pkmn_get_boost_version(
                NULL, // boost_version_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("boost_version_out");

    error = pkmn_get_boost_version(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_pksav_version
     */

    error = pkmn_get_pksav_version(
                NULL, // pksav_version_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("pksav_version_out");

    error = pkmn_get_pksav_version(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_sqlite3_version
     */

    error = pkmn_get_sqlite3_version(
                NULL, // sqlite3_version_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("sqlite3_version_out");

    error = pkmn_get_sqlite3_version(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_sqlitecpp_version
     */

    error = pkmn_get_sqlitecpp_version(
                NULL, // sqlitecpp_version_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("sqlitecpp_version_out");

    error = pkmn_get_sqlitecpp_version(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);
}

/*
 * <pkmn-c/item_bag.h>
 */
static void item_bag_error_test() {
    pkmn_item_bag_handle_t item_bag = NULL;
    error = pkmn_item_bag_make(
                &item_bag,
                "Red"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_item_bag_handle_t null_item_bag = NULL;

    pkmn_item_list_handle_t item_list = NULL;

    /*
     * pkmn_item_bag_make
     */

    error = pkmn_item_bag_make(
                NULL, // handle_ptr
                "Red"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_item_bag_make(
                &item_bag,
                NULL // game_name
            );
    TEST_NULL_POINTER_RETURN("game_name");

    /*
     * pkmn_item_bag_free
     */

    error = pkmn_item_bag_free(
                NULL // handle_ptr
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_item_bag_free(
                &null_item_bag
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_item_bag_strerror
     */

    const char* item_bag_strerror = pkmn_item_bag_strerror(
                                        NULL // handle
                                    );
    TEST_ASSERT_NULL(item_bag_strerror);

    /*
     * pkmn_item_bag_get_game
     */

    error = pkmn_item_bag_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_bag_get_game(
                item_bag,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "game_out");

    error = pkmn_item_bag_get_game(
                item_bag,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_item_bag_get_pocket
     */

    error = pkmn_item_bag_get_pocket(
                NULL, // handle
                "Items",
                &item_list
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_bag_get_pocket(
                item_bag,
                NULL, // name
                &item_list
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "name");

    error = pkmn_item_bag_get_pocket(
                item_bag,
                "Items",
                NULL // item_list_out
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "item_list_out");

    /*
     * pkmn_item_bag_get_pocket_names
     */

    error = pkmn_item_bag_get_pocket_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_bag_get_pocket_names(
                item_bag,
                NULL // pocket_names_out
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "pocket_names_out");

    /*
     * pkmn_item_bag_add
     */

    error = pkmn_item_bag_add(
                NULL, // handle
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_bag_add(
                item_bag,
                NULL, // name
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "name");

    /*
     * pkmn_item_bag_remove
     */

    error = pkmn_item_bag_remove(
                NULL, // handle
                "Potion",
                5
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_bag_remove(
                item_bag,
                NULL, // name
                5
            );
    TEST_ITEM_BAG_NULL_POINTER_RETURN(item_bag, "name");

    error = pkmn_item_bag_free(&item_bag);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
}

/*
 * <pkmn-c/item_list.h>
 */
static void item_list_error_test() {
    pkmn_item_list_handle_t item_list = NULL;
    error = pkmn_item_list_make(
                &item_list,
                "Items",
                "Red"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_item_list_handle_t null_item_list = NULL;

    pkmn_item_slot_t dummy_pkmn_item_slot_t = {
        .item = NULL,
        .amount = 0
    };

    pkmn_item_slots_t dummy_pkmn_item_slots_t = {
        .item_slots = NULL,
        .length = 0
    };

    /*
     * pkmn_item_list_make
     */

    error = pkmn_item_list_make(
                NULL, // handle_ptr
                "Items",
                "Red"
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_item_list_make(
                &item_list,
                NULL, // item_list_name
                "Red"
            );
    TEST_NULL_POINTER_RETURN("item_list_name");

    error = pkmn_item_list_make(
                &item_list,
                "Items",
                NULL // game_name
            );
    TEST_NULL_POINTER_RETURN("game_name");

    /*
     * pkmn_item_list_free
     */

    error = pkmn_item_list_free(
                NULL // handle_ptr
            );
    TEST_NULL_POINTER_RETURN("handle_ptr");

    error = pkmn_item_list_free(
                &null_item_list
            );
    TEST_NULL_POINTER_RETURN("(*handle_ptr)");

    /*
     * pkmn_item_list_strerror
     */

    const char* item_list_strerror = pkmn_item_list_strerror(
                                         NULL // handle
                                     );
    TEST_ASSERT_NULL(item_list_strerror);

    /*
     * pkmn_item_list_get_name
     */

    error = pkmn_item_list_get_name(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_get_name(
                item_list,
                NULL, // name_out
                sizeof(strbuffer)
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "name_out");

    error = pkmn_item_list_get_name(
                item_list,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_item_list_get_game
     */

    error = pkmn_item_list_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_get_game(
                item_list,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "game_out");

    error = pkmn_item_list_get_game(
                item_list,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_item_list_get_capacity
     */

    error = pkmn_item_list_get_capacity(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_get_capacity(
                item_list,
                NULL // capacity_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "capacity_out");

    /*
     * pkmn_item_list_get_num_items
     */

    error = pkmn_item_list_get_num_items(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_get_num_items(
                item_list,
                NULL // num_items_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "num_items_out");

    /*
     * pkmn_item_list_at
     */

    error = pkmn_item_list_at(
                NULL, // handle
                0,
                &dummy_pkmn_item_slot_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_at(
                item_list,
                0,
                NULL // item_slot_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "item_slot_out");

    /*
     * pkmn_item_list_add
     */

    error = pkmn_item_list_add(
                NULL, // handle
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_add(
                item_list,
                NULL, // name
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "name");

    /*
     * pkmn_item_list_remove
     */

    error = pkmn_item_list_remove(
                NULL, // handle
                "Potion",
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_remove(
                item_list,
                NULL, // name
                0
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "name");

    /*
     * pkmn_item_list_move
     */

    error = pkmn_item_list_move(
                NULL, // handle
                0,
                1
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_item_list_get_valid_items
     */

    error = pkmn_item_list_get_valid_items(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_get_valid_items(
                item_list,
                NULL // string_list_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "string_list_out");

    /*
     * pkmn_item_list_as_array
     */

    error = pkmn_item_list_as_array(
                NULL, // handle
                &dummy_pkmn_item_slots_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_item_list_as_array(
                item_list,
                NULL // array_out
            );
    TEST_ITEM_LIST_NULL_POINTER_RETURN(item_list, "array_out");

    error = pkmn_item_list_free(&item_list);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

    pkmn_move_slots_t dummy_pkmn_move_slots_t = {
        .move_slots = NULL,
        .length = 0
    };
    pkmn_database_pokemon_entry_t pokemon_entry;

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_species(
                pokemon,
                NULL, // species_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "species_out");

    error = pkmn_pokemon_get_species(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_pokemon_get_game
     */

    error = pkmn_pokemon_get_game(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_game(
                pokemon,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "game_out");

    /*
     * pkmn_pokemon_get_form
     */

    error = pkmn_pokemon_get_form(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_form(
                pokemon,
                NULL, // form_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "form_out");

    error = pkmn_pokemon_get_form(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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

    error = pkmn_pokemon_get_game(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_nickname(
                pokemon,
                NULL, // nickname_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "nickname_out");

    error = pkmn_pokemon_get_nickname(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_held_item(
                pokemon,
                NULL, // held_item_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "held_item_out");

    error = pkmn_pokemon_get_held_item(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
     * pkmn_pokemon_get_trainer_name
     */

    error = pkmn_pokemon_get_trainer_name(
                NULL, // handle
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                NULL, // trainer_name_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_name_out");

    error = pkmn_pokemon_get_trainer_name(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_pokemon_set_trainer_name
     */

    error = pkmn_pokemon_set_trainer_name(
                NULL, // handle
                strbuffer
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_set_trainer_name(
                pokemon,
                NULL // trainer_name
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_name");

    /*
     * pkmn_pokemon_get_trainer_public_id
     */

    error = pkmn_pokemon_get_trainer_public_id(
                NULL, // handle
                &dummy_uint16_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_trainer_public_id(
                pokemon,
                NULL // trainer_public_id_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_public_id_out");

    /*
     * pkmn_pokemon_set_trainer_public_id
     */

    error = pkmn_pokemon_set_trainer_public_id(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_trainer_secret_id
     */

    error = pkmn_pokemon_get_trainer_secret_id(
                NULL, // handle
                &dummy_uint16_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_trainer_secret_id(
                pokemon,
                NULL // trainer_secret_id_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_secret_id_out");

    /*
     * pkmn_pokemon_set_trainer_secret_id
     */

    error = pkmn_pokemon_set_trainer_secret_id(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_trainer_id
     */

    error = pkmn_pokemon_get_trainer_id(
                NULL, // handle
                &dummy_uint32_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_trainer_id(
                pokemon,
                NULL // trainer_id_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_id_out");

    /*
     * pkmn_pokemon_set_trainer_id
     */

    error = pkmn_pokemon_set_trainer_id(
                NULL, // handle
                0
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_trainer_gender
     */

    error = pkmn_pokemon_get_trainer_gender(
                NULL, // handle
                &dummy_pkmn_gender_t
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_trainer_gender(
                pokemon,
                NULL // trainer_gender_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "trainer_gender_out");

    /*
     * pkmn_pokemon_set_trainer_gender
     */

    error = pkmn_pokemon_set_trainer_gender(
                NULL, // handle
                PKMN_MALE
            );
    TEST_NULL_POINTER_RETURN("handle");

    /*
     * pkmn_pokemon_get_friendship
     */

    error = pkmn_pokemon_get_friendship(
                NULL, // handle
                &dummy_int
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_friendship(
                pokemon,
                NULL // friendship_out
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "friendship_out");

    /*
     * pkmn_pokemon_set_friendship
     */

    error = pkmn_pokemon_set_friendship(
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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_ability(
                pokemon,
                NULL, // ability_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ability_out");

    error = pkmn_pokemon_get_ability(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_ball(
                pokemon,
                NULL, // ball_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "ball_out");

    error = pkmn_pokemon_get_ball(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_location_met(
                pokemon,
                true,
                NULL, // location_met_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "location_met_out");

    error = pkmn_pokemon_get_location_met(
                pokemon,
                false,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                NULL, // original_game_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_NULL_POINTER_RETURN(pokemon, "original_game_out");

    error = pkmn_pokemon_get_original_game(
                pokemon,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_name(
                pokemon_box,
                NULL, // name_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "name_out");

    error = pkmn_pokemon_box_get_name(
                pokemon_box,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_box_get_game(
                pokemon_box,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_BOX_NULL_POINTER_RETURN(pokemon_box, "game_out");

    error = pkmn_pokemon_box_get_game(
                pokemon_box,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_party_get_game(
                pokemon_party,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_PARTY_NULL_POINTER_RETURN(pokemon_party, "game_out");

    error = pkmn_pokemon_party_get_game(
                pokemon_party,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("handle");

    error = pkmn_pokemon_pc_get_game(
                pokemon_pc,
                NULL, // game_out
                sizeof(strbuffer)
            );
    TEST_POKEMON_PC_NULL_POINTER_RETURN(pokemon_pc, "game_out");

    error = pkmn_pokemon_pc_get_game(
                pokemon_pc,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("form_out");

    error = pkmn_calculations_gen2_unown_form(
                0,
                0,
                0,
                0,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_calculations_gen3_unown_form
     */

    error = pkmn_calculations_gen3_unown_form(
                0,
                NULL, // form_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("form_out");

    error = pkmn_calculations_gen3_unown_form(
                0,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
 * <pkmn-c/calculations/hidden_power.h>
 */
static void calculations_hidden_power_error_test() {
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);

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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_icon_filepath(
                &entry,
                false,
                NULL, // icon_filepath_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("icon_filepath_out");

    error = pkmn_database_pokemon_entry_icon_filepath(
                &entry,
                false,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_database_pokemon_entry_sprite_filepath
     */

    error = pkmn_database_pokemon_entry_sprite_filepath(
                NULL, // pokemon_entry
                false,
                false,
                strbuffer,
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("pokemon_entry");

    error = pkmn_database_pokemon_entry_sprite_filepath(
                &entry,
                false,
                false,
                NULL, // sprite_filepath_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("sprite_filepath_out");

    error = pkmn_database_pokemon_entry_sprite_filepath(
                &entry,
                false,
                false,
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
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
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("appdata_dir_out");

    error = pkmn_get_appdata_dir(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_database_path
     */

    error = pkmn_get_database_path(
                NULL, // database_path_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("database_path_out");

    error = pkmn_get_database_path(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_images_dir
     */

    error = pkmn_get_images_dir(
                NULL, // images_dir_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("images_dir_out");

    error = pkmn_get_images_dir(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);

    /*
     * pkmn_get_tmp_dir
     */

    error = pkmn_get_tmp_dir(
                NULL, // tmp_dir_out
                sizeof(strbuffer)
            );
    TEST_NULL_POINTER_RETURN("tmp_dir_out");

    error = pkmn_get_tmp_dir(
                strbuffer,
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_BUFFER_TOO_SMALL);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(build_info_error_test)
    PKMN_C_TEST(item_bag_error_test)
    PKMN_C_TEST(item_list_error_test)
    PKMN_C_TEST(pokemon_error_test)
    PKMN_C_TEST(pokemon_box_error_test)
    PKMN_C_TEST(pokemon_party_error_test)
    PKMN_C_TEST(pokemon_pc_error_test)
    PKMN_C_TEST(calculations_form_error_test)
    PKMN_C_TEST(calculations_gender_error_test)
    PKMN_C_TEST(calculations_hidden_power_error_test)
    PKMN_C_TEST(calculations_shininess_error_test)
    PKMN_C_TEST(calculations_size_error_test)
    PKMN_C_TEST(calculations_spinda_spots_error_test)
    PKMN_C_TEST(calculations_stats_error_test)
    PKMN_C_TEST(database_item_entry_error_test)
    PKMN_C_TEST(database_lists_error_test)
    PKMN_C_TEST(database_move_entry_error_test)
    PKMN_C_TEST(database_pokemon_entry_error_test)
    PKMN_C_TEST(utils_paths_error_test)
)
