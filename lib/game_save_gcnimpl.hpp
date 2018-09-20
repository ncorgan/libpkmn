/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GCNIMPL_HPP
#define PKMN_GAME_SAVE_GCNIMPL_HPP

#include "game_save_impl.hpp"

#include "pkmgc/includes.hpp"

#include <memory>
#include <vector>

namespace pkmn {

    class game_save_gcnimpl: public game_save_impl {
        public:
            game_save_gcnimpl(
                const std::string& filepath,
                std::vector<uint8_t>&& raw
            );
            ~game_save_gcnimpl() = default;

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

            pkmn::e_gender get_trainer_gender() final;

            void set_trainer_gender(
                pkmn::e_gender trainer_gender
            ) final;

            std::string get_rival_name() final;

            void set_rival_name(
                const std::string& rival_name
            ) final;

            int get_money() final;

            void set_money(
                int money
            ) final;

        private:
            std::unique_ptr<LibPkmGC::GC::SaveEditing::Save> _libpkmgc_save_uptr;
            LibPkmGC::GC::SaveEditing::SaveSlot* _current_slot;

            bool _colosseum, _has_gci_data;

            // For some reason, attempting to set the value of one PokemonBox
            // pointer to another crashes, so we need to make an extra copy,
            // but LibPkmGC::GC::PokemonBox is an abstract type, so we need
            // to do it per-type.
            template <typename libpkmgc_box_type>
            void save_native_pokemon_boxes()
            {
                const std::unique_ptr<libpkmgc_box_type>* p_libpkmgc_box_uptrs =
                    static_cast<const std::unique_ptr<libpkmgc_box_type>*>(
                        _pokemon_pc->get_native()
                    );
                for(size_t box_index = 0;
                    box_index < _current_slot->PC->nbBoxes;
                    ++box_index)
                {
                    libpkmgc_box_type box_copy(*p_libpkmgc_box_uptrs[box_index].get());
                    _current_slot->PC->boxes[box_index]->swap(box_copy);
                }
            }
    };

}

#endif /* PKMN_GAME_SAVE_GCNIMPL_HPP */
