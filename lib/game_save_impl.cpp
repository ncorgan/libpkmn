/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_impl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen2impl.hpp"
#include "game_save_gbaimpl.hpp"
#include "game_save_gcnimpl.hpp"
#include "database/id_to_string.hpp"

#include "libpkmgc_includes.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <boost/filesystem.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    typedef enum {
        PKMN_SAVE_TYPE_NONE = 0,
        PKMN_SAVE_TYPE_RED_BLUE_YELLOW,
        PKMN_SAVE_TYPE_GOLD_SILVER,
        PKMN_SAVE_TYPE_CRYSTAL,
        PKMN_SAVE_TYPE_RUBY_SAPPHIRE,
        PKMN_SAVE_TYPE_EMERALD,
        PKMN_SAVE_TYPE_FIRERED_LEAFGREEN,
        PKMN_SAVE_TYPE_COLOSSEUM_XD,
    } pkmn_save_type_t;

    static BOOST_CONSTEXPR const char* SAVE_TYPE_NAMES[] = {
        "None",
        "Red/Blue/Yellow",
        "Gold/Silver",
        "Crystal",
        "Ruby/Sapphire",
        "Emerald",
        "FireRed/LeafGreen",
        "Colosseum/XD"
    };

    BOOST_STATIC_CONSTEXPR size_t GB_SAVE_SIZE  = 0x8000;
    BOOST_STATIC_CONSTEXPR size_t GBA_SAVE_SIZE = 0x10000;

    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_BIN_SIZE = 0x60000;
    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_GCI_SIZE = 0x60040;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_BIN_SIZE = 0x56000;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_GCI_SIZE = 0x56040;

    static pkmn_save_type_t _detect_type(
        const std::string &filepath
    ) {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> raw(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(raw.data()), filesize);
        ifile.close();

        pkmn::shared_ptr<LibPkmGC::GC::SaveEditing::Save> gcn_save;
        if(filesize == GCN_COLOSSEUM_BIN_SIZE or filesize == GCN_COLOSSEUM_GCI_SIZE) {
            gcn_save.reset(new LibPkmGC::Colosseum::SaveEditing::Save(raw.data(), (filesize == GCN_COLOSSEUM_GCI_SIZE)));
        } if(filesize == GCN_XD_BIN_SIZE or filesize == GCN_XD_GCI_SIZE) {
            gcn_save.reset(new LibPkmGC::XD::SaveEditing::Save(raw.data(), (filesize == GCN_XD_GCI_SIZE)));
        }
        if(gcn_save) {
            size_t index = 0;
            if(gcn_save->getMostRecentValidSlot(0, &index)) {
                return PKMN_SAVE_TYPE_COLOSSEUM_XD;
            }
        }

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

            case PKMN_SAVE_TYPE_COLOSSEUM_XD:
                return pkmn::make_shared<game_save_gcnimpl>(filepath);

            case PKMN_SAVE_TYPE_NONE:
            default:
                throw std::invalid_argument("Invalid save (or unimplemented).");
        }
    }

    game_save_impl::game_save_impl(
        const std::string &filepath
    ): game_save(),
       _game_id(0)
    {
        if(not fs::exists(filepath)) {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_impl::get_filepath() {
        return _filepath;
    }

    void game_save_impl::save() {
        save_as(_filepath);
    }

    std::string game_save_impl::get_game() {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::pokemon_party::sptr game_save_impl::get_pokemon_party() {
        return _pokemon_party;
    }

    pkmn::pokemon_pc::sptr game_save_impl::get_pokemon_pc() {
        return _pokemon_pc;
    }

    pkmn::item_bag::sptr game_save_impl::get_item_bag() {
        return _item_bag;
    }

    pkmn::item_list::sptr game_save_impl::get_item_pc() {
        return _item_pc;
    }
}
