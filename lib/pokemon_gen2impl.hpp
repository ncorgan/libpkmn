/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_GEN2IMPL_HPP
#define PKMN_POKEMON_GEN2IMPL_HPP

#include "pokemon_impl.hpp"

#include <pksav/gen2/pokemon.h>

namespace pkmn {

    class pokemon_gen2impl: public pokemon_impl {
        public:
            pokemon_gen2impl() {}
            pokemon_gen2impl(
                int pokemon_index,
                int game_id,
                int level
            );
            pokemon_gen2impl(
                pksav_gen2_pc_pokemon_t* pc,
                int game_id
            );
            pokemon_gen2impl(
                pksav_gen2_party_pokemon_t* party,
                int game_id
            );

            ~pokemon_gen2impl();

            std::string get_nickname();

            void set_nickname(
                const std::string &nickname
            );

            std::string get_trainer_name();

            void set_trainer_name(
                const std::string &trainer_name
            );

            uint16_t get_trainer_public_id();

            uint16_t get_trainer_secret_id();

            uint32_t get_trainer_id();

            void set_trainer_public_id(
                uint16_t public_id
            );

            void set_trainer_secret_id(
                uint16_t secret_id
            );

            void set_trainer_id(
                uint32_t id
            );

            std::string get_trainer_gender();

            void set_trainer_gender(
                const std::string &gender
            );

            std::string get_ability();

            void set_ability(
                const std::string &ability
            );

            std::string get_ball();

            void set_ball(
                const std::string &ball
            );

            std::string get_location_caught();

            void set_location_caught(
                const std::string &location
            );

            std::string get_original_game();

            void set_original_game(
                const std::string &game
            );

            uint32_t get_personality();

            void set_personality(
                uint32_t personality
            );

            int get_experience();

            void set_experience(
                int experience
            );

            int get_level();

            void set_level(
                int level
            );

            void set_marking(
                const std::string &marking,
                bool value
            );

            void set_ribbon(
                const std::string &ribbon,
                bool value
            );

            void set_move(
                const std::string &move,
                int index
            );

        private:
            void _calculate_stats();

            void _update_moves(
                int index
            );

            void _update_held_item();
            void _update_markings_map();
            void _update_ribbons_map();
            void _update_EV_map();
            void _update_IV_map();
            void _update_stat_map();

            std::string _nickname, _trainer_name;
    };

}

#endif /* PKMN_POKEMON_GEN2IMPL_HPP */
