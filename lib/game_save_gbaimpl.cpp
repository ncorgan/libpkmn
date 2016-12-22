/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gbaimpl.hpp"
#include "item_bag_gbaimpl.hpp"
#include "item_list_modernimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int RUBY      = 7;
    BOOST_STATIC_CONSTEXPR int SAPPHIRE  = 8;
    BOOST_STATIC_CONSTEXPR int EMERALD   = 9;
    BOOST_STATIC_CONSTEXPR int FIRERED   = 10;
    BOOST_STATIC_CONSTEXPR int LEAFGREEN = 11;

    BOOST_STATIC_CONSTEXPR int RS_PC      = 20;
    BOOST_STATIC_CONSTEXPR int EMERALD_PC = 26;
    BOOST_STATIC_CONSTEXPR int FRLG_PC    = 32;

    game_save_gbaimpl::game_save_gbaimpl(
        const std::string &filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gba_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        std::string filename = boost::algorithm::to_lower_copy(
                                   fs::path(filepath).stem().string()
                               );
        boost::erase_all(filename, " ");
        switch(_pksav_save.gba_game) {
            case PKSAV_GBA_RS:
                /*
                 * As there is no way to distinguish Ruby and Sapphire saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("ruby") != std::string::npos) {
                    _game_id = RUBY;
                } else if(filename.find("sapphire") != std::string::npos) {
                    _game_id = SAPPHIRE;
                } else {
                    // Default to Ruby, doesn't practically matter within a version group
                    _game_id = RUBY;
                }

                _item_pc = pkmn::make_shared<item_list_modernimpl>(
                                RS_PC, _game_id,
                                _pksav_save.item_storage->rs.pc_items,
                                50, false
                           );
                break;

            case PKSAV_GBA_FRLG:
                /*
                 * As there is no way to distinguish FireRed and LeafGreen saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("firered") != std::string::npos or
                   filename.find("fr") != std::string::npos
                ) {
                    _game_id = FIRERED;
                } else if(filename.find("leafgreen") != std::string::npos or
                          filename.find("lg") != std::string::npos)
                {
                    _game_id = LEAFGREEN;
                } else {
                    // Default to FireRed, doesn't practically matter within a version group
                    _game_id = FIRERED;
                }

                _item_pc = pkmn::make_shared<item_list_modernimpl>(
                                FRLG_PC, _game_id,
                                _pksav_save.item_storage->frlg.pc_items,
                                50, false
                           );
                break;

            default: // Emerald
                _game_id = EMERALD;

                _item_pc = pkmn::make_shared<item_list_modernimpl>(
                                EMERALD_PC, _game_id,
                                _pksav_save.item_storage->emerald.pc_items,
                                50, false
                           );
                break;
        }

        _item_bag = pkmn::make_shared<item_bag_gbaimpl>(
                        _game_id, _pksav_save.item_storage
                    );
    }

    game_save_gbaimpl::~game_save_gbaimpl() {
        pksav_gba_save_free(&_pksav_save);
    }

    std::string game_save_gbaimpl::get_trainer_name() {
        char trainer_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gba(
                _pksav_save.trainer_info->name,
                trainer_name,
                7
            );
        )

        return std::string(trainer_name);
    }

    uint32_t game_save_gbaimpl::get_trainer_id() {
        return pksav_littleendian32(_pksav_save.trainer_info->trainer_id.id);
    }

    uint16_t game_save_gbaimpl::get_trainer_public_id() {
        return pksav_littleendian16(_pksav_save.trainer_info->trainer_id.pid);
    }

    uint16_t game_save_gbaimpl::get_trainer_secret_id() {
        return pksav_littleendian16(_pksav_save.trainer_info->trainer_id.sid);
    }

    std::string game_save_gbaimpl::get_trainer_gender() {
        return (_pksav_save.trainer_info->gender == 0) ? "Male" : "Female";
    }

    std::string game_save_gbaimpl::get_rival_name() {
        if(_pksav_save.gba_game == PKSAV_GBA_FRLG) {
            char rival_name[8] = {0};
            PKSAV_CALL(
                pksav_text_from_gba(
                    _pksav_save.rival_name,
                    rival_name,
                    7
                );
            )

            return std::string(rival_name);
        } else {
            return (_pksav_save.trainer_info->gender == 0) ? "MAY" : "BRENDAN";
        }
    }

    int game_save_gbaimpl::get_money() {
        return int(pksav_littleendian32(*_pksav_save.money));
    }
}
