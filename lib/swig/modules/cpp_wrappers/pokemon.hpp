/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_HPP
#define CPP_WRAPPERS_HPP

#include "exception_internal.hpp"

#include "attribute_maps.hpp"
#include "pokemon_helpers.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon.hpp>

namespace pkmn { namespace swig {

    /*
     * This class is a thin wrapper around pkmn::pokemon::sptr and
     * will be what some SWIG wrappers will use instead of the class
     * itself. It will allow syntax like the following to be used:
     *
     * bulbasaur.EVs["Attack"] = 100
     */
    class pokemon
    {
        public:
            pokemon():
                _pokemon(nullptr)
            {}

            pokemon(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                _init();
            }

            pokemon(
                const std::string& species,
                const std::string& game,
                const std::string& form,
                int level
            ): _pokemon(pkmn::pokemon::make(species, game, form, level))
            {
                _init();
            }

            pokemon(
                const std::string& filepath
            ): _pokemon(pkmn::pokemon::from_file(filepath))
            {
                _init();
            }

            inline pokemon to_game(
                const std::string& game
            )
            {
                return pokemon(_pokemon->to_game(game));
            }

            inline void export_to_file(
                const std::string& filepath
            )
            {
                _pokemon->export_to_file(filepath);
            }

            inline std::string get_species()
            {
                return _pokemon->get_species();
            }

            inline std::string get_game()
            {
                return _pokemon->get_game();
            }

            inline std::string get_form()
            {
                return _pokemon->get_form();
            }

            inline void set_form(
                const std::string& form
            )
            {
                _pokemon->set_form(form);
            }

            inline bool is_egg()
            {
                return _pokemon->is_egg();
            }

            inline void set_is_egg(bool is_egg)
            {
                _pokemon->set_is_egg(is_egg);
            }

            // Copying is cheap, and we can't do SWIG attributes without it.
            inline pkmn::database::pokemon_entry get_database_entry()
            {
                return _pokemon->get_database_entry();
            }

            inline std::string get_condition()
            {
                return _pokemon->get_condition();
            }

            inline void set_condition(
                const std::string& condition
            )
            {
                _pokemon->set_condition(condition);
            }

            inline std::string get_nickname()
            {
                return _pokemon->get_nickname();
            }

            inline void set_nickname(
                const std::string& nickname
            )
            {
                _pokemon->set_nickname(nickname);
            }

            inline std::string get_gender()
            {
                return _pokemon->get_gender();
            }

            inline void set_gender(
                const std::string& gender
            )
            {
                _pokemon->set_gender(gender);
            }

            inline bool is_shiny()
            {
                return _pokemon->is_shiny();
            }

            inline void set_shininess(
                bool value
            )
            {
                _pokemon->set_shininess(value);
            }

            inline std::string get_held_item()
            {
                return _pokemon->get_held_item();
            }

            inline void set_held_item(
                const std::string& held_item
            )
            {
                _pokemon->set_held_item(held_item);
            }

            inline int get_pokerus_duration()
            {
                return _pokemon->get_pokerus_duration();
            }

            inline void set_pokerus_duration(int duration)
            {
                _pokemon->set_pokerus_duration(duration);
            }

            inline std::string get_original_trainer_name()
            {
                return _pokemon->get_original_trainer_name();
            }

            inline void set_original_trainer_name(
                const std::string& trainer_name
            )
            {
                _pokemon->set_original_trainer_name(trainer_name);
            }

            inline uint16_t get_original_trainer_public_id()
            {
                return _pokemon->get_original_trainer_public_id();
            }

            inline uint16_t get_original_trainer_secret_id()
            {
                return _pokemon->get_original_trainer_secret_id();
            }

            inline uint32_t get_original_trainer_id()
            {
                return _pokemon->get_original_trainer_id();
            }

            inline void set_original_trainer_public_id(
                uint16_t public_id
            )
            {
                _pokemon->set_original_trainer_public_id(public_id);
            }

            inline void set_original_trainer_secret_id(
                uint16_t secret_id
            )
            {
                _pokemon->set_original_trainer_secret_id(secret_id);
            }

            inline void set_original_trainer_id(
                uint32_t public_id
            )
            {
                _pokemon->set_original_trainer_id(public_id);
            }

            inline std::string get_original_trainer_gender()
            {
                return _pokemon->get_original_trainer_gender();
            }

            inline void set_original_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _pokemon->set_original_trainer_gender(trainer_gender);
            }

            inline int get_current_trainer_friendship()
            {
                return _pokemon->get_current_trainer_friendship();
            }

