/*
 * Copyright (c) 2016-201PKSAV_GEN2_TRAINER_NAME_LENGTH + 1 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "game_save_gen2impl.hpp"
#include "item_bag_gen2impl.hpp"
#include "item_list_gbimpl.hpp"
#include "pokedex_gbimpl.hpp"
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

// TODO: put enum back in PKSav
#define PKSAV_GEN2_GENDER_MALE 0
#define PKSAV_GEN2_GENDER_FEMALE 1

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int GOLD_GAME_ID    = 4;
    BOOST_STATIC_CONSTEXPR int SILVER_GAME_ID  = 5;
    BOOST_STATIC_CONSTEXPR int CRYSTAL_GAME_ID = 6;

    BOOST_STATIC_CONSTEXPR int GS_PC_ID = 9;
    BOOST_STATIC_CONSTEXPR int CRYSTAL_PC_ID = 14;

    game_save_gen2impl::game_save_gen2impl(
        const std::string& filepath,
        std::vector<uint8_t>&& raw
    ): game_save_impl(filepath, std::move(raw))
    {
        PKSAV_CALL(
            pksav_gen2_load_save_from_buffer(
                _raw.data(),
                _raw.size(),
                &_pksav_save
            );
        )

        if(_pksav_save.save_type == PKSAV_GEN2_SAVE_TYPE_CRYSTAL)
        {
            _game_id = CRYSTAL_GAME_ID;

            BOOST_ASSERT(_pksav_save.item_storage.item_pc_ptr != nullptr);
            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            CRYSTAL_PC_ID,
                            _game_id,
                            _pksav_save.item_storage.item_pc_ptr
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
                _game_id = GOLD_GAME_ID;
            }
            else if(filename_lower.find("silver") != std::string::npos)
            {
                _game_id = SILVER_GAME_ID;
            }
            else
            {
                // Default to Gold, doesn't practically matter within a version group
                _game_id = GOLD_GAME_ID;
            }

            BOOST_ASSERT(_pksav_save.item_storage.item_pc_ptr != nullptr);
            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            GS_PC_ID,
                            _game_id,
                            _pksav_save.item_storage.item_pc_ptr
                       );
        }

        BOOST_ASSERT(_pksav_save.pokedex_lists.seen_ptr != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex_lists.owned_ptr != nullptr);
        _pokedex = std::make_shared<pokedex_gbimpl>(
                       _game_id,
                       _pksav_save.pokedex_lists.seen_ptr,
                       _pksav_save.pokedex_lists.owned_ptr
                   );

        BOOST_ASSERT(_pksav_save.pokemon_storage.party_ptr != nullptr);
        _pokemon_party = std::make_shared<pokemon_party_gen2impl>(
                             _game_id,
                             _pksav_save.pokemon_storage.party_ptr
                         );

        BOOST_ASSERT(_pksav_save.pokemon_storage.box_ptrs != nullptr);
        BOOST_ASSERT(_pksav_save.pokemon_storage.box_names_ptr != nullptr);
        _pokemon_pc = std::make_shared<pokemon_pc_gen2impl>(
                          _game_id,
                          _pksav_save.pokemon_storage.box_ptrs,
                          _pksav_save.pokemon_storage.box_names_ptr,
                          false
                      );

        BOOST_ASSERT(_pksav_save.item_storage.item_bag_ptr != nullptr);
        _item_bag = std::make_shared<item_bag_gen2impl>(
                        _game_id,
                        _pksav_save.item_storage.item_bag_ptr
                    );

        // When a Pokémon is added to the PC or party, it should be
        // reflected in the Pokédex.

        pokemon_party_impl* party_impl_ptr = dynamic_cast<pokemon_party_impl*>(_pokemon_party.get());
        pokemon_pc_impl* pc_impl_ptr = dynamic_cast<pokemon_pc_impl*>(_pokemon_pc.get());

        BOOST_ASSERT(party_impl_ptr != nullptr);
        BOOST_ASSERT(pc_impl_ptr != nullptr);

        party_impl_ptr->set_pokedex(_pokedex);
        pc_impl_ptr->set_pokedex(_pokedex);
    }

    game_save_gen2impl::~game_save_gen2impl()
    {
        pksav_gen2_free_save(&_pksav_save);
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

        BOOST_ASSERT(_pksav_save.trainer_info.name_ptr != nullptr);

        char trainer_name[PKSAV_GEN2_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                _pksav_save.trainer_info.name_ptr,
                trainer_name,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
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
            PKSAV_GEN2_TRAINER_NAME_LENGTH
        );

        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.name_ptr != nullptr);

        PKSAV_CALL(
            pksav_gen2_export_text(
                trainer_name.c_str(),
                _pksav_save.trainer_info.name_ptr,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
            );
        )
    }

    uint32_t game_save_gen2impl::get_trainer_id()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.id_ptr != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.id_ptr);
    }

    void game_save_gen2impl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(trainer_id);

        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.id_ptr != nullptr);

        *_pksav_save.trainer_info.id_ptr = pksav_bigendian16(uint16_t(trainer_id));
    }

    uint16_t game_save_gen2impl::get_trainer_public_id()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.id_ptr != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.id_ptr);
    }

    void game_save_gen2impl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.id_ptr != nullptr);

        *_pksav_save.trainer_info.id_ptr = pksav_bigendian16(trainer_public_id);
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

        if(_game_id == CRYSTAL_GAME_ID)
        {
            ret = (*_pksav_save.trainer_info.gender_ptr == PKSAV_GEN2_GENDER_MALE) ? "Male" : "Female";
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

        if(_game_id == CRYSTAL_GAME_ID)
        {
            BOOST_ASSERT(_pksav_save.trainer_info.gender_ptr != nullptr);

            if(trainer_gender == "Male")
            {
                *_pksav_save.trainer_info.gender_ptr = uint8_t(PKSAV_GEN2_GENDER_MALE);
            }
            else if(trainer_gender == "Female")
            {
                *_pksav_save.trainer_info.gender_ptr = uint8_t(PKSAV_GEN2_GENDER_FEMALE);
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

        BOOST_ASSERT(_pksav_save.misc_fields.rival_name_ptr != nullptr);

        char rival_name[PKSAV_GEN2_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                _pksav_save.misc_fields.rival_name_ptr,
                rival_name,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
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
            PKSAV_GEN2_TRAINER_NAME_LENGTH
        );

        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.rival_name_ptr != nullptr);

        PKSAV_CALL(
            pksav_gen2_export_text(
                rival_name.c_str(),
                _pksav_save.misc_fields.rival_name_ptr,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
            );
        )
    }

    int game_save_gen2impl::get_money()
    {
        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.money_ptr != nullptr);

        size_t money_from_pksav = 0;
        PKSAV_CALL(
            pksav_import_base256(
                _pksav_save.trainer_info.money_ptr,
                3,
                &money_from_pksav
            );
        )

        return int(money_from_pksav);
    }

    void game_save_gen2impl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.money_ptr != nullptr);

        PKSAV_CALL(
            pksav_export_base256(
                size_t(money),
                _pksav_save.trainer_info.money_ptr,
                3
            );
        )
    }
}
