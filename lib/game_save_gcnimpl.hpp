/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GCNIMPL_HPP
#define PKMN_GAME_SAVE_GCNIMPL_HPP

#include "game_save_impl.hpp"

#include "libpkmgc_includes.hpp"

#include <memory>
#include <vector>

namespace pkmn {

    class game_save_gcnimpl: public game_save_impl {
        public:
            game_save_gcnimpl() {};
            game_save_gcnimpl(
                const std::string& filepath,
                std::vector<uint8_t>&& raw
            );

            ~game_save_gcnimpl();

            void save_as(
                const std::string &filepath
            ) override final;

            pkmn::time_duration get_time_played() override final;

            void set_time_played(
                const pkmn::time_duration& time_played
            ) override final;

            std::string get_trainer_name() override final;

            void set_trainer_name(
                const std::string &trainer_name
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
                const std::string &trainer_gender
            ) override final;

            std::string get_rival_name() override final;

            void set_rival_name(
                const std::string &rival_name
            ) override final;

            int get_money() override final;

            void set_money(
                int money
            ) override final;

            pkmn::pokedex::sptr get_pokedex() override final;

        private:
            std::unique_ptr<LibPkmGC::GC::SaveEditing::Save> _libpkmgc_save;
            LibPkmGC::GC::SaveEditing::SaveSlot* _current_slot;

            bool _colosseum, _has_gci_data;
    };

}

#endif /* PKMN_GAME_SAVE_GCNIMPL_HPP */
