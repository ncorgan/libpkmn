/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gcnimpl.hpp"
#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"
#include "pokemon_pc_gcnimpl.hpp"

#include "misc_common.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <fstream>

namespace fs = boost::filesystem;

namespace pkmn {

    #define GCN_PC_ID (_colosseum ? 68 : 75)

    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_BIN_SIZE = 0x60000;
    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_GCI_SIZE = 0x60040;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_BIN_SIZE = 0x56000;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_GCI_SIZE = 0x56040;

    game_save_gcnimpl::game_save_gcnimpl(
        const std::string &filepath
    ): game_save_impl(filepath)
    {
        size_t filesize = size_t(fs::file_size(filepath));
        _data.resize(filesize);

        std::ifstream ifile(filepath, std::ios::binary);
        ifile.read((char*)_data.data(), filesize);
        ifile.close();

        _has_gci_data = false;
        if(filesize == GCN_COLOSSEUM_BIN_SIZE or filesize == GCN_COLOSSEUM_GCI_SIZE) {
            _colosseum = true;
            _game_id = COLOSSEUM;

            _has_gci_data = (filesize == GCN_COLOSSEUM_GCI_SIZE);
            _libpkmgc_save.reset(new LibPkmGC::Colosseum::SaveEditing::Save(_data.data(), _has_gci_data));
        } else if(filesize == GCN_XD_BIN_SIZE or filesize == GCN_XD_GCI_SIZE) {
            _colosseum = false;
            _game_id = XD;

            _has_gci_data = (filesize == GCN_XD_GCI_SIZE);
            _libpkmgc_save.reset(new LibPkmGC::XD::SaveEditing::Save(_data.data(), _has_gci_data));
        } else {
            throw std::invalid_argument("Not a valid Gamecube save.");
        }

        size_t index = 0;
        _current_slot = _libpkmgc_save->getMostRecentValidSlot(0, &index);
        if(!_current_slot) {
            throw std::invalid_argument("Could not find a save slot.");
        }

        _pokemon_party = pkmn::make_shared<pokemon_party_gcnimpl>(
                             _game_id,
                             _current_slot->player->trainer->party
                         );
        _pokemon_pc = pkmn::make_shared<pokemon_pc_gcnimpl>(
                          _game_id,
                          _current_slot->PC->boxes
                      );

        _item_bag = pkmn::make_shared<item_bag_gcnimpl>(
                        _game_id,
                        _current_slot->player->bag
                    );
        _item_pc = pkmn::make_shared<item_list_gcnimpl>(
                       GCN_PC_ID, _game_id,
                       _current_slot->PC->items,
                       235, false
                   );
    }

    game_save_gcnimpl::~game_save_gcnimpl() {}

    void game_save_gcnimpl::save_as(
        const std::string &filepath
    ) {
        _libpkmgc_save->saveEncrypted((LibPkmGC::u8*)_data.data(), _has_gci_data);

        std::ofstream ofile(filepath, std::ios::binary);
        ofile.write((const char*)_data.data(), _data.size());
        ofile.close();

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gcnimpl::get_trainer_name() {
        return std::string(_current_slot->player->trainer->trainerName->toUTF8());
    }

    void game_save_gcnimpl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() == 0 or trainer_name.size() > 7) {
            throw std::invalid_argument("trainer_name: valid length 1-7");
        }

        _current_slot->player->trainer->trainerName->fromUTF8(trainer_name.c_str());
    }

    uint32_t game_save_gcnimpl::get_trainer_id() {
        return _current_slot->player->trainer->TID | (uint32_t(_current_slot->player->trainer->SID) << 16);
    }

    void game_save_gcnimpl::set_trainer_id(
        uint32_t trainer_id
    ) {
        _current_slot->player->trainer->TID = LibPkmGC::u16(trainer_id & 0xFFFF);
        _current_slot->player->trainer->SID = LibPkmGC::u16(trainer_id >> 16);
    }

    uint16_t game_save_gcnimpl::get_trainer_public_id() {
        return _current_slot->player->trainer->TID;
    }

    void game_save_gcnimpl::set_trainer_public_id(
        uint16_t trainer_public_id
    ) {
        _current_slot->player->trainer->TID = trainer_public_id;
    }

    uint16_t game_save_gcnimpl::get_trainer_secret_id() {
        return _current_slot->player->trainer->SID;
    }

    void game_save_gcnimpl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    ) {
        _current_slot->player->trainer->SID = trainer_secret_id;
    }

    std::string game_save_gcnimpl::get_trainer_gender() {
        return (_current_slot->player->trainerGender == LibPkmGC::Male) ? "Male" : "Female";
    }

    void game_save_gcnimpl::set_trainer_gender(
        PKMN_UNUSED(const std::string &trainer_gender)
    ) {
        throw pkmn::feature_not_in_game_error("All trainers are male in Gamecube games.");
    }

    std::string game_save_gcnimpl::get_rival_name() {
        throw pkmn::feature_not_in_game_error("Rivals", get_game());
    }

    void game_save_gcnimpl::set_rival_name(
        PKMN_UNUSED(const std::string &rival_name)
    ) {
        throw pkmn::feature_not_in_game_error("Rivals", get_game());
    }

    int game_save_gcnimpl::get_money() {
        return int(_current_slot->player->pokeDollars);
    }

    void game_save_gcnimpl::set_money(
        int money
    ) {
        if(money < 0 or money > MONEY_MAX_VALUE) {
            pkmn::throw_out_of_range("money", 0, MONEY_MAX_VALUE);
        }

        _current_slot->player->pokeDollars = LibPkmGC::u32(money);
    }
}
