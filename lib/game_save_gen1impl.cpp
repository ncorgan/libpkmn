/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "game_save_gen1impl.hpp"
#include "item_bag_gen1impl.hpp"
#include "item_list_gbimpl.hpp"
#include "pokedex_gbimpl.hpp"
#include "pokemon_party_gbimpl.hpp"
#include "pokemon_pc_gen1impl.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/pksav_call.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>

#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/filesystem.hpp>

#include <boost/thread/lock_guard.hpp>

#include <stdexcept>

namespace fs = boost::filesystem;

namespace pkmn {

    BOOST_STATIC_CONSTEXPR int RED_GAME_ID    = 1;
    BOOST_STATIC_CONSTEXPR int BLUE_GAME_ID   = 2;
    BOOST_STATIC_CONSTEXPR int YELLOW_GAME_ID = 3;

    BOOST_STATIC_CONSTEXPR int RB_PC_ID = 2;
    BOOST_STATIC_CONSTEXPR int YELLOW_PC_ID = 4;

    game_save_gen1impl::game_save_gen1impl(
        const std::string& filepath,
        std::vector<uint8_t>&& raw
    ): game_save_impl(filepath, std::move(raw))
    {
        PKSAV_CALL(
            pksav_gen1_load_save_from_buffer(
                _raw.data(),
                _raw.size(),
                &_pksav_save
            );
        )

        if(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW)
        {
            _game_id = YELLOW_GAME_ID;
        }
        else
        {
            /*
             * As there is no way to distinguish Red and Blue saves from the saves
             * themselves, we'll try to depend on the fact that .sav files match
             * the name of their game's ROM, which are usually the game titles, so
             * we'll check for the version in the filename.
             */
            std::string filename_lower = boost::algorithm::to_lower_copy(
                                              fs::path(filepath).stem().string()
                                         );
            if(filename_lower.find("red") != std::string::npos)
            {
                _game_id = RED_GAME_ID;
            }
            else if(filename_lower.find("blue") != std::string::npos)
            {
                _game_id = BLUE_GAME_ID;
            }
            else
            {
                // Default to Red, doesn't practically matter within a version group
                _game_id = RED_GAME_ID;
            }
        }

        BOOST_ASSERT(_pksav_save.pokedex_lists.p_seen != nullptr);
        BOOST_ASSERT(_pksav_save.pokedex_lists.p_owned != nullptr);
        _pokedex = std::make_shared<pokedex_gen1impl>(
                       _game_id,
                       &_pksav_save.pokedex_lists
                   );

        BOOST_ASSERT(_pksav_save.pokemon_storage.p_party != nullptr);
        _pokemon_party = std::make_shared<pokemon_party_gen1impl>(
                             _game_id,
                             _pksav_save.pokemon_storage.p_party
                         );

        _pokemon_pc = std::make_shared<pokemon_pc_gen1impl>(
                          _game_id,
                          &_pksav_save.pokemon_storage
                      );

        BOOST_ASSERT(_pksav_save.item_storage.p_item_bag != nullptr);
        _item_bag = std::make_shared<item_bag_gen1impl>(
                        _game_id,
                        _pksav_save.item_storage.p_item_bag
                    );

        BOOST_ASSERT(_pksav_save.item_storage.p_item_pc != nullptr);
        _item_pc = std::make_shared<item_list_gen1_pcimpl>(
                        (_game_id == YELLOW_GAME_ID) ? YELLOW_PC_ID : RB_PC_ID,
                        _game_id,
                        _pksav_save.item_storage.p_item_pc
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

    game_save_gen1impl::~game_save_gen1impl()
    {
        pksav_gen1_free_save(&_pksav_save);
    }

    void game_save_gen1impl::save_as(
        const std::string& filepath
    )
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        // These get_native() calls will call the mutex for every subclass
        // it copies, so we don't need to worry about that here.
        pkmn::rcast_equal<struct pksav_gen1_item_bag>(
            _item_bag->get_native(),
            _pksav_save.item_storage.p_item_bag
        );
        pkmn::rcast_equal<struct pksav_gen1_item_pc>(
            _item_pc->get_native(),
            _pksav_save.item_storage.p_item_pc
        );
        pkmn::rcast_equal<struct pksav_gen1_pokemon_party>(
            _pokemon_party->get_native(),
            _pksav_save.pokemon_storage.p_party
        );

        // The PC is stored in multiple pointers, so this is more manual.
        const struct pksav_gen1_pokemon_storage* p_pokemon_storage =
            static_cast<const struct pksav_gen1_pokemon_storage*>(
                _pokemon_pc->get_native()
            );
        for(size_t box_index = 0;
            box_index < PKSAV_GEN1_NUM_POKEMON_BOXES;
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
            PKSAV_GEN1_POKEDEX_BUFFER_SIZE_BYTES
        );

        PKSAV_CALL(
            pksav_gen1_save_save(
                filepath.c_str(),
                &_pksav_save
            );
        )

        _filepath = fs::absolute(filepath).string();
    }

    pkmn::time_duration game_save_gen1impl::get_time_played()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.p_time_played != nullptr);

