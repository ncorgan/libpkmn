/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "types/rng.hpp"

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>
#include <pkmn/pokemon.hpp>
#include <pkmn/utils/paths.hpp>

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

PKMN_CONSTEXPR_OR_CONST static size_t NUM_ITERATIONS = 500;

namespace fs = boost::filesystem;

static const fs::path TMP_DIR(pkmn::get_tmp_dir());

static void randomize_vector(
    size_t filesize,
    std::vector<uint8_t>* output_vector_ptr
)
{
    ASSERT_NE(nullptr, output_vector_ptr);

    output_vector_ptr->resize(filesize);

    // Perform the fewest number of RNG calls necessary.
    if((filesize % 8) == 0)
    {
        pkmn::rng<uint64_t> rng64;
        uint64_t* data_ptr = reinterpret_cast<uint64_t*>(output_vector_ptr->data());
        for(size_t data_index = 0; data_index < (filesize / 8); ++data_index)
        {
            data_ptr[data_index] = rng64.rand();
        }
    }
    else if((filesize % 4) == 0)
    {
        pkmn::rng<uint32_t> rng32;
        uint32_t* data_ptr = reinterpret_cast<uint32_t*>(output_vector_ptr->data());
        for(size_t data_index = 0; data_index < (filesize / 4); ++data_index)
        {
            data_ptr[data_index] = rng32.rand();
        }
    }
    else if((filesize % 2) == 0)
    {
        pkmn::rng<uint16_t> rng16;
        uint16_t* data_ptr = reinterpret_cast<uint16_t*>(output_vector_ptr->data());
        for(size_t data_index = 0; data_index < (filesize / 2); ++data_index)
        {
            data_ptr[data_index] = rng16.rand();
        }
    }
    else
    {
        pkmn::rng<uint8_t> rng8;
        for(size_t data_index = 0; data_index < filesize; ++data_index)
        {
            (*output_vector_ptr)[data_index] = rng8.rand();
        }
    }
}

static std::string get_random_filepath(const std::string& extension)
{
    pkmn::rng<uint64_t> filename_rng;
    std::string filename = std::to_string(filename_rng.rand()) + extension;

    fs::path tmp_filepath = TMP_DIR / filename;
    return tmp_filepath.string();
}

template <typename vector_type>
static inline void write_vector_to_file(
    const std::vector<vector_type>& output_vector,
    const std::string& filepath
)
{
    static_assert(
        std::is_fundamental<vector_type>::value,
        "Must be a fundamental type"
    );

    size_t filesize = (output_vector.size() / sizeof(vector_type));

    std::ofstream output_stream(filepath.c_str(), std::ios::binary);
    output_stream.write((char*)output_vector.data(), filesize);
    output_stream.close();
}

static std::string write_random_tmp_file(
    size_t filesize,
    const std::string& extension
)
{
    std::vector<uint8_t> output_vector;
    randomize_vector(filesize, &output_vector);

    std::string tmp_filepath = get_random_filepath(extension);
    write_vector_to_file(output_vector, tmp_filepath);

    return tmp_filepath;
}

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

        std::remove(tmp_filepath.c_str());
    }
}

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

            std::remove(tmp_filepath.c_str());
        }
    }
}

INSTANTIATE_TEST_CASE_P(
    pokemon_fuzzing_test,
    pokemon_fuzzing_test,
    ::testing::ValuesIn(POKEMON_FUZZING_TEST_PARAMS)
);
