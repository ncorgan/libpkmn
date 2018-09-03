/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gbaimpl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_index.hpp"

#include "io/3gpkm.hpp"
#include "io/read_write.hpp"

#include <pksav/gba/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    bool vector_is_valid_3gpkm(
        const std::vector<uint8_t> &buffer,
        int* game_id_out
    )
    {
        // Validate size
        if(buffer.size() != sizeof(struct pksav_gba_pc_pokemon) and
           buffer.size() != sizeof(struct pksav_gba_party_pokemon))
        {
            return false;
        }

        const struct pksav_gba_pc_pokemon* native = reinterpret_cast<const struct pksav_gba_pc_pokemon*>(buffer.data());
        const struct pksav_gba_pokemon_growth_block* growth = &native->blocks.growth;
        const struct pksav_gba_pokemon_misc_block* misc = &native->blocks.misc;

        BOOST_STATIC_CONSTEXPR int RUBY_ID = 8;

        // Validate species
        try
        {
            (void)pkmn::database::pokemon_index_to_id(
                      pksav_littleendian16(growth->species),
                      RUBY_ID
                  );
        }
        catch(const std::invalid_argument&)
        {
            return false;
        }

        // Validate game
        try
        {
            int game_id = pkmn::database::game_index_to_id(
                              PKSAV_GBA_POKEMON_ORIGIN_GAME(misc->origin_info)
                          );
            if(pkmn::database::game_id_to_generation(game_id) != 3)
            {
                return false;
            }

            *game_id_out = game_id;
        }
        catch(const std::invalid_argument&)
        {
            return false;
        }

        return true;
    }

    pkmn::pokemon::sptr load_3gpkm(
        const std::vector<uint8_t> &buffer
    )
    {
        int game_id = 0;

        if(not vector_is_valid_3gpkm(buffer, &game_id))
        {
            throw std::runtime_error("Invalid .3gpkm.");
        }

        return std::make_shared<pokemon_gbaimpl>(
                   reinterpret_cast<const struct pksav_gba_pc_pokemon*>(buffer.data()),
                   game_id
               );
    }

    pkmn::pokemon::sptr load_3gpkm(
        const std::string& filepath
    )
    {
        if(not fs::exists(filepath))
        {
            throw std::invalid_argument(
                      str(boost::format("The file \"%s\" does not exist.")
                          % filepath.c_str())
                  );
        }

        return load_3gpkm(read_file(filepath));
    }
}}
