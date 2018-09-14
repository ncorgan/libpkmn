/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_impl.hpp"
#include "game_save_gen1impl.hpp"
#include "game_save_gen2impl.hpp"
#include "game_save_gbaimpl.hpp"
#include "game_save_gcnimpl.hpp"

#include "database/enum_conversions.hpp"
#include "database/id_to_string.hpp"

#include "io/read_write.hpp"

#include "pkmgc/includes.hpp"
#include "pksav/pksav_call.hpp"

#include "types/mutex_helpers.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <boost/filesystem.hpp>

#include <fstream>
#include <memory>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_BIN_SIZE = 0x60000;
    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_GCI_SIZE = 0x60040;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_BIN_SIZE = 0x56000;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_GCI_SIZE = 0x56040;

    // Map PKSav save type enums to LibPKMN save type enum. The std::hash<int>
    // is needed because enums can't be used as unordered_map keys until C++14.
    template <typename pksav_enum_type>
    using pksav_enum_map_t = std::unordered_map<pksav_enum_type, pkmn::e_game_save_type, std::hash<int>>;

    static const pksav_enum_map_t<enum pksav_gen1_save_type> PKSAV_GEN1_ENUM_MAP =
    {
        {PKSAV_GEN1_SAVE_TYPE_NONE,     pkmn::e_game_save_type::NONE},
        {PKSAV_GEN1_SAVE_TYPE_RED_BLUE, pkmn::e_game_save_type::RED_BLUE},
        {PKSAV_GEN1_SAVE_TYPE_YELLOW,   pkmn::e_game_save_type::YELLOW}
    };
    static const pksav_enum_map_t<enum pksav_gen2_save_type> PKSAV_GEN2_ENUM_MAP =
    {
        {PKSAV_GEN2_SAVE_TYPE_NONE,    pkmn::e_game_save_type::NONE},
        {PKSAV_GEN2_SAVE_TYPE_GS,      pkmn::e_game_save_type::GOLD_SILVER},
        {PKSAV_GEN2_SAVE_TYPE_CRYSTAL, pkmn::e_game_save_type::CRYSTAL}
    };
    static const pksav_enum_map_t<enum pksav_gba_save_type> PKSAV_GBA_ENUM_MAP =
    {
        {PKSAV_GBA_SAVE_TYPE_NONE,    pkmn::e_game_save_type::NONE},
        {PKSAV_GBA_SAVE_TYPE_RS,      pkmn::e_game_save_type::RUBY_SAPPHIRE},
        {PKSAV_GBA_SAVE_TYPE_EMERALD, pkmn::e_game_save_type::EMERALD},
        {PKSAV_GBA_SAVE_TYPE_FRLG,    pkmn::e_game_save_type::FIRERED_LEAFGREEN}
    };

    static pkmn::e_game_save_type _detect_save_type(
        const std::vector<uint8_t>& data
    )
    {
        pkmn::e_game_save_type ret = pkmn::e_game_save_type::NONE;

        size_t data_size = data.size();

        std::unique_ptr<LibPkmGC::GC::SaveEditing::Save> gcn_save_uptr;
        if((data_size == GCN_COLOSSEUM_BIN_SIZE) or (data_size == GCN_COLOSSEUM_GCI_SIZE))
        {
            gcn_save_uptr.reset(
                new LibPkmGC::Colosseum::SaveEditing::Save(
                        data.data(),
                        (data_size == GCN_COLOSSEUM_GCI_SIZE)
                    )
            );
        }
        else if((data_size == GCN_XD_BIN_SIZE) or (data_size == GCN_XD_GCI_SIZE))
        {
            gcn_save_uptr.reset(
                new LibPkmGC::XD::SaveEditing::Save(
                        data.data(),
                        (data_size == GCN_XD_GCI_SIZE)
                    )
            );
        }

        if(gcn_save_uptr.get() != nullptr)
        {
            size_t save_index = 0;
            if(gcn_save_uptr->getMostRecentValidSlot(0, &save_index))
            {
                ret = pkmn::e_game_save_type::COLOSSEUM_XD;
            }
        }

        if(ret == pkmn::e_game_save_type::NONE)
        {
            enum pksav_gba_save_type gba_save_type = PKSAV_GBA_SAVE_TYPE_NONE;
            PKSAV_CALL(
                pksav_gba_get_buffer_save_type(
                    data.data(),
                    data_size,
                    &gba_save_type
                );
            );

            BOOST_ASSERT(PKSAV_GBA_ENUM_MAP.count(gba_save_type) > 0);
            ret = PKSAV_GBA_ENUM_MAP.at(gba_save_type);

            // Checking Gen I before Gen II tends to remove Generation II saves
            // registering as Generation I.
            if(ret == pkmn::e_game_save_type::NONE)
            {
                enum pksav_gen1_save_type gen1_save_type = PKSAV_GEN1_SAVE_TYPE_NONE;
                PKSAV_CALL(
                    pksav_gen1_get_buffer_save_type(
                        data.data(),
                        data_size,
                        &gen1_save_type
                    );
                );

                BOOST_ASSERT(PKSAV_GEN1_ENUM_MAP.count(gen1_save_type) > 0);
                ret = PKSAV_GEN1_ENUM_MAP.at(gen1_save_type);

                if(ret == pkmn::e_game_save_type::NONE)
                {
                    enum pksav_gen2_save_type gen2_save_type = PKSAV_GEN2_SAVE_TYPE_NONE;
                    PKSAV_CALL(
                        pksav_gen2_get_buffer_save_type(
                            data.data(),
                            data_size,
                            &gen2_save_type
                        );
                    );

                    BOOST_ASSERT(PKSAV_GEN2_ENUM_MAP.count(gen2_save_type) > 0);
                    ret = PKSAV_GEN2_ENUM_MAP.at(gen2_save_type);
                }
            }
        }

        return ret;
    }

    pkmn::e_game_save_type game_save::detect_type(
        const std::string& filepath
    )
    {
        if(!fs::exists(filepath))
        {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> raw(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(raw.data()), filesize);
        ifile.close();

        return _detect_save_type(raw);
    }

    game_save::sptr game_save::from_file(
        const std::string& filepath
    )
    {
        std::vector<uint8_t> raw = pkmn::io::read_file(filepath);

        pkmn::e_game_save_type save_type = _detect_save_type(raw);

        switch(save_type)
        {
            case pkmn::e_game_save_type::RED_BLUE:
            case pkmn::e_game_save_type::YELLOW:
                return std::make_shared<game_save_gen1impl>(
                           filepath,
                           std::move(raw)
                       );

            case pkmn::e_game_save_type::GOLD_SILVER:
            case pkmn::e_game_save_type::CRYSTAL:
                return std::make_shared<game_save_gen2impl>(
                           filepath,
                           std::move(raw)
                       );

            case pkmn::e_game_save_type::RUBY_SAPPHIRE:
            case pkmn::e_game_save_type::EMERALD:
            case pkmn::e_game_save_type::FIRERED_LEAFGREEN:
                return std::make_shared<game_save_gbaimpl>(
                           filepath,
                           std::move(raw)
                       );

            case pkmn::e_game_save_type::COLOSSEUM_XD:
                return std::make_shared<game_save_gcnimpl>(
                           filepath,
                           std::move(raw)
                       );

            case pkmn::e_game_save_type::NONE:
            default:
                throw std::invalid_argument("Invalid save (or unimplemented).");
        }
    }

    game_save_impl::game_save_impl(
        const std::string& filepath,
        std::vector<uint8_t>&& raw
    ): game_save(),
       _game_id(0),
       _raw(std::move(raw))
    {
        if(not fs::exists(filepath))
        {
            throw std::invalid_argument("The given filepath does not exist.");
        }

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_impl::get_filepath()
    {
        return _filepath;
    }

    void game_save_impl::save()
    {
        save_as(_filepath);
    }

    pkmn::e_game game_save_impl::get_game()
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    const pkmn::pokedex::sptr& game_save_impl::get_pokedex()
    {
        return _pokedex;
    }

    const pkmn::pokemon_party::sptr& game_save_impl::get_pokemon_party()
    {
        return _pokemon_party;
    }

    const pkmn::pokemon_pc::sptr& game_save_impl::get_pokemon_pc()
    {
        return _pokemon_pc;
    }

    const pkmn::item_bag::sptr& game_save_impl::get_item_bag()
    {
        return _item_bag;
    }

    const pkmn::item_list::sptr& game_save_impl::get_item_pc()
    {
        return _item_pc;
    }
}
