/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_GBAIMPL_HPP
#define PKMN_POKEMON_GBAIMPL_HPP

#include "pokemon_impl.hpp"

#include <pksav/gba/pokemon.h>

namespace pkmn {

    class pokemon_gbaimpl: public pokemon_impl {
        public:
            pokemon_gbaimpl() {}
            pokemon_gbaimpl(
                pkmn::database::pokemon_entry&& database_entry,
                int level
            );
            pokemon_gbaimpl(
                const struct pksav_gba_pc_pokemon* p_pc_pokemon,
                int game_id
            );
            pokemon_gbaimpl(
                const struct pksav_gba_party_pokemon* p_party_pokemon,
                int game_id
            );

            // TODO
            pokemon_gbaimpl(const pokemon_gbaimpl&) = delete;
            pokemon_gbaimpl(pokemon_gbaimpl&&) = delete;

            pokemon_gbaimpl& operator=(const pokemon_gbaimpl&) = delete;
            pokemon_gbaimpl& operator=(pokemon_gbaimpl&&) = delete;

            ~pokemon_gbaimpl() = default;

            pokemon::sptr to_game(pkmn::e_game game) final;

            void export_to_file(
                const std::string& filepath
            ) final;

            void set_form(
                const std::string& form
            ) final;

            bool is_egg() final;

            void set_is_egg(
                bool is_egg
            ) final;

            std::string get_condition() final;

            void set_condition(
                const std::string& condition
            ) final;

            std::string get_nickname() final;

            void set_nickname(
                const std::string& nickname
            ) final;

            pkmn::e_gender get_gender() final;

            void set_gender(
                pkmn::e_gender gender
            ) final;

            bool is_shiny() final;

            void set_shininess(
                bool value
            ) final;

            pkmn::e_item get_held_item() final;

            void set_held_item(
                pkmn::e_item held_item
            ) final;

            pkmn::e_nature get_nature() final;

            void set_nature(pkmn::e_nature nature) final;

            int get_pokerus_duration() final;

            void set_pokerus_duration(
                int duration
            ) final;

            std::string get_original_trainer_name() final;

            void set_original_trainer_name(
                const std::string& trainer_name
            ) final;

            uint16_t get_original_trainer_public_id() final;

            uint16_t get_original_trainer_secret_id() final;

            uint32_t get_original_trainer_id() final;

            void set_original_trainer_public_id(
                uint16_t public_id
            ) final;

            void set_original_trainer_secret_id(
                uint16_t secret_id
            ) final;

            void set_original_trainer_id(
                uint32_t id
            ) final;

            pkmn::e_gender get_original_trainer_gender() final;

            void set_original_trainer_gender(
                pkmn::e_gender gender
            ) final;

            pkmn::e_language get_language() final;

            void set_language(
                pkmn::e_language language
            ) final;

            int get_current_trainer_friendship() final;

            void set_current_trainer_friendship(
                int friendship
            ) final;

            pkmn::e_ability get_ability() final;

            void set_ability(pkmn::e_ability ability) final;

            pkmn::e_ball get_ball() final;

            void set_ball(
                pkmn::e_ball ball
            ) final;

            int get_level_met() final;

            void set_level_met(
                int level
            ) final;

            std::string get_location_met(
                bool as_egg
            ) final;

            void set_location_met(
                const std::string& location,
                bool as_egg
            ) final;

            pkmn::e_game get_original_game() final;

            void set_original_game(pkmn::e_game game) final;

            uint32_t get_personality() final;

            void set_personality(
                uint32_t personality
            ) final;

            int get_experience() final;

            void set_experience(
                int experience
            ) final;

            int get_level() final;

            void set_level(
                int level
            ) final;

            void set_IV(
                pkmn::e_stat stat,
                int value
            ) final;

            void set_marking(
                const std::string& marking,
                bool value
            ) final;

            void set_ribbon(
                const std::string& ribbon,
                bool value
            ) final;

            void set_contest_stat(
                const std::string& stat,
                int value
            ) final;

            void set_move(
                pkmn::e_move move,
                int index
            ) final;

            void set_move_pp(
                int index,
                int pp
            ) final;

            void set_EV(
                pkmn::e_stat stat,
                int value
            ) final;

            int get_current_hp() final;

            void set_current_hp(
                int hp
            ) final;

            std::string get_sprite_filepath() final;

            void* get_native_pc_data() final;

            // Functions for attributes

            bool get_is_obedient();

            void set_is_obedient(bool is_obedient);

        private:

            void _set_contest_ribbon(
                const std::string& ribbon,
                bool value
            );

            void _populate_party_data() final;

            void _update_moves(int index) final;

            void _set_ability(pkmn::e_ability ability) final;

            void _set_unown_form_from_personality();
            void _set_unown_personality_from_form();

            void _update_markings_map() final;
            void _update_ribbons_map() final;
            void _update_EV_map() final;
            void _update_stat_map() final;

            void _register_attributes();

            struct pksav_gba_party_pokemon          _pksav_pokemon;
            struct pksav_gba_pokemon_growth_block*  _p_growth_block;
            struct pksav_gba_pokemon_attacks_block* _p_attacks_block;
            struct pksav_gba_pokemon_effort_block*  _p_effort_block;
            struct pksav_gba_pokemon_misc_block*    _p_misc_block;
    };

}

#endif /* PKMN_POKEMON_GBAIMPL_HPP */
