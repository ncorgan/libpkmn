/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen1impl.hpp"
#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"
#include "pokedex_impl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_pc_gen1impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/filesystem.hpp>

#include <stdexcept>

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

        _pokedex = std::make_shared<pokedex_impl>(
                       _game_id,
                       _pksav_save.pokedex_seen,
                       _pksav_save.pokedex_owned
                   );

        _pokemon_party = std::make_shared<pokemon_party_gen1impl>(
                             _game_id,
                             _pksav_save.pokemon_party
                         );
        _pokemon_pc = std::make_shared<pokemon_pc_gen1impl>(
                          _game_id,
                          _pksav_save.pokemon_boxes,
                          false
                      );
        _item_bag = std::make_shared<item_bag_gen1impl>(
                        _game_id,
                        _pksav_save.item_bag
                    );
        _item_pc = std::make_shared<item_list_gen1_pcimpl>(
                        (_game_id == YELLOW) ? YELLOW_PC : RB_PC,
                        _game_id,
                        _pksav_save.item_pc
                   );

        // When a Pokémon is added to the PC or party, it should be
        // reflected in the Pokédex.

        pokemon_party_impl* party_impl_ptr = dynamic_cast<pokemon_party_impl*>(_pokemon_party.get());
        pokemon_pc_impl* pc_impl_ptr = dynamic_cast<pokemon_pc_impl*>(_pokemon_pc.get());

        BOOST_ASSERT(party_impl_ptr);
        BOOST_ASSERT(pc_impl_ptr);

        party_impl_ptr->set_pokedex(_pokedex);
        pc_impl_ptr->set_pokedex(_pokedex);

        _register_attributes();
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
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

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
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(trainer_id);

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
    )
    {
        pkmn::enforce_string_length(
            "Rival name",
            rival_name,
            1,
            7
        );

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
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(money),
                _pksav_save.money,
                3
            )
        )
    }

    // Functions for attributes

    int game_save_gen1impl::get_casino_coins()
    {
        uint32_t ret = 0;

        PKSAV_CALL(
            pksav_from_bcd(
                _pksav_save.casino_coins,
                2,
                &ret
            )
        );

        return int(ret);
    }

    void game_save_gen1impl::set_casino_coins(
        int casino_coins
    )
    {
        pkmn::enforce_bounds(
            "Casino coins",
            casino_coins,
            0,
            9999
        );

        // Work around PKSav not taking in a buffer size parameter on set
        // by clearing the buffer and manually setting smaller numbers at
        // the right-most byte.
        std::memset(_pksav_save.casino_coins, 0, 2);

        uint8_t* casino_coins_ptr = _pksav_save.casino_coins;
        if(casino_coins > 99)
        {
        }

        PKSAV_CALL(
            pksav_to_bcd(
                uint32_t(casino_coins),
                casino_coins_ptr
            )
        );
    }

    int game_save_gen1impl::get_pikachu_friendship()
    {
        return *_pksav_save.pikachu_friendship;
    }

    void game_save_gen1impl::set_pikachu_friendship(
        int pikachu_friendship
    )
    {
        pkmn::enforce_bounds(
            "Pikachu friendship",
            pikachu_friendship,
            0,
            255
        );

        *_pksav_save.pikachu_friendship = uint8_t(pikachu_friendship);
    }

    void game_save_gen1impl::_register_attributes()
    {
        using std::placeholders::_1;

        _numeric_attribute_engine.register_attribute_fcns(
            "Casino coins",
            std::bind(&game_save_gen1impl::get_casino_coins, this),
            std::bind(&game_save_gen1impl::set_casino_coins, this, _1)
        );

        if(_game_id == YELLOW)
        {
            _numeric_attribute_engine.register_attribute_fcns(
                "Pikachu friendship",
                std::bind(&game_save_gen1impl::get_pikachu_friendship, this),
                std::bind(&game_save_gen1impl::set_pikachu_friendship, this, _1)
            );
        }
    }
}
