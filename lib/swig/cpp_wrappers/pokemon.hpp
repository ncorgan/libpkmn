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
     *
     * Note: "override" is specified by convention, but "virtual" is
     *       necessary for SWIG to generate director functions.
     */
    class pokemon: public pkmn::pokemon
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

            virtual ~pokemon() = default;

            static const uint32_t DEFAULT_TRAINER_ID;
            static const std::string DEFAULT_TRAINER_NAME;

            virtual pokemon to_game_swig(pkmn::e_game game)
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

            virtual pokemon clone_swig() const
            {
                pokemon ret;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    ret = pokemon(_pokemon->clone());
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }

                return ret;
            }

            virtual void export_to_file(
                const std::string& filepath
            ) override
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

            virtual pkmn::e_species get_species() const override
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

            virtual pkmn::e_game get_game() const override
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

            virtual std::string get_form() const override
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

            virtual void set_form(
                const std::string& form
            ) override
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

            virtual bool get_is_egg() const
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

            virtual void set_is_egg(bool is_egg) override
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
            pkmn::database::pokemon_entry get_database_entry_swig() const
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    return _pokemon->get_database_entry();
                }
                else
                {
                    return pkmn::pokemon::get_database_entry();
                }
            }

            virtual pkmn::e_condition get_condition() const override
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

            virtual void set_condition(
                pkmn::e_condition condition
            ) override
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

            virtual std::string get_nickname() const override
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

            virtual void set_nickname(
                const std::string& nickname
            ) override
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

            virtual pkmn::e_gender get_gender() const override
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

            virtual void set_gender(
                pkmn::e_gender gender
            ) override
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

            virtual bool get_is_shiny() const
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

            virtual void set_shininess(
                bool value
            ) override
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

            virtual pkmn::e_item get_held_item() const override
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

            virtual void set_held_item(
                pkmn::e_item held_item
            ) override
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

            virtual pkmn::e_nature get_nature() const override
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

            virtual void set_nature(pkmn::e_nature nature) override
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

            virtual int get_pokerus_duration() const override
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

            virtual void set_pokerus_duration(int duration) override
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

            virtual std::string get_original_trainer_name() const override
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

            virtual void set_original_trainer_name(
                const std::string& trainer_name
            ) override
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

            virtual uint16_t get_original_trainer_public_id() const override
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

            virtual uint16_t get_original_trainer_secret_id() const override
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

            virtual uint32_t get_original_trainer_id() const override
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

            virtual void set_original_trainer_public_id(
                uint16_t public_id
            ) override
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

            virtual void set_original_trainer_secret_id(
                uint16_t secret_id
            ) override
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

            virtual void set_original_trainer_id(
                uint32_t public_id
            ) override
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

            virtual pkmn::e_gender get_original_trainer_gender() const override
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

            virtual void set_original_trainer_gender(
                pkmn::e_gender trainer_gender
            ) override
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

            virtual pkmn::e_language get_language() const override
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

            virtual void set_language(pkmn::e_language language) override
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

            virtual int get_current_trainer_friendship() const override
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

            virtual void set_current_trainer_friendship(
                int friendship
            ) override
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

            virtual pkmn::e_ability get_ability() const override
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

            virtual void set_ability(pkmn::e_ability ability) override
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

            virtual pkmn::e_ball get_ball() const override
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

            virtual void set_ball(
                pkmn::e_ball ball
            ) override
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

            virtual int get_level_met() const override
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

            virtual void set_level_met(
                int level_met
            ) override
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

            virtual std::string get_location_met() const
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

            virtual void set_location_met(
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

            virtual std::string get_location_met_as_egg() const
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

            virtual void set_location_met_as_egg(
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

            virtual pkmn::e_game get_original_game() const override
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

            virtual void set_original_game(pkmn::e_game original_game) override
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

            virtual uint32_t get_personality() const override
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

            virtual void set_personality(
                uint32_t personality
            ) override
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

            virtual int get_experience() const override
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

            virtual void set_experience(
                int experience
            ) override
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

            virtual int get_level() const override
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

            virtual void set_level(
                int level
            ) override
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

            virtual int get_current_hp() const override
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

            virtual void set_current_hp(int hp) override
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

            EV_map get_EVs_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return EV_map(_pokemon);
            }

            IV_map get_IVs_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return IV_map(_pokemon);
            }

            marking_map get_markings_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return marking_map(_pokemon);
            }

            ribbon_map get_ribbons_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return ribbon_map(_pokemon);
            }

            contest_stat_map get_contest_stats_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return contest_stat_map(_pokemon);
            }

            move_slots get_moves_helper()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return move_slots(_pokemon);
            }

            std::map<pkmn::e_stat, int> get_stats_swig()
            {
                std::map<pkmn::e_stat, int> stats;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    stats = _pokemon->get_stats();
                }
                else
                {
                    stats = pkmn::pokemon::get_stats();
                }

                return stats;
            }

            virtual std::string get_icon_filepath() const override
            {
                std::string icon_filepath;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    icon_filepath = _pokemon->get_icon_filepath();
                }
                else
                {
                    icon_filepath = pkmn::pokemon::get_icon_filepath();
                }

                return icon_filepath;
            }

            virtual std::string get_sprite_filepath() const override
            {
                std::string sprite_filepath;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    sprite_filepath = _pokemon->get_sprite_filepath();
                }
                else
                {
                    sprite_filepath = pkmn::pokemon::get_sprite_filepath();
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

            const pkmn::pokemon::sptr& get_internal() const
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return _pokemon;
            }