            inline void set_current_trainer_friendship(
                int friendship
            )
            {
                _pokemon->set_current_trainer_friendship(friendship);
            }

            inline std::string get_ability()
            {
                return _pokemon->get_ability();
            }

            inline void set_ability(
                const std::string& ability
            )
            {
                _pokemon->set_ability(ability);
            }

            inline std::string get_ball()
            {
                return _pokemon->get_ball();
            }

            inline void set_ball(
                const std::string& ball
            )
            {
                _pokemon->set_ball(ball);
            }

            inline int get_level_met()
            {
                return _pokemon->get_level_met();
            }

            inline void set_level_met(
                int level_met
            )
            {
                _pokemon->set_level_met(level_met);
            }

            inline std::string get_location_met()
            {
                return _pokemon->get_location_met(false);
            }

            inline void set_location_met(
                const std::string& location
            )
            {
                _pokemon->set_location_met(location, false);
            }

            inline std::string get_location_met_as_egg()
            {
                return _pokemon->get_location_met(true);
            }

            inline void set_location_met_as_egg(
                const std::string& location
            )
            {
                _pokemon->set_location_met(location, true);
            }

            inline std::string get_original_game()
            {
                return _pokemon->get_original_game();
            }

            inline void set_original_game(
                const std::string& original_game
            )
            {
                _pokemon->set_original_game(original_game);
            }

            inline uint32_t get_personality()
            {
                return _pokemon->get_personality();
            }

            inline void set_personality(
                uint32_t personality
            )
            {
                _pokemon->set_personality(personality);
            }

            inline int get_experience()
            {
                return _pokemon->get_experience();
            }

            inline void set_experience(
                int experience
            )
            {
                _pokemon->set_experience(experience);
            }

            inline int get_level()
            {
                return _pokemon->get_level();
            }

            inline void set_level(
                int level
            )
            {
                _pokemon->set_level(level);
            }

            inline int get_current_hp()
            {
                return _pokemon->get_current_hp();
            }

            inline void set_current_hp(int hp)
            {
                _pokemon->set_current_hp(hp);
            }

            inline EV_map get_EVs()
            {
                return _EV_map;
            }

            inline IV_map get_IVs()
            {
                return _IV_map;
            }

            inline marking_map get_markings()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_markings();

                return _marking_map;
            }

            inline ribbon_map get_ribbons()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_ribbons();

                return _ribbon_map;
            }

            inline contest_stat_map get_contest_stats()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_contest_stats();

                return _contest_stat_map;
            }

            inline move_slots get_moves()
            {
                return _move_slots;
            }

            // Stats are read-only, so no need to wrap.
            inline const std::map<std::string, int> get_stats()
            {
                return _pokemon->get_stats();
            }

            inline std::string get_icon_filepath()
            {
                return _pokemon->get_icon_filepath();
            }

            inline std::string get_sprite_filepath()
            {
                return _pokemon->get_sprite_filepath();
            }

            numeric_attribute_map<pkmn::pokemon> get_numeric_attributes()
            {
                return _numeric_attribute_map;
            }

            string_attribute_map<pkmn::pokemon> get_string_attributes()
            {
                return _string_attribute_map;
            }

            inline pkmn::pokemon::sptr get_internal() const
            {
                return _pokemon;
            }

            // TODO: ifdef for specific wrappers

            bool operator==(
                const pokemon& rhs
            ) const
            {
                return (_pokemon == rhs._pokemon);
            }

            // For hash code functions
            uintmax_t cptr()
            {
                return uintmax_t(_pokemon.get());
            }

        private:
            pkmn::pokemon::sptr _pokemon;
            EV_map _EV_map;
            IV_map _IV_map;
            marking_map _marking_map;
            ribbon_map _ribbon_map;
            contest_stat_map _contest_stat_map;
            move_slots _move_slots;
            numeric_attribute_map<pkmn::pokemon> _numeric_attribute_map;
            string_attribute_map<pkmn::pokemon> _string_attribute_map;

            void _init()
            {
                _EV_map = EV_map(_pokemon);
                _IV_map = IV_map(_pokemon);
                _marking_map = marking_map(_pokemon);
                _ribbon_map = ribbon_map(_pokemon);
                _contest_stat_map = contest_stat_map(_pokemon);
                _move_slots = move_slots(_pokemon);
                _numeric_attribute_map = numeric_attribute_map<pkmn::pokemon>(_pokemon);
                _string_attribute_map = string_attribute_map<pkmn::pokemon>(_pokemon);
            }
    };

}}

#endif /* CPP_WRAPPERS_HPP */