        return pkmn::time_duration(
                   pksav_littleendian16(_pksav_save.p_time_played->hours),
                   _pksav_save.p_time_played->minutes,
                   _pksav_save.p_time_played->seconds,
                   0 // frames
               );
    }

    void game_save_gen1impl::set_time_played(
        const pkmn::time_duration& time_played
    )
    {
        pkmn::enforce_bounds(
            "Hours played",
            time_played.hours,
            0,
            65535
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
        // Silently ignore frames

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.p_time_played != nullptr);

        _pksav_save.p_time_played->hours = pksav_littleendian16(
                                               static_cast<uint16_t>(time_played.hours)
                                           );
        _pksav_save.p_time_played->minutes = static_cast<uint8_t>(time_played.minutes);
        _pksav_save.p_time_played->seconds = static_cast<uint8_t>(time_played.seconds);
    }

    std::string game_save_gen1impl::get_trainer_name()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        char trainer_name[PKSAV_GEN1_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen1_import_text(
                _pksav_save.trainer_info.p_name,
                trainer_name,
                PKSAV_GEN1_TRAINER_NAME_LENGTH
            );
        )

        return std::string(trainer_name);
    }

    void game_save_gen1impl::set_trainer_name(
        const std::string& trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            PKSAV_GEN1_TRAINER_NAME_LENGTH
        );
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_name != nullptr);

        PKSAV_CALL(
            pksav_gen1_export_text(
                trainer_name.c_str(),
                _pksav_save.trainer_info.p_name,
                PKSAV_GEN1_TRAINER_NAME_LENGTH
            );
        )
    }

    uint32_t game_save_gen1impl::get_trainer_id()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.p_id);
    }

    void game_save_gen1impl::set_trainer_id(
        uint32_t trainer_id
    )
    {
        pkmn::enforce_gb_trainer_id_bounds(trainer_id);

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        *_pksav_save.trainer_info.p_id = pksav_bigendian16(uint16_t(trainer_id));
    }

    uint16_t game_save_gen1impl::get_trainer_public_id()
    {
        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        return pksav_bigendian16(*_pksav_save.trainer_info.p_id);
    }

    void game_save_gen1impl::set_trainer_public_id(
        uint16_t trainer_public_id
    )
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_id != nullptr);

        *_pksav_save.trainer_info.p_id = pksav_bigendian16(trainer_public_id);
    }

    uint16_t game_save_gen1impl::get_trainer_secret_id()
    {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation I");
    }

    void game_save_gen1impl::set_trainer_secret_id(
        PKMN_UNUSED(uint16_t trainer_secret_id)
    )
    {
        throw pkmn::feature_not_in_game_error("Secret ID", "Generation I");
    }

    std::string game_save_gen1impl::get_trainer_gender()
    {
        return "Male";
    }

    void game_save_gen1impl::set_trainer_gender(
        PKMN_UNUSED(const std::string& trainer_gender)
    )
    {
        throw pkmn::feature_not_in_game_error("All trainers are male in Generation I.");
    }

    std::string game_save_gen1impl::get_rival_name()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

        char rival_name[PKSAV_GEN1_TRAINER_NAME_LENGTH + 1] = {0};
        PKSAV_CALL(
            pksav_gen1_import_text(
                _pksav_save.misc_fields.p_rival_name,
                rival_name,
                PKSAV_GEN1_TRAINER_NAME_LENGTH
            );
        )

        return std::string(rival_name);
    }

    void game_save_gen1impl::set_rival_name(
        const std::string& rival_name
    )
    {
        pkmn::enforce_string_length(
            "Rival name",
            rival_name,
            1,
            PKSAV_GEN1_TRAINER_NAME_LENGTH
        );

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_rival_name != nullptr);

        PKSAV_CALL(
            pksav_gen1_export_text(
                rival_name.c_str(),
                _pksav_save.misc_fields.p_rival_name,
                PKSAV_GEN1_TRAINER_NAME_LENGTH
            );
        )
    }

    int game_save_gen1impl::get_money()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        size_t money_from_pksav = 0;
        PKSAV_CALL(
            pksav_import_bcd(
                _pksav_save.trainer_info.p_money,
                PKSAV_GEN1_SAVE_MONEY_BUFFER_SIZE_BYTES,
                &money_from_pksav
            );
        )

        return int(money_from_pksav);
    }

    void game_save_gen1impl::set_money(
        int money
    )
    {
        pkmn::enforce_bounds("Money", money, 0, MONEY_MAX_VALUE);

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.trainer_info.p_money != nullptr);

        PKSAV_CALL(
            pksav_export_bcd(
                size_t(money),
                _pksav_save.trainer_info.p_money,
                PKSAV_GEN1_SAVE_MONEY_BUFFER_SIZE_BYTES
            )
        )
    }

    // Functions for attributes

    int game_save_gen1impl::get_casino_coins()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        size_t casino_coins_from_pksav = 0;
        PKSAV_CALL(
            pksav_import_bcd(
                _pksav_save.misc_fields.p_casino_coins,
                PKSAV_GEN1_SAVE_CASINO_COINS_BUFFER_SIZE_BYTES,
                &casino_coins_from_pksav
            )
        );

        return int(casino_coins_from_pksav);
    }

    void game_save_gen1impl::set_casino_coins(
        int casino_coins
    )
    {
        pkmn::enforce_bounds(
            "Casino coins",
            casino_coins,
            0,
            PKSAV_GEN1_SAVE_CASINO_COINS_MAX_VALUE
        );

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.misc_fields.p_casino_coins != nullptr);

        PKSAV_CALL(
            pksav_export_bcd(
                size_t(casino_coins),
                _pksav_save.misc_fields.p_casino_coins,
                PKSAV_GEN1_SAVE_CASINO_COINS_BUFFER_SIZE_BYTES
            )
        );
    }

    int game_save_gen1impl::get_pikachu_friendship()
    {
        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW);
        BOOST_ASSERT(_pksav_save.misc_fields.p_pikachu_friendship != nullptr);

        return *_pksav_save.misc_fields.p_pikachu_friendship;
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

        boost::lock_guard<game_save_gen1impl> lock(*this);

        BOOST_ASSERT(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_YELLOW);
        BOOST_ASSERT(_pksav_save.misc_fields.p_pikachu_friendship != nullptr);

        *_pksav_save.misc_fields.p_pikachu_friendship = uint8_t(pikachu_friendship);
    }

    // TODO: figure out where this is in Yellow
    std::string game_save_gen1impl::get_text_speed()
    {
        BOOST_ASSERT(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE);
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        boost::lock_guard<game_save_gen1impl> lock(*this);

        // Sensible default in the case of a corrupted save
        std::string text_speed = "Normal";

        uint8_t raw_text_speed = (*_pksav_save.p_options & PKSAV_GEN1_RB_OPTIONS_TEXT_SPEED_MASK);

        const pksav::gen1_rb_text_speed_bimap_t& gen1_rb_text_speed_bimap =
            pksav::get_gen1_rb_text_speed_bimap();
        auto gen1_rb_text_speed_iter =
            gen1_rb_text_speed_bimap.right.find(
                static_cast<enum pksav_gen1_rb_text_speed>(raw_text_speed)
            );

        if(gen1_rb_text_speed_iter != gen1_rb_text_speed_bimap.right.end())
        {
            text_speed = gen1_rb_text_speed_iter->second;
        }

        return text_speed;
    }

    void game_save_gen1impl::set_text_speed(
        const std::string& text_speed
    )
    {
        BOOST_ASSERT(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE);
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        const pksav::gen1_rb_text_speed_bimap_t& gen1_rb_text_speed_bimap =
            pksav::get_gen1_rb_text_speed_bimap();

        pkmn::enforce_value_in_map_keys(
            "Text speed",
            text_speed,
            gen1_rb_text_speed_bimap.left
        );

        boost::lock_guard<game_save_gen1impl> lock(*this);

        *_pksav_save.p_options &= ~PKSAV_GEN1_RB_OPTIONS_TEXT_SPEED_MASK;
        *_pksav_save.p_options |= static_cast<uint8_t>(
                                        gen1_rb_text_speed_bimap.left.at(text_speed)
                                    );
    }

    std::string game_save_gen1impl::get_sound_output()
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        boost::lock_guard<game_save_gen1impl> lock(*this);

        // Sensible default in the case of a corrupted save
        std::string sound_output = "Mono";

        if(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE)
        {
            bool is_stereo = bool(*_pksav_save.p_options & PKSAV_GEN1_RB_OPTIONS_SOUND_STEREO_MASK);

            sound_output = is_stereo ? "Stereo" : "Mono";
        }
        else
        {
            uint8_t raw_sound_output = PKSAV_GEN1_YELLOW_SOUND_OPTION(*_pksav_save.p_options);

            const pksav::gen1_yellow_sound_option_bimap_t& gen1_yellow_sound_option_bimap =
                pksav::get_gen1_yellow_sound_option_bimap();
            auto gen1_yellow_sound_option_iter =
                gen1_yellow_sound_option_bimap.right.find(
                    static_cast<enum pksav_gen1_yellow_sound_option>(raw_sound_output)
                );

            if(gen1_yellow_sound_option_iter != gen1_yellow_sound_option_bimap.right.end())
            {
                sound_output = gen1_yellow_sound_option_iter->second;
            }
        }

        return sound_output;
    }

    void game_save_gen1impl::set_sound_output(
        const std::string& sound_output
    )
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        if(_pksav_save.save_type == PKSAV_GEN1_SAVE_TYPE_RED_BLUE)
        {
            pkmn::enforce_value_in_vector(
                "Sound output",
                sound_output,
                {"Stereo", "Mono"}
            );

            boost::lock_guard<game_save_gen1impl> lock(*this);

            if(sound_output == "Stereo")
            {
                *_pksav_save.p_options |= PKSAV_GEN1_RB_OPTIONS_SOUND_STEREO_MASK;
            }
            else
            {
                *_pksav_save.p_options &= ~PKSAV_GEN1_RB_OPTIONS_SOUND_STEREO_MASK;
            }
        }
        else
        {
            const pksav::gen1_yellow_sound_option_bimap_t& gen1_yellow_sound_option_bimap =
                pksav::get_gen1_yellow_sound_option_bimap();

            pkmn::enforce_value_in_map_keys(
                "Sound output",
                sound_output,
                gen1_yellow_sound_option_bimap.left
            );

            boost::lock_guard<game_save_gen1impl> lock(*this);

            *_pksav_save.p_options &= ~PKSAV_GEN1_YELLOW_OPTIONS_SOUND_MASK;
            uint8_t raw_sound_output = static_cast<uint8_t>(
                                           gen1_yellow_sound_option_bimap.left.at(sound_output)
                                       );
            raw_sound_output <<= PKSAV_GEN1_YELLOW_OPTIONS_SOUND_OFFSET;

            *_pksav_save.p_options |= raw_sound_output;
        }
    }

    std::string game_save_gen1impl::get_battle_style()
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        bool is_battle_style_set = (*_pksav_save.p_options & PKSAV_GEN1_OPTIONS_BATTLE_STYLE_SET_MASK);

        return is_battle_style_set ? "Set" : "Shift";
    }

    void game_save_gen1impl::set_battle_style(
        const std::string& battle_style
    )
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        pkmn::enforce_value_in_vector(
            "Battle style",
            battle_style,
            {"Set", "Shift"}
        );

        if(battle_style == "Set")
        {
            *_pksav_save.p_options |= PKSAV_GEN1_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
        else
        {
            *_pksav_save.p_options &= ~PKSAV_GEN1_OPTIONS_BATTLE_STYLE_SET_MASK;
        }
    }

    bool game_save_gen1impl::get_are_battle_effects_enabled()
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the result.
        return !(*_pksav_save.p_options & PKSAV_GEN1_OPTIONS_BATTLE_EFFECTS_DISABLE_MASK);
    }

    void game_save_gen1impl::set_are_battle_effects_enabled(
        bool are_battle_effects_enabled
    )
    {
        BOOST_ASSERT(_pksav_save.p_options != nullptr);

        // The save stored whether the effects are disabled, so reverse
        // the input.
        if(are_battle_effects_enabled)
        {
            *_pksav_save.p_options &= ~PKSAV_GEN1_OPTIONS_BATTLE_EFFECTS_DISABLE_MASK;
        }
        else
        {
            *_pksav_save.p_options |= PKSAV_GEN1_OPTIONS_BATTLE_EFFECTS_DISABLE_MASK;
        }
    }

    void game_save_gen1impl::_register_attributes()
    {
        using std::placeholders::_1;

        _numeric_attribute_engine.register_attribute_fcns(
            "Casino coins",
            std::bind(&game_save_gen1impl::get_casino_coins, this),
            std::bind(&game_save_gen1impl::set_casino_coins, this, _1)
        );

        _string_attribute_engine.register_attribute_fcns(
            "Sound output",
            std::bind(&game_save_gen1impl::get_sound_output, this),
            std::bind(&game_save_gen1impl::set_sound_output, this, _1)
        );
        _string_attribute_engine.register_attribute_fcns(
            "Battle style",
            std::bind(&game_save_gen1impl::get_battle_style, this),
            std::bind(&game_save_gen1impl::set_battle_style, this, _1)
        );

        _boolean_attribute_engine.register_attribute_fcns(
            "Enable battle effects?",
            std::bind(&game_save_gen1impl::get_are_battle_effects_enabled, this),
            std::bind(&game_save_gen1impl::set_are_battle_effects_enabled, this, _1)
        );

        if(_game_id == YELLOW_GAME_ID)
        {
            _numeric_attribute_engine.register_attribute_fcns(
                "Pikachu friendship",
                std::bind(&game_save_gen1impl::get_pikachu_friendship, this),
                std::bind(&game_save_gen1impl::set_pikachu_friendship, this, _1)
            );
        }
        else
        {
            _string_attribute_engine.register_attribute_fcns(
                "Text speed",
                std::bind(&game_save_gen1impl::get_text_speed, this),
                std::bind(&game_save_gen1impl::set_text_speed, this, _1)
            );
        }
    }
}
