/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_impl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen2impl.hpp"
#include "game_save_gbaimpl.hpp"
#include "database/id_to_string.hpp"

#include "pksav/pksav_call.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <boost/filesystem.hpp>

#include <fstream>

namespace fs = boost::filesystem;

namespace pkmn {

    typedef enum {
        PKMN_SAVE_TYPE_NONE = 0,
        PKMN_SAVE_TYPE_RED_BLUE_YELLOW,
        PKMN_SAVE_TYPE_GOLD_SILVER,
        PKMN_SAVE_TYPE_CRYSTAL,
        PKMN_SAVE_TYPE_RUBY_SAPPHIRE,
        PKMN_SAVE_TYPE_EMERALD,
        PKMN_SAVE_TYPE_FIRERED_LEAFGREEN
    } pkmn_save_type_t;

    static BOOST_CONSTEXPR const char* SAVE_TYPE_NAMES[] = {
        "None",
        "Red/Blue/Yellow",
        "Gold/Silver",
        "Crystal",
        "Ruby/Sapphire",
        "Emerald",
        "FireRed/LeafGreen"
    };

    BOOST_STATIC_CONSTEXPR size_t GB_SAVE_SIZE  = 0x8000;
    BOOST_STATIC_CONSTEXPR size_t GBA_SAVE_SIZE = 0x10000;

    static pkmn_save_type_t _detect_type(
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

        if(filesize >= GBA_SAVE_SIZE) {
            // Check for a Ruby/Sapphire save.
            PKSAV_CALL(
                pksav_buffer_is_gba_save(
                    raw.data(),
                    filesize,
                    PKSAV_GBA_RS,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_RUBY_SAPPHIRE;
            }

            // Check for an Emerald save.
            PKSAV_CALL(
                pksav_buffer_is_gba_save(
                    raw.data(),
                    filesize,
                    PKSAV_GBA_EMERALD,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_EMERALD;
            }

            // Check for a FireRed/LeafGreen save.
            PKSAV_CALL(
                pksav_buffer_is_gba_save(
                    raw.data(),
                    filesize,
                    PKSAV_GBA_FRLG,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_FIRERED_LEAFGREEN;
            }
        }

        if(filesize >= GB_SAVE_SIZE) {
            // Check for a Generation I save.
            PKSAV_CALL(
                pksav_buffer_is_gen1_save(
                    raw.data(),
                    filesize,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_RED_BLUE_YELLOW;
            }

            // Check for a Gold/Silver save.
            PKSAV_CALL(
                pksav_buffer_is_gen2_save(
                    raw.data(),
                    filesize,
                    false,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_GOLD_SILVER;
            }

            // Check for a Crystal save.
            PKSAV_CALL(
                pksav_buffer_is_gen2_save(
                    raw.data(),
                    filesize,
                    true,
                    &type_found
                );
            );
            if(type_found) {
                return PKMN_SAVE_TYPE_CRYSTAL;
            }
        }

        return PKMN_SAVE_TYPE_NONE;
    }

    std::string game_save::detect_type(
        const std::string &filepath
    ) {
        return SAVE_TYPE_NAMES[_detect_type(filepath)];
    }

    game_save::sptr game_save::from_file(
        const std::string &filepath
    ) {
        pkmn_save_type_t save_type = _detect_type(filepath);

        switch(save_type) {
            case PKMN_SAVE_TYPE_RED_BLUE_YELLOW:
                return pkmn::make_shared<game_save_gen1impl>(filepath);

            case PKMN_SAVE_TYPE_GOLD_SILVER:
            case PKMN_SAVE_TYPE_CRYSTAL:
                return pkmn::make_shared<game_save_gen2impl>(filepath);

            case PKMN_SAVE_TYPE_RUBY_SAPPHIRE:
            case PKMN_SAVE_TYPE_EMERALD:
            case PKMN_SAVE_TYPE_FIRERED_LEAFGREEN:
                return pkmn::make_shared<game_save_gbaimpl>(filepath);

            case PKMN_SAVE_TYPE_NONE:
            default:
                throw std::invalid_argument("Invalid save (or unimplemented).");
        }
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
