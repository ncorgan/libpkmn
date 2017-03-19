/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_HPP
#define PKMN_GAME_SAVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/item_bag.hpp>
#include <pkmn/item_list.hpp>
#include <pkmn/pokemon_party.hpp>
#include <pkmn/pokemon_pc.hpp>
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

            static sptr from_file(
                const std::string &filepath
            );

            #ifndef __DOXYGEN__
            game_save() {};
            virtual ~game_save() {};
            #endif

            virtual std::string get_filepath() = 0;

            virtual void save() = 0;

            virtual void save_as(
                const std::string &filepath
            ) = 0;

            virtual std::string get_game() = 0;

            virtual std::string get_trainer_name() = 0;

            virtual void set_trainer_name(
                const std::string &trainer_name
            ) = 0;

            virtual uint32_t get_trainer_id() = 0;

            virtual void set_trainer_id(
                uint32_t trainer_id
            ) = 0;

            virtual uint16_t get_trainer_public_id() = 0;

            virtual void set_trainer_public_id(
                uint16_t trainer_public_id
            ) = 0;

            virtual uint16_t get_trainer_secret_id() = 0;

            virtual void set_trainer_secret_id(
                uint16_t trainer_secret_id
            ) = 0;

            virtual std::string get_trainer_gender() = 0;

            virtual void set_trainer_gender(
                const std::string &trainer_gender
            ) = 0;

            virtual std::string get_rival_name() = 0;

            virtual void set_rival_name(
                const std::string &rival_name
            ) = 0;

            virtual int get_money() = 0;

            virtual void set_money(
                int money
            ) = 0;

            virtual pkmn::pokemon_party::sptr get_pokemon_party() = 0;

            virtual pkmn::pokemon_pc::sptr get_pokemon_pc() = 0;

            virtual pkmn::item_bag::sptr get_item_bag() = 0;

            virtual pkmn::item_list::sptr get_item_pc() = 0;
    };
}

#endif /* PKMN_GAME_SAVE_HPP */
