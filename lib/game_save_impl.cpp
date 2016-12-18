/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_impl.hpp"
#include "database/id_to_string.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <boost/filesystem.hpp>

#include <fstream>

namespace fs = boost::filesystem;

namespace pkmn {

    std::string game_save::detect_type(
        const std::string &filepath
    ) {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> raw(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read((char*)raw.data(), filesize);
        ifile.close();

        bool type_found = false;

        // Check for a Generation I save.
        PKSAV_CALL(
            pksav_buffer_is_gen1_save(
                raw.data(),
                filesize,
                &type_found
            );
        )
        if(type_found) {
            return "Red/Blue/Yellow";
        }

        // Check for a Gold/Silver save.
        PKSAV_CALL(
            pksav_buffer_is_gen2_save(
                raw.data(),
                filesize,
                false,
                &type_found
            );
        )
        if(type_found) {
            return "Gold/Silver";
        }

        // Check for a Crystal save.
        PKSAV_CALL(
            pksav_buffer_is_gen2_save(
                raw.data(),
                filesize,
                true,
                &type_found
            );
        )
        if(type_found) {
            return "Crystal";
        }

        // Check for a Ruby/Sapphire save.
        PKSAV_CALL(
            pksav_buffer_is_gba_save(
                raw.data(),
                filesize,
                PKSAV_GBA_RS,
                &type_found
            );
        )
        if(type_found) {
            return "Ruby/Sapphire";
        }

        // Check for an Emerald save.
        PKSAV_CALL(
            pksav_buffer_is_gba_save(
                raw.data(),
                filesize,
                PKSAV_GBA_EMERALD,
                &type_found
            );
        )
        if(type_found) {
            return "Emerald";
        }

        // Check for a FireRed/LeafGreen save.
        PKSAV_CALL(
            pksav_buffer_is_gba_save(
                raw.data(),
                filesize,
                PKSAV_GBA_FRLG,
                &type_found
            );
        )
        if(type_found) {
            return "FireRed/LeafGreen";
        }

        return "None";
    }

    game_save_impl::game_save_impl(
        const std::string &filepath
    ): game_save()
    {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_impl::get_filepath() {
        return _filepath;
    }

    std::string game_save_impl::get_game() {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::item_bag::sptr game_save_impl::get_item_bag() {
        return _item_bag;
    }

    pkmn::item_list::sptr game_save_impl::get_item_pc() {
        return _item_pc;
    }
}
