/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "game_save_gen6impl.hpp"
#include "item_bag_gen6impl.hpp"
#include "item_list_modernimpl.hpp"
#include "pokemon_party_gen6impl.hpp"
#include "pokemon_pc_gen6impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    game_save_gen6impl::game_save_gen6impl(
        const std::string& filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gen6_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        _game_id = _pksav_save.trainer_card->game;

        _pokemon_party = pkmn::make_shared<pokemon_party_gen6impl>(
                             _game_id,
                             _pksav_save.pokemon_party
                         );

        _pokemon_pc = pkmn::make_shared<pokemon_pc_gen6impl>(
                          _game_id,
                          _pksav_save.pokemon_pc,
                          _pksav_save.pokemon_box_info,
                          false // copy
                      );

        _item_bag = pkmn::make_shared<item_bag_gen6impl>(
                        _game_id,
                        _pksav_save.item_bag
                    );
    }

    game_save_gen6impl::~game_save_gen6impl()
    {
        pksav_gen6_save_free(&_pksav_save);
    }

    void game_save_gen6impl::save_as(
        const std::string& filepath
    )
    {
        PKSAV_CALL(
            pksav_gen6_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gen6impl::get_trainer_name()
    {
        char trainer_name[13] = {0};
        PKSAV_CALL(
            pksav_text_from_gen6(
                _pksav_save.trainer_card->name,
                trainer_name,
                12
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gen6impl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            12
        );

        PKSAV_CALL(
            pksav_text_to_gen6(
                trainer_name.c_str(),
                _pksav_save.trainer_card->name,
                12
            );
        )
    }

    uint32_t game_save_gen6impl::get_trainer_id()
    {
        return pksav_littleendian32(_pksav_save.trainer_card->id.id);
    }

    void game_save_gen6impl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        _pksav_save.trainer_card->id.id = pksav_littleendian32(trainer_id);
    }

    uint16_t game_save_gen6impl::get_trainer_public_id()
    {
        return pksav_littleendian16(_pksav_save.trainer_card->id.pid);
    }

    void game_save_gen6impl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        _pksav_save.trainer_card->id.pid = pksav_littleendian16(trainer_public_id);
    }

    uint16_t game_save_gen6impl::get_trainer_secret_id()
    {
        return pksav_littleendian16(_pksav_save.trainer_card->id.sid);
    }

    void game_save_gen6impl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    )
    {
        _pksav_save.trainer_card->id.sid = pksav_littleendian16(trainer_secret_id);
    }

    std::string game_save_gen6impl::get_trainer_gender()
    {
        return (_pksav_save.trainer_card->gender == 0) ? "Male" : "Female";
    }

    void game_save_gen6impl::set_trainer_gender(
        const std::string& trainer_gender
    ) {
        if(trainer_gender == "Male")
        {
            _pksav_save.trainer_card->gender = 0;
        }
        else if(trainer_gender == "Female")
        {
            _pksav_save.trainer_card->gender = 1;
        }
        else
        {
            throw std::invalid_argument("trainer_gender: valid values \"Male\", \"Female\"");
        }
    }

    std::string game_save_gen6impl::get_rival_name()
    {
        throw pkmn::unimplemented_error();
    }

    void game_save_gen6impl::set_rival_name(
        PKMN_UNUSED(const std::string& rival_name)
    )
    {
        throw pkmn::unimplemented_error();
    }

    int game_save_gen6impl::get_money()
    {
        throw pkmn::unimplemented_error();
    }

    void game_save_gen6impl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        throw pkmn::unimplemented_error();
    }
}
