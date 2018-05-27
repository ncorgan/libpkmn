/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pkm.hpp"
#include "../pokemon_ndsimpl.hpp"
#include "../database/database_common.hpp"
#include "../database/id_to_index.hpp"

#include <pksav/common/nds_pokemon.h>
#include <pksav/math/endian.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    bool vector_is_valid_pkm(
        const std::vector<uint8_t> &buffer,
        int* game_id_out
    ) {
        // Validate size
        if(buffer.size() != sizeof(pksav_nds_pc_pokemon_t) and
           buffer.size() != sizeof(pksav_nds_party_pokemon_t))
        {
            return false;
        }

        const pksav_nds_pc_pokemon_t* native = reinterpret_cast<const pksav_nds_pc_pokemon_t*>(buffer.data());
        const pksav_nds_pokemon_blockA_t* blockA = &native->blocks.blockA;
        const pksav_nds_pokemon_blockC_t* blockC = &native->blocks.blockC;

        // Validate game
        try {
            int game_id = pkmn::database::game_index_to_id(blockC->hometown);
            int generation = pkmn::database::game_id_to_generation(game_id);

            if(generation < 3 or generation > 5) {
                return false;
            }

            *game_id_out = game_id;
        } catch(const std::invalid_argument&) {
            return false;
        }

        // Validate species
        try {
            (void)pkmn::database::pokemon_index_to_id(
                      pksav_littleendian16(blockA->species),
                      (*game_id_out)
                  );
        } catch(const std::invalid_argument&) {
            return false;
        }

        return true;
    }

    pkmn::pokemon::sptr load_pkm(
        const std::vector<uint8_t> &buffer
    ) {
        int game_id = 0;

        if(not vector_is_valid_pkm(buffer, &game_id)) {
            throw std::runtime_error("Invalid .pkm.");
        }

        // Make sure it's from a valid game
        BOOST_STATIC_CONSTEXPR int DIAMOND = 12;
        game_id = std::max<int>(game_id, DIAMOND);

        return std::make_shared<pokemon_ndsimpl>(
                   *reinterpret_cast<const pksav_nds_pc_pokemon_t*>(buffer.data()),
                   game_id
               );
    }

    pkmn::pokemon::sptr load_pkm(
        const std::string& filepath
    ) {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument(
                      str(boost::format("The file \"%s\" does not exist.")
                          % filepath.c_str())
                  );
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> buffer(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read((char*)buffer.data(), filesize);
        ifile.close();

        return load_pkm(buffer);
    }

    void save_pkm(
        pkmn::pokemon::sptr libpkmn_pokemon,
        const std::string& filepath
    ) {
        int generation = pkmn::database::game_id_to_generation(
                             libpkmn_pokemon->get_database_entry().get_game_id()
                         );

        if(generation != 4 and generation != 5) {
            throw std::invalid_argument("Only Generation IV-V Pokémon can be saved to .pkm files.");
        }

        std::ofstream ofile(filepath.c_str(), std::ios::binary);
        ofile.write((char*)libpkmn_pokemon->get_native_pc_data(), sizeof(pksav_nds_pc_pokemon_t));
        ofile.close();
    }

}}
