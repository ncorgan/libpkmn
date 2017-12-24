/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_HPP
#define CPP_WRAPPERS_HPP

#include "exception_internal.hpp"

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

            pokemon(
                const pokemon& other
            ): _pokemon(other._pokemon)
            {
                _init();
            }

            bool operator==(
                const pokemon& rhs
            ) const
            {
                return (_pokemon == rhs._pokemon);
            }

            PKMN_INLINE pkmn::pokemon::sptr get_internal() const
            {
                return _pokemon;
            }

            PKMN_INLINE std::string get_species()
            {
                return _pokemon->get_species();
            }

            PKMN_INLINE std::string get_game()
            {
                return _pokemon->get_game();
            }

            PKMN_INLINE std::string get_form()
            {
                return _pokemon->get_form();
            }

            PKMN_INLINE void set_form(
                const std::string& form
            )
            {
                _pokemon->set_form(form);
            }

            PKMN_INLINE const pkmn::database::pokemon_entry& get_database_entry()
            {
                return _pokemon->get_database_entry();
            }

            PKMN_INLINE std::string get_nickname()
            {
                return _pokemon->get_nickname();
            }

            PKMN_INLINE void set_nickname(
                const std::string& nickname
            )
            {
                _pokemon->set_nickname(nickname);
            }

            PKMN_INLINE std::string get_gender()
            {
                return _pokemon->get_gender();
            }

            PKMN_INLINE void set_gender(
                const std::string& gender
            )
            {
                _pokemon->set_gender(gender);
            }

            PKMN_INLINE bool is_shiny()
            {
                return _pokemon->is_shiny();
            }

            PKMN_INLINE void set_shininess(
                bool value
            )
            {
                _pokemon->set_shininess(value);
            }

            PKMN_INLINE std::string get_held_item()
            {
                return _pokemon->get_held_item();
            }

            PKMN_INLINE void set_held_item(
                const std::string& held_item
            )
            {
                _pokemon->set_held_item(held_item);
            }

            PKMN_INLINE std::string get_original_trainer_name()
            {
                return _pokemon->get_original_trainer_name();
            }

            PKMN_INLINE void set_original_trainer_name(
                const std::string& trainer_name
            )
            {
                _pokemon->set_original_trainer_name(trainer_name);
            }

            PKMN_INLINE uint16_t get_original_trainer_public_id()
            {
                return _pokemon->get_original_trainer_public_id();
            }

            PKMN_INLINE uint16_t get_original_trainer_secret_id()
            {
                return _pokemon->get_original_trainer_secret_id();
            }

            PKMN_INLINE uint32_t get_original_trainer_id()
            {
                return _pokemon->get_original_trainer_id();
            }

            PKMN_INLINE void set_original_trainer_public_id(
                uint16_t public_id
            )
            {
                _pokemon->set_original_trainer_public_id(public_id);
            }

            PKMN_INLINE void set_original_trainer_secret_id(
                uint16_t secret_id
            )
            {
                _pokemon->set_original_trainer_secret_id(secret_id);
            }

            PKMN_INLINE void set_original_trainer_id(
                uint32_t public_id
            )
            {
                _pokemon->set_original_trainer_id(public_id);
            }

            PKMN_INLINE std::string get_original_trainer_gender()
            {
                return _pokemon->get_original_trainer_gender();
            }

            PKMN_INLINE void set_original_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _pokemon->set_original_trainer_gender(trainer_gender);
            }

            PKMN_INLINE int get_current_trainer_friendship()
            {
                return _pokemon->get_current_trainer_friendship();
            }

            PKMN_INLINE void set_current_trainer_friendship(
                int friendship
            )
            {
                _pokemon->set_current_trainer_friendship(friendship);
            }

            PKMN_INLINE std::string get_ability()
            {
                return _pokemon->get_ability();
            }

            PKMN_INLINE void set_ability(
                const std::string& ability
            )
            {
                _pokemon->set_ability(ability);
            }

            PKMN_INLINE std::string get_ball()
            {
                return _pokemon->get_ball();
            }

            PKMN_INLINE void set_ball(
                const std::string& ball
            )
            {
                _pokemon->set_ball(ball);
            }

            PKMN_INLINE int get_level_met()
            {
                return _pokemon->get_level_met();
            }

            PKMN_INLINE void set_level_met(
                int level_met
            )
            {
                _pokemon->set_level_met(level_met);
            }

            PKMN_INLINE std::string get_location_met(
                bool as_egg
            )
            {
                return _pokemon->get_location_met(as_egg);
            }

            PKMN_INLINE void set_location_met(
                const std::string& location,
                bool as_egg
            )
            {
                _pokemon->set_location_met(location, as_egg);
            }

            PKMN_INLINE std::string get_original_game()
            {
                return _pokemon->get_original_game();
            }

            PKMN_INLINE void set_original_game(
                const std::string& original_game
            )
            {
                _pokemon->set_original_game(original_game);
            }

            PKMN_INLINE uint32_t get_personality()
            {
                return _pokemon->get_personality();
            }

            PKMN_INLINE void set_personality(
                uint32_t personality
            )
            {
                _pokemon->set_personality(personality);
            }

            PKMN_INLINE int get_experience()
            {
                return _pokemon->get_experience();
            }

            PKMN_INLINE void set_experience(
                int experience
            )
            {
                _pokemon->set_experience(experience);
            }

            PKMN_INLINE int get_level()
            {
                return _pokemon->get_level();
            }

            PKMN_INLINE void set_level(
                int level
            )
            {
                _pokemon->set_level(level);
            }

            PKMN_INLINE EV_map& get_EVs()
            {
                return _EV_map;
            }

            PKMN_INLINE IV_map& get_IVs()
            {
                return _IV_map;
            }

            PKMN_INLINE marking_map& get_markings()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_markings();

                return _marking_map;
            }

            PKMN_INLINE ribbon_map& get_ribbons()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_ribbons();

                return _ribbon_map;
            }

            PKMN_INLINE contest_stat_map& get_contest_stats()
            {
                // To throw pkmn::unimplemented_error if appropriate.
                (void)_pokemon->get_contest_stats();

                return _contest_stat_map;
            }

            PKMN_INLINE move_slots2& get_moves()
            {
                return _move_slots;
            }

            // Stats are read-only, so no need to wrap.
            PKMN_INLINE const std::map<std::string, int>& get_stats()
            {
                return _pokemon->get_stats();
            }

            PKMN_INLINE std::string get_icon_filepath()
            {
                return _pokemon->get_icon_filepath();
            }

            PKMN_INLINE std::string get_sprite_filepath()
            {
                return _pokemon->get_sprite_filepath();
            }

            PKMN_INLINE int get_numeric_attribute(
                const std::string& attribute_name
            )
            {
                return _pokemon->get_numeric_attribute(attribute_name);
            }

            PKMN_INLINE void set_numeric_attribute(
                const std::string& attribute_name,
                int value
            )
            {
                _pokemon->set_numeric_attribute(attribute_name, value);
            }

            PKMN_INLINE std::vector<std::string> get_numeric_attribute_names()
            {
                return _pokemon->get_numeric_attribute_names();
            }

            PKMN_INLINE std::string get_string_attribute(
                const std::string& attribute_name
            )
            {
                return _pokemon->get_string_attribute(attribute_name);
            }

            PKMN_INLINE void set_string_attribute(
                const std::string& attribute_name,
                const std::string& value
            )
            {
                _pokemon->set_string_attribute(attribute_name, value);
            }

            PKMN_INLINE std::vector<std::string> get_string_attribute_names()
            {
                return _pokemon->get_string_attribute_names();
            }

            // TODO: ifdef for specific wrappers

            // For wrappers whose language doesn't support the == operator
            bool equals(const pokemon& rhs)
            {
                return operator==(rhs);
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
            move_slots2 _move_slots;

            void _init()
            {
                _EV_map = EV_map(_pokemon);
                _IV_map = IV_map(_pokemon);
                _marking_map = marking_map(_pokemon);
                _ribbon_map = ribbon_map(_pokemon);
                _contest_stat_map = contest_stat_map(_pokemon);
                _move_slots = move_slots2(_pokemon);
            }
    };

}}

#endif /* CPP_WRAPPERS_HPP */
