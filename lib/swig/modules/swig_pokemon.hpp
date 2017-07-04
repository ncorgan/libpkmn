/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef SWIG_POKEMON_HPP
#define SWIG_POKEMON_HPP

#include <pkmn/config.hpp>

#include <pkmn/pokemon.hpp>

#include "swig_map_wrappers.hpp"

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
            pokemon(
                const pkmn::pokemon::sptr& cpp_pokemon
            ): _pokemon(cpp_pokemon)
            {
                _EV_map = pokemon_EV_map(_pokemon);
                _IV_map = pokemon_IV_map(_pokemon);
            }

            pokemon(
                const std::string& species,
                const std::string& game,
                const std::string& form,
                int level
            ): _pokemon(pkmn::pokemon::make(species, game, form, level))
            {
                _EV_map = pokemon_EV_map(_pokemon);
                _IV_map = pokemon_IV_map(_pokemon);
            }

            pokemon(
                const std::string& filepath
            ): _pokemon(pkmn::pokemon::from_file(filepath))
            {
                _EV_map = pokemon_EV_map(_pokemon);
                _IV_map = pokemon_IV_map(_pokemon);
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

            PKMN_INLINE std::string get_trainer_name()
            {
                return _pokemon->get_trainer_name();
            }

            PKMN_INLINE void set_trainer_name(
                const std::string& trainer_name
            )
            {
                _pokemon->set_trainer_name(trainer_name);
            }

            PKMN_INLINE uint16_t get_trainer_public_id()
            {
                return _pokemon->get_trainer_public_id();
            }

            PKMN_INLINE uint16_t get_trainer_secret_id()
            {
                return _pokemon->get_trainer_secret_id();
            }

            PKMN_INLINE uint32_t get_trainer_id()
            {
                return _pokemon->get_trainer_id();
            }

            PKMN_INLINE void set_trainer_public_id(
                uint16_t public_id
            )
            {
                _pokemon->set_trainer_public_id(public_id);
            }

            PKMN_INLINE void set_trainer_secret_id(
                uint16_t secret_id
            )
            {
                _pokemon->set_trainer_secret_id(secret_id);
            }

            PKMN_INLINE void set_trainer_id(
                uint32_t public_id
            )
            {
                _pokemon->set_trainer_id(public_id);
            }

            PKMN_INLINE std::string get_trainer_gender()
            {
                return _pokemon->get_trainer_gender();
            }

            PKMN_INLINE void set_trainer_gender(
                const std::string& trainer_gender
            )
            {
                _pokemon->set_trainer_gender(trainer_gender);
            }

            PKMN_INLINE int get_friendship()
            {
                return _pokemon->get_friendship();
            }

            PKMN_INLINE void set_friendship(
                int friendship
            )
            {
                _pokemon->set_friendship(friendship);
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

            // TODO: markings map
            // TODO: ribbons map
            // TODO: contest stats map
            // TODO: move slots

            PKMN_INLINE pokemon_EV_map get_EVs()
            {
                return _EV_map;
            }

            PKMN_INLINE pokemon_IV_map get_IVs()
            {
                return _IV_map;
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

        private:
            pkmn::pokemon::sptr _pokemon;
            pokemon_EV_map _EV_map;
            pokemon_IV_map _IV_map;
    };

}}

#endif /* SWIG_POKEMON_HPP */
