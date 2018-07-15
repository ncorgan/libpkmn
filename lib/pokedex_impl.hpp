/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEDEX_IMPL_HPP
#define PKMN_POKEDEX_IMPL_HPP

#include <pkmn/pokedex.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn
{
    class pokedex_impl: public pokedex,
                        public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokedex_impl() {}
            pokedex_impl(
                int game_id
            );

            virtual ~pokedex_impl() {};

            pkmn::e_game get_game() final;

            void set_has_seen(
                pkmn::e_species species,
                bool has_seen_value
            ) final;

            const std::vector<pkmn::e_species>& get_all_seen() final;

            int get_num_seen() final;

            void set_has_caught(
                pkmn::e_species species,
                bool has_caught_value
            ) final;

            const std::vector<pkmn::e_species>& get_all_caught() final;

            int get_num_caught() final;

            void* get_native() final;

        protected:
            int _game_id;
            int _generation;
            size_t _num_pokemon;

            std::vector<pkmn::e_species> _all_seen;
            bool _dirty_seen;

            std::vector<pkmn::e_species> _all_caught;
            bool _dirty_caught;

            void* _p_native;

            void _update_member_vector_with_pksav(
                const uint8_t* native_list,
                std::vector<pkmn::e_species>& member_vector
            );

            virtual void _set_has_seen(
                int species_id,
                bool has_seen_value
            ) = 0;
            virtual void _set_has_caught(
                int species_id,
                bool has_caught_value
            ) = 0;

            virtual void _update_all_seen() = 0;
            virtual void _update_all_caught() = 0;
    };
}

#endif /* PKMN_POKEDEX_IMPL_HPP */
