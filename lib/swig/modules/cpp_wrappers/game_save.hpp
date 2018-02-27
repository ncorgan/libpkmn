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

#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
#    include <boost/locale/encoding_utf.hpp>
#endif

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

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier verisons of SWIG.
 */
#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
            game_save(
                const std::wstring& filepath
            ): _game_save(pkmn::game_save::from_file(
                   boost::locale::conv::utf_to_utf<char>(filepath)
               ))
            {
            }

            static std::string detect_type(
                const std::string& filepath
            )
            {
                return pkmn::game_save::detect_type(
                           boost::locale::conv::utf_to_utf<char>(filepath)
                       );
            }

            inline void save_as(
                const std::wstring& filepath
            )
            {
                _game_save->save_as(
                    boost::locale::conv::utf_to_utf<char>(filepath)
                );
            }

            inline std::wstring get_trainer_name()
            {
                return boost::locale::conv::utf_to_utf<wchar_t>(
                           _game_save->get_trainer_name()
                       );
            }

            inline void set_trainer_name(
                const std::wstring& trainer_name
            )
            {
                _game_save->set_trainer_name(
                    boost::locale::conv::utf_to_utf<char>(trainer_name)
                );
            }

            inline std::wstring get_rival_name()
            {
                return boost::locale::conv::utf_to_utf<wchar_t>(
                           _game_save->get_rival_name()
                       );
            }

            inline void set_rival_name(
                const std::wstring& rival_name
            )
            {
                _game_save->set_rival_name(
                    boost::locale::conv::utf_to_utf<char>(rival_name)
                );
            }
#else
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

            inline void save_as(
                const std::string& filepath
            )
            {
                _game_save->save_as(filepath);
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
#endif
            inline std::string get_filepath()
            {
                return _game_save->get_filepath();
            }

            inline void save()
            {
                _game_save->save();
            }

            inline std::string get_game()
            {
                return _game_save->get_game();
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

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_game_save.get());
            }
#else
            inline bool operator==(const game_save& rhs) const
            {
                return (_game_save == rhs._game_save);
            }

            inline bool operator!=(const game_save& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::game_save::sptr _game_save;
    };

}}

#endif /* CPP_WRAPPERS_GAME_SAVE_HPP */
