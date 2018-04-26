/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.hpp"

#include "types/rng.hpp"

#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <fstream>

namespace fs = boost::filesystem;

static const fs::path TMP_DIR(pkmn::get_tmp_dir());

void randomize_vector(
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

std::string get_random_filepath(const std::string& extension)
{
    pkmn::rng<uint64_t> filename_rng;
    std::string filename = std::to_string(filename_rng.rand()) + extension;

    fs::path tmp_filepath = TMP_DIR / filename;
    return tmp_filepath.string();
}

std::string write_random_tmp_file(
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
