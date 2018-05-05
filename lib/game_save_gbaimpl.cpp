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

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/exception.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int RUBY_GAME_ID      = 7;
    BOOST_STATIC_CONSTEXPR int SAPPHIRE_GAME_ID  = 8;
    BOOST_STATIC_CONSTEXPR int EMERALD_GAME_ID   = 9;
    BOOST_STATIC_CONSTEXPR int FIRERED_GAME_ID   = 10;
    BOOST_STATIC_CONSTEXPR int LEAFGREEN_GAME_ID = 11;

    BOOST_STATIC_CONSTEXPR int RS_PC_ID      = 20;
    BOOST_STATIC_CONSTEXPR int EMERALD_PC_ID = 26;
    BOOST_STATIC_CONSTEXPR int FRLG_PC_ID    = 32;

    game_save_gbaimpl::game_save_gbaimpl(
        const std::string& filepath,
        std::vector<uint8_t>&& raw
    ): game_save_impl(filepath, std::move(raw))
    {
        PKSAV_CALL(
            pksav_gba_load_save_from_buffer(
                _raw.data(),
                _raw.size(),
                &_pksav_save
            );
        )

        int item_pc_id = 0;

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
                    _game_id = RUBY_GAME_ID;
                }
                else if(filename.find("sapphire") != std::string::npos)
                {
                    _game_id = SAPPHIRE_GAME_ID;
                }
                else
                {
                    // Default to Ruby, doesn't practically matter within a version group
                    _game_id = RUBY_GAME_ID;
                }

                item_pc_id = RS_PC_ID;
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
                    _game_id = FIRERED_GAME_ID;
                }
                else if(filename.find("leafgreen") != std::string::npos or
                        filename.find("lg") != std::string::npos)
                {
                    _game_id = LEAFGREEN_GAME_ID;
                }
                else
                {
                    // Default to FireRed, doesn't practically matter within a version group
                    _game_id = FIRERED_GAME_ID;
                }

                item_pc_id = FRLG_PC_ID;
                break;

            default: // Emerald
                _game_id = EMERALD_GAME_ID;
                item_pc_id = EMERALD_PC_ID;
                break;
        }

        BOOST_ASSERT(_pksav_save.pokedex.p_seenA != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_seenB != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_seenC != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_owned != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_nat_pokedex_unlockedB != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_nat_pokedex_unlockedC != nullptr);
        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            BOOST_ASSERT(_pksav_save.pokedex.p_frlg_nat_pokedex_unlockedA != nullptr);
        }
        else
        {
            BOOST_ASSERT(_pksav_save.pokedex.p_rse_nat_pokedex_unlockedA != nullptr);
        }
        _pokedex = std::make_shared<pokedex_gbaimpl>(
                       _game_id,
                       &_pksav_save.pokedex
                   );

        BOOST_ASSERT(_pksav_save.pokemon_storage.p_party != nullptr);
        _pokemon_party = std::make_shared<pokemon_party_gbaimpl>(
                             _game_id,
                             _pksav_save.pokemon_storage.p_party
                         );

        BOOST_ASSERT(_pksav_save.pokemon_storage.p_pc != nullptr);
        _pokemon_pc = std::make_shared<pokemon_pc_gbaimpl>(
                          _game_id,
                          _pksav_save.pokemon_storage.p_pc
                      );

        BOOST_ASSERT(_pksav_save.item_storage.p_bag != nullptr);
        _item_bag = std::make_shared<item_bag_gbaimpl>(
                        _game_id, _pksav_save.item_storage.p_bag
                    );

        BOOST_ASSERT(_pksav_save.item_storage.p_pc != nullptr);
        _item_pc = std::make_shared<item_list_modernimpl>(
                        item_pc_id, _game_id,
                        _pksav_save.item_storage.p_pc->items,
                        PKSAV_GBA_ITEM_PC_NUM_ITEMS,
                        false
                   );

        // When a Pokémon is added to the PC or party, it should be
        // reflected in the Pokédex.

        pokemon_party_impl* p_party_impl = dynamic_cast<pokemon_party_impl*>(_pokemon_party.get());
        pokemon_pc_impl* p_pc_impl = dynamic_cast<pokemon_pc_impl*>(_pokemon_pc.get());

        BOOST_ASSERT(p_party_impl != nullptr);
        BOOST_ASSERT(p_pc_impl != nullptr);

        p_party_impl->set_pokedex(_pokedex);
        p_pc_impl->set_pokedex(_pokedex);

        _register_attributes();
    }

    game_save_gbaimpl::~game_save_gbaimpl()
    {
        pksav_gba_free_save(&_pksav_save);
    }

    void game_save_gbaimpl::save_as(
        const std::string& filepath
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        // Make sure any updating is performed.
        (void)_pokemon_party->get_native();
        (void)_pokemon_pc->get_native();

        PKSAV_CALL(
            pksav_gba_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    std::string game_save_gbaimpl::get_trainer_name()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        char trainer_name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gba_import_text(
                _pksav_save.trainer_info.p_name,
                trainer_name,
                PKSAV_GBA_TRAINER_NAME_LENGTH
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gbaimpl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            PKSAV_GBA_TRAINER_NAME_LENGTH
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        PKSAV_CALL(
            pksav_gba_export_text(
                trainer_name.c_str(),
                _pksav_save.trainer_info.p_name,
                PKSAV_GBA_TRAINER_NAME_LENGTH
            );
        )
    }

    uint32_t game_save_gbaimpl::get_trainer_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_littleendian32(_pksav_save.trainer_info.p_id->id);
    }

    void game_save_gbaimpl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        _pksav_save.trainer_info.p_id->id = pksav_littleendian32(trainer_id);
    }

    uint16_t game_save_gbaimpl::get_trainer_public_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_littleendian16(_pksav_save.trainer_info.p_id->pid);
    }

    void game_save_gbaimpl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        _pksav_save.trainer_info.p_id->pid = pksav_littleendian16(trainer_public_id);
    }

    uint16_t game_save_gbaimpl::get_trainer_secret_id()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_littleendian16(_pksav_save.trainer_info.p_id->sid);
    }

    void game_save_gbaimpl::set_trainer_secret_id(
        uint16_t trainer_secret_id
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        _pksav_save.trainer_info.p_id->sid = pksav_littleendian16(trainer_secret_id);
    }

    std::string game_save_gbaimpl::get_trainer_gender()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_gender != nullptr);

        return (*_pksav_save.trainer_info.p_gender == 0) ? "Male" : "Female";
    }

    // TODO: gender enum
    void game_save_gbaimpl::set_trainer_gender(
        const std::string& trainer_gender
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_gender != nullptr);

        if(trainer_gender == "Male")
        {
            *_pksav_save.trainer_info.p_gender = 0;
        }
        else if(trainer_gender == "Female")
        {
            *_pksav_save.trainer_info.p_gender = 1;
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
            BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

            char rival_name[PKSAV_GBA_TRAINER_NAME_LENGTH + 1] = {0};
            PKSAV_CALL(
                pksav_gba_import_text(
                    _pksav_save.misc_fields.p_rival_name,
                    rival_name,
                    PKSAV_GBA_TRAINER_NAME_LENGTH
                );
            )

            ret = std::string(rival_name);
        }
        else
        {
            BOOST_ASSERT(_pksav_save.trainer_info.p_gender != nullptr);
            ret = (*_pksav_save.trainer_info.p_gender == 0) ? "MAY" : "BRENDAN";
        }

        return ret;
    }

    void game_save_gbaimpl::set_rival_name(
        const std::string& rival_name
    )
    {
        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            pkmn::enforce_string_length(
                "Rival name",
                rival_name,
                1,
                PKSAV_GBA_TRAINER_NAME_LENGTH
            );

            boost::lock_guard<game_save_gbaimpl> lock(*this);

            BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

            PKSAV_CALL(
                pksav_gba_export_text(
                    rival_name.c_str(),
                    _pksav_save.misc_fields.p_rival_name,
                    PKSAV_GBA_TRAINER_NAME_LENGTH
                );
            )
        }
        else
        {
            throw pkmn::feature_not_in_game_error("Rivals cannot be renamed in Ruby/Sapphire/Emerald.");
        }
    }

    int game_save_gbaimpl::get_money()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        return int(pksav_littleendian32(*_pksav_save.trainer_info.p_money));
    }

    void game_save_gbaimpl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        *_pksav_save.trainer_info.p_money = pksav_littleendian32(uint32_t(money));
    }

    // Functions for attributes

    int game_save_gbaimpl::get_casino_coins()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        return pksav_littleendian16(*_pksav_save.misc_fields.p_casino_coins);
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

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        *_pksav_save.misc_fields.p_casino_coins = pksav_littleendian16(uint16_t(casino_coins));
    }

    bool game_save_gbaimpl::get_is_national_dex_unlocked()
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        BOOST_ASSERT(_pksav_save.pokedex.p_nat_pokedex_unlockedB != nullptr);

        // Easiest since it's the same for all types
        return bool(*_pksav_save.pokedex.p_nat_pokedex_unlockedB &
                    PKSAV_GBA_NAT_POKEDEX_UNLOCKED_B_FLAG);
    }

    void game_save_gbaimpl::set_is_national_dex_unlocked(
        bool is_national_dex_unlocked
    )
    {
        boost::lock_guard<game_save_gbaimpl> lock(*this);

        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            BOOST_ASSERT(_pksav_save.pokedex.p_frlg_nat_pokedex_unlockedA != nullptr);
        }
        else
        {
            BOOST_ASSERT(_pksav_save.pokedex.p_rse_nat_pokedex_unlockedA != nullptr);
        }
        BOOST_ASSERT(_pksav_save.pokedex.p_nat_pokedex_unlockedB != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex.p_nat_pokedex_unlockedC != nullptr);

        PKSAV_CALL(
            pksav_gba_pokedex_set_national_pokedex_unlocked(
                &_pksav_save.pokedex,
                _pksav_save.save_type,
                is_national_dex_unlocked
            );
        )
    }

    std::string game_save_gbaimpl::get_button_mode()
    {
        BOOST_ASSERT(_pksav_save.options.p_button_mode != nullptr);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        std::string button_mode;

        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            // Sensible default in case of save corruption
            button_mode = "Help";

            const pksav::gba_frlg_button_mode_bimap_t& gba_frlg_button_mode_bimap =
                pksav::get_gba_frlg_button_mode_bimap();

            auto gba_frlg_button_mode_iter = gba_frlg_button_mode_bimap.right.find(
                                                 (enum pksav_gba_frlg_button_mode)(
                                                     *_pksav_save.options.p_button_mode
                                                 )
                                             );
            if(gba_frlg_button_mode_iter != gba_frlg_button_mode_bimap.right.end())
            {
                button_mode = gba_frlg_button_mode_iter->second;
            }
        }
        else
        {
            // Sensible default in case of save corruption
            button_mode = "Normal";

            const pksav::gba_rse_button_mode_bimap_t& gba_rse_button_mode_bimap =
                pksav::get_gba_rse_button_mode_bimap();

            auto gba_rse_button_mode_iter = gba_rse_button_mode_bimap.right.find(
                                                (enum pksav_gba_rse_button_mode)(
                                                    *_pksav_save.options.p_button_mode
                                                )
                                            );
            if(gba_rse_button_mode_iter != gba_rse_button_mode_bimap.right.end())
            {
                button_mode = gba_rse_button_mode_iter->second;
            }
        }

        return button_mode;
    }

    void game_save_gbaimpl::set_button_mode(const std::string& button_mode)
    {
        BOOST_ASSERT(_pksav_save.options.p_button_mode != nullptr);

        if(_pksav_save.save_type == PKSAV_GBA_SAVE_TYPE_FRLG)
        {
            const pksav::gba_frlg_button_mode_bimap_t& gba_frlg_button_mode_bimap =
                pksav::get_gba_frlg_button_mode_bimap();

            pkmn::enforce_value_in_map_keys(
                "Button mode",
                button_mode,
                gba_frlg_button_mode_bimap.left
            );

            boost::lock_guard<game_save_gbaimpl> lock(*this);

            *_pksav_save.options.p_button_mode =
                static_cast<uint8_t>(gba_frlg_button_mode_bimap.left.at(
                    button_mode
                ));
        }
        else
        {
            const pksav::gba_rse_button_mode_bimap_t& gba_rse_button_mode_bimap =
                pksav::get_gba_rse_button_mode_bimap();

            pkmn::enforce_value_in_map_keys(
                "Button mode",
                button_mode,
                gba_rse_button_mode_bimap.left
            );

            boost::lock_guard<game_save_gbaimpl> lock(*this);

            *_pksav_save.options.p_button_mode =
                static_cast<uint8_t>(gba_rse_button_mode_bimap.left.at(
                    button_mode
                ));
        }
    }

    std::string game_save_gbaimpl::get_text_speed()
    {
        BOOST_ASSERT(_pksav_save.options.p_text_options != nullptr);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        // Sensible default in case of corrupted save
        std::string text_speed = "Medium";

        uint8_t raw_text_speed =
            (*_pksav_save.options.p_text_options & PKSAV_GBA_OPTIONS_TEXT_SPEED_MASK);

        const pksav::gba_text_speed_bimap_t& gba_text_speed_bimap =
            pksav::get_gba_text_speed_bimap();

        auto gba_text_speed_iter = gba_text_speed_bimap.right.find(
                                       static_cast<enum pksav_gba_text_speed>(
                                           raw_text_speed
                                       )
                                   );
        if(gba_text_speed_iter != gba_text_speed_bimap.right.end())
        {
            text_speed = gba_text_speed_iter->second;
        }

        return text_speed;
    }

    void game_save_gbaimpl::set_text_speed(const std::string& text_speed)
    {
        BOOST_ASSERT(_pksav_save.options.p_text_options != nullptr);

        const pksav::gba_text_speed_bimap_t& gba_text_speed_bimap =
            pksav::get_gba_text_speed_bimap();

        pkmn::enforce_value_in_map_keys(
            "Text speed",
            text_speed,
            gba_text_speed_bimap.left
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        *_pksav_save.options.p_text_options &= ~PKSAV_GBA_OPTIONS_TEXT_SPEED_MASK;
        *_pksav_save.options.p_text_options |= static_cast<uint8_t>(
                                                     gba_text_speed_bimap.left.at(
                                                         text_speed
                                                     )
                                                 );
    }

    int game_save_gbaimpl::get_textbox_frame_index()
    {
        BOOST_ASSERT(_pksav_save.options.p_text_options != nullptr);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        return static_cast<int>(
                   PKSAV_GBA_OPTIONS_TEXTBOX_FRAME(*_pksav_save.options.p_text_options)
               ) + 1;
    }

    void game_save_gbaimpl::set_textbox_frame_index(int textbox_frame_index)
    {
        BOOST_ASSERT(_pksav_save.options.p_text_options != nullptr);

        pkmn::enforce_bounds(
            "Textbox frame",
            textbox_frame_index,
            static_cast<int>(PKSAV_GBA_OPTIONS_TEXTBOX_MIN_FRAME + 1),
            static_cast<int>(PKSAV_GBA_OPTIONS_TEXTBOX_MAX_FRAME + 1)
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        // The value is stored 0-based.
        uint8_t raw_textbox_frame_index = static_cast<uint8_t>(textbox_frame_index) - 1;
        raw_textbox_frame_index <<= PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_OFFSET;

        *_pksav_save.options.p_text_options &= ~PKSAV_GBA_OPTIONS_TEXTBOX_FRAME_MASK;
        *_pksav_save.options.p_text_options |= raw_textbox_frame_index;
    }

    std::string game_save_gbaimpl::get_sound_output()
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        // Sensible default in the case of a corrupted save
        std::string sound_output = "Mono";

        bool is_stereo =
            bool(*_pksav_save.options.p_sound_battle_options & PKSAV_GBA_OPTIONS_SOUND_STEREO_MASK);

        sound_output = is_stereo ? "Stereo" : "Mono";

        return sound_output;
    }

    void game_save_gbaimpl::set_sound_output(
        const std::string& sound_output
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        pkmn::enforce_value_in_vector(
            "Sound output",
            sound_output,
            {"Stereo", "Mono"}
        );

        boost::lock_guard<game_save_gbaimpl> lock(*this);

        if(sound_output == "Stereo")
        {
            *_pksav_save.options.p_sound_battle_options |= PKSAV_GBA_OPTIONS_SOUND_STEREO_MASK;
        }
        else
        {
            *_pksav_save.options.p_sound_battle_options &= ~PKSAV_GBA_OPTIONS_SOUND_STEREO_MASK;
        }
    }

    std::string game_save_gbaimpl::get_battle_style()
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        bool is_battle_style_set =
            (*_pksav_save.options.p_sound_battle_options & PKSAV_GBA_OPTIONS_BATTLE_STYLE_SET_MASK);

        return is_battle_style_set ? "Set" : "Shift";
    }

    void game_save_gbaimpl::set_battle_style(
        const std::string& battle_style
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        pkmn::enforce_value_in_vector(
            "Battle style",
            battle_style,
            {"Set", "Shift"}
        );

        if(battle_style == "Set")
        {
            *_pksav_save.options.p_sound_battle_options |= PKSAV_GBA_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
        else
        {
            *_pksav_save.options.p_sound_battle_options &= ~PKSAV_GBA_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
    }

    bool game_save_gbaimpl::get_is_battle_scene_enabled()
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the result.
        return !(*_pksav_save.options.p_sound_battle_options & PKSAV_GBA_OPTIONS_BATTLE_SCENE_DISABLE_MASK);
    }

    void game_save_gbaimpl::set_is_battle_scene_enabled(
        bool is_battle_scene_enabled
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_sound_battle_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the input.
        if(is_battle_scene_enabled)
        {
            *_pksav_save.options.p_sound_battle_options &= ~PKSAV_GBA_OPTIONS_BATTLE_SCENE_DISABLE_MASK;
        }
        else
        {
            *_pksav_save.options.p_sound_battle_options |= PKSAV_GBA_OPTIONS_BATTLE_SCENE_DISABLE_MASK;
        }
    }

    void game_save_gbaimpl::_register_attributes()
    {
        using std::placeholders::_1;

        _numeric_attribute_engine.register_attribute_fcns(
            "Casino coins",
            std::bind(&game_save_gbaimpl::get_casino_coins, this),
            std::bind(&game_save_gbaimpl::set_casino_coins, this, _1)
        );
        _numeric_attribute_engine.register_attribute_fcns(
            "Textbox frame",
            std::bind(&game_save_gbaimpl::get_textbox_frame_index, this),
            std::bind(&game_save_gbaimpl::set_textbox_frame_index, this, _1)
        );

        _string_attribute_engine.register_attribute_fcns(
            "Button mode",
            std::bind(&game_save_gbaimpl::get_button_mode, this),
            std::bind(&game_save_gbaimpl::set_button_mode, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Text speed",
            std::bind(&game_save_gbaimpl::get_text_speed, this),
            std::bind(&game_save_gbaimpl::set_text_speed, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Sound output",
            std::bind(&game_save_gbaimpl::get_sound_output, this),
            std::bind(&game_save_gbaimpl::set_sound_output, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Battle style",
            std::bind(&game_save_gbaimpl::get_battle_style, this),
            std::bind(&game_save_gbaimpl::set_battle_style, this, _1)
        );

        // Don't use the whole word "Pokédex" to make life easier for
        // Python users.
        _boolean_attribute_engine.register_attribute_fcns(
            "National Dex unlocked?",
            std::bind(&game_save_gbaimpl::get_is_national_dex_unlocked, this),
            std::bind(&game_save_gbaimpl::set_is_national_dex_unlocked, this, _1)
        );
        _boolean_attribute_engine.register_attribute_fcns(
            "Enable battle scene?",
            std::bind(&game_save_gbaimpl::get_is_battle_scene_enabled, this),
            std::bind(&game_save_gbaimpl::set_is_battle_scene_enabled, this, _1)
        );
    }
}
