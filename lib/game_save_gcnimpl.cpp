/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "game_save_gcnimpl.hpp"
#include "item_bag_gcnimpl.hpp"
#include "item_list_gcnimpl.hpp"
#include "pokemon_party_gcnimpl.hpp"
#include "pokemon_pc_gcnimpl.hpp"

#include "utils/misc.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <fstream>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    #define GCN_PC_ID (_colosseum ? 68 : 75)

    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_BIN_SIZE = 0x60000;
    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_GCI_SIZE = 0x60040;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_BIN_SIZE = 0x56000;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_GCI_SIZE = 0x56040;

    game_save_gcnimpl::game_save_gcnimpl(
        const std::string& filepath,
        std::vector<uint8_t>&& raw
    ): game_save_impl(filepath, std::move(raw))
    {
        size_t save_size = _raw.size();

        _has_gci_data = false;
        if(save_size == GCN_COLOSSEUM_BIN_SIZE or save_size == GCN_COLOSSEUM_GCI_SIZE)
        {
            _colosseum = true;
            _game_id = COLOSSEUM_ID;

            _has_gci_data = (save_size == GCN_COLOSSEUM_GCI_SIZE);
            _libpkmgc_save.reset(new LibPkmGC::Colosseum::SaveEditing::Save(_raw.data(), _has_gci_data));
        }
        else if(save_size == GCN_XD_BIN_SIZE or save_size == GCN_XD_GCI_SIZE)
        {
            _colosseum = false;
            _game_id = XD_ID;

            _has_gci_data = (save_size == GCN_XD_GCI_SIZE);
            _libpkmgc_save.reset(new LibPkmGC::XD::SaveEditing::Save(_raw.data(), _has_gci_data));
        }
        else
        {
            throw std::invalid_argument("Not a valid Gamecube save.");
        }

        size_t index = 0;
        _current_slot = _libpkmgc_save->getMostRecentValidSlot(0, &index);
        if(!_current_slot)
        {
            throw std::invalid_argument("Could not find a save slot.");
        }

        _pokemon_party = std::make_shared<pokemon_party_gcnimpl>(
                             _game_id,
                             _current_slot->player->trainer->party
                         );
        _pokemon_pc = std::make_shared<pokemon_pc_gcnimpl>(
                          _game_id,
                          _current_slot->PC->boxes
                      );

        _item_bag = std::make_shared<item_bag_gcnimpl>(
                        _game_id,
                        _current_slot->player->bag
                    );
        _item_pc = std::make_shared<item_list_gcnimpl>(
                       GCN_PC_ID, _game_id,
                       _current_slot->PC->items,
                       235, false
                   );
    }

    game_save_gcnimpl::~game_save_gcnimpl()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);
    }

    void game_save_gcnimpl::save_as(
        const std::string& filepath
    )
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        // Make sure any updating is performed.
        (void)_pokemon_party->get_native();
        (void)_pokemon_pc->get_native();

        _libpkmgc_save->saveEncrypted(_raw.data(), _has_gci_data);

        std::ofstream ofile(filepath, std::ios::binary);
        ofile.write(reinterpret_cast<const char*>(_raw.data()), _raw.size());
        ofile.close();

        _filepath = fs::absolute(filepath).string();
    }

    pkmn::time_duration game_save_gcnimpl::get_time_played()
    {
        throw pkmn::unimplemented_error();
    }

    void game_save_gcnimpl::set_time_played(const pkmn::time_duration&)
    {
        throw pkmn::unimplemented_error();
    }

    std::string game_save_gcnimpl::get_trainer_name()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return std::string(_current_slot->player->trainer->trainerName->toUTF8());
    }

    void game_save_gcnimpl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<game_save_gcnimpl> lock(*this);

        _current_slot->player->trainer->trainerName->fromUTF8(trainer_name.c_str());
    }

    uint32_t game_save_gcnimpl::get_trainer_id()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return _current_slot->player->trainer->TID | (uint32_t(_current_slot->player->trainer->SID) << 16);
    }

    void game_save_gcnimpl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        _current_slot->player->trainer->TID = LibPkmGC::u16(trainer_id & 0xFFFF);
        _current_slot->player->trainer->SID = LibPkmGC::u16(trainer_id >> 16);
    }

    uint16_t game_save_gcnimpl::get_trainer_public_id()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return _current_slot->player->trainer->TID;
    }

    void game_save_gcnimpl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        _current_slot->player->trainer->TID = trainer_public_id;
    }

    uint16_t game_save_gcnimpl::get_trainer_secret_id()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return _current_slot->player->trainer->SID;
    }

    void game_save_gcnimpl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    )
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        _current_slot->player->trainer->SID = trainer_secret_id;
    }

    std::string game_save_gcnimpl::get_trainer_gender()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return (_current_slot->player->trainerGender == LibPkmGC::Male) ? "Male" : "Female";
    }

    void game_save_gcnimpl::set_trainer_gender(
        PKMN_UNUSED(const std::string& trainer_gender)
    )
    {
        throw pkmn::feature_not_in_game_error("All trainers are male in Gamecube games.");
    }

    std::string game_save_gcnimpl::get_rival_name()
    {
        throw pkmn::feature_not_in_game_error("Rivals", get_game());
    }

    void game_save_gcnimpl::set_rival_name(
        PKMN_UNUSED(const std::string& rival_name)
    )
    {
        throw pkmn::feature_not_in_game_error("Rivals", get_game());
    }

    int game_save_gcnimpl::get_money()
    {
        boost::lock_guard<game_save_gcnimpl> lock(*this);

        return int(_current_slot->player->pokeDollars);
    }

    void game_save_gcnimpl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gcnimpl> lock(*this);

        _current_slot->player->pokeDollars = LibPkmGC::u32(money);
    }

    pkmn::pokedex::sptr game_save_gcnimpl::get_pokedex()
    {
        throw pkmn::unimplemented_error();
    }
}
