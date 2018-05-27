/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GEN1IMPL_HPP
#define PKMN_GAME_SAVE_GEN1IMPL_HPP

#include "game_save_impl.hpp"

#include <pksav/gen1.h>

namespace pkmn {

    class game_save_gen1impl: public game_save_impl
    {
        public:
            game_save_gen1impl() {};
            game_save_gen1impl(
                const std::string& filepath,
                std::vector<uint8_t>&& raw
            );

            ~game_save_gen1impl();

            void save_as(
                const std::string& filepath
            ) override final;

            pkmn::time_duration get_time_played() override final;

            void set_time_played(
                const pkmn::time_duration& time_played
            ) override final;

            std::string get_trainer_name() override final;

            void set_trainer_name(
                const std::string& trainer_name
            ) override final;

            uint32_t get_trainer_id() override final;

            void set_trainer_id(
                uint32_t trainer_id
            ) override final;

            uint16_t get_trainer_public_id() override final;

            void set_trainer_public_id(
                uint16_t trainer_public_id
            ) override final;

            uint16_t get_trainer_secret_id() override final;

            void set_trainer_secret_id(
                uint16_t trainer_secret_id
            ) override final;

            std::string get_trainer_gender() override final;

            void set_trainer_gender(
                const std::string& trainer_gender
            ) override final;

            std::string get_rival_name() override final;

            void set_rival_name(
                const std::string& rival_name
            ) override final;

            int get_money() override final;

            void set_money(
                int money
            ) override final;

            // Functions for attributes

            int get_casino_coins();

            void set_casino_coins(
                int casino_coins
            );

            int get_pikachu_friendship();

            void set_pikachu_friendship(
                int pikachu_friendship
            );

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

            bool get_are_battle_effects_enabled();

            void set_are_battle_effects_enabled(
                bool are_battle_effects_enabled
            );

        private:
            struct pksav_gen1_save _pksav_save;

            void _register_attributes();
    };

}

#endif /* PKMN_GAME_SAVE_GEN1IMPL_HPP */
