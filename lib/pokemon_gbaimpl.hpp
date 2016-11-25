/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
                int pokemon_index, int game_id,
                int move1_id, int move2_id,
                int move3_id, int move4_id,
                int level
            );
            pokemon_gbaimpl(
                pksav_gba_pc_pokemon_t* pc,
                int game_id
            );
            pokemon_gbaimpl(
                pksav_gba_party_pokemon_t* party,
                int game_id
            );

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

            std::string get_location_caught();

            void set_location_caught(
                const std::string &location
            );

            int get_experience();

            void set_experience(
                int experience
            );

            int get_level();

            void set_level(
                int level
            );

        private:
            void _calculate_stats();

            void _update_moves(
                int index
            );

            void _update_markings_map();
            void _update_ribbons_map();
            void _update_EV_map();
            void _update_IV_map();
            void _update_stat_map();

            pksav_gba_pokemon_growth_t*  _growth;
            pksav_gba_pokemon_attacks_t* _attacks;
            pksav_gba_pokemon_effort_t*  _effort;
            pksav_gba_pokemon_misc_t*    _misc;
    };

}

#endif /* PKMN_POKEMON_GBAIMPL_HPP */
