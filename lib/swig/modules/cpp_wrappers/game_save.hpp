/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_GAME_SAVE_HPP
#define CPP_WRAPPERS_GAME_SAVE_HPP

#include "attribute_maps.hpp"
#include "item_list.hpp"
#include "item_bag.hpp"
#include "pokedex.hpp"
#include "pokemon_party.hpp"
#include "pokemon_pc.hpp"
#include "time_duration.hpp"

#include "private_exports.hpp"
#include "utils/misc.hpp"

#include <pkmn/config.hpp>
#include <pkmn/game_save.hpp>

#include <boost/assert.hpp>

#include <string>
#include <vector>

namespace pkmn { namespace swig {

    class game_save
    {
        public:
            explicit game_save(
                const pkmn::game_save::sptr& cpp_game_save
            ): _game_save(cpp_game_save),
               _generation(pkmn::priv::game_enum_to_generation(cpp_game_save->get_game()))
            {
                BOOST_ASSERT(_game_save.get() != nullptr);
            }

            explicit game_save(
                const std::string& filepath
            ): _game_save(pkmn::game_save::from_file(filepath))
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _generation = pkmn::priv::game_enum_to_generation(_game_save->get_game());
            }

            static std::string detect_type(
                const std::string& filepath
            )
            {
                return pkmn::game_save::detect_type(filepath);
            }

            inline std::string get_filepath()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_filepath();
            }

            inline void save()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->save();
            }

            inline void save_as(
                const std::string& filepath
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->save_as(filepath);
            }

            inline pkmn::e_game get_game()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_game();
            }

#ifdef SWIGCSHARP
            inline pkmn::time_duration get_time_played()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_time_played();
            }

            inline void set_time_played(
                const pkmn::time_duration& time_played
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_time_played(time_played);
            }
#else
            inline pkmn::swig::game_save_time_played get_time_played()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return pkmn::swig::game_save_time_played(_game_save);
            }
#endif

            inline std::string get_trainer_name()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_trainer_name();
            }

            inline void set_trainer_name(
                const std::string& trainer_name
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_trainer_name(trainer_name);
            }

            inline uint32_t get_trainer_id()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_trainer_id();
            }

            inline void set_trainer_id(
                uint32_t trainer_id
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_trainer_id(trainer_id);
            }

            inline uint16_t get_trainer_public_id()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_trainer_public_id();
            }

            inline void set_trainer_public_id(
                uint16_t trainer_public_id
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_trainer_public_id(trainer_public_id);
            }

            inline uint16_t get_trainer_secret_id()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                if(_generation >= 3)
                {
                    return _game_save->get_trainer_secret_id();
                }
                else
                {
                    return 0;
                }
            }

            inline void set_trainer_secret_id(
                uint16_t trainer_secret_id
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_trainer_secret_id(trainer_secret_id);
            }

            inline pkmn::e_gender get_trainer_gender()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                if(_generation >= 2)
                {
                    return _game_save->get_trainer_gender();
                }
                else
                {
                    return pkmn::e_gender::MALE;
                }
            }

            inline void set_trainer_gender(
                pkmn::e_gender trainer_gender
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_trainer_gender(trainer_gender);
            }

            inline std::string get_rival_name()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                static const std::vector<pkmn::e_game> GAMES_WITH_NO_RIVALS =
                {
                    pkmn::e_game::COLOSSEUM,
                    pkmn::e_game::XD
                };
                if(pkmn::does_vector_contain_value(GAMES_WITH_NO_RIVALS, this->get_game()))
                {
                    return "";
                }
                else
                {
                    return _game_save->get_rival_name();
                }
            }

            inline void set_rival_name(
                const std::string& rival_name
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_rival_name(rival_name);
            }

            inline int get_money()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return _game_save->get_money();
            }

            inline void set_money(
                int money
            )
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                _game_save->set_money(money);
            }

            inline pkmn::swig::pokedex get_pokedex()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                pkmn::e_game game = this->get_game();

                if((game == pkmn::e_game::COLOSSEUM) || (game == pkmn::e_game::XD))
                {
                    return pkmn::swig::pokedex();
                }
                else
                {
                    return pkmn::swig::pokedex(_game_save->get_pokedex());
                }
            }

            inline pkmn::swig::pokemon_party get_pokemon_party()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return pkmn::swig::pokemon_party(_game_save->get_pokemon_party());
            }

            inline pkmn::swig::pokemon_pc get_pokemon_pc()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return pkmn::swig::pokemon_pc(_game_save->get_pokemon_pc());
            }

            inline pkmn::swig::item_bag get_item_bag()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return pkmn::swig::item_bag(_game_save->get_item_bag());
            }

            inline pkmn::swig::item_list get_item_pc()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return pkmn::swig::item_list(_game_save->get_item_pc());
            }

            numeric_attribute_map<pkmn::game_save> get_numeric_attributes()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return numeric_attribute_map<pkmn::game_save>(_game_save);
            }

            string_attribute_map<pkmn::game_save> get_string_attributes()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return string_attribute_map<pkmn::game_save>(_game_save);
            }

            boolean_attribute_map<pkmn::game_save> get_boolean_attributes()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return boolean_attribute_map<pkmn::game_save>(_game_save);
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return uintmax_t(_game_save.get());
            }
#else
            inline bool operator==(const game_save& rhs) const
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return (_game_save == rhs._game_save);
            }

            inline bool operator!=(const game_save& rhs) const
            {
                BOOST_ASSERT(_game_save.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::game_save::sptr _game_save;
            int _generation;
    };

}}

#endif /* CPP_WRAPPERS_GAME_SAVE_HPP */
