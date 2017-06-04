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
#include <string.h>

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

static bool do_files_match(
    const char* filepath1,
    const char* filepath2
)
{
    TEST_ASSERT_TRUE(file_exists(filepath1));
    TEST_ASSERT_TRUE(file_exists(filepath2));

    bool ret = false;

    FILE* file1 = NULL;
    FILE* file2 = NULL;

    size_t size1 = 0;
    size_t size2 = 0;

    void* buffer1 = NULL;
    void* buffer2 = NULL;

    file1 = fopen(filepath1, "rb");
    TEST_ASSERT_NOT_NULL(file1);
    file2 = fopen(filepath2, "rb");
    if(!file2)
    {
        fclose(file1);
        TEST_ASSERT_NOT_NULL(file2);
    }

    fseek(file1, 0, SEEK_END);
    size1 = (size_t)ftell(file1);
    fseek(file1, 0, SEEK_SET);

    fseek(file2, 0, SEEK_END);
    size2 = (size_t)ftell(file2);
    fseek(file2, 0, SEEK_SET);

    if(size1 != size2)
    {
        fclose(file1);
        fclose(file2);
        return false;
    }

    buffer1 = malloc(size1);
    TEST_ASSERT_NOT_NULL(buffer1);
    buffer2 = malloc(size2);
    if(!buffer2)
    {
        free(buffer1);
        TEST_ASSERT_NOT_NULL(buffer2);
    }

    size_t file1_read = fread(buffer1, 1, size1, file1);
    (void)file1_read; // GCC won't let us just do (void)fread
    fclose(file1);
    size_t file2_read = fread(buffer2, 1, size2, file2);
    (void)file2_read; // GCC won't let us just do (void)fread
    fclose(file2);

    ret = (memcmp(buffer1, buffer2, size1) == 0);
    free(buffer1);
    free(buffer2);

    return ret;
}

static inline void delete_file(
    const char* filepath
)
{
#ifdef PKMN_PLATFORM_WIN32
    TEST_ASSERT_NOT_EQUAL(0, DeleteFile(filepath));
#else
    TEST_ASSERT_EQUAL(0, remove(filepath));
#endif
}

static void spinda_test()
{
    pkmn_error_t error = PKMN_ERROR_NONE;

    char qt_version[STRBUFFER_LEN] = {0};
    error = pkmn_get_qt_version(qt_version, sizeof(qt_version));
    TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

    if(strcmp(qt_version, "N/A"))
    {
        const char* SPINDA_FORMAT = "spinda_%d_%u_%d.png";

        const uint32_t personality       = 0x88888888;
        const uint32_t personality_shiny = 0xF81C8021;

        const char* LIBPKMN_TEST_FILES = getenv("LIBPKMN_TEST_FILES");

        char PKMN_TMP_DIR[STRBUFFER_LEN] = {0};
        error = pkmn_get_tmp_dir(
                    PKMN_TMP_DIR,
                    sizeof(PKMN_TMP_DIR)
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);

        for(int generation = 3; generation <= 5; ++generation)
        {
            char spinda_filename[STRBUFFER_LEN] = {0};
            snprintf(
                spinda_filename,
                sizeof(spinda_filename),
                SPINDA_FORMAT,
                generation,
                0,
                personality
            );

            char test_files_spinda_filepath[STRBUFFER_LEN] = {0};
            snprintf(
                test_files_spinda_filepath,
                sizeof(test_files_spinda_filepath),
                "%s%sspinda%s%s",
                LIBPKMN_TEST_FILES,
                FS_SEPARATOR,
                FS_SEPARATOR,
                spinda_filename
            );

            char test_spinda_filepath[STRBUFFER_LEN] = {0};
            snprintf(
                test_spinda_filepath,
                sizeof(test_spinda_filepath),
                "%s%sspinda%s%s",
                LIBPKMN_TEST_FILES,
                FS_SEPARATOR,
                FS_SEPARATOR,
                spinda_filename
            );

            error = pkmn_gui_generate_spinda_sprite_at_filepath(
                        generation,
                        personality,
                        false,
                        test_spinda_filepath
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_TRUE(file_exists(test_spinda_filepath));
            TEST_ASSERT_TRUE(do_files_match(test_files_spinda_filepath, test_spinda_filepath));
            delete_file(test_spinda_filepath);

            snprintf(
                spinda_filename,
                sizeof(spinda_filename),
                SPINDA_FORMAT,
                generation,
                1,
                personality_shiny
            );

            snprintf(
                test_files_spinda_filepath,
                sizeof(test_files_spinda_filepath),
                "%s%sspinda%s%s",
                LIBPKMN_TEST_FILES,
                FS_SEPARATOR,
                FS_SEPARATOR,
                spinda_filename
            );

            snprintf(
                test_spinda_filepath,
                sizeof(test_spinda_filepath),
                "%s%sspinda%s%s",
                LIBPKMN_TEST_FILES,
                FS_SEPARATOR,
                FS_SEPARATOR,
                spinda_filename
            );

            error = pkmn_gui_generate_spinda_sprite_at_filepath(
                        generation,
                        personality_shiny,
                        true,
                        test_spinda_filepath
                    );
            TEST_ASSERT_EQUAL(PKMN_ERROR_NONE, error);
            TEST_ASSERT_TRUE(file_exists(test_spinda_filepath));
            TEST_ASSERT_TRUE(do_files_match(test_files_spinda_filepath, test_spinda_filepath));
            delete_file(test_spinda_filepath);
        }
    }
    else
    {
        error = pkmn_gui_generate_spinda_sprite_at_filepath(
                    0,
                    0,
                    false,
                    ""
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_FEATURE_NOT_IN_BUILD_ERROR, error);
    }
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(spinda_test)
)
