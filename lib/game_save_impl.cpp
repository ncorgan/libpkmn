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
#include "database/id_to_string.hpp"

#include "libpkmgc_includes.hpp"
#include "pksav/pksav_call.hpp"

#include <pksav/gen1/save.h>
#include <pksav/gen2/save.h>
#include <pksav/gba/save.h>

#include <boost/filesystem.hpp>

#include <fstream>
#include <memory>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    typedef enum
    {
        PKMN_SAVE_TYPE_NONE = 0,
        PKMN_SAVE_TYPE_RED_BLUE,
        PKMN_SAVE_TYPE_YELLOW,
        PKMN_SAVE_TYPE_GOLD_SILVER,
        PKMN_SAVE_TYPE_CRYSTAL,
        PKMN_SAVE_TYPE_RUBY_SAPPHIRE,
        PKMN_SAVE_TYPE_EMERALD,
        PKMN_SAVE_TYPE_FIRERED_LEAFGREEN,
        PKMN_SAVE_TYPE_COLOSSEUM_XD,
    } pkmn_save_type_t;

    static const std::vector<std::string> SAVE_TYPE_NAMES =
    {
        "None",
        "Red/Blue",
        "Yellow",
        "Gold/Silver",
        "Crystal",
        "Ruby/Sapphire",
        "Emerald",
        "FireRed/LeafGreen",
        "Colosseum/XD"
    };

    BOOST_STATIC_CONSTEXPR size_t GB_SAVE_SIZE  = 0x8000;
    BOOST_STATIC_CONSTEXPR size_t GBA_SAVE_SIZE = 0x10000;

    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_BIN_SIZE = 0x60000;
    BOOST_STATIC_CONSTEXPR size_t GCN_COLOSSEUM_GCI_SIZE = 0x60040;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_BIN_SIZE = 0x56000;
    BOOST_STATIC_CONSTEXPR size_t GCN_XD_GCI_SIZE = 0x56040;

    // Map PKSav save type enums to LibPKMN save type enum. The std::hash<int>
    // is needed because enums can't be used as unordered_map keys until C++14.
    template <typename pksav_enum_type>
    using pksav_enum_map_t = std::unordered_map<pksav_enum_type, pkmn_save_type_t, std::hash<int>>;

    static const pksav_enum_map_t<enum pksav_gen1_save_type> PKSAV_GEN1_ENUM_MAP =
    {
        {PKSAV_GEN1_SAVE_TYPE_NONE,     PKMN_SAVE_TYPE_NONE},
        {PKSAV_GEN1_SAVE_TYPE_RED_BLUE, PKMN_SAVE_TYPE_RED_BLUE},
        {PKSAV_GEN1_SAVE_TYPE_YELLOW,   PKMN_SAVE_TYPE_YELLOW}
    };
    static const pksav_enum_map_t<enum pksav_gen2_save_type> PKSAV_GEN2_ENUM_MAP =
    {
        {PKSAV_GEN2_SAVE_TYPE_NONE,    PKMN_SAVE_TYPE_NONE},
        {PKSAV_GEN2_SAVE_TYPE_GS,      PKMN_SAVE_TYPE_GOLD_SILVER},
        {PKSAV_GEN2_SAVE_TYPE_CRYSTAL, PKMN_SAVE_TYPE_CRYSTAL}
    };
    static const pksav_enum_map_t<enum pksav_gba_save_type> PKSAV_GBA_ENUM_MAP =
    {
        {PKSAV_GBA_SAVE_TYPE_NONE,    PKMN_SAVE_TYPE_NONE},
        {PKSAV_GBA_SAVE_TYPE_RS,      PKMN_SAVE_TYPE_RUBY_SAPPHIRE},
        {PKSAV_GBA_SAVE_TYPE_EMERALD, PKMN_SAVE_TYPE_EMERALD},
        {PKSAV_GBA_SAVE_TYPE_FRLG,    PKMN_SAVE_TYPE_FIRERED_LEAFGREEN}
    };

    static pkmn_save_type_t _detect_save_type(
        const std::vector<uint8_t>& data
    )
    {
        pkmn_save_type_t ret = PKMN_SAVE_TYPE_NONE;

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
                ret = PKMN_SAVE_TYPE_COLOSSEUM_XD;
            }
        }

        if(ret == PKMN_SAVE_TYPE_NONE)
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
            if(ret == PKMN_SAVE_TYPE_NONE)
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

                if(ret == PKMN_SAVE_TYPE_NONE)
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

    static pkmn_save_type_t _detect_save_type(
        const std::string &filepath
    )
    {
        if(not fs::exists(filepath))
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

    std::string game_save::detect_type(
        const std::string &filepath
    )
    {
        pkmn_save_type_t save_type = _detect_save_type(filepath);
        BOOST_ASSERT(save_type >= PKMN_SAVE_TYPE_NONE);
        BOOST_ASSERT(save_type < SAVE_TYPE_NAMES.size());

        return SAVE_TYPE_NAMES[save_type];
    }

    game_save::sptr game_save::from_file(
        const std::string &filepath
    )
    {
        size_t filesize = size_t(fs::file_size(filepath));
        std::vector<uint8_t> raw(filesize);

        std::ifstream ifile(filepath.c_str(), std::ios::binary);
        ifile.read(reinterpret_cast<char*>(raw.data()), filesize);
        ifile.close();

        pkmn_save_type_t save_type = _detect_save_type(raw);

        switch(save_type)
        {
            case PKMN_SAVE_TYPE_RED_BLUE:
            case PKMN_SAVE_TYPE_YELLOW:
                return std::make_shared<game_save_gen1impl>(
                           filepath,
                           std::move(raw)
                       );

            case PKMN_SAVE_TYPE_GOLD_SILVER:
            case PKMN_SAVE_TYPE_CRYSTAL:
                return std::make_shared<game_save_gen2impl>(
                           filepath,
                           std::move(raw)
                       );

            case PKMN_SAVE_TYPE_RUBY_SAPPHIRE:
            case PKMN_SAVE_TYPE_EMERALD:
            case PKMN_SAVE_TYPE_FIRERED_LEAFGREEN:
                return std::make_shared<game_save_gbaimpl>(
                           filepath,
                           std::move(raw)
                       );

            case PKMN_SAVE_TYPE_COLOSSEUM_XD:
                return std::make_shared<game_save_gcnimpl>(
                           filepath,
                           std::move(raw)
                       );

            case PKMN_SAVE_TYPE_NONE:
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

    std::string game_save_impl::get_game()
    {
        return pkmn::database::game_id_to_name(_game_id);
    }

    pkmn::pokedex::sptr game_save_impl::get_pokedex()
    {
        return _pokedex;
    }

    pkmn::pokemon_party::sptr game_save_impl::get_pokemon_party()
    {
        return _pokemon_party;
    }

    pkmn::pokemon_pc::sptr game_save_impl::get_pokemon_pc()
    {
        return _pokemon_pc;
    }

    pkmn::item_bag::sptr game_save_impl::get_item_bag()
    {
        return _item_bag;
    }

    pkmn::item_list::sptr game_save_impl::get_item_pc()
    {
        return _item_pc;
    }
}
