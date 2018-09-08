/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_POKEMON_SUBCLASS_HPP
#define PKMNTEST_POKEMON_SUBCLASS_HPP

#include <pkmntest/config.hpp>

#include <pkmn/pokemon.hpp>

namespace pkmntest
{
    class PKMNTEST_API pokemon_subclass: public pkmn::pokemon
    {
        public:
            pokemon_subclass(
                pkmn::e_species species,
                pkmn::e_game game
            );
            ~pokemon_subclass() = default;

            pokemon_subclass(const pokemon_subclass& other) = default;
            pokemon_subclass(pokemon_subclass&&) = default;

            pokemon_subclass& operator=(const pokemon_subclass&) = default;
            pokemon_subclass& operator=(pokemon_subclass&&) = default;

            pkmn::pokemon::sptr clone() const final;

            pkmn::pokemon::sptr to_game(pkmn::e_game game) final;

            void export_to_file(const std::string& filepath) final;

            pkmn::e_species get_species() const final;

            pkmn::e_game get_game() const final;

            std::string get_form() const final;

            void set_form(const std::string& form) final;

            bool is_egg() const final;

            void set_is_egg(bool is_egg) final;

            const pkmn::database::pokemon_entry& get_database_entry() const;

            pkmn::e_condition get_condition() const final;

            void set_condition(pkmn::e_condition condition) final;

            std::string get_nickname() const final;

            void set_nickname(const std::string& nickname) final;

            pkmn::e_gender get_gender() const final;

            void set_gender(pkmn::e_gender gender) final;

            bool is_shiny() const final;

            void set_shininess(bool is_shiny) final;

            pkmn::e_item get_held_item() const final;

            void set_held_item(pkmn::e_item held_item) final;

            pkmn::e_nature get_nature() const final;

            void set_nature(pkmn::e_nature nature) final;

            int get_pokerus_duration() const final;

            void set_pokerus_duration(int pokerus_duration) final;

            std::string get_original_trainer_name() const final;

            void set_original_trainer_name(const std::string& original_trainer_name) final;

            uint16_t get_original_trainer_public_id() const final;

            uint16_t get_original_trainer_secret_id() const final;

            uint32_t get_original_trainer_id() const final;

            void set_original_trainer_public_id(
                uint16_t original_trainer_public_id
            ) final;

            void set_original_trainer_secret_id(
                uint16_t original_trainer_secret_id
            ) final;

            void set_original_trainer_id(
                uint32_t original_trainer_id
            ) final;

            pkmn::e_gender get_original_trainer_gender() const;

            void set_original_trainer_gender(
                pkmn::e_gender original_trainer_gender
            );

            pkmn::e_language get_language() const final;

            void set_language(pkmn::e_language language) final;

            int get_current_trainer_friendship() const final;

            void set_current_trainer_friendship(
                int current_trainer_friendship
            ) final;

            pkmn::e_ability get_ability() const final;

            void set_ability(pkmn::e_ability ability) final;

            pkmn::e_ball get_ball() const final;

            void set_ball(pkmn::e_ball ball) final;

            int get_level_met() const final;

            void set_level_met(int level_met) final;

            std::string get_location_met(bool as_egg) const final;

            void set_location_met(
                const std::string& location,
                bool as_egg
            ) final;

            pkmn::e_game get_original_game() const final;

            void set_original_game(pkmn::e_game original_game) final;

            uint32_t get_personality() const final;

            void set_personality(uint32_t personality) final;

            int get_experience() const final;

            void set_experience(int experience) final;

            int get_level() const final;

            void set_level(int level) final;

            const std::map<pkmn::e_marking, bool>& get_markings() const final;

            void set_marking(
                pkmn::e_marking marking,
                bool value
            ) final;

            const std::map<std::string, bool>& get_ribbons() const final;

            void set_ribbon(
                const std::string& ribbon,
                bool value
            ) final;

            const std::map<pkmn::e_contest_stat, int>& get_contest_stats() const final;

            void set_contest_stat(
                pkmn::e_contest_stat stat,
                int value
            ) final;

            const pkmn::move_slots_t& get_moves() const final;

            void set_move(
                pkmn::e_move move,
                int index
            ) final;

            void set_move_pp(
                int index,
                int pp
            ) final;

            const std::map<pkmn::e_stat, int>& get_EVs() const final;

            void set_EV(
                pkmn::e_stat stat,
                int value
            ) final;

            const std::map<pkmn::e_stat, int>& get_IVs() const final;

            void set_IV(
                pkmn::e_stat stat,
                int value
            ) final;

            int get_current_hp() const final;

            void set_current_hp(int current_hp) final;

            const std::map<pkmn::e_stat, int>& get_stats() const final;

            std::string get_icon_filepath() const;

            std::string get_sprite_filepath() const;

            void* get_native() const;

        private:
            pkmn::database::pokemon_entry _database_entry;

            pkmn::e_species _species;
            pkmn::e_game _game;
            pkmn::e_condition _condition;
            pkmn::e_gender _gender;
            pkmn::e_item _held_item;
            pkmn::e_nature _nature;
            pkmn::e_gender _original_trainer_gender;
            pkmn::e_language _language;
            pkmn::e_ability _ability;
            pkmn::e_ball _ball;
            pkmn::e_game _original_game;
            bool _is_egg;
            bool _is_shiny;

            int _pokerus_duration;
            int _current_trainer_friendship;
            int _level_met;
            uint32_t _personality;
            int _experience;
            int _level;
            int _current_hp;

            std::string _nickname;
            std::string _original_trainer_name;
            std::string _location_met;
            std::string _location_met_as_egg;

            union
            {
                uint32_t id;
                union
                {
                    uint16_t public_id;
                    uint16_t secret_id;
                };
            } _original_trainer_id;

            std::map<pkmn::e_marking, bool> _markings;
            std::map<std::string, bool> _ribbons;
            std::map<pkmn::e_contest_stat, int> _contest_stats;
            std::map<pkmn::e_stat, int> _EVs;
            std::map<pkmn::e_stat, int> _IVs;
            std::map<pkmn::e_stat, int> _stats;

            pkmn::move_slots_t _moves;
    };
}

#endif /* PKMNTEST_POKEMON_SUBCLASS_HPP */