#ifdef SWIGCSHARP
            uintmax_t cptr()
            {
                BOOST_ASSERT(_pokemon.get() != nullptr);

                return uintmax_t(_pokemon.get());
            }
#else
            bool operator==(const pokemon& rhs) const
            {
                bool ret = false;

                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    ret = (_pokemon == rhs._pokemon);
                }
                else
                {
                }

                return ret;
            }

            bool operator!=(const pokemon& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        // These are unfortunately needed for subclasses to access the
        // abstraction maps.

        std::map<pkmn::e_stat, int>& get_EVs_map_internal()
        {
            return _EVs;
        }

        std::map<pkmn::e_stat, int>& get_IVs_map_internal()
        {
            return _IVs;
        }

        std::map<pkmn::e_stat, int>& get_stats_map_internal()
        {
            return _stats;
        }

        // The API for this functionality is intended to be different on the SWIG
        // layer (using the helper classes), but this underlying functionality is
        // necessary for the helper classes to call.
        protected:

            virtual void set_marking(
                pkmn::e_marking marking,
                bool value
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_marking(marking, value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_ribbon(
                const std::string& ribbon,
                bool value
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_ribbon(ribbon, value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_contest_stat(
                pkmn::e_contest_stat contest_stat,
                int value
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_contest_stat(contest_stat, value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_move(
                pkmn::e_move move,
                int index
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_move(move, index);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_move_pp(
                int index,
                int pp
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_move_pp(index, pp);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_EV(
                pkmn::e_stat EV,
                int value
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_EV(EV, value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

            virtual void set_IV(
                pkmn::e_stat IV,
                int value
            ) override
            {
                if(_is_from_libpkmn)
                {
                    BOOST_ASSERT(_pokemon.get() != nullptr);
                    _pokemon->set_IV(IV, value);
                }
                else
                {
                    throw pkmn::unimplemented_error();
                }
            }

        private:
            pkmn::pokemon::sptr _pokemon;
            int _generation;
            bool _is_from_libpkmn;

            // Functions not to be exposed, as the functionality is done
            // differently on the SWIG layer.

            bool is_egg() const final
            {
                return false;
            }

            bool is_shiny() const final
            {
                return false;
            }

            pkmn::pokemon::sptr to_game(pkmn::e_game) final
            {
                return pkmn::pokemon::sptr();
            }

            pkmn::pokemon::sptr clone() const final
            {
                return pkmn::pokemon::sptr();
            }

            const pkmn::database::pokemon_entry& get_database_entry() const final
            {
                return pkmn::pokemon::get_database_entry();
            }

            std::string get_location_met(bool) const final
            {
                return "";
            }

            void set_location_met(const std::string&, bool) final
            {
            }

            const void* get_native() const final
            {
                return nullptr;
            }

    };

    const uint32_t pokemon::DEFAULT_TRAINER_ID = pkmn::pokemon::DEFAULT_TRAINER_ID;
    const std::string pokemon::DEFAULT_TRAINER_NAME = pkmn::pokemon::DEFAULT_TRAINER_NAME;

}}

#endif /* CPP_WRAPPERS_POKEMON_HPP */
