/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_HPP
#define CPP_WRAPPERS_POKEMON_HPP

#include "exception_internal.hpp"
#include "private_exports.hpp"

#include "attribute_maps.hpp"
#include "pokemon_helpers.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

#include <boost/assert.hpp>

namespace pkmn { namespace swig {

    /*
     * This class is a thin wrapper around pkmn::pokemon::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * bulbasaur.EVs["Attack"] = 100
     *
     * Per conventions, when used as attributes, these getters won't
     * throw exceptions and will instead return a default value.
     */
    class pokemon
    {
        public:
            explicit pokemon(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon),
               _generation(pkmn::priv::game_name_to_generation(cpp_pokemon->get_game()))
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            pokemon(
                const std::string& species,
                const std::string& game,
                const std::string& form,
                int level
            ): _pokemon(pkmn::pokemon::make(species, game, form, level)),
               _generation(pkmn::priv::game_name_to_generation(game))
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            explicit pokemon(
                const std::string& filepath
            ): _pokemon(pkmn::pokemon::from_file(filepath))
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _generation = pkmn::priv::game_name_to_generation(_pokemon->get_game());
            }

            static const uint32_t DEFAULT_TRAINER_ID;
            static const std::string DEFAULT_TRAINER_NAME;

            inline pokemon to_game(
                const std::string& game
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return pokemon(_pokemon->to_game(game));
            }

            inline void export_to_file(
                const std::string& filepath
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->export_to_file(filepath);
            }

            inline std::string get_species()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_species();
            }

