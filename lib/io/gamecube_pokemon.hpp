/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef POKEMON_IO_GAMECUBE_POKEMON_HPP
#define  POKEMON_IO_GAMECUBE_POKEMON_HPP

#include "pokemon_gcnimpl.hpp"
#include "utils/misc.hpp"

#include <LibPkmGC/Colosseum/Common/Pokemon.h>
#include <LibPkmGC/XD/Common/Pokemon.h>

#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

namespace fs = boost::filesystem;

namespace pkmn { namespace io {

    namespace internal
    {
        static inline bool is_species_valid(LibPkmGC::PokemonSpeciesIndex species)
        {
            return ((species >= LibPkmGC::Bulbasaur) &&
                    (species <= LibPkmGC::Chimecho));
        }

        template <typename libpkmgc_pokemon_type>
        bool is_vector_valid_gamecube_pokemon(
            const std::vector<uint8_t>& buffer
        )
        {
            bool is_valid = false;

            if(buffer.size() == libpkmgc_pokemon_type::size)
            {
                libpkmgc_pokemon_type libpkmgc_pokemon(buffer.data());

                // Validate species and game. LibPkmGC will set the VersionInfo
                // values to 0 if they are invalid and provides a handy function
                // to check for that.
                is_valid = is_species_valid(libpkmgc_pokemon.species) &&
                           !libpkmgc_pokemon.version.isIncomplete();
            }

            return is_valid;
        }

        template <typename libpkmgc_pokemon_type>
        pkmn::pokemon::sptr load_gamecube_pokemon(
            const std::vector<uint8_t>& buffer,
            const std::string& extension,
            int game_id
        )
        {
            if(!is_vector_valid_gamecube_pokemon<libpkmgc_pokemon_type>(buffer))
            {
                std::string error_message = str(boost::format("Invalid %s.")
                                                % extension.c_str());
                throw std::runtime_error(error_message);
            }

            libpkmgc_pokemon_type libpkmgc_pokemon(buffer.data());

            return std::make_shared<pokemon_gcnimpl>(
                       &libpkmgc_pokemon,
                       game_id
                   );
        }

        template <typename libpkmgc_pokemon_type>
        pkmn::pokemon::sptr load_gamecube_pokemon(
            const std::string& filepath,
            const std::string& extension,
            int game_id
        )
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

            return load_gamecube_pokemon<libpkmgc_pokemon_type>(
                       buffer,
                       extension,
                       game_id
                   );
        }
    }

    //
    // Colosseum (.ck3)
    //

    BOOST_STATIC_CONSTEXPR auto& is_vector_valid_ck3 =
        internal::is_vector_valid_gamecube_pokemon<LibPkmGC::Colosseum::Pokemon>;

    inline pkmn::pokemon::sptr load_ck3(const std::vector<uint8_t>& buffer)
    {
        return internal::load_gamecube_pokemon<LibPkmGC::Colosseum::Pokemon>(
            buffer,
            ".ck3",
            pkmn::COLOSSEUM_ID
        );
    }

    inline pkmn::pokemon::sptr load_ck3(const std::string& filepath)
    {
        return internal::load_gamecube_pokemon<LibPkmGC::Colosseum::Pokemon>(
            filepath,
            ".ck3",
            pkmn::COLOSSEUM_ID
        );
    }

    //
    // XD (.xk3)
    //

    BOOST_STATIC_CONSTEXPR auto& is_vector_valid_xk3 =
        internal::is_vector_valid_gamecube_pokemon<LibPkmGC::XD::Pokemon>;

    inline pkmn::pokemon::sptr load_xk3(const std::vector<uint8_t>& buffer)
    {
        return internal::load_gamecube_pokemon<LibPkmGC::XD::Pokemon>(
            buffer,
            ".xk3",
            pkmn::XD_ID
        );
    }

    inline pkmn::pokemon::sptr load_xk3(const std::string& filepath)
    {
        return internal::load_gamecube_pokemon<LibPkmGC::XD::Pokemon>(
            filepath,
            ".xk3",
            pkmn::XD_ID
        );
    }
}}

#endif /* PKMN_IO_GAMECUBE_POKEMON_HPP */
