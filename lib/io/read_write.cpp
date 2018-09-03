/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "3gpkm.hpp"
#include "../pokemon_gbaimpl.hpp"
#include "../database/database_common.hpp"
#include "../database/id_to_index.hpp"

#include <pksav/gba/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    std::vector<uint8_t> read_file(const std::string& filepath)
    {
        if(!fs::exists(filepath))
        {
            throw std::invalid_argument(
                      str(boost::format("The file \"%s\" does not exist.")
                          % filepath.c_str())
                  );
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> buffer(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(buffer.data()), filesize);
        ifile.close();

        return buffer;
    }
}}
