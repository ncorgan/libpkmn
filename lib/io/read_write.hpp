/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_IO_READ_WRITE_HPP
#define PKMN_IO_READ_WRITE_HPP

#include <boost/assert.hpp>

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace pkmn { namespace io {

    std::vector<uint8_t> read_file(const std::string& filepath);

    inline void write_file(
        const std::string& filepath,
        const std::vector<uint8_t>& contents
    )
    {
        std::ofstream ofile(filepath, std::ios::binary);
        ofile.write((const char*)contents.data(), contents.size());
    }

    inline void write_file(
        const std::string& filepath,
        const void* p_buffer,
        size_t buffer_size
    )
    {
        BOOST_ASSERT(p_buffer != nullptr);

        std::ofstream ofile(filepath, std::ios::binary);
        ofile.write((const char*)p_buffer, buffer_size);
    }

}}

#endif /* PKMN_IO_READ_WRITE_HPP */
