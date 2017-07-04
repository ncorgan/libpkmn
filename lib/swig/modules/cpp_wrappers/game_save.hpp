/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_GAME_SAVE_HPP
#define CPP_WRAPPERS_GAME_SAVE_HPP

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>

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
                _init();
            }

            game_save(
                const std::string& filepath 
            ): _game_save(pkmn::game_save::from_file(filepath))
            {
                _init();
            }

            game_save(
                const game_save& other
            ): _game_save(other._game_save)
            {
                _init();
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

            PKMN_INLINE std::string get_filepath()
            {
                return _game_save->get_filepath();
            }

            PKMN_INLINE void save()
            {
                _game_save->save();
            }

            PKMN_INLINE void save_as(
                const std::string& filepath
            )
            {
                _game_save->save_as(filepath);
            }

            PKMN_INLINE std::string get_game()
            {
                return _game_save->get_game();
            }

            PKMN_INLINE std::string get_trainer_name()
            {
                return _game_save->get_trainer_name();
            }

            PKMN_INLINE void set_trainer_name(
                const std::string& trainer_name
            )
            {
                _game_save->set_trainer_name(trainer_name);
            }

            PKMN_INLINE uint32_t get_trainer_id()
            {
                return _game_save->get_trainer_id();
            }

            PKMN_INLINE void set_trainer_id(
                uint32_t trainer_id
            )
            {
                _game_save->set_trainer_id(trainer_id);
            }

            PKMN_INLINE uint16_t get_trainer_public_id()
            {
                return _game_save->get_trainer_public_id();
            }

            PKMN_INLINE void set_trainer_public_id(
                uint16_t trainer_public_id
            )
            {
                _game_save->set_trainer_public_id(trainer_public_id);
            }

            PKMN_INLINE uint16_t get_trainer_secret_id()
            {
                return _game_save->get_trainer_secret_id();
            }

            PKMN_INLINE void set_trainer_secret_id(
                uint16_t trainer_secret_id
            )
            {
                _game_save->set_trainer_secret_id(trainer_secret_id);
            }

            PKMN_INLINE std::string get_trainer_gender()
            {
                return _game_save->get_trainer_gender();
            }

            PKMN_INLINE void set_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _game_save->set_trainer_gender(trainer_gender);
            }

            PKMN_INLINE std::string get_rival_name()
            {
                return _game_save->get_rival_name();
            }

            PKMN_INLINE void set_rival_name(
                const std::string& rival_name
            )
            {
                _game_save->set_rival_name(rival_name);
            }

            PKMN_INLINE int get_money()
            {
                return _game_save->get_money();
            }

            PKMN_INLINE void set_money(
                int money
            )
            {
                _game_save->set_money(money);
            }

            PKMN_INLINE pkmn::swig::pokemon_party& get_pokemon_party()
            {
                return _pokemon_party;
            }

            PKMN_INLINE pkmn::swig::pokemon_pc& get_pokemon_pc()
            {
                return _pokemon_pc;
            }

            PKMN_INLINE pkmn::item_bag::sptr get_item_bag()
            {
                return _game_save->get_item_bag();
            }

            PKMN_INLINE pkmn::item_list::sptr get_item_pc()
            {
                return _game_save->get_item_pc();
            }

        private:
            pkmn::game_save::sptr _game_save;

            pkmn::swig::pokemon_party _pokemon_party;
            pkmn::swig::pokemon_pc _pokemon_pc;

            void _init()
            {
                _pokemon_party = pkmn::swig::pokemon_party(_game_save->get_pokemon_party());
                _pokemon_pc = pkmn::swig::pokemon_pc(_game_save->get_pokemon_pc());
            }
    };

}}

#endif /* CPP_WRAPPERS_GAME_SAVE_HPP */
