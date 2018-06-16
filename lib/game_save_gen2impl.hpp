/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GEN2IMPL_HPP
#define PKMN_GAME_SAVE_GEN2IMPL_HPP

#include "game_save_impl.hpp"

#include <pksav/gen2.h>

namespace pkmn {

    class game_save_gen2impl: public game_save_impl
    {
        public:
            game_save_gen2impl() {};
            game_save_gen2impl(
                const std::string& filepath,
                std::vector<uint8_t>&& raw
            );

            ~game_save_gen2impl();

            void save_as(
                const std::string& filepath
            ) final;

            pkmn::time_duration get_time_played() final;

            void set_time_played(
                const pkmn::time_duration& time_played
            ) final;

            std::string get_trainer_name() final;

            void set_trainer_name(
                const std::string& trainer_name
            ) final;

            uint32_t get_trainer_id() final;

            void set_trainer_id(
                uint32_t trainer_id
            ) final;

            uint16_t get_trainer_public_id() final;

            void set_trainer_public_id(
                uint16_t trainer_public_id
            ) final;

            uint16_t get_trainer_secret_id() final;

            void set_trainer_secret_id(
                uint16_t trainer_secret_id
            ) final;

            std::string get_trainer_gender() final;

            void set_trainer_gender(
                const std::string& trainer_gender
            ) final;

            std::string get_rival_name() final;

            void set_rival_name(
                const std::string& rival_name
            ) final;

            int get_money() final;

            void set_money(
                int money
            ) final;

            // Functions for attributes

            bool get_is_daylight_savings();

            void set_is_daylight_savings(bool is_daylight_savings);

            std::string get_palette();

            void set_palette(const std::string& palette);

            std::string get_text_speed();

            void set_text_speed(
                const std::string& text_speed
            );

            std::string get_sound_output();

            void set_sound_output(
                const std::string& sound_output
            );

            std::string get_battle_style();

            void set_battle_style(
                const std::string& battle_style
            );

            bool get_is_battle_scene_enabled();

            void set_is_battle_scene_enabled(
                bool is_battle_scene_enabled
            );

            int get_textbox_frame_index();

            void set_textbox_frame_index(int textbox_frame_index);

            std::string get_gameboy_printer_brightness();

            void set_gameboy_printer_brightness(
                const std::string& gameboy_printer_brightness
            );

            bool get_is_menu_account_enabled();

            void set_is_menu_account_enabled(bool is_menu_account_enabled);

            int get_money_with_mom();

            void set_money_with_mom(int money_with_mom);

            std::string get_mom_money_policy();

            void set_mom_money_policy(
                const std::string& mom_money_policy
            );

            int get_casino_coins();

            void set_casino_coins(int casino_coins);

        private:
            struct pksav_gen2_save _pksav_save;

            void _register_attributes();
    };

}

#endif /* PKMN_GAME_SAVE_GEN2IMPL_HPP */
