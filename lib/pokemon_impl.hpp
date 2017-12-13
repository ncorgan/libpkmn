/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_IMPL_HPP
#define PKMN_POKEMON_IMPL_HPP

#include <pkmn/pokemon.hpp>

#include <pksav/common/contest_stats.h>
#include <pksav/common/stats.h>

#include <boost/assign.hpp>
#include <boost/noncopyable.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <stdexcept>
#include <unordered_map>

static const std::unordered_map<std::string, pksav_battle_stat_t> pkmn_stats_to_pksav = boost::assign::map_list_of
    ("HP",              PKSAV_STAT_HP)
    ("Attack",          PKSAV_STAT_ATTACK)
    ("Defense",         PKSAV_STAT_DEFENSE)
    ("Speed",           PKSAV_STAT_SPEED)
    ("Special",         PKSAV_STAT_SPECIAL)
    ("Special Attack",  PKSAV_STAT_SPATK)
    ("Special Defense", PKSAV_STAT_SPDEF)
;

namespace pkmn {

    // Forward declarations
    class pokemon_impl;
    class pokemon_box_impl;
    class pokemon_party_impl;

    class pokemon_impl: public pokemon,
                        public boost::noncopyable,
                        public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokemon_impl() {}
            pokemon_impl(
                int pokemon_index,
                int game_id
            );
            explicit pokemon_impl(
                pkmn::database::pokemon_entry&& database_entry
            );

            virtual ~pokemon_impl() {}

            std::string get_species() override final;

            std::string get_form() override final;

            std::string get_game() override final;

            const pkmn::database::pokemon_entry& get_database_entry() override final;

            std::string get_current_trainer_name() override;

            void set_current_trainer_name(
                const std::string& trainer_name
            ) override;

            std::string get_current_trainer_gender() override;

            void set_current_trainer_gender(
                const std::string& trainer_gender
            ) override;

            int get_original_trainer_friendship() override;

            void set_original_trainer_friendship(
                int friendship
            ) override;

            const std::map<std::string, bool>& get_markings() override final;

            const std::map<std::string, bool>& get_ribbons() override final;

            virtual void set_super_training_medal(
                const std::string& super_training_medal,
                bool value
            ) override;

            const std::map<std::string, bool>& get_super_training_medals() override final;

            const std::map<std::string, int>& get_contest_stats() override final;

            const pkmn::move_slots_t& get_moves() override final;

            const std::map<std::string, int>& get_EVs() override final;

            const std::map<std::string, int>& get_IVs() override final;

            const std::map<std::string, int>& get_stats() override final;

            virtual std::string get_icon_filepath() override;

            virtual std::string get_sprite_filepath() override;

            void* get_native_pc_data() override final;

            void* get_native_party_data() override final;

            // Make the box implementations friend classes so they can access the internals.
            friend class pokemon_box_impl;
            friend class pokemon_box_gbaimpl;
            friend class pokemon_box_gcnimpl;
            friend class pokemon_box_gen6impl;

            template
            <typename list_type,
             typename pksav_pc_pokemon_type,
             typename pksav_pokemon_party_data_type,
             typename libpkmn_pokemon_type>
            friend class pokemon_box_gbimpl;

            // Make the party implementations friend classes so they can access the internals.
            friend class pokemon_party_impl;
            friend class pokemon_party_gbaimpl;
            friend class pokemon_party_gcnimpl;
            friend class pokemon_party_gen6impl;

            template
            <typename list_type,
             typename pksav_pc_pokemon_type,
             typename pksav_pokemon_party_data_type,
             typename pksav_party_pokemon_type,
             typename libpkmn_pokemon_type>
            friend class pokemon_party_gbimpl;

        protected:
            pkmn::move_slots_t _moves;
            std::map<std::string, int> _contest_stats, _stats, _EVs, _IVs;
            std::map<std::string, bool> _markings, _ribbons, _super_training_medals;

            pkmn::database::pokemon_entry _database_entry;

            int _generation;

            bool _our_pc_mem;
            bool _our_party_mem;

            void* _native_pc;
            void* _native_party;

            boost::recursive_mutex _mem_mutex;

            virtual void _populate_party_data() = 0;

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

            void _set_nature(
                uint32_t* personality_ptr,
                const std::string &nature
            );

            int _get_pokerus_duration(
                uint8_t* pokerus_ptr
            );

            void _set_pokerus_duration(
                uint8_t* pokerus_ptr,
                int duration
            );

            void _set_modern_gender(
                uint32_t* personality_ptr,
                const std::string &gender
            );

            void _set_modern_shininess(
                uint32_t* personality_ptr,
                bool value
            );

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

            template <typename native_type, typename map_type>
            void _set_ribbon(
                const std::string &ribbon,
                bool value,
                native_type* native,
                const map_type& masks
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

            // Functions for attributes

            std::string get_gen4_encounter_type(
                uint8_t* native_ptr
            );

            void set_gen4_encounter_type(
                uint8_t* native_ptr,
                const std::string& gen4_encounter_type
            );

            std::string get_region(
                uint8_t* native_ptr
            );

            void set_region(
                uint8_t* native_ptr,
                const std::string& region
            );

            virtual void _update_held_item() {}
            virtual void _update_markings_map() {}
            virtual void _update_ribbons_map() {}
            virtual void _update_EV_map() = 0;
            virtual void _update_stat_map() = 0;
    };

}

#endif /* PKMN_POKEMON_IMPL_HPP */
