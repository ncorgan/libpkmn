/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmn.h>

#ifdef PKMN_PLATFORM_WIN32
#    include <windows.h>
#    define FS_SEPARATOR "\\"
#else
#    include <unistd.h>
#    define FS_SEPARATOR "/"
#endif

#define STRBUFFER_LEN 1024

#include <stdio.h>

static bool file_exists(
    const char* filepath
)
{
    FILE* file = fopen(filepath, "rb");
    if(file)
    {
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

static void spinda_test()
{
    pkmn_error_t error = PKMN_ERROR_NONE;
    bool successful = false;

    char PKMN_TMP_DIR[STRBUFFER_LEN] = {0};
    error = pkmn_get_tmp_dir(
                PKMN_TMP_DIR,
                sizeof(PKMN_TMP_DIR)
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    for(int generation = 3; generation <= 5; ++generation)
    {
        char filepath[STRBUFFER_LEN] = {0};
        snprintf(
            filepath,
            sizeof(filepath),
            "%s%sspinda_0.png",
            PKMN_TMP_DIR,
            FS_SEPARATOR
        );

        error = pkmn_gui_generate_spinda_sprite_at_filepath(
                    generation,
                    0x88888888,
                    false,
                    filepath,
                    &successful
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(successful);
        TEST_ASSERT_TRUE(file_exists(filepath));

#ifdef PKMN_PLATFORM_WIN32
        TEST_ASSERT_NOT_EQUAL(0, DeleteFile(filepath));
#else
        TEST_ASSERT_EQUAL(0, remove(filepath));
#endif

        snprintf(
            filepath,
            sizeof(filepath),
            "%s%sspinda_1.png",
            PKMN_TMP_DIR,
            FS_SEPARATOR
        );

        error = pkmn_gui_generate_spinda_sprite_at_filepath(
                    generation,
                    0x88888888,
                    true,
                    filepath,
                    &successful
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
        TEST_ASSERT_TRUE(successful);
        TEST_ASSERT_TRUE(file_exists(filepath));

#ifdef PKMN_PLATFORM_WIN32
        TEST_ASSERT_NOT_EQUAL(0, DeleteFile(filepath));
#else
        TEST_ASSERT_EQUAL(0, remove(filepath));
#endif
    }
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(spinda_test)
)
