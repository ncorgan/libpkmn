/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pk6.hpp"
#include "pokemon_gen6impl.hpp"
#include "../database/database_common.hpp"
#include "../database/id_to_index.hpp"

#include <pksav/gen6/pokemon.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    static const int OMEGA_RUBY = 25;

    bool vector_is_valid_pk6(
        const std::vector<uint8_t>& buffer,
        int* game_id_out
    )
    {
        // Validate size
        if(buffer.size() != sizeof(pksav_gen6_pc_pokemon_t) and
           buffer.size() != sizeof(pksav_gen6_party_pokemon_t))
        {
            return false;
        }

        const pksav_gen6_pc_pokemon_t* native = reinterpret_cast<const pksav_gen6_pc_pokemon_t*>(buffer.data());

        // Validate species and get game.
        try {
            (void)pkmn::database::pokemon_index_to_id(
                      native->blocks.blockA.species,
                      OMEGA_RUBY
                  );

            int game_id = pkmn::database::game_index_to_id(native->blocks.blockD.ot_game);
            int generation = pkmn::database::game_id_to_generation(game_id);

            if(generation == 6)
            {
                // Assume it's from this game.
                // TODO: make smarter by checking for game-specific items,
                // etc.
                *game_id_out = game_id;
            }
            else if(generation >= 3)
            {
                *game_id_out = OMEGA_RUBY;
            }
            else
            {
                return false;
            }
        }
        catch(const std::invalid_argument&)
        {
            return false;
        }

        return true;
    }

    pkmn::pokemon::sptr load_pk6(
        const std::vector<uint8_t>& buffer
    )
    {
        int game_id = 0;

        if(not vector_is_valid_pk6(buffer, &game_id))
        {
            throw std::runtime_error("Invalid .pk6.");
        }

        // TODO: smarter game choosing, Gen VI stores "not OT" data
        return pkmn::make_shared<pokemon_gen6impl>(
                   *reinterpret_cast<const pksav_gen6_pc_pokemon_t*>(buffer.data()),
                   game_id
               );
    }

    pkmn::pokemon::sptr load_pk6(
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

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> buffer(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(buffer.data()), filesize);
        ifile.close();

        return load_pk6(buffer);
    }
}}
