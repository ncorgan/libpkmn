/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_GEN1IMPL_HPP
#define PKMN_POKEMON_GEN1IMPL_HPP

#include "pokemon_impl.hpp"

#include <pksav/gen1/pokemon.h>

namespace pkmn {

    class pokemon_gen1impl: public pokemon_impl {
        public:
            pokemon_gen1impl() {}
            pokemon_gen1impl(
                int pokemon_index, int game_id,
                int move1_id, int move2_id,
                int move3_id, int move4_id
            );
            pokemon_gen1impl(
                pksav_gen1_pc_pokemon_t* pc,
                int game_id
            );
            pokemon_gen1impl(
                pksav_gen1_party_pokemon_t* party,
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

            void _update_EV_map();
            void _update_IV_map();
            void _update_stat_map();

            std::string _nickname, _trainer_name;
    };

}

#endif /* PKMN_POKEMON_GEN1IMPL_HPP */
