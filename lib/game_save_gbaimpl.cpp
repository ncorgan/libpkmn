/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "game_save_gbaimpl.hpp"
#include "item_bag_gbaimpl.hpp"
#include "item_list_modernimpl.hpp"
#include "pokedex_gbaimpl.hpp"
#include "pokemon_party_gbaimpl.hpp"
#include "pokemon_pc_gbaimpl.hpp"

#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

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
            pksav_gba_load_save_from_file(
                _filepath.c_str(),
                &_pksav_save
            );
        )

        std::string filename = boost::algorithm::to_lower_copy(
                                   fs::path(filepath).stem().string()
                               );
        boost::erase_all(filename, " ");
        switch(_pksav_save.save_type)
        {
            case PKSAV_GBA_SAVE_TYPE_RS:
                /*
                 * As there is no way to distinguish Ruby and Sapphire saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("ruby") != std::string::npos)
                {
                    _game_id = RUBY;
                }
                else if(filename.find("sapphire") != std::string::npos)
                {
                    _game_id = SAPPHIRE;
                }
                else
                {
                    // Default to Ruby, doesn't practically matter within a version group
                    _game_id = RUBY;
                }

                _item_pc = std::make_shared<item_list_modernimpl>(
                                RS_PC, _game_id,
                                _pksav_save.item_storage.pc_ptr->items,
                                50, false
                           );
                break;

            case PKSAV_GBA_SAVE_TYPE_FRLG:
                /*
                 * As there is no way to distinguish FireRed and LeafGreen saves from the saves
                 * themselves, we'll try to depend on the fact that .sav files match
                 * the name of their game's ROM, which are usually the game titles, so
                 * we'll check for the version in the filename.
                 */
                if(filename.find("firered") != std::string::npos or
                   filename.find("fr") != std::string::npos
                )
                {
                    _game_id = FIRERED;
                }
                else if(filename.find("leafgreen") != std::string::npos or
                        filename.find("lg") != std::string::npos)
                {
                    _game_id = LEAFGREEN;
                }
                else
                {
                    // Default to FireRed, doesn't practically matter within a version group
                    _game_id = FIRERED;
                }

                _item_pc = std::make_shared<item_list_modernimpl>(
                                FRLG_PC, _game_id,
                                _pksav_save.item_storage.pc_ptr->items,
                                50, false
                           );
                break;

            default: // Emerald
                _game_id = EMERALD;

                _item_pc = std::make_shared<item_list_modernimpl>(
                                EMERALD_PC, _game_id,
                                _pksav_save.item_storage.pc_ptr->items,
                                50, false
                           );
                break;
        }

        _pokedex = std::make_shared<pokedex_gbaimpl>(
                       _game_id,
                       &_pksav_save.pokedex
                   );

        _pokemon_party = std::make_shared<pokemon_party_gbaimpl>(
                             _game_id,
                             _pksav_save.pokemon_storage.party_ptr
                         );
        _pokemon_pc = std::make_shared<pokemon_pc_gbaimpl>(
                          _game_id,
                          _pksav_save.pokemon_storage.pc_ptr
                      );

        _item_bag = std::make_shared<item_bag_gbaimpl>(
                        _game_id, _pksav_save.item_storage.bag_ptr
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

    game_save_gbaimpl::~game_save_gbaimpl()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        pksav_gba_free_save(&_pksav_save);
    }

    void game_save_gbaimpl::save_as(
        const std::string &filepath
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gbaimpl::get_trainer_name()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        char trainer_name[8] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                _pksav_save.trainer_info.name_ptr,
                trainer_name,
                7
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gbaimpl::set_trainer_name(
        const std::string &trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        PKSAV_CALL(
            pksav_gba_export_text(
                trainer_name.c_str(),
                _pksav_save.trainer_info.name_ptr,
                7
            );
        )
    }

    uint32_t game_save_gbaimpl::get_trainer_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return pksav_littleendian32(_pksav_save.trainer_info.id_ptr->id);
    }

    void game_save_gbaimpl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        _pksav_save.trainer_info.id_ptr->id = pksav_littleendian32(trainer_id);
    }

    uint16_t game_save_gbaimpl::get_trainer_public_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return pksav_littleendian16(_pksav_save.trainer_info.id_ptr->pid);
    }

    void game_save_gbaimpl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        _pksav_save.trainer_info.id_ptr->pid = pksav_littleendian16(trainer_public_id);
    }

    uint16_t game_save_gbaimpl::get_trainer_secret_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return pksav_littleendian16(_pksav_save.trainer_info.id_ptr->sid);
    }

    void game_save_gbaimpl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        _pksav_save.trainer_info.id_ptr->sid = pksav_littleendian16(trainer_secret_id);
    }

    std::string game_save_gbaimpl::get_trainer_gender()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return (*_pksav_save.trainer_info.gender_ptr == 0) ? "Male" : "Female";
    }

    void game_save_gbaimpl::set_trainer_gender(
        const std::string &trainer_gender
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        if(trainer_gender == "Male")
        {
            *_pksav_save.trainer_info.gender_ptr = 0;
        }
        else if(trainer_gender == "Female")
        {
            *_pksav_save.trainer_info.gender_ptr = 1;
        }
        else
        {
            throw std::invalid_argument("trainer_gender: valid values \"Male\", \"Female\"");
        }
    }

    std::string game_save_gbaimpl::get_rival_name()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        std::string ret;

        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            char rival_name[8] = {0};
            PKSAV_CALL(
                pksav_gba_import_text(
                    _pksav_save.misc_fields.rival_name_ptr,
                    rival_name,
                    7
                );
            )

            ret = std::string(rival_name);
        }
        else
        {
            ret = (*_pksav_save.trainer_info.gender_ptr == 0) ? "MAY" : "BRENDAN";
        }

        return ret;
    }

    void game_save_gbaimpl::set_rival_name(
        const std::string &rival_name
    )
    {
        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            pkmn::enforce_string_length(
                "Rival name",
                rival_name,
                1,
                7
            );

            boost::lock_guard<game_save_gbaimpl> lock(*this);

            PKSAV_CALL(
                pksav_gba_export_text(
                    rival_name.c_str(),
                    _pksav_save.misc_fields.rival_name_ptr,
                    7
                );
            )
        }
        else
        {
            throw pkmn::feature_not_in_game_error("Rivals cannot be renamed in Ruby/Sapphire/Emerald.");
        }
    }

    int game_save_gbaimpl::get_money() {
        return int(pksav_littleendian32(*_pksav_save.trainer_info.money_ptr));
    }

    void game_save_gbaimpl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        *_pksav_save.trainer_info.money_ptr = pksav_littleendian32(uint32_t(money));
    }

    // Functions for attributes

    int game_save_gbaimpl::get_casino_coins()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return pksav_littleendian16(*_pksav_save.misc_fields.casino_coins_ptr);
    }

    void game_save_gbaimpl::set_casino_coins(
        int casino_coins
    )
    {
        pkmn::enforce_bounds(
            "Casino coins",
            casino_coins,
            0,
            9999
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        *_pksav_save.misc_fields.casino_coins_ptr = pksav_littleendian16(uint16_t(casino_coins));
    }

    // TODO: Nat Pokedex unlocked?
    void game_save_gbaimpl::_register_attributes()
    {
        using std::placeholders::_1;

        _numeric_attribute_engine.register_attribute_fcns(
            "Casino coins",
            std::bind(&game_save_gbaimpl::get_casino_coins, this),
            std::bind(&game_save_gbaimpl::set_casino_coins, this, _1)
        );
    }
}
