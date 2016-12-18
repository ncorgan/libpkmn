/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_GBAIMPL_HPP
#define PKMN_GAME_SAVE_GBAIMPL_HPP

#include "game_save_impl.hpp"

#include <pksav/gba.h>

namespace pkmn {

    class game_save_gbaimpl: public game_save_impl {
        public:
            game_save_gbaimpl() {};
            game_save_gbaimpl(
                const std::string &filepath
            );

            ~game_save_gbaimpl();

            std::string get_trainer_name();

            uint32_t get_trainer_id();

            uint16_t get_trainer_public_id();
            
            uint16_t get_trainer_secret_id();

            std::string get_trainer_gender();

            std::string get_rival_name();

            int get_money();

        private:
            pksav_gba_save_t _pksav_save;
    };

}

#endif /* PKMN_GAME_SAVE_GBAIMPL_HPP */
