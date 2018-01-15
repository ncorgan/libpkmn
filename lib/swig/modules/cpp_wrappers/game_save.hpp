/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_GAME_SAVE_HPP
#define CPP_WRAPPERS_GAME_SAVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>

#include "swig/modules/cpp_wrappers/item_list.hpp"
#include "swig/modules/cpp_wrappers/item_bag.hpp"
#include "swig/modules/cpp_wrappers/pokedex.hpp"
#include "swig/modules/cpp_wrappers/pokemon_party.hpp"
#include "swig/modules/cpp_wrappers/pokemon_pc.hpp"

namespace pkmn { namespace swig {

    class game_save
    {
        public:
            game_save():
                _game_save(nullptr)
            {}

            game_save(
                const pkmn::game_save::sptr& cpp_game_save
            ): _game_save(cpp_game_save)
            {
            }

            game_save(
                const std::string& filepath
            ): _game_save(pkmn::game_save::from_file(filepath))
            {
            }

            static std::string detect_type(
                const std::string& filepath
            )
            {
                return pkmn::game_save::detect_type(filepath);
            }

            bool operator==(
                const game_save& rhs
            ) const
            {
                return (_game_save == rhs._game_save);
            }

            inline std::string get_filepath()
            {
                return _game_save->get_filepath();
            }

            inline void save()
            {
                _game_save->save();
            }

            inline void save_as(
                const std::string& filepath
            )
            {
                _game_save->save_as(filepath);
            }

            inline std::string get_game()
            {
                return _game_save->get_game();
            }

            inline std::string get_trainer_name()
            {
                return _game_save->get_trainer_name();
            }

            inline void set_trainer_name(
                const std::string& trainer_name
            )
            {
                _game_save->set_trainer_name(trainer_name);
            }

            inline uint32_t get_trainer_id()
            {
                return _game_save->get_trainer_id();
            }

            inline void set_trainer_id(
                uint32_t trainer_id
            )
            {
                _game_save->set_trainer_id(trainer_id);
            }

            inline uint16_t get_trainer_public_id()
            {
                return _game_save->get_trainer_public_id();
            }

            inline void set_trainer_public_id(
                uint16_t trainer_public_id
            )
            {
                _game_save->set_trainer_public_id(trainer_public_id);
            }

            inline uint16_t get_trainer_secret_id()
            {
                return _game_save->get_trainer_secret_id();
            }

            inline void set_trainer_secret_id(
                uint16_t trainer_secret_id
            )
            {
                _game_save->set_trainer_secret_id(trainer_secret_id);
            }

            inline std::string get_trainer_gender()
            {
                return _game_save->get_trainer_gender();
            }

            inline void set_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _game_save->set_trainer_gender(trainer_gender);
            }

            inline std::string get_rival_name()
            {
                return _game_save->get_rival_name();
            }

            inline void set_rival_name(
                const std::string& rival_name
            )
            {
                _game_save->set_rival_name(rival_name);
            }

            inline int get_money()
            {
                return _game_save->get_money();
            }

            inline void set_money(
                int money
            )
            {
                _game_save->set_money(money);
            }

            inline pkmn::swig::pokedex get_pokedex()
            {
                return pkmn::swig::pokedex(_game_save->get_pokedex());
            }

            inline pkmn::swig::pokemon_party get_pokemon_party()
            {
                return pkmn::swig::pokemon_party(_game_save->get_pokemon_party());
            }

            inline pkmn::swig::pokemon_pc get_pokemon_pc()
            {
                return pkmn::swig::pokemon_pc(_game_save->get_pokemon_pc());
            }

            inline pkmn::swig::item_bag get_item_bag()
            {
                return pkmn::swig::item_bag(_game_save->get_item_bag());
            }

            inline pkmn::swig::item_list get_item_pc()
            {
                return pkmn::swig::item_list(_game_save->get_item_pc());
            }

            inline uintmax_t cptr()
            {
                return uintmax_t(_game_save.get());
            }

        private:
            pkmn::game_save::sptr _game_save;
    };

}}

#endif /* CPP_WRAPPERS_GAME_SAVE_HPP */
