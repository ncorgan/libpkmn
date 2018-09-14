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
#include "pokedex_gbimpl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_pc_gen2impl.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include "types/mutex_helpers.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen2/palette.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/filesystem.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

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

            BOOST_ASSERT(_pksav_save.item_storage.p_item_pc != nullptr);
            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            CRYSTAL_PC_ID,
                            _game_id,
                            _pksav_save.item_storage.p_item_pc
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

            BOOST_ASSERT(_pksav_save.item_storage.p_item_pc != nullptr);
            _item_pc = std::make_shared<item_list_gen2_pcimpl>(
                            GS_PC_ID,
                            _game_id,
                            _pksav_save.item_storage.p_item_pc
                       );
        }

        BOOST_ASSERT(_pksav_save.pokedex_lists.p_seen != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex_lists.p_owned != nullptr);
        _pokedex = std::make_shared<pokedex_gen2impl>(
                       _game_id,
                       &_pksav_save.pokedex_lists
                   );

        BOOST_ASSERT(_pksav_save.pokemon_storage.p_party != nullptr);
        _pokemon_party = std::make_shared<pokemon_party_gen2impl>(
                             _game_id,
                             _pksav_save.pokemon_storage.p_party
                         );

        BOOST_ASSERT(_pksav_save.pokemon_storage.p_box_names != nullptr);
        _pokemon_pc = std::make_shared<pokemon_pc_gen2impl>(
                          _game_id,
                          &_pksav_save.pokemon_storage
                      );

        BOOST_ASSERT(_pksav_save.item_storage.p_item_bag != nullptr);
        _item_bag = std::make_shared<item_bag_gen2impl>(
                        _game_id,
                        _pksav_save.item_storage.p_item_bag
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

    game_save_gen2impl::~game_save_gen2impl()
    {
        pksav_gen2_free_save(&_pksav_save);
    }

    void game_save_gen2impl::save_as(
        const std::string& filepath
    )
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // These get_native() calls will call the mutex for every subclass
        // it copies, so we don't need to worry about that here.
        pkmn::rcast_equal<struct pksav_gen2_item_bag>(
            _item_bag->get_native(),
            _pksav_save.item_storage.p_item_bag
        );
        pkmn::rcast_equal<struct pksav_gen2_item_pc>(
            _item_pc->get_native(),
            _pksav_save.item_storage.p_item_pc
        );
        pkmn::rcast_equal<struct pksav_gen2_pokemon_party>(
            _pokemon_party->get_native(),
            _pksav_save.pokemon_storage.p_party
        );

        // The PC is stored in multiple pointers, so this is more manual.
        const struct pksav_gen2_pokemon_storage* p_pokemon_storage =
            static_cast<const struct pksav_gen2_pokemon_storage*>(
                _pokemon_pc->get_native()
            );
        for(size_t box_index = 0;
            box_index < PKSAV_GEN2_NUM_POKEMON_BOXES;
            ++box_index)
        {
            *(_pksav_save.pokemon_storage.pp_boxes[box_index]) =
                *(p_pokemon_storage->pp_boxes[box_index]);
        }
        *(_pksav_save.pokemon_storage.p_current_box_num) =
            *(p_pokemon_storage->p_current_box_num);
        *(_pksav_save.pokemon_storage.p_current_box) =
            *(p_pokemon_storage->p_current_box);

        save_gb_pokedex(
            &_pksav_save.pokedex_lists,
            PKSAV_GEN2_POKEDEX_BUFFER_SIZE_BYTES
        );

        PKSAV_CALL(
            pksav_gen2_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    pkmn::time_duration game_save_gen2impl::get_time_played()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.save_time.p_time_played != nullptr);

        return pkmn::time_duration(
                   _pksav_save.save_time.p_time_played->hours,
                   _pksav_save.save_time.p_time_played->minutes,
                   _pksav_save.save_time.p_time_played->seconds,
                   _pksav_save.save_time.p_time_played->frames
               );
    }

    void game_save_gen2impl::set_time_played(
        const pkmn::time_duration& time_played
    )
    {
        pkmn::enforce_bounds(
            "Hours played",
            time_played.hours,
            0,
            255
        );
        pkmn::enforce_bounds(
            "Minutes played",
            time_played.minutes,
            0,
            59
        );
        pkmn::enforce_bounds(
            "Seconds played",
            time_played.seconds,
            0,
            59
        );
        pkmn::enforce_bounds(
            "Frames played",
            time_played.frames,
            0,
            59
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.save_time.p_time_played != nullptr);

        _pksav_save.save_time.p_time_played->hours = static_cast<uint8_t>(time_played.hours);
        _pksav_save.save_time.p_time_played->minutes = static_cast<uint8_t>(time_played.minutes);
        _pksav_save.save_time.p_time_played->seconds = static_cast<uint8_t>(time_played.seconds);
        _pksav_save.save_time.p_time_played->frames = static_cast<uint8_t>(time_played.frames);
    }

    std::string game_save_gen2impl::get_trainer_name()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        char trainer_name[PKSAV_GEN2_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                _pksav_save.trainer_info.p_name,
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

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        PKSAV_CALL(
            pksav_gen2_export_text(
                trainer_name.c_str(),
                _pksav_save.trainer_info.p_name,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
            );
        )
    }

    uint32_t game_save_gen2impl::get_trainer_id()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.p_id);
    }

    void game_save_gen2impl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(trainer_id);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        *_pksav_save.trainer_info.p_id = pksav_bigendian16(uint16_t(trainer_id));
    }

    uint16_t game_save_gen2impl::get_trainer_public_id()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.p_id);
    }

    void game_save_gen2impl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        *_pksav_save.trainer_info.p_id = pksav_bigendian16(trainer_public_id);
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

    pkmn::e_gender game_save_gen2impl::get_trainer_gender()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        pkmn::e_gender ret;

        if(_game_id == CRYSTAL_GAME_ID)
        {
            ret = (*_pksav_save.trainer_info.p_gender == PKSAV_GEN2_GENDER_MALE) ? pkmn::e_gender::MALE
                                                                                 : pkmn::e_gender::FEMALE;
        }
        else
        {
            ret = pkmn::e_gender::MALE;
        }

        return ret;
    }

    void game_save_gen2impl::set_trainer_gender(pkmn::e_gender trainer_gender)
    {
        if(_game_id == CRYSTAL_GAME_ID)
        {
            BOOST_ASSERT(_pksav_save.trainer_info.p_gender != nullptr);

            const pksav::gen2_gender_bimap_t& gen2_gender_bimap = pksav::get_gen2_gender_bimap();
            pkmn::enforce_value_in_map_keys(
                "Trainer gender",
                trainer_gender,
                gen2_gender_bimap.left
            );

            pkmn::lock_guard<game_save_gen2impl> lock(*this);

            *_pksav_save.trainer_info.p_gender = uint8_t(gen2_gender_bimap.left.at(trainer_gender));
        }
        else
        {
            throw pkmn::feature_not_in_game_error("All trainers are male in Gold/Silver.");
        }
    }

    std::string game_save_gen2impl::get_rival_name()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

        char rival_name[PKSAV_GEN2_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen2_import_text(
                _pksav_save.misc_fields.p_rival_name,
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

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

        PKSAV_CALL(
            pksav_gen2_export_text(
                rival_name.c_str(),
                _pksav_save.misc_fields.p_rival_name,
                PKSAV_GEN2_TRAINER_NAME_LENGTH
            );
        )
    }

    int game_save_gen2impl::get_money()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        size_t money_from_pksav = 0;
        PKSAV_CALL(
            pksav_import_base256(
                _pksav_save.trainer_info.p_money,
                PKSAV_GEN2_SAVE_MONEY_BUFFER_SIZE_BYTES,
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

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        PKSAV_CALL(
            pksav_export_base256(
                size_t(money),
                _pksav_save.trainer_info.p_money,
                PKSAV_GEN2_SAVE_MONEY_BUFFER_SIZE_BYTES
            );
        )
    }

    // Functions for attributes

    bool game_save_gen2impl::get_is_daylight_savings()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        return bool(*_pksav_save.save_time.p_daylight_savings &
                    PKSAV_GEN2_DAYLIGHT_SAVINGS_TIME_MASK);
    }

    void game_save_gen2impl::set_is_daylight_savings(bool is_daylight_savings)
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        if(is_daylight_savings)
        {
            *_pksav_save.save_time.p_daylight_savings |= PKSAV_GEN2_DAYLIGHT_SAVINGS_TIME_MASK;
        }
        else
        {
            *_pksav_save.save_time.p_daylight_savings &= ~PKSAV_GEN2_DAYLIGHT_SAVINGS_TIME_MASK;
        }
    }

    typedef boost::bimap<std::string, enum pksav_gen2_palette> gen2_palette_bimap_t;
    static const gen2_palette_bimap_t GEN2_PALETTE_BIMAP =
    boost::assign::list_of<gen2_palette_bimap_t::relation>
        ("Red",        PKSAV_GEN2_PALETTE_RED)
        ("Blue",       PKSAV_GEN2_PALETTE_BLUE)
        ("Green",      PKSAV_GEN2_PALETTE_GREEN)
        ("Brown",      PKSAV_GEN2_PALETTE_BROWN)
        ("Orange",     PKSAV_GEN2_PALETTE_ORANGE)
        ("Gray",       PKSAV_GEN2_PALETTE_GRAY)
        ("Dark Green", PKSAV_GEN2_PALETTE_DARK_GREEN)
        ("Dark Red",   PKSAV_GEN2_PALETTE_DARK_RED)
    ;

    std::string game_save_gen2impl::get_palette()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // If the save file is screwed up enough, the palette may not be
        // valid, so just return something.
        std::string ret = "Red";

        auto palette_iter = GEN2_PALETTE_BIMAP.right.find((enum pksav_gen2_palette)
                                (*_pksav_save.trainer_info.p_palette)
                            );
        if(palette_iter != GEN2_PALETTE_BIMAP.right.end())
        {
            ret = palette_iter->second;
        }

        return ret;
    }

    void game_save_gen2impl::set_palette(const std::string& palette)
    {
        pkmn::enforce_value_in_map_keys(
            "Player palette",
            palette,
            GEN2_PALETTE_BIMAP.left
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        *_pksav_save.trainer_info.p_palette = uint8_t(GEN2_PALETTE_BIMAP.left.at(palette));
    }

    // TODO: is there enough in common with the Gen I implementation to consolidate
    // these?

    std::string game_save_gen2impl::get_text_speed()
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // Sensible default in the case of a corrupted save
        std::string text_speed = "Normal";

        uint8_t raw_text_speed =
            (*_pksav_save.options.p_misc_options & PKSAV_GEN2_OPTIONS_TEXT_SPEED_MASK);

        const pksav::gen2_text_speed_bimap_t& gen2_text_speed_bimap =
            pksav::get_gen2_text_speed_bimap();
        auto gen2_text_speed_iter =
            gen2_text_speed_bimap.right.find(
                static_cast<enum pksav_gen2_text_speed>(raw_text_speed)
            );

        if(gen2_text_speed_iter != gen2_text_speed_bimap.right.end())
        {
            text_speed = gen2_text_speed_iter->second;
        }

        return text_speed;
    }

    void game_save_gen2impl::set_text_speed(
        const std::string& text_speed
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        const pksav::gen2_text_speed_bimap_t& gen2_text_speed_bimap =
            pksav::get_gen2_text_speed_bimap();

        pkmn::enforce_value_in_map_keys(
            "Text speed",
            text_speed,
            gen2_text_speed_bimap.left
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        *_pksav_save.options.p_misc_options &= ~PKSAV_GEN2_OPTIONS_TEXT_SPEED_MASK;
        *_pksav_save.options.p_misc_options |= static_cast<uint8_t>(
                                        gen2_text_speed_bimap.left.at(text_speed)
                                    );
    }

    std::string game_save_gen2impl::get_sound_output()
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // Sensible default in the case of a corrupted save
        std::string sound_output = "Mono";

        bool is_stereo =
            bool(*_pksav_save.options.p_misc_options & PKSAV_GEN2_OPTIONS_SOUND_STEREO_MASK);

        sound_output = is_stereo ? "Stereo" : "Mono";

        return sound_output;
    }

    void game_save_gen2impl::set_sound_output(
        const std::string& sound_output
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        pkmn::enforce_value_in_vector(
            "Sound output",
            sound_output,
            {"Stereo", "Mono"}
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        if(sound_output == "Stereo")
        {
            *_pksav_save.options.p_misc_options |= PKSAV_GEN2_OPTIONS_SOUND_STEREO_MASK;
        }
        else
        {
            *_pksav_save.options.p_misc_options &= ~PKSAV_GEN2_OPTIONS_SOUND_STEREO_MASK;
        }
    }

    std::string game_save_gen2impl::get_battle_style()
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        bool is_battle_style_set =
            (*_pksav_save.options.p_misc_options & PKSAV_GEN2_OPTIONS_BATTLE_STYLE_SET_MASK);

        return is_battle_style_set ? "Set" : "Shift";
    }

    void game_save_gen2impl::set_battle_style(
        const std::string& battle_style
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        pkmn::enforce_value_in_vector(
            "Battle style",
            battle_style,
            {"Set", "Shift"}
        );

        if(battle_style == "Set")
        {
            *_pksav_save.options.p_misc_options |= PKSAV_GEN2_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
        else
        {
            *_pksav_save.options.p_misc_options &= ~PKSAV_GEN2_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
    }

    bool game_save_gen2impl::get_is_battle_scene_enabled()
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the result.
        //
        // TODO: verify, the disassembly is unclear
        return !(*_pksav_save.options.p_misc_options & PKSAV_GEN2_OPTIONS_BATTLE_SCENE_MASK);
    }

    void game_save_gen2impl::set_is_battle_scene_enabled(
        bool is_battle_scene_enabled
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_misc_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the input.
        //
        // TODO: verify, the disassembly is unclear
        if(is_battle_scene_enabled)
        {
            *_pksav_save.options.p_misc_options &= ~PKSAV_GEN2_OPTIONS_BATTLE_SCENE_MASK;
        }
        else
        {
            *_pksav_save.options.p_misc_options |= PKSAV_GEN2_OPTIONS_BATTLE_SCENE_MASK;
        }
    }

    int game_save_gen2impl::get_textbox_frame_index()
    {
        BOOST_ASSERT(_pksav_save.options.p_textbox_frame_index != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        return (*_pksav_save.options.p_textbox_frame_index & PKSAV_GEN2_OPTIONS_TEXTBOX_FRAME_MASK);
    }

    void game_save_gen2impl::set_textbox_frame_index(int textbox_frame_index)
    {
        BOOST_ASSERT(_pksav_save.options.p_textbox_frame_index != nullptr);

        pkmn::enforce_bounds(
            "Textbox frame",
            textbox_frame_index,
            static_cast<int>(PKSAV_GEN2_OPTIONS_TEXTBOX_MIN_FRAME + 1),
            static_cast<int>(PKSAV_GEN2_OPTIONS_TEXTBOX_MAX_FRAME + 1)
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // The value is stored 0-based.
        uint8_t raw_textbox_frame_index = static_cast<uint8_t>(textbox_frame_index) - 1;

        *_pksav_save.options.p_textbox_frame_index &= ~PKSAV_GEN2_OPTIONS_TEXTBOX_FRAME_MASK;
        *_pksav_save.options.p_textbox_frame_index |= raw_textbox_frame_index;
    }

    std::string game_save_gen2impl::get_gameboy_printer_brightness()
    {
        BOOST_ASSERT(_pksav_save.options.p_gbprinter_brightness != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        // Sensible default in case of save corruption
        std::string gameboy_printer_brightness = "Normal";

        uint8_t raw_gameboy_printer_brightness =
            (*_pksav_save.options.p_gbprinter_brightness & PKSAV_GEN2_GBPRINTER_BRIGHTNESS_MASK);

        const pksav::gen2_gbprinter_brightness_bimap_t& gen2_gbprinter_brightness_bimap =
            pksav::get_gen2_gbprinter_brightness_bimap();

        auto gen2_gbprinter_brightness_iter = gen2_gbprinter_brightness_bimap.right.find(
                                                  static_cast<enum pksav_gen2_gbprinter_brightness>(
                                                      raw_gameboy_printer_brightness
                                                  )
                                              );
        if(gen2_gbprinter_brightness_iter != gen2_gbprinter_brightness_bimap.right.end())
        {
            gameboy_printer_brightness = gen2_gbprinter_brightness_iter->second;
        }

        return gameboy_printer_brightness;
    }

    void game_save_gen2impl::set_gameboy_printer_brightness(
        const std::string& gameboy_printer_brightness
    )
    {
        BOOST_ASSERT(_pksav_save.options.p_gbprinter_brightness != nullptr);

        const pksav::gen2_gbprinter_brightness_bimap_t& gen2_gbprinter_brightness_bimap =
            pksav::get_gen2_gbprinter_brightness_bimap();

        pkmn::enforce_value_in_map_keys(
            "Game Boy Printer brightness",
            gameboy_printer_brightness,
            gen2_gbprinter_brightness_bimap.left
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        *_pksav_save.options.p_gbprinter_brightness &= ~PKSAV_GEN2_GBPRINTER_BRIGHTNESS_MASK;
        *_pksav_save.options.p_gbprinter_brightness |=
            static_cast<uint8_t>(
                gen2_gbprinter_brightness_bimap.left.at(gameboy_printer_brightness)
            );
    }

    bool game_save_gen2impl::get_is_menu_account_enabled()
    {
        BOOST_ASSERT(_pksav_save.options.p_menu_account != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        return bool(*_pksav_save.options.p_menu_account & PKSAV_GEN2_OPTIONS_MENU_ACCOUNT_MASK);
    }

    void game_save_gen2impl::set_is_menu_account_enabled(bool is_menu_account_enabled)
    {
        BOOST_ASSERT(_pksav_save.options.p_menu_account != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        if(is_menu_account_enabled)
        {
            *_pksav_save.options.p_menu_account |= PKSAV_GEN2_OPTIONS_MENU_ACCOUNT_MASK;
        }
        else
        {
            *_pksav_save.options.p_menu_account &= ~PKSAV_GEN2_OPTIONS_MENU_ACCOUNT_MASK;
        }
    }

    int game_save_gen2impl::get_money_with_mom()
    {
        BOOST_ASSERT(_pksav_save.misc_fields.p_money_with_mom != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        size_t money_with_mom;
        PKSAV_CALL(
            pksav_import_bcd(
                _pksav_save.misc_fields.p_money_with_mom,
                PKSAV_GEN2_SAVE_MONEY_BUFFER_SIZE_BYTES,
                &money_with_mom
            );
        )

        return static_cast<int>(money_with_mom);
    }

    void game_save_gen2impl::set_money_with_mom(int money_with_mom)
    {
        pkmn::enforce_bounds(
            "Money with Mom",
            money_with_mom,
            0,
            PKSAV_GEN2_SAVE_MONEY_MAX_VALUE
        );

        BOOST_ASSERT(_pksav_save.misc_fields.p_money_with_mom != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        PKSAV_CALL(
            pksav_export_bcd(
                static_cast<size_t>(money_with_mom),
                _pksav_save.misc_fields.p_money_with_mom,
                PKSAV_GEN2_SAVE_MONEY_BUFFER_SIZE_BYTES
            );
        )
    }

    std::string game_save_gen2impl::get_mom_money_policy()
    {
        BOOST_ASSERT(_pksav_save.misc_fields.p_mom_money_policy != nullptr);

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        std::string mom_money_policy = "Not saving money";

        // If this flag isn't set, the player hasn't reached the point where this is
        // introduced, so naturally, Mom's not saving money.
        if(*_pksav_save.misc_fields.p_mom_money_policy & PKSAV_GEN2_MOM_MONEY_POLICY_ACTIVE_FLAG)
        {
            uint8_t raw_mom_money_policy =
                (*_pksav_save.misc_fields.p_mom_money_policy & ~PKSAV_GEN2_MOM_MONEY_POLICY_ACTIVE_FLAG);

            const pksav::gen2_mom_money_policy_bimap_t& gen2_mom_money_policy_bimap =
                pksav::get_gen2_mom_money_policy_bimap();

            // Go with the default above in the case of save corruption.
            auto gen2_mom_money_policy_iter = gen2_mom_money_policy_bimap.right.find(
                                                  static_cast<enum pksav_gen2_mom_money_policy>(
                                                      raw_mom_money_policy
                                                  )
                                              );
            if(gen2_mom_money_policy_iter != gen2_mom_money_policy_bimap.right.end())
            {
                mom_money_policy = gen2_mom_money_policy_iter->second;
            }
        }

        return mom_money_policy;
    }

    void game_save_gen2impl::set_mom_money_policy(
        const std::string& mom_money_policy
    )
    {
        BOOST_ASSERT(_pksav_save.misc_fields.p_mom_money_policy != nullptr);

        const pksav::gen2_mom_money_policy_bimap_t& gen2_mom_money_policy_bimap =
            pksav::get_gen2_mom_money_policy_bimap();

        pkmn::enforce_value_in_map_keys(
            "Mom money policy",
            mom_money_policy,
            gen2_mom_money_policy_bimap.left
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        auto mom_money_policy_iter = gen2_mom_money_policy_bimap.left.find(
                                         mom_money_policy
                                     );
        BOOST_ASSERT(mom_money_policy_iter != gen2_mom_money_policy_bimap.left.end());

        uint8_t raw_mom_money_policy = static_cast<uint8_t>(
                                           mom_money_policy_iter->second
                                       );

        // This separate flag indicates the event where this was introduced has
        // occurred.
        raw_mom_money_policy |= PKSAV_GEN2_MOM_MONEY_POLICY_ACTIVE_FLAG;

        *_pksav_save.misc_fields.p_mom_money_policy = raw_mom_money_policy;
    }

    int game_save_gen2impl::get_casino_coins()
    {
        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        size_t casino_coins_from_pksav = 0;
        PKSAV_CALL(
            pksav_import_bcd(
                _pksav_save.misc_fields.p_casino_coins,
                PKSAV_GEN2_SAVE_CASINO_COINS_BUFFER_SIZE_BYTES,
                &casino_coins_from_pksav
            )
        );

        return int(casino_coins_from_pksav);
    }

    void game_save_gen2impl::set_casino_coins(
        int casino_coins
    )
    {
        pkmn::enforce_bounds(
            "Casino coins",
            casino_coins,
            0,
            PKSAV_GEN2_SAVE_CASINO_COINS_MAX_VALUE
        );

        pkmn::lock_guard<game_save_gen2impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        PKSAV_CALL(
            pksav_export_bcd(
                size_t(casino_coins),
                _pksav_save.misc_fields.p_casino_coins,
                PKSAV_GEN2_SAVE_CASINO_COINS_BUFFER_SIZE_BYTES
            )
        );
    }

    void game_save_gen2impl::_register_attributes()
    {
        using std::placeholders::_1;

        _numeric_attribute_engine.register_attribute_fcns(
            "Textbox frame",
            std::bind(&game_save_gen2impl::get_textbox_frame_index, this),
            std::bind(&game_save_gen2impl::set_textbox_frame_index, this, _1)
        );
        _numeric_attribute_engine.register_attribute_fcns(
            "Money with Mom",
            std::bind(&game_save_gen2impl::get_money_with_mom, this),
            std::bind(&game_save_gen2impl::set_money_with_mom, this, _1)
        );
        _numeric_attribute_engine.register_attribute_fcns(
            "Casino coins",
            std::bind(&game_save_gen2impl::get_casino_coins, this),
            std::bind(&game_save_gen2impl::set_casino_coins, this, _1)
        );

        _string_attribute_engine.register_attribute_fcns(
            "Player palette",
            std::bind(&game_save_gen2impl::get_palette, this),
            std::bind(&game_save_gen2impl::set_palette, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Text speed",
            std::bind(&game_save_gen2impl::get_text_speed, this),
            std::bind(&game_save_gen2impl::set_text_speed, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Sound output",
            std::bind(&game_save_gen2impl::get_sound_output, this),
            std::bind(&game_save_gen2impl::set_sound_output, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Battle style",
            std::bind(&game_save_gen2impl::get_battle_style, this),
            std::bind(&game_save_gen2impl::set_battle_style, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Game Boy Printer brightness",
            std::bind(&game_save_gen2impl::get_gameboy_printer_brightness, this),
            std::bind(&game_save_gen2impl::set_gameboy_printer_brightness, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Mom money policy",
            std::bind(&game_save_gen2impl::get_mom_money_policy, this),
            std::bind(&game_save_gen2impl::set_mom_money_policy, this, _1)
        );

        _boolean_attribute_engine.register_attribute_fcns(
            "Daylight savings time?",
            std::bind(&game_save_gen2impl::get_is_daylight_savings, this),
            std::bind(&game_save_gen2impl::set_is_daylight_savings, this, _1)
        );
        _boolean_attribute_engine.register_attribute_fcns(
            "Enable battle scene?",
            std::bind(&game_save_gen2impl::get_is_battle_scene_enabled, this),
            std::bind(&game_save_gen2impl::set_is_battle_scene_enabled, this, _1)
        );
        _boolean_attribute_engine.register_attribute_fcns(
            "Enable menu account?",
            std::bind(&game_save_gen2impl::get_is_menu_account_enabled, this),
            std::bind(&game_save_gen2impl::set_is_menu_account_enabled, this, _1)
        );
    }
}
