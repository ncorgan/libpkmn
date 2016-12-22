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
 * <pkmn/item_bag.h>
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
                &null_item_bag,
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

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(build_info_null_pointer_test)
    PKMN_C_TEST(item_bag_null_pointer_test)
)
