/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.hpp"

#include "types/rng.hpp"

#include <pkmn/game_save.hpp>

// Testing PKSav includes until files are separated out
#include "gen1/save_internal.h"
#include "gen2/save_internal.h"
#include "gba/save_internal.h"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

namespace fs = boost::filesystem;

TEST(input_fuzzing_test, test_fuzzing_game_save)
{
    static const std::string SAVE_EXTENSION = ".sav";

    static const size_t MIN_SAVE_SIZE = 0;
    static const size_t MAX_SAVE_SIZE = 1e6;

    pkmn::rng<size_t> size_rng;

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        std::string tmp_filepath = write_random_tmp_file(
                                       size_rng.rand(MIN_SAVE_SIZE, MAX_SAVE_SIZE),
                                       SAVE_EXTENSION
                                   );

        // We don't expect these calls to succeed, but we want to make sure
        // it doesn't crash.
        try
        {
            (void)pkmn::game_save::detect_type(tmp_filepath);
        }
        catch(...) {}
        try
        {
            (void)pkmn::game_save::from_file(tmp_filepath);
        }
        catch(...) {}

        fs::remove(tmp_filepath.c_str());
    }
}
