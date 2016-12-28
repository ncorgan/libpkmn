/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GEN1IMPL_HPP
#define PKMN_GAME_SAVE_GEN1IMPL_HPP

#include "game_save_impl.hpp"

#include <pksav/gen1.h>

namespace pkmn {

    class game_save_gen1impl: public game_save_impl {
        public:
            game_save_gen1impl() {};
            game_save_gen1impl(
                const std::string &filepath
            );

            ~game_save_gen1impl();

            void save_as(
                const std::string &filepath
            );

            std::string get_trainer_name();

            void set_trainer_name(
                const std::string &trainer_name
            );

            uint32_t get_trainer_id();

            void set_trainer_id(
                uint32_t trainer_id
            );

            uint16_t get_trainer_public_id();

            void set_trainer_public_id(
                uint16_t trainer_public_id
            );

            uint16_t get_trainer_secret_id();

            void set_trainer_secret_id(
                uint16_t trainer_secret_id
            );

            std::string get_trainer_gender();

            void set_trainer_gender(
                const std::string &trainer_gender
            );

            std::string get_rival_name();

            void set_rival_name(
                const std::string &rival_name
            );

            int get_money();

            void set_money(
                int money
            );

        private:
            pksav_gen1_save_t _pksav_save;
    };

}

#endif /* PKMN_GAME_SAVE_GEN1IMPL_HPP */
