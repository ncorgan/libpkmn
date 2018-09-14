/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_IMPL_HPP
#define PKMN_POKEMON_IMPL_HPP

#include "types/mutex_helpers.hpp"

#include <pkmn/pokemon.hpp>

#include <pksav/common/contest_stats.h>
#include <pksav/common/stats.h>

#include <boost/thread/recursive_mutex.hpp>

#include <stdexcept>
#include <unordered_map>

namespace pkmn {

    // Forward declarations
    class pokemon_impl;
    class pokemon_box_impl;
    class pokemon_party_impl;

    class pokemon_impl: public pokemon,
                        public pkmn::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            pokemon_impl(
                int pokemon_index,
                int game_id
            );
            explicit pokemon_impl(
                pkmn::database::pokemon_entry&& database_entry
            );

            pokemon_impl(const pokemon_impl&);
            pokemon_impl(pokemon_impl&&) = default;

            pokemon_impl& operator=(const pokemon_impl&);
            pokemon_impl& operator=(pokemon_impl&&) = default;

            virtual ~pokemon_impl() = default;

            pkmn::e_species get_species() const final;

            std::string get_form() const final;

            pkmn::e_game get_game() const final;

            const pkmn::database::pokemon_entry& get_database_entry() const final;

            const std::map<pkmn::e_marking, bool>& get_markings() const final;

            const std::map<std::string, bool>& get_ribbons() const final;

            const std::map<pkmn::e_contest_stat, int>& get_contest_stats() const final;

            const pkmn::move_slots_t& get_moves() const final;

            const std::map<pkmn::e_stat, int>& get_EVs() const final;

            const std::map<pkmn::e_stat, int>& get_IVs() const final;

            const std::map<pkmn::e_stat, int>& get_stats() const final;

            virtual std::string get_icon_filepath() const override;

            virtual std::string get_sprite_filepath() const override;

            // Make the box implementations friend classes so they can access the internals.
            friend class pokemon_box_impl;
            friend class pokemon_box_gbaimpl;
            friend class pokemon_box_gcnimpl;

            template
            <typename list_type,
             typename pksav_pc_pokemon_type,
             typename libpkmn_pokemon_type>
            friend class pokemon_box_gbimpl;

            // Make the party implementations friend classes so they can access the internals.
            friend class pokemon_party_impl;
            friend class pokemon_party_gbaimpl;
            friend class pokemon_party_gcnimpl;

            template
            <typename list_type,
             typename pksav_pc_pokemon_type,
             typename pksav_pokemon_party_data_type,
             typename pksav_party_pokemon_type,
             typename libpkmn_pokemon_type>
            friend class pokemon_party_gbimpl;

            // Make the daycare implementations friend classes so they can access the internals.
            friend class daycare_impl;
            friend class daycare_gen1impl;
            friend class daycare_gen2impl;
            friend class daycare_gcnimpl;

        protected:
            int _generation;

            virtual void _populate_party_data() = 0;

            virtual void _update_moves(
                int index
            ) = 0;

            // Shared abstraction initializers

            void _init_default_moves_for_level();

            void _init_gb_IV_map(
                const uint16_t* iv_data_ptr
            );

            void _init_modern_IV_map(
                const uint32_t* iv_data_ptr
            );

            void _init_contest_stat_map(
                const struct pksav_contest_stats* p_native
            );

            void _init_markings_map(
                const uint8_t* native_ptr
            );

            // Shared setters

            int _get_pokerus_duration(
                const uint8_t* pokerus_ptr
            ) const;

            void _set_pokerus_duration(
                uint8_t* pokerus_ptr,
                int duration
            );

            void _set_modern_gender(
                uint32_t* personality_ptr,
                pkmn::e_gender gender
            );

            void _set_modern_shininess(
                uint32_t* personality_ptr,
                const uint32_t* trainer_id_ptr,
                bool value
            );

            void _set_gb_IV(
                pkmn::e_stat stat,
                int value,
                uint16_t* iv_data_ptr
            );

            void _set_modern_IV(
                pkmn::e_stat stat,
                int value,
                uint32_t* iv_data_ptr
            );

            void _set_contest_stat(
                pkmn::e_contest_stat stat,
                int value,
                struct pksav_contest_stats* p_native
            );

            void _set_marking(
                pkmn::e_marking marking,
                bool value,
                uint8_t* p_native
            );

            template <typename native_type, typename mask_type>
            void _set_ribbon(
                const std::string& ribbon,
                bool value,
                native_type* native,
                const std::unordered_map<std::string, mask_type> &masks
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

            // Set without applying policy
            virtual void _set_ability(pkmn::e_ability) {}
            void _set_ability_from_personality();

            virtual void _update_held_item() {}
            virtual void _update_markings_map() {}
            virtual void _update_ribbons_map() {}
            virtual void _update_EV_map() = 0;
            virtual void _update_stat_map() = 0;
    };

}

#endif /* PKMN_POKEMON_IMPL_HPP */
