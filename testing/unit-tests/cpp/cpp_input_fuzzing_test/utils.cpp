/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.hpp"

#include "pkmntest/util.hpp"

#include "types/rng.hpp"

#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

#include <fstream>

namespace fs = boost::filesystem;

static const fs::path TMP_DIR(pkmn::get_tmp_dir());

void randomize_buffer(
    uint8_t* p_buffer,
    size_t buffer_size
)
{
    ASSERT_NE(nullptr, p_buffer);

    // Perform the fewest number of RNG calls necessary.
    if((buffer_size % 8) == 0)
    {
        pkmn::rng<uint64_t> rng64;
        uint64_t* data_ptr = reinterpret_cast<uint64_t*>(p_buffer);
        for(size_t data_index = 0; data_index < (buffer_size / 8); ++data_index)
        {
            data_ptr[data_index] = rng64.rand();
        }
    }
    else if((buffer_size % 4) == 0)
    {
        pkmn::rng<uint32_t> rng32;
        uint32_t* data_ptr = reinterpret_cast<uint32_t*>(p_buffer);
        for(size_t data_index = 0; data_index < (buffer_size / 4); ++data_index)
        {
            data_ptr[data_index] = rng32.rand();
        }
    }
    else if((buffer_size % 2) == 0)
    {
        pkmn::rng<uint16_t> rng16;
        uint16_t* data_ptr = reinterpret_cast<uint16_t*>(p_buffer);
        for(size_t data_index = 0; data_index < (buffer_size / 2); ++data_index)
        {
            data_ptr[data_index] = rng16.rand();
        }
    }
    else
    {
        pkmn::rng<uint8_t> rng8;
        for(size_t data_index = 0; data_index < buffer_size; ++data_index)
        {
            p_buffer[data_index] = rng8.rand();
        }
    }
}

void randomize_vector(
    size_t filesize,
    std::vector<uint8_t>* output_vector_ptr
)
{
    ASSERT_NE(nullptr, output_vector_ptr);

    output_vector_ptr->resize(filesize);

    randomize_buffer(
        output_vector_ptr->data(),
        filesize
    );
}

std::string get_random_filepath(const std::string& extension)
{
    pkmn::rng<uint64_t> filename_rng;
    std::string filename = std::to_string(filename_rng.rand()) + extension;

    fs::path tmp_filepath = TMP_DIR / filename;
    return tmp_filepath.string();
}

void read_file_to_vector(
    const std::string& filepath,
    std::vector<uint8_t>* p_file_vector
)
{
    p_file_vector->clear();
    p_file_vector->resize(static_cast<size_t>(fs::file_size(filepath)));

    std::ifstream ifile(filepath.c_str(), std::ios::binary);
    ifile.read(reinterpret_cast<char*>(p_file_vector->data()), p_file_vector->size());
    ifile.close();
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

// Make sure parsing doesn't crash.
void read_all_pokemon_fields(const pkmn::pokemon::sptr& pokemon)
{
    std::string game = pokemon->get_game();
    int generation = game_generations.at(game);
    (void)generation;

    (void)pokemon->get_species();
    (void)pokemon->get_form();
    (void)pokemon->get_condition();
    (void)pokemon->get_nickname();
    (void)pokemon->get_original_trainer_name();
    (void)pokemon->get_experience();
    (void)pokemon->get_level();
    (void)pokemon->get_moves();
    (void)pokemon->get_EVs();
    (void)pokemon->get_IVs();
    (void)pokemon->get_current_hp();
    (void)pokemon->get_stats();
    (void)pokemon->get_icon_filepath();
    (void)pokemon->get_sprite_filepath();

    for(const std::string& attribute: pokemon->get_numeric_attribute_names())
    {
        (void)pokemon->get_numeric_attribute(attribute);
    }
    for(const std::string& attribute: pokemon->get_string_attribute_names())
    {
        (void)pokemon->get_string_attribute(attribute);
    }
    for(const std::string& attribute: pokemon->get_boolean_attribute_names())
    {
        (void)pokemon->get_boolean_attribute(attribute);
    }

    if(generation >= 2)
    {
        (void)pokemon->is_egg();
        (void)pokemon->get_gender();
        (void)pokemon->is_shiny();
        (void)pokemon->get_held_item();
        (void)pokemon->get_pokerus_duration();
        (void)pokemon->get_original_trainer_gender();
        (void)pokemon->get_current_trainer_friendship();
        (void)pokemon->get_level_met();
        (void)pokemon->get_location_met(false);
    }
    if(generation >= 3)
    {
        (void)pokemon->get_ability();
        (void)pokemon->get_ball();
        (void)pokemon->get_personality();
        (void)pokemon->get_markings();
        (void)pokemon->get_ribbons();
        (void)pokemon->get_contest_stats();
    }
    if(generation >= 4)
    {
        (void)pokemon->get_location_met(true);
    }
}
