/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_IMPL_HPP
#define PKMN_POKEMON_IMPL_HPP

#include "mem/scoped_lock.hpp"

#include <pkmn/pokemon.hpp>

#include <pksav/common/contest_stats.h>
#include <pksav/common/stats.h>

#include <boost/assign.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

#include <stdexcept>
#include <map>

static const std::map<std::string, pksav_battle_stat_t> pkmn_stats_to_pksav = boost::assign::map_list_of
    ("HP",              PKSAV_STAT_HP)
    ("Attack",          PKSAV_STAT_ATTACK)
    ("Defense",         PKSAV_STAT_DEFENSE)
    ("Speed",           PKSAV_STAT_SPEED)
    ("Special",         PKSAV_STAT_SPECIAL)
    ("Special Attack",  PKSAV_STAT_SPATK)
    ("Special Defense", PKSAV_STAT_SPDEF)
;

namespace pkmn {

    class pokemon_impl: public pokemon, public boost::noncopyable {
        public:
            pokemon_impl() {}
            pokemon_impl(
                int pokemon_index,
                int game_id
            );
            pokemon_impl(
                pkmn::database::pokemon_entry&& database_entry
            );

            virtual ~pokemon_impl() {}

            std::string get_species();

            std::string get_form();

            std::string get_game();

            const pkmn::database::pokemon_entry& get_database_entry();

            const pkmn::database::item_entry& get_held_item();

            const std::map<std::string, bool>& get_markings();

            const std::map<std::string, bool>& get_ribbons();

            const std::map<std::string, int>& get_contest_stats();

            const pkmn::move_slots_t& get_moves();

            const std::map<std::string, int>& get_EVs();

            const std::map<std::string, int>& get_IVs();

            const std::map<std::string, int>& get_stats();

            void* get_native_pc_data();

            void* get_native_party_data();

            typedef pkmn::mem::scoped_lock<pokemon_impl> pokemon_scoped_lock;
            friend pokemon_scoped_lock;

        protected:
            pkmn::move_slots_t _moves;
            std::map<std::string, int> _contest_stats, _stats, _EVs, _IVs;
            std::map<std::string, bool> _markings, _ribbons;

            pkmn::database::pokemon_entry _database_entry;
            pkmn::database::item_entry _held_item;

            int _generation;

            bool _our_pc_mem;
            bool _our_party_mem;

            void* _native_pc;
            void* _native_party;

            boost::mutex _mem_mutex;

            virtual void _calculate_stats() = 0;

            virtual void _update_moves(
                int index
            ) = 0;

            // Shared abstraction initializers

            void _init_gb_IV_map(
                const uint16_t* iv_data_ptr
            );

            void _init_modern_IV_map(
                const uint32_t* iv_data_ptr
            );

            void _init_contest_stat_map(
                const pksav_contest_stats_t* native_ptr
            );

            void _init_markings_map(
                const uint8_t* native_ptr
            );

            // Shared setters

            void _set_gb_IV(
                const std::string &stat,
                int value,
                uint16_t* iv_data_ptr
            );

            void _set_modern_IV(
                const std::string &stat,
                int value,
                uint32_t* iv_data_ptr
            );

            void _set_contest_stat(
                const std::string &stat,
                int value,
                pksav_contest_stats_t* native_ptr
            );

            void _set_marking(
                const std::string &marking,
                bool value,
                uint8_t* native_ptr
            );

            template <typename native_type, typename mask_type>
            void _set_ribbon(
                const std::string &ribbon,
                bool value,
                native_type* native,
                const std::map<std::string, mask_type> &masks
            ) {
                if(masks.find(ribbon) == masks.end()) {
                    throw std::invalid_argument("Invalid ribbon.");
                }

                if(value) {
                    *native |= native_type(masks.at(ribbon));
                } else {
                    *native &= ~native_type(masks.at(ribbon));
                }

                _update_ribbons_map();
            }

            virtual void _update_held_item() {}
            virtual void _update_ribbons_map() {}
            virtual void _update_EV_map() = 0;
            virtual void _update_stat_map() = 0;
    };

}

#endif /* PKMN_POKEMON_IMPL_HPP */
