/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen4impl.hpp"
#include "item_bag_gen4impl.hpp"
#include "pokemon_party_gen4impl.hpp"
#include "pokemon_pc_gen4impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int DIAMOND    = 12;
    BOOST_STATIC_CONSTEXPR int PEARL      = 13;
    BOOST_STATIC_CONSTEXPR int PLATINUM   = 14;
    BOOST_STATIC_CONSTEXPR int HEARTGOLD  = 15;
    BOOST_STATIC_CONSTEXPR int SOULSILVER = 16;

    game_save_gen4impl::game_save_gen4impl(
        const std::string &filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gen4_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        std::string filename = boost::algorithm::to_lower_copy(
                                   fs::path(filepath).stem().string()
                               );
        boost::erase_all(filename, " ");
        switch(_pksav_save.gen4_game) {
            case PKSAV_GEN4_DP:
                /*
                 * As there is no way to distinguish Diamond and Pearl saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("diamond") != std::string::npos) {
                    _game_id = DIAMOND;
                } else if(filename.find("pearl") != std::string::npos) {
                    _game_id = PEARL;
                } else {
                    // Default to Diamond, doesn't practically matter within a version group
                    _game_id = DIAMOND;
                }
                break;

            case PKSAV_GEN4_HGSS:
                /*
                 * As there is no way to distinguish HeartGold and SoulSilver saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("heartgold") != std::string::npos or
                   filename.find("hg") != std::string::npos
                ) {
                    _game_id = HEARTGOLD;
                } else if(filename.find("soulsilver") != std::string::npos or
                          filename.find("ss") != std::string::npos)
                {
                    _game_id = SOULSILVER;
                } else {
                    // Default to FireRed, doesn't practically matter within a version group
                    _game_id = HEARTGOLD;
                }
                break;

            default: // Platinum
                _game_id = PLATINUM;
                break;
        }

        _pokemon_party = pkmn::make_shared<pokemon_party_gen4impl>(
                             _game_id,
                             _pksav_save.pokemon_party
                         );
        _pokemon_pc = pkmn::make_shared<pokemon_pc_gen4impl>(
                          _game_id,
                          _pksav_save.pokemon_pc
                      );

        _item_bag = pkmn::make_shared<item_bag_gen4impl>(
                        _game_id, _pksav_save.item_bag
                    );
    }

    game_save_gen4impl::~game_save_gen4impl() {
        pksav_gen4_save_free(&_pksav_save);
    }

    void game_save_gen4impl::save_as(
        const std::string &filepath
    ) {
        PKSAV_CALL(
            pksav_gen4_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gen4impl::get_trainer_name() {
        char trainer_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen4(
                _pksav_save.trainer_name,
                trainer_name,
                7
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gen4impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() == 0 or trainer_name.size() > 7) {
            throw std::invalid_argument("trainer_name: valid length 1-7");
        }

        PKSAV_CALL(
            pksav_text_to_gen4(
                trainer_name.c_str(),
                _pksav_save.trainer_name,
                7
            );
        )
    }

    uint32_t game_save_gen4impl::get_trainer_id() {
        return pksav_littleendian32(_pksav_save.trainer_id->id);
    }

    void game_save_gen4impl::set_trainer_id(
        uint32_t trainer_id
    ) {
        _pksav_save.trainer_id->id = pksav_littleendian32(trainer_id);
    }

    uint16_t game_save_gen4impl::get_trainer_public_id() {
        return pksav_littleendian16(_pksav_save.trainer_id->pid);
    }

    void game_save_gen4impl::set_trainer_public_id(
        uint16_t trainer_public_id
    ) {
        _pksav_save.trainer_id->pid = pksav_littleendian16(trainer_public_id);
    }

    uint16_t game_save_gen4impl::get_trainer_secret_id() {
        return pksav_littleendian16(_pksav_save.trainer_id->sid);
    }

    void game_save_gen4impl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    ) {
        _pksav_save.trainer_id->sid = pksav_littleendian16(trainer_secret_id);
    }

    std::string game_save_gen4impl::get_trainer_gender() {
        return (_pksav_save.trainer_gender == 0) ? "Male" : "Female";
    }

    void game_save_gen4impl::set_trainer_gender(
        const std::string &trainer_gender
    ) {
        if(trainer_gender == "Male") {
            *_pksav_save.trainer_gender = 0;
        } else if(trainer_gender == "Female") {
            *_pksav_save.trainer_gender = 1;
        } else {
            throw std::invalid_argument("trainer_gender: valid values \"Male\", \"Female\"");
        }
    }

    std::string game_save_gen4impl::get_rival_name() {
        char rival_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen4(
                _pksav_save.rival_name,
                rival_name,
                7
            );
        )

        return std::string(rival_name);
    }

    void game_save_gen4impl::set_rival_name(
        const std::string &rival_name
    ) {
        if(rival_name.size() == 0 or rival_name.size() > 7) {
            throw std::invalid_argument("rival_name: valid length 1-7");
        }

        PKSAV_CALL(
            pksav_text_to_gen4(
                rival_name.c_str(),
                _pksav_save.rival_name,
                7
            );
        )
    }

    int game_save_gen4impl::get_money() {
        return int(pksav_littleendian32(*_pksav_save.money));
    }

    void game_save_gen4impl::set_money(
        int money
    ) {
        if(money < 0 or money > MONEY_MAX_VALUE) {
            pkmn::throw_out_of_range("money", 0, MONEY_MAX_VALUE);
        }

        *_pksav_save.money = pksav_littleendian32(uint32_t(money));
    }
}
