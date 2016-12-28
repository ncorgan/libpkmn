/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen1impl.hpp"
#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int RED    = 1;
    BOOST_STATIC_CONSTEXPR int BLUE   = 2;
    BOOST_STATIC_CONSTEXPR int YELLOW = 3;

    BOOST_STATIC_CONSTEXPR int RB_PC = 2;
    BOOST_STATIC_CONSTEXPR int YELLOW_PC = 4;

    game_save_gen1impl::game_save_gen1impl(
        const std::string &filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gen1_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        if(_pksav_save.yellow) {
            _game_id = YELLOW;
        } else {
            /*
             * As there is no way to distinguish Red and Blue saves from the saves
             * themselves, we'll try to depend on the fact that .sav files match
             * the name of their game's ROM, which are usually the game titles, so
             * we'll check for the version in the filename. We also check for Yellow
             * here because PKSav's check is valid 99.9% of the time, but there is
             * a small chance the trainer's Pikachu despises him enough for the
             * friendship value to be 0.
             */
            std::string filename_lower = boost::algorithm::to_lower_copy(
                                              fs::path(filepath).stem().string()
                                         );
            if(filename_lower.find("red") != std::string::npos) {
                _game_id = RED;
            } else if(filename_lower.find("blue") != std::string::npos) {
                _game_id = BLUE;
            } else if(filename_lower.find("yellow") != std::string::npos) {
                _game_id = YELLOW;
            } else {
                // Default to Red, doesn't practically matter within a version group
                _game_id = RED;
            }
        }

        _item_bag = pkmn::make_shared<item_bag_gen1impl>(
                        _game_id, _pksav_save.item_bag
                    );
        _item_pc = pkmn::make_shared<item_list_gen1_pcimpl>(
                        (_game_id == YELLOW) ? YELLOW_PC : RB_PC,
                        _game_id, _pksav_save.item_pc
                   );
    }

    game_save_gen1impl::~game_save_gen1impl() {
        pksav_gen1_save_free(&_pksav_save);
    }

    void game_save_gen1impl::save_as(
        const std::string &filepath
    ) {
        PKSAV_CALL(
            pksav_gen1_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gen1impl::get_trainer_name() {
        char trainer_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen1(
                _pksav_save.trainer_name,
                trainer_name,
                7
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gen1impl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() == 0 or trainer_name.size() > 7) {
            throw std::invalid_argument("trainer_name: length must be 1-7");
        }

        PKSAV_CALL(
            pksav_text_to_gen1(
                trainer_name.c_str(),
                _pksav_save.trainer_name,
                7
            );
        )
    }

    uint32_t game_save_gen1impl::get_trainer_id() {
        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    void game_save_gen1impl::set_trainer_id(
        uint32_t trainer_id
    ) {
        if(trainer_id >= 65535) {
            throw pkmn::range_error("trainer_id", 0, 65535);
        }

        *_pksav_save.trainer_id = pksav_bigendian16(uint16_t(trainer_id));
    }

    uint16_t game_save_gen1impl::get_trainer_public_id() {
        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    void game_save_gen1impl::set_trainer_public_id(
        uint16_t trainer_public_id
    ) {
        *_pksav_save.trainer_id = pksav_bigendian16(trainer_public_id);
    }

    uint16_t game_save_gen1impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation I");
    }

    void game_save_gen1impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t trainer_secret_id)
    ) {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation I");
    }

    std::string game_save_gen1impl::get_trainer_gender() {
        return "Male";
    }

    void game_save_gen1impl::set_trainer_gender(
        PKMN_UNUSED(const std::string &trainer_gender)
    ) {
        throw pkmn::feature_not_in_game_error("All trainers are male in Generation I.");
    }

    std::string game_save_gen1impl::get_rival_name() {
        char rival_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen1(
                _pksav_save.rival_name,
                rival_name,
                7
            );
        )

        return std::string(rival_name);
    }

    void game_save_gen1impl::set_rival_name(
        const std::string &rival_name
    ) {
        if(rival_name.size() == 0 or rival_name.size() > 7) {
            throw std::invalid_argument("rival_name: length must be 1-7");
        }

        PKSAV_CALL(
            pksav_text_to_gen1(
                rival_name.c_str(),
                _pksav_save.rival_name,
                7
            );
        )
    }

    int game_save_gen1impl::get_money() {
        uint32_t ret = 0;
        PKSAV_CALL(
            pksav_from_base256(
                _pksav_save.money,
                3,
                &ret
            );
        )

        return int(ret);
    }

    void game_save_gen1impl::set_money(
        int money
    ) {
        if(money < 0 or money > MONEY_MAX_VALUE) {
            throw pkmn::range_error("money", 0, MONEY_MAX_VALUE);
        }

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(money),
                _pksav_save.money,
                3
            )
        )
    }
}
