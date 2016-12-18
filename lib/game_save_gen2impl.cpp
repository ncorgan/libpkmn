/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen2impl.hpp"
#include "item_bag_gen2impl.hpp"
#include "item_list_gbimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int GOLD    = 4;
    BOOST_STATIC_CONSTEXPR int SILVER  = 5;
    BOOST_STATIC_CONSTEXPR int CRYSTAL = 6;

    BOOST_STATIC_CONSTEXPR int GS_PC = 9;
    BOOST_STATIC_CONSTEXPR int CRYSTAL_PC = 14;

    game_save_gen2impl::game_save_gen2impl(
        const std::string &filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gen2_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        if(_pksav_save.gen2_game == PKSAV_GEN2_CRYSTAL) {
            _game_id = CRYSTAL;
        } else {
            /*
             * As there is no way to distinguish Gold and Silver saves from the saves
             * themselves, we'll try to depend on the fact that .sav files match
             * the name of their game's ROM, which are usually the game titles, so
             * we'll check for the version in the filename.
             */
            std::string filename_lower = boost::algorithm::to_lower_copy(
                                              fs::path(filepath).stem().string()
                                         );
            if(filename_lower.find("gold") != std::string::npos) {
                _game_id = GOLD;
            } else if(filename_lower.find("silver") != std::string::npos) {
                _game_id = SILVER;
            } else {
                // Default to Gold, doesn't practically matter within a version group
                _game_id = GOLD;
            }
        }

        _item_bag = pkmn::make_shared<item_bag_gen2impl>(
                        _game_id, _pksav_save.item_bag
                    );
        _item_pc = pkmn::make_shared<item_list_gen2_pcimpl>(
                        (_game_id == CRYSTAL) ? CRYSTAL_PC : GS_PC,
                        _game_id, _pksav_save.item_pc
                   );
    }

    game_save_gen2impl::~game_save_gen2impl() {
        pksav_gen2_save_free(&_pksav_save);
    }

    std::string game_save_gen2impl::get_trainer_name() {
        char trainer_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen2(
                _pksav_save.trainer_name,
                trainer_name,
                7
            );
        )

        return std::string(trainer_name);
    }

    uint32_t game_save_gen2impl::get_trainer_id() {
        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    uint16_t game_save_gen2impl::get_trainer_public_id() {
        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    uint16_t game_save_gen2impl::get_trainer_secret_id() {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation II");
    }

    std::string game_save_gen2impl::get_trainer_gender() {
        if(_game_id == CRYSTAL) {
            return (*_pksav_save.trainer_gender == PKSAV_GEN2_MALE) ? "Male" : "Female";
        } else {
            return "Male";
        }
    }

    std::string game_save_gen2impl::get_rival_name() {
        char rival_name[8] = {0};
        PKSAV_CALL(
            pksav_text_from_gen2(
                _pksav_save.rival_name,
                rival_name,
                7
            );
        )

        return std::string(rival_name);
    }

    int game_save_gen2impl::get_money() {
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
}
