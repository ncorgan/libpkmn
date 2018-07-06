/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_GEN2IMPL_HPP
#define PKMN_POKEMON_GEN2IMPL_HPP

#include "pokemon_impl.hpp"

#include <pksav/gen2/pokemon.h>

#include <boost/config.hpp>

namespace pkmn {

    BOOST_STATIC_CONSTEXPR uint8_t GEN2_EGG_ID = 0xFD;

    class pokemon_gen2impl: public pokemon_impl {
        public:
            pokemon_gen2impl() {}
            pokemon_gen2impl(
                pkmn::database::pokemon_entry&& database_entry,
                int level
            );
            pokemon_gen2impl(
                const struct pksav_gen2_pc_pokemon* p_pc_pokemon,
                int game_id
            );
            pokemon_gen2impl(
                const struct pksav_gen2_party_pokemon* p_party_pokemon,
                int game_id
            );

            // TODO
            pokemon_gen2impl(const pokemon_gen2impl&) = delete;
            pokemon_gen2impl(pokemon_gen2impl&&) = delete;

            pokemon_gen2impl& operator=(const pokemon_gen2impl&) = delete;
            pokemon_gen2impl& operator=(pokemon_gen2impl&&) = delete;

            ~pokemon_gen2impl() = default;

            pokemon::sptr to_game(
                const std::string& game
            ) final;

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

            std::string get_gender() final;

            void set_gender(
                const std::string& gender
            ) final;

            bool is_shiny() final;

            void set_shininess(
                bool value
            ) final;

            std::string get_held_item() final;

            void set_held_item(
                const std::string& held_item
            ) final;

            std::string get_nature() final;

            void set_nature(
                const std::string& nature
            ) final;

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

            std::string get_original_trainer_gender() final;

            void set_original_trainer_gender(
                const std::string& gender
            ) final;

            std::string get_language() final;

            void set_language(
                const std::string& language
            ) final;

            int get_current_trainer_friendship() final;

            void set_current_trainer_friendship(
                int friendship
            ) final;

            std::string get_ability() final;

            void set_ability(
                const std::string& ability
            ) final;

            std::string get_ball() final;

            void set_ball(
                const std::string& ball
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

            std::string get_original_game() final;

            void set_original_game(
                const std::string& game
            ) final;

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
                const std::string& stat,
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
                const std::string& move,
                int index
            ) final;

            void set_move_pp(
                int index,
                int pp
            ) final;

            void set_EV(
                const std::string& stat,
                int value
            ) final;

            int get_current_hp() final;

            void set_current_hp(
                int hp
            ) final;

        private:
            void _populate_party_data() final;

            void _update_moves(
                int index
            ) final;

            void _update_EV_map() final;
            void _update_stat_map() final;

            void _set_unown_form_from_IVs();
            void _set_unown_IVs_from_form(
                const std::string& form
            );

            struct pksav_gen2_party_pokemon _pksav_pokemon;

            std::string _nickname, _trainer_name;

            bool _is_egg;
    };

}

#endif /* PKMN_POKEMON_GEN2IMPL_HPP */
