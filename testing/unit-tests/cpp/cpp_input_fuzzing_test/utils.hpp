/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_INPUT_FUZZING_TEST_UTILS_HPP
#define CPP_INPUT_FUZZING_TEST_UTILS_HPP

#include <pkmn/config.hpp>

#include <cassert>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

PKMN_CONSTEXPR_OR_CONST static size_t NUM_ITERATIONS = 500;

void randomize_vector(
    size_t filesize,
    std::vector<uint8_t>* output_vector_ptr
);

std::string get_random_filepath(const std::string& extension);

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

std::string write_random_tmp_file(
    size_t filesize,
    const std::string& extension
);

#endif /* CPP_INPUT_FUZZING_TEST_UTILS_HPP */
