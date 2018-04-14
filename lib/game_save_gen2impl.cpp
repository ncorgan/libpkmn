/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "game_save_gen2impl.hpp"
#include "item_bag_gen2impl.hpp"
#include "item_list_gbimpl.hpp"
#include "pokedex_impl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_pc_gen2impl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int GOLD    = 4;
    BOOST_STATIC_CONSTEXPR int SILVER  = 5;
    BOOST_STATIC_CONSTEXPR int CRYSTAL = 6;

    BOOST_STATIC_CONSTEXPR int GS_PC = 9;
    BOOST_STATIC_CONSTEXPR int CRYSTAL_PC = 14;

    game_save_gen2impl::game_save_gen2impl(
        const std::string& filepath
    ): game_save_impl(filepath)
    {
        PKSAV_CALL(
            pksav_gen2_save_load(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        if(_pksav_save.gen2_game == PKSAV_GEN2_CRYSTAL)
        {
            _game_id = CRYSTAL;

            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            CRYSTAL_PC,
                            _game_id,
                            _pksav_save.item_pc
                       );
        }
        else
        {
            /*
             * As there is no way to distinguish Gold and Silver saves from the saves
             * themselves, we'll try to depend on the fact that .sav files match
             * the name of their game's ROM, which are usually the game titles, so
             * we'll check for the version in the filename.
             */
            std::string filename_lower = boost::algorithm::to_lower_copy(
                                              fs::path(filepath).stem().string()
                                         );
            if(filename_lower.find("gold") != std::string::npos)
            {
                _game_id = GOLD;
            }
            else if(filename_lower.find("silver") != std::string::npos)
            {
                _game_id = SILVER;
            }
            else
            {
                // Default to Gold, doesn't practically matter within a version group
                _game_id = GOLD;
            }

            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            GS_PC,
                            _game_id,
                            _pksav_save.item_pc
                       );
        }

        _pokedex = std::make_shared<pokedex_impl>(
                       _game_id,
                       _pksav_save.pokedex_seen,
                       _pksav_save.pokedex_owned
                   );

        _pokemon_party = std::make_shared<pokemon_party_gen2impl>(
                             _game_id,
                             _pksav_save.pokemon_party
                         );
        _pokemon_pc = std::make_shared<pokemon_pc_gen2impl>(
                          _game_id,
                          _pksav_save.pokemon_boxes,
                          _pksav_save.pokemon_box_names,
                          false
                      );
        _item_bag = std::make_shared<item_bag_gen2impl>(
                        _game_id,
                        _pksav_save.item_bag
                    );

        // When a Pokémon is added to the PC or party, it should be
        // reflected in the Pokédex.

        pokemon_party_impl* party_impl_ptr = dynamic_cast<pokemon_party_impl*>(_pokemon_party.get());
        pokemon_pc_impl* pc_impl_ptr = dynamic_cast<pokemon_pc_impl*>(_pokemon_pc.get());

        BOOST_ASSERT(party_impl_ptr);
        BOOST_ASSERT(pc_impl_ptr);

        party_impl_ptr->set_pokedex(_pokedex);
        pc_impl_ptr->set_pokedex(_pokedex);
    }

    game_save_gen2impl::~game_save_gen2impl()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        pksav_gen2_save_free(&_pksav_save);
    }

    void game_save_gen2impl::save_as(
        const std::string& filepath
    )
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        PKSAV_CALL(
            pksav_gen2_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gen2impl::get_trainer_name()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

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

    void game_save_gen2impl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<game_save_gen2impl> lock(*this);

        PKSAV_CALL(
            pksav_text_to_gen2(
                trainer_name.c_str(),
                _pksav_save.trainer_name,
                7
            );
        )
    }

    uint32_t game_save_gen2impl::get_trainer_id()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    void game_save_gen2impl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(trainer_id);

        boost::lock_guard<game_save_gen2impl> lock(*this);

        *_pksav_save.trainer_id = pksav_bigendian16(uint16_t(trainer_id));
    }

    uint16_t game_save_gen2impl::get_trainer_public_id()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        return pksav_bigendian16(*_pksav_save.trainer_id);
    }

    void game_save_gen2impl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        *_pksav_save.trainer_id = pksav_bigendian16(trainer_public_id);
    }

    uint16_t game_save_gen2impl::get_trainer_secret_id()
    {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation II");
    }

    void game_save_gen2impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t trainer_secret_id)
    )
    {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation II");
    }

    std::string game_save_gen2impl::get_trainer_gender()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        std::string ret;

        if(_game_id == CRYSTAL)
        {
            ret = (*_pksav_save.trainer_gender == PKSAV_GEN2_MALE) ? "Male" : "Female";
        }
        else
        {
            ret = "Male";
        }

        return ret;
    }

    void game_save_gen2impl::set_trainer_gender(
        const std::string& trainer_gender
    )
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        if(_game_id == CRYSTAL)
        {
            if(trainer_gender == "Male")
            {
                *_pksav_save.trainer_gender = uint8_t(PKSAV_GEN2_MALE);
            }
            else if(trainer_gender == "Female")
            {
                *_pksav_save.trainer_gender = uint8_t(PKSAV_GEN2_FEMALE);
            }
            else
            {
                throw std::invalid_argument("trainer_gender: valid values \"Male\", \"Female\"");
            }
        }
        else
        {
            throw pkmn::feature_not_in_game_error("All trainers are male in Gold/Silver.");
        }
    }

    std::string game_save_gen2impl::get_rival_name()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

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

    void game_save_gen2impl::set_rival_name(
        const std::string& rival_name
    )
    {
        pkmn::enforce_string_length(
            "Rival name",
            rival_name,
            1,
            7
        );

        boost::lock_guard<game_save_gen2impl> lock(*this);

        PKSAV_CALL(
            pksav_text_to_gen2(
                rival_name.c_str(),
                _pksav_save.rival_name,
                7
            );
        )
    }

    int game_save_gen2impl::get_money()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

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

    void game_save_gen2impl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gen2impl> lock(*this);

        PKSAV_CALL(
            pksav_to_base256(
                uint32_t(money),
                _pksav_save.money,
                3
            );
        )
    }
}
