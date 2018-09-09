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
            pokemon():
                _pokemon(nullptr),
                _generation(0),
                _is_from_libpkmn(false)
            {
            }

            explicit pokemon(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon),
               _generation(pkmn::priv::game_enum_to_generation(cpp_pokemon->get_game())),
               _is_from_libpkmn(true)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            pokemon(
                pkmn::e_species species,
                pkmn::e_game game,
                const std::string& form,
                int level
            ): _pokemon(pkmn::pokemon::make(species, game, form, level)),
               _generation(pkmn::priv::game_enum_to_generation(game)),
               _is_from_libpkmn(true)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);
            }

            explicit pokemon(
                const std::string& filepath
            ): _pokemon(pkmn::pokemon::from_file(filepath)),
               _is_from_libpkmn(true)
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                _generation = pkmn::priv::game_enum_to_generation(_pokemon->get_game());
            }

            static const uint32_t DEFAULT_TRAINER_ID;
            static const std::string DEFAULT_TRAINER_NAME;

            inline pokemon to_game(pkmn::e_game game)
            {
                pokemon ret;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    ret = pokemon(_pokemon->to_game(game));
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return ret;
            }

            inline void export_to_file(
                const std::string& filepath
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->export_to_file(filepath);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_species get_species()
            {
                pkmn::e_species species = pkmn::e_species::NONE;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    species = _pokemon->get_species();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return species;
            }

            inline pkmn::e_game get_game()
            {
                pkmn::e_game game = pkmn::e_game::NONE;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    game = _pokemon->get_game();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return game;
            }

            inline std::string get_form()
            {
                std::string form;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    form = _pokemon->get_form();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return form;
            }

            inline void set_form(
                const std::string& form
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_form(form);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline bool is_egg()
            {
                bool is_egg = false;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    is_egg = _pokemon->is_egg();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return is_egg;
            }

            inline void set_is_egg(bool is_egg)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_is_egg(is_egg);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            // Copy the entry, since the const in the reference is casted away.
            // TODO: use base class's version when implemented
            inline pkmn::database::pokemon_entry get_database_entry()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_database_entry();
            }

            inline pkmn::e_condition get_condition()
            {
                pkmn::e_condition condition;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    condition = _pokemon->get_condition();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return condition;
            }

            inline void set_condition(
                pkmn::e_condition condition
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_condition(condition);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline std::string get_nickname()
            {
                std::string nickname;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    nickname = _pokemon->get_nickname();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return nickname;
            }

            inline void set_nickname(
                const std::string& nickname
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_nickname(nickname);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_gender get_gender()
            {
                pkmn::e_gender gender;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        gender = _pokemon->get_gender();
                    }
                    else
                    {
                        gender = pkmn::e_gender::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return gender;
            }

            inline void set_gender(
                pkmn::e_gender gender
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_gender(gender);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline bool is_shiny()
            {
                bool is_shiny = false;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        is_shiny = _pokemon->is_shiny();
                    }
                    else
                    {
                        is_shiny = false;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return is_shiny;
            }

            inline void set_shininess(
                bool value
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_shininess(value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_item get_held_item()
            {
                pkmn::e_item held_item;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        held_item = _pokemon->get_held_item();
                    }
                    else
                    {
                        held_item = pkmn::e_item::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return held_item;
            }

            inline void set_held_item(
                pkmn::e_item held_item
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_held_item(held_item);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_nature get_nature()
            {
                pkmn::e_nature nature;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        nature = _pokemon->get_nature();
                    }
                    else
                    {
                        nature = pkmn::e_nature::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return nature;
            }

            inline void set_nature(pkmn::e_nature nature)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_nature(nature);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_pokerus_duration()
            {
                int pokerus_duration = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        pokerus_duration = _pokemon->get_pokerus_duration();
                    }
                    else
                    {
                        pokerus_duration = 0;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return pokerus_duration;
            }

            inline void set_pokerus_duration(int duration)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_pokerus_duration(duration);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline std::string get_original_trainer_name()
            {
                std::string original_trainer_name;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    original_trainer_name = _pokemon->get_original_trainer_name();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return original_trainer_name;
            }

            inline void set_original_trainer_name(
                const std::string& trainer_name
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_trainer_name(trainer_name);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline uint16_t get_original_trainer_public_id()
            {
                uint16_t original_trainer_public_id = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    original_trainer_public_id = _pokemon->get_original_trainer_public_id();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return original_trainer_public_id;
            }

            inline uint16_t get_original_trainer_secret_id()
            {
                uint16_t original_trainer_secret_id = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        original_trainer_secret_id = _pokemon->get_original_trainer_secret_id();
                    }
                    else
                    {
                        original_trainer_secret_id = 0;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return original_trainer_secret_id;
            }

            inline uint32_t get_original_trainer_id()
            {
                uint32_t original_trainer_id = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    original_trainer_id = _pokemon->get_original_trainer_id();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return original_trainer_id;
            }

            inline void set_original_trainer_public_id(
                uint16_t public_id
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_trainer_public_id(public_id);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline void set_original_trainer_secret_id(
                uint16_t secret_id
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_trainer_secret_id(secret_id);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline void set_original_trainer_id(
                uint32_t public_id
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_trainer_id(public_id);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_gender get_original_trainer_gender()
            {
                pkmn::e_gender gender;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        gender = _pokemon->get_original_trainer_gender();
                    }
                    else
                    {
                        gender = pkmn::e_gender::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return gender;
            }

            inline void set_original_trainer_gender(
                pkmn::e_gender trainer_gender
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_trainer_gender(trainer_gender);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_language get_language()
            {
                pkmn::e_language language;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        language = _pokemon->get_language();
                    }
                    else
                    {
                        language = pkmn::e_language::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return language;
            }

            inline void set_language(pkmn::e_language language)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_language(language);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_current_trainer_friendship()
            {
                int current_trainer_friendship = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        current_trainer_friendship = _pokemon->get_current_trainer_friendship();
                    }
                    else
                    {
                        current_trainer_friendship = 0;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return current_trainer_friendship;
            }

            inline void set_current_trainer_friendship(
                int friendship
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_current_trainer_friendship(friendship);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_ability get_ability()
            {
                pkmn::e_ability ability;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        ability = _pokemon->get_ability();
                    }
                    else
                    {
                        ability = pkmn::e_ability::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return ability;
            }

            inline void set_ability(pkmn::e_ability ability)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_ability(ability);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_ball get_ball()
            {
                pkmn::e_ball ball;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        ball = _pokemon->get_ball();
                    }
                    else
                    {
                        ball = pkmn::e_ball::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return ball;
            }

            inline void set_ball(
                pkmn::e_ball ball
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_ball(ball);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_level_met()
            {
                int level_met = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        level_met = _pokemon->get_level_met();
                    }
                    else
                    {
                        level_met = 0;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return level_met;
            }

            inline void set_level_met(
                int level_met
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_level_met(level_met);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline std::string get_location_met()
            {
                std::string location_met;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 2)
                    {
                        location_met = _pokemon->get_location_met(false);
                    }
                    else
                    {
                        location_met = "";
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return location_met;
            }

            inline void set_location_met(
                const std::string& location
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_location_met(location, false);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline std::string get_location_met_as_egg()
            {
                std::string location_met_as_egg;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 4)
                    {
                        location_met_as_egg = _pokemon->get_location_met(true);
                    }
                    else
                    {
                        location_met_as_egg = "";
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return location_met_as_egg;
            }

            inline void set_location_met_as_egg(
                const std::string& location
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_location_met(location, true);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline pkmn::e_game get_original_game()
            {
                pkmn::e_game game;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);

                    if(_generation >= 3)
                    {
                        game = _pokemon->get_original_game();
                    }
                    else
                    {
                        game = pkmn::e_game::NONE;
                    }
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return game;
            }

            inline void set_original_game(pkmn::e_game original_game)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_original_game(original_game);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline uint32_t get_personality()
            {
                uint32_t personality = 0;

                if(_is_from_libpkmn)
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
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return personality;
            }

            inline void set_personality(
                uint32_t personality
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_personality(personality);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_experience()
            {
                int experience = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_experience();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return experience;
            }

            inline void set_experience(
                int experience
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_experience(experience);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_level()
            {
                int level = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_level();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return level;
            }

            inline void set_level(
                int level
            )
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_level(level);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            inline int get_current_hp()
            {
                int current_hp = 0;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_current_hp();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return current_hp;
            }

            inline void set_current_hp(int hp)
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_current_hp(hp);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
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
            // TODO: call base class's function when implemented
            inline std::map<pkmn::e_stat, int> get_stats()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon->get_stats();
            }

            inline std::string get_icon_filepath()
            {
                std::string icon_filepath;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_icon_filepath();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return icon_filepath;
            }

            inline std::string get_sprite_filepath()
            {
                std::string sprite_filepath;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_sprite_filepath();
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return sprite_filepath;
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
            bool _is_from_libpkmn;
    };

    const uint32_t pokemon::DEFAULT_TRAINER_ID = pkmn::pokemon::DEFAULT_TRAINER_ID;
    const std::string pokemon::DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;

}}

#endif /* CPP_WRAPPERS_POKEMON_HPP */
