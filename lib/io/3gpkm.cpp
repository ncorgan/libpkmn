/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    bool vector_is_valid_3gpkm(
        const std::vector<uint8_t> &buffer,
        int* game_id_out
    ) {
        // Validate size
        if(buffer.size() != sizeof(pksav_gba_pc_pokemon_t) and
           buffer.size() != sizeof(pksav_gba_party_pokemon_t))
        {
            return false;
        }

        const pksav_gba_pc_pokemon_t* native = reinterpret_cast<const pksav_gba_pc_pokemon_t*>(buffer.data());
        const pksav_gba_pokemon_growth_t* growth = &native->blocks.growth;
        const pksav_gba_pokemon_misc_t* misc = &native->blocks.misc;

        BOOST_STATIC_CONSTEXPR int RUBY = 8;

        // Validate species
        try {
            (void)pkmn::database::pokemon_index_to_id(
                      pksav_littleendian16(growth->species),
                      RUBY
                  );
        } catch(const std::invalid_argument&) {
            return false;
        }

        // Validate game
        try {
            uint16_t origin_game = misc->origin_info & PKSAV_GBA_ORIGIN_GAME_MASK;
            origin_game >>= PKSAV_GBA_ORIGIN_GAME_OFFSET;

            int game_id = pkmn::database::game_index_to_id(origin_game);
            if(pkmn::database::game_id_to_generation(game_id) != 3) {
                return false;
            }

            *game_id_out = game_id;
        } catch(const std::invalid_argument&) {
            return false;
        }

        return true;
    }

    pkmn::pokemon::sptr load_3gpkm(
        const std::vector<uint8_t> &buffer
    ) {
        int game_id = 0;

        if(not vector_is_valid_3gpkm(buffer, &game_id)) {
            throw std::runtime_error("Invalid .3gpkm.");
        }

        return pkmn::make_shared<pokemon_gbaimpl>(
                   *reinterpret_cast<const pksav_gba_pc_pokemon_t*>(buffer.data()),
                   game_id
               );
    }

    pkmn::pokemon::sptr load_3gpkm(
        const std::string &filepath
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
        ifile.read(reinterpret_cast<char*>(buffer.data()), filesize);
        ifile.close();

        return load_3gpkm(buffer);
    }

    void save_3gpkm(
        pkmn::pokemon::sptr libpkmn_pokemon,
        const std::string &filepath
    ) {
        int generation = pkmn::database::game_id_to_generation(
                             libpkmn_pokemon->get_database_entry().get_game_id()
                         );

        if(generation != 3) {
            throw std::invalid_argument("Only GBA Pokémon can be saved to .3gpkm files.");
        }

        std::ofstream ofile(filepath.c_str(), std::ios::binary);
        ofile.write(static_cast<char*>(libpkmn_pokemon->get_native_pc_data()), sizeof(pksav_gba_pc_pokemon_t));
        ofile.close();
    }

}}
