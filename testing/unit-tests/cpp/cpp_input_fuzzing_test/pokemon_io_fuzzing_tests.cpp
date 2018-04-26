/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.hpp"

#include <pkmn/config.hpp>
#include <pkmn/pokemon.hpp>

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <cstdint>

namespace fs = boost::filesystem;

static const std::string PK1_EXTENSION = ".pk1";
static const std::string PK2_EXTENSION = ".pk2";
static const std::string _3GPKM_EXTENSION = ".3gpkm";

static const std::vector<size_t> PK1_SIZES =
{
    sizeof(struct pksav_gen1_pc_pokemon),
    sizeof(struct pksav_gen1_party_pokemon)
};
static const std::vector<size_t> PK2_SIZES =
{
    sizeof(struct pksav_gen2_pc_pokemon),
    sizeof(struct pksav_gen2_party_pokemon)
};
static const std::vector<size_t> _3GPKM_SIZES =
{
    sizeof(struct pksav_gba_pc_pokemon),
    sizeof(struct pksav_gba_party_pokemon)
};

struct pokemon_fuzzing_params
{
    std::string extension;
    std::vector<size_t> filesizes;
};

// The extension will force LibPKMN to test the given file import functionality.
// The file sizes will force LibPKMN to attempt to parse the file.
static const std::vector<pokemon_fuzzing_params> POKEMON_FUZZING_TEST_PARAMS =
{
    {PK1_EXTENSION, PK1_SIZES},
    {PK2_EXTENSION, PK2_SIZES},
    {_3GPKM_EXTENSION, _3GPKM_SIZES}
};

class pokemon_fuzzing_test: public ::testing::TestWithParam<pokemon_fuzzing_params>
{};

TEST_P(pokemon_fuzzing_test, test_fuzzing_pokemon_io)
{
    pokemon_fuzzing_params params = GetParam();

    for(size_t iteration = 0; iteration < NUM_ITERATIONS; ++iteration)
    {
        for(size_t filesize: params.filesizes)
        {
            std::string tmp_filepath = write_random_tmp_file(
                                           filesize,
                                           params.extension
                                       );

            // We don't expect this call to succeed, but we want to make sure
            // it doesn't crash.
            try
            {
                (void)pkmn::pokemon::from_file(tmp_filepath);
            }
            catch(...) {}

            fs::remove(tmp_filepath.c_str());
        }
    }
}

INSTANTIATE_TEST_CASE_P(
    pokemon_fuzzing_test,
    pokemon_fuzzing_test,
    ::testing::ValuesIn(POKEMON_FUZZING_TEST_PARAMS)
);