            inline std::string get_game()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_game();
            }

            inline std::string get_form()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_form();
            }

            inline void set_form(
                const std::string& form
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_form(form);
            }

            inline bool is_egg()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->is_egg();
            }

            inline void set_is_egg(bool is_egg)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_is_egg(is_egg);
            }

            // Copy the entry, since the const in the reference is casted away.
            inline pkmn::database::pokemon_entry get_database_entry()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_database_entry();
            }

            inline std::string get_condition()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_condition();
            }

            inline void set_condition(
                const std::string& condition
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_condition(condition);
            }

            inline std::string get_nickname()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_nickname();
            }

            inline void set_nickname(
                const std::string& nickname
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_nickname(nickname);
            }

            inline pkmn::e_gender get_gender()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_gender();
                }
                else
                {
                    return pkmn::e_gender::NONE;
                }
            }

            inline void set_gender(
                pkmn::e_gender gender
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_gender(gender);
            }

            inline bool is_shiny()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->is_shiny();
                }
                else
                {
                    return false;
                }
            }

            inline void set_shininess(
                bool value
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_shininess(value);
            }

            inline std::string get_held_item()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_held_item();
                }
                else
                {
                    return "";
                }
            }

            inline void set_held_item(
                const std::string& held_item
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_held_item(held_item);
            }

            inline int get_pokerus_duration()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_pokerus_duration();
                }
                else
                {
                    return 0;
                }
            }

            inline void set_pokerus_duration(int duration)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_pokerus_duration(duration);
            }

            inline std::string get_original_trainer_name()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_original_trainer_name();
            }

            inline void set_original_trainer_name(
                const std::string& trainer_name
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_trainer_name(trainer_name);
            }

            inline uint16_t get_original_trainer_public_id()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_original_trainer_public_id();
            }

            inline uint16_t get_original_trainer_secret_id()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 3)
                {
                    return _pokemon->get_original_trainer_secret_id();
                }
                else
                {
                    return 0;
                }
            }

            inline uint32_t get_original_trainer_id()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_original_trainer_id();
            }

            inline void set_original_trainer_public_id(
                uint16_t public_id
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_trainer_public_id(public_id);
            }

            inline void set_original_trainer_secret_id(
                uint16_t secret_id
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_trainer_secret_id(secret_id);
            }

            inline void set_original_trainer_id(
                uint32_t public_id
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_trainer_id(public_id);
            }

            inline pkmn::e_gender get_original_trainer_gender()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_original_trainer_gender();
                }
                else
                {
                    return pkmn::e_gender::NONE;
                }
            }

            inline void set_original_trainer_gender(
                pkmn::e_gender trainer_gender
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_trainer_gender(trainer_gender);
            }

            inline int get_current_trainer_friendship()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_current_trainer_friendship();
                }
                else
                {
                    return 0;
                }
            }

            inline void set_current_trainer_friendship(
                int friendship
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_current_trainer_friendship(friendship);
            }

            inline std::string get_ability()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 3)
                {
                    return _pokemon->get_ability();
                }
                else
                {
                    return "";
                }
            }

            inline void set_ability(
                const std::string& ability
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_ability(ability);
            }

            inline std::string get_ball()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 3)
                {
                    return _pokemon->get_ball();
                }
                else
                {
                    return "";
                }
            }

            inline void set_ball(
                const std::string& ball
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_ball(ball);
            }

            inline int get_level_met()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_level_met();
                }
                else
                {
                    return 0;
                }
            }

            inline void set_level_met(
                int level_met
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_level_met(level_met);
            }

            inline std::string get_location_met()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 2)
                {
                    return _pokemon->get_location_met(false);
                }
                else
                {
                    return "";
                }
            }

            inline void set_location_met(
                const std::string& location
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_location_met(location, false);
            }

            inline std::string get_location_met_as_egg()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 4)
                {
                    return _pokemon->get_location_met(true);
                }
                else
                {
                    return "";
                }
            }

            inline void set_location_met_as_egg(
                const std::string& location
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_location_met(location, true);
            }

            inline std::string get_original_game()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 3)
                {
                    return _pokemon->get_original_game();
                }
                else
                {
                    return "";
                }
            }

            inline void set_original_game(
                const std::string& original_game
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_original_game(original_game);
            }

            inline uint32_t get_personality()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                if(_generation >= 3)
                {
                    return _pokemon->get_personality();
                }
                else
                {
                    return 0U;
                }
            }

            inline void set_personality(
                uint32_t personality
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_personality(personality);
            }

            inline int get_experience()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_experience();
            }

            inline void set_experience(
                int experience
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_experience(experience);
            }

            inline int get_level()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_level();
            }

            inline void set_level(
                int level
            )
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_level(level);
            }

            inline int get_current_hp()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_current_hp();
            }

            inline void set_current_hp(int hp)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _pokemon->set_current_hp(hp);
            }

            inline EV_map get_EVs()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return EV_map(_pokemon);
            }

            inline IV_map get_IVs()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return IV_map(_pokemon);
            }

            inline marking_map get_markings()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return marking_map(_pokemon);
            }

            inline ribbon_map get_ribbons()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return ribbon_map(_pokemon);
            }

            inline contest_stat_map get_contest_stats()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return contest_stat_map(_pokemon);
            }

            inline move_slots get_moves()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return move_slots(_pokemon);
            }

            // Stats are read-only, so no need to wrap.
            inline std::map<std::string, int> get_stats()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_stats();
            }

            inline std::string get_icon_filepath()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_icon_filepath();
            }

            inline std::string get_sprite_filepath()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_sprite_filepath();
            }

            numeric_attribute_map<pkmn::pokemon> get_numeric_attributes()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return numeric_attribute_map<pkmn::pokemon>(_pokemon);
            }

            string_attribute_map<pkmn::pokemon> get_string_attributes()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return string_attribute_map<pkmn::pokemon>(_pokemon);
            }

            boolean_attribute_map<pkmn::pokemon> get_boolean_attributes()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return boolean_attribute_map<pkmn::pokemon>(_pokemon);
            }

            inline const pkmn::pokemon::sptr& get_internal() const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon;
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            inline bool operator==(const pokemon& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return (_pokemon == rhs._pokemon);
            }

            inline bool operator!=(const pokemon& rhs) const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokemon::sptr _pokemon;
            int _generation;
    };

    const uint32_t pokemon::DEFAULT_TRAINER_ID = pkmn::pokemon::DEFAULT_TRAINER_ID;
    const std::string pokemon::DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;

}}

#endif /* CPP_WRAPPERS_POKEMON_HPP */
