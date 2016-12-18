/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_HPP
#define PKMN_GAME_SAVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API game_save {
        public:
            typedef pkmn::shared_ptr<game_save> sptr;

            static std::string detect_type(
                const std::string &filepath
            );

            #ifndef __DOXYGEN__
            game_save() {};
            virtual ~game_save() {};
            #endif

            virtual std::string get_filepath() = 0;

            virtual std::string get_game() = 0;

            virtual std::string get_trainer_name() = 0;

            virtual uint32_t get_trainer_id() = 0;

            virtual uint16_t get_trainer_public_id() = 0;

            virtual uint16_t get_trainer_secret_id() = 0;

            virtual std::string get_trainer_gender() = 0;

            virtual std::string get_rival_name() = 0;

            virtual int get_money() = 0;

            virtual pkmn::item_bag::sptr get_item_bag() = 0;

            virtual pkmn::item_list::sptr get_item_pc() = 0;
    };
}

#endif /* PKMN_GAME_SAVE_HPP */
