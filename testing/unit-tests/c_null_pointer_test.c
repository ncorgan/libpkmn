/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

static pkmn_error_t error = PKMN_ERROR_NONE;

#define STRBUFFER_LEN 1024
static char strbuffer[STRBUFFER_LEN] = {0};
static int dummy_int = 0;
static size_t dummy_size_t = 0;
static pkmn_string_list_t dummy_pkmn_string_list_t = {
    .strings = NULL,
    .length = 0
};

/*
 * <pkmn-c/build_info.h>
 */
static void build_info_null_pointer_test() {
    /*
     * pkmn_get_boost_version
     */

    error = pkmn_get_boost_version(
                NULL, // boost_version_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_boost_version(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_pksav_version
     */

    error = pkmn_get_pksav_version(
                NULL, // pksav_version_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_pksav_version(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_sqlite3_version
     */

    error = pkmn_get_sqlite3_version(
                NULL, // sqlite3_version_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_sqlite3_version(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_sqlitecpp_version
     */

    error = pkmn_get_sqlitecpp_version(
                NULL, // sqlitecpp_version_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_sqlitecpp_version(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/item_bag.h>
 */
static void item_bag_null_pointer_test() {
    pkmn_item_bag_handle_t item_bag;
    pkmn_item_bag_handle_t null_item_bag = NULL;

    pkmn_item_list_handle_t item_list;
    pkmn_item_list_handle_t null_item_list = NULL;

    /*
     * pkmn_item_bag_make
     */

    error = pkmn_item_bag_make(
                NULL, // handle_ptr
                "Red"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_make(
                &item_bag,
                NULL // game_name
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_bag_free
     */

    error = pkmn_item_bag_free(
                NULL // handle_ptr
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_free(
                &null_item_bag // handle_ptr
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

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
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_game(
                item_bag,
                NULL, // game_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_game(
                item_bag,
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_bag_get_pocket
     */

    error = pkmn_item_bag_get_pocket(
                NULL, // handle
                "Items",
                &item_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_pocket(
                item_bag,
                NULL, // name
                &item_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_pocket(
                item_bag,
                "Items",
                NULL // item_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_pocket(
                NULL, // handle
                "Items",
                &null_item_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_bag_get_pocket_names
     */

    error = pkmn_item_bag_get_pocket_names(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_get_pocket_names(
                item_bag,
                NULL // pocket_names_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_bag_add
     */

    error = pkmn_item_bag_add(
                NULL, // handle
                "Potion",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_add(
                item_bag,
                NULL, // item_name
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_bag_remove
     */

    error = pkmn_item_bag_remove(
                NULL, // handle
                "Potion",
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_bag_remove(
                item_bag,
                NULL, // item_name
                5
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/item_list.h>
 */
static void item_list_null_pointer_test() {
    pkmn_item_list_handle_t item_list;
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_make(
                &item_list,
                NULL, // item_list_name
                "Red"
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_make(
                &item_list,
                "Items",
                NULL // game_name
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_free
     */

    error = pkmn_item_list_free(
                NULL // handle_ptr
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_free(
                &null_item_list
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

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
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_name(
                item_list,
                NULL, // name_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_name(
                item_list,
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_get_game
     */

    error = pkmn_item_list_get_game(
                NULL, // handle
                strbuffer,
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_game(
                item_list,
                NULL, // game_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_game(
                item_list,
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_get_capacity
     */

    error = pkmn_item_list_get_capacity(
                NULL, // handle
                &dummy_int
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_capacity(
                item_list,
                NULL // capacity_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_get_num_items
     */

    error = pkmn_item_list_get_num_items(
                NULL, // handle
                &dummy_int
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_num_items(
                item_list,
                NULL // num_items_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_at
     */

    error = pkmn_item_list_at(
                NULL, // handle
                0,
                &dummy_pkmn_item_slot_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_at(
                item_list,
                0,
                NULL // item_slot_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_add
     */

    error = pkmn_item_list_add(
                NULL, // handle
                "Potion",
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_add(
                item_list,
                NULL, // item_name
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_remove
     */

    error = pkmn_item_list_remove(
                NULL, // handle
                "Potion",
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_remove(
                item_list,
                NULL, // item_name
                0
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_move
     */

    error = pkmn_item_list_move(
                NULL, // handle
                0,
                1
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_get_valid_items
     */

    error = pkmn_item_list_get_valid_items(
                NULL, // handle
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_get_valid_items(
                item_list,
                NULL // string_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_item_list_as_array
     */

    error = pkmn_item_list_as_array(
                NULL, // handle
                &dummy_pkmn_item_slots_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_item_list_as_array(
                item_list,
                NULL // array_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/form.h>
 */
static void calculations_form_null_pointer_test() {
    /*
     * pkmn_calculations_gen2_unown_form
     */

    error = pkmn_calculations_gen2_unown_form(
                0,
                0,
                0,
                0,
                NULL, // form_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_calculations_gen2_unown_form(
                0,
                0,
                0,
                0,
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_gen3_unown_form
     */

    error = pkmn_calculations_gen3_unown_form(
                0,
                NULL, // form_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_calculations_gen3_unown_form(
                0,
                strbuffer,
                STRBUFFER_LEN,
                NULL
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_wurmple_becomes_silcoon
     */

    error = pkmn_calculations_wurmple_becomes_silcoon(
                0,
                false,
                NULL // evolves_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/gender.h>
 */
static void calculations_gender_null_pointer_test() {
    pkmn_gender_t dummy_pkmn_gender_t = PKMN_MALE;

    /*
     * pkmn_calculations_gen2_pokemon_gender
     */

    error = pkmn_calculations_gen2_pokemon_gender(
                NULL, // species
                0,
                &dummy_pkmn_gender_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_calculations_gen2_pokemon_gender(
                strbuffer,
                0,
                NULL // gender_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_modern_pokemon_gender
     */

    error = pkmn_calculations_modern_pokemon_gender(
                NULL, // species
                0,
                &dummy_pkmn_gender_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_calculations_modern_pokemon_gender(
                strbuffer,
                0,
                NULL // gender_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/hidden_power.h>
 */
static void calculations_hidden_power_null_pointer_test() {
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/shininess.h>
 */
static void calculations_shininess_null_pointer_test() {
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_modern_shiny
     */

    error = pkmn_calculations_modern_shiny(
                0,
                0,
                NULL // shiny_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/spinda_spots.h>
 */
static void calculations_spinda_spots_null_pointer_test() {
    /*
     * pkmn_calculations_spinda_spot_offset
     */

    error = pkmn_calculations_spinda_spot_offset(
                0,
                NULL // spinda_spots_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/calculations/stats.h>
 */
static void calculations_stats_null_pointer_test() {
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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_gb_stat_range
     */

    error = pkmn_calculations_gb_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL // stat_range_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

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
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_calculations_modern_stat_range
     */

    error = pkmn_calculations_modern_stat_range(
                PKMN_STAT_HP,
                0,
                0,
                NULL // stat_range_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/database/item_entry.h>
 */
static void database_item_entry_null_pointer_test() {
    pkmn_database_item_entry_t dummy_pkmn_database_item_entry_t;

    /*
     * pkmn_database_get_item_entry
     */

    error = pkmn_database_get_item_entry(
                NULL, // item_name
                strbuffer,
                &dummy_pkmn_database_item_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_item_entry(
                strbuffer,
                NULL, // item_game
                &dummy_pkmn_database_item_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_item_entry(
                strbuffer,
                strbuffer,
                NULL // item_entry_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_item_entry_free
     */

    error = pkmn_database_item_entry_free(
                NULL // item_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/database/lists.h>
 */
static void database_lists_null_pointer_test() {
    /*
     * pkmn_database_ability_list
     */

    error = pkmn_database_ability_list(
                1,
                NULL // ability_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_game_list
     */

    error = pkmn_database_game_list(
                1,
                false,
                NULL // game_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_item_list
     */

    error = pkmn_database_item_list(
                NULL, // game
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_item_list(
                strbuffer,
                NULL // item_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_location_list
     */

    error = pkmn_database_location_list(
                NULL, // game
                false,
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_location_list(
                strbuffer,
                false,
                NULL // location_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_move_list
     */

    error = pkmn_database_move_list(
                NULL, // game
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_move_list(
                strbuffer,
                NULL // move_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_nature_list
     */

    error = pkmn_database_nature_list(
                NULL // nature_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_pokemon_list
     */

    error = pkmn_database_pokemon_list(
                1,
                false,
                NULL // pokemon_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_region_list
     */

    error = pkmn_database_region_list(
                NULL // region_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_ribbon_list
     */

    error = pkmn_database_ribbon_list(
                1,
                NULL // ribbon_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_super_training_medal_list
     */

    error = pkmn_database_super_training_medal_list(
                NULL // super_training_medal_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_type_list
     */

    error = pkmn_database_type_list(
                NULL, // game
                &dummy_pkmn_string_list_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_type_list(
                strbuffer,
                NULL // type_list_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/database/move_entry.h>
 */
static void database_move_entry_null_pointer_test() {
    pkmn_database_move_entry_t dummy_pkmn_database_move_entry_t;

    /*
     * pkmn_database_get_move_entry
     */

    error = pkmn_database_get_move_entry(
                NULL, // move_name
                strbuffer,
                &dummy_pkmn_database_move_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_move_entry(
                strbuffer,
                NULL, // move_game
                &dummy_pkmn_database_move_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_move_entry(
                strbuffer,
                strbuffer,
                NULL // move_entry_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_move_entry_free
     */

    error = pkmn_database_move_entry_free(
                NULL // move_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/database/pokemon_entry.h>
 */
static void database_pokemon_entry_null_pointer_test() {
    pkmn_database_pokemon_entry_t dummy_pkmn_database_pokemon_entry_t;

    /*
     * pkmn_database_get_pokemon_entry
     */

    error = pkmn_database_get_pokemon_entry(
                NULL, // species
                strbuffer,
                strbuffer,
                &dummy_pkmn_database_pokemon_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                NULL, // game
                strbuffer,
                &dummy_pkmn_database_pokemon_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                NULL, // form
                &dummy_pkmn_database_pokemon_entry_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_get_pokemon_entry(
                strbuffer,
                strbuffer,
                strbuffer,
                NULL // pokemon_entry_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_pokemon_entry_set_form
     */

    error = pkmn_database_pokemon_entry_set_form(
                NULL, // pokemon_entry
                strbuffer
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_pokemon_entry_set_form(
                &dummy_pkmn_database_pokemon_entry_t,
                NULL // form
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_pokemon_entry_experience_at_level
     */

    error = pkmn_database_pokemon_entry_experience_at_level(
                NULL, // pokemon_entry
                1,
                &dummy_int
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_pokemon_entry_experience_at_level(
                &dummy_pkmn_database_pokemon_entry_t,
                1,
                NULL // experience_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_pokemon_entry_level_at_experience
     */

    error = pkmn_database_pokemon_entry_level_at_experience(
                NULL, // pokemon_entry
                1,
                &dummy_int
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_database_pokemon_entry_level_at_experience(
                &dummy_pkmn_database_pokemon_entry_t,
                1,
                NULL // level_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_database_pokemon_entry_free
     */

    error = pkmn_database_pokemon_entry_free(
                NULL // pokemon_entry
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

/*
 * <pkmn-c/utils/paths.h>
 */
static void utils_paths_null_pointer_test() {
    /*
     * pkmn_get_appdata_dir
     */

    error = pkmn_get_appdata_dir(
                NULL, // appdata_dir_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_appdata_dir(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_database_path
     */

    error = pkmn_get_database_path(
                NULL, // database_path_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_database_path(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_images_dir
     */

    error = pkmn_get_images_dir(
                NULL, // images_dir_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_images_dir(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    /*
     * pkmn_get_tmp_dir
     */

    error = pkmn_get_tmp_dir(
                NULL, // tmp_dir_out
                STRBUFFER_LEN,
                &dummy_size_t
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);

    error = pkmn_get_tmp_dir(
                strbuffer,
                STRBUFFER_LEN,
                NULL // actual_strlen_out
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NULL_POINTER);
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(build_info_null_pointer_test)
    PKMN_C_TEST(item_bag_null_pointer_test)
    PKMN_C_TEST(item_list_null_pointer_test)
    PKMN_C_TEST(calculations_form_null_pointer_test)
    PKMN_C_TEST(calculations_gender_null_pointer_test)
    PKMN_C_TEST(calculations_hidden_power_null_pointer_test)
    PKMN_C_TEST(calculations_shininess_null_pointer_test)
    PKMN_C_TEST(calculations_spinda_spots_null_pointer_test)
    PKMN_C_TEST(calculations_stats_null_pointer_test)
    PKMN_C_TEST(database_item_entry_null_pointer_test)
    PKMN_C_TEST(database_lists_null_pointer_test)
    PKMN_C_TEST(database_move_entry_null_pointer_test)
    PKMN_C_TEST(database_pokemon_entry_null_pointer_test)
    PKMN_C_TEST(utils_paths_null_pointer_test)
)
