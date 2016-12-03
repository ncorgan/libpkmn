/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_HPP
#define PKMN_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/move_slot.hpp>
#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace pkmn {

    class PKMN_API pokemon {
        public:
            typedef pkmn::shared_ptr<pokemon> sptr;

            #ifndef __DOXYGEN__
            pokemon() {}
            virtual ~pokemon () {}
            #endif

            static sptr make(
                const std::string &species,
                const std::string &game,
                const std::string &form,
                int level
            );

            static PKMN_CONSTEXPR uint32_t LIBPKMN_OT_ID = 2105214279;

            static const std::string LIBPKMN_OT_NAME;

            virtual std::string get_species() = 0;

            virtual std::string get_form() = 0;

            virtual std::string get_game() = 0;

            virtual const pkmn::database::pokemon_entry& get_database_entry() = 0;

            virtual std::string get_nickname() = 0;

            virtual void set_nickname(
                const std::string &nickname
            ) = 0;

            virtual const pkmn::database::item_entry& get_held_item() = 0;

            virtual std::string get_trainer_name() = 0;

            virtual void set_trainer_name(
                const std::string &trainer_name
            ) = 0;

            virtual uint16_t get_trainer_public_id() = 0;

            virtual uint16_t get_trainer_secret_id() = 0;

            virtual uint32_t get_trainer_id() = 0;

            virtual void set_trainer_public_id(
                uint16_t public_id
            ) = 0;

            virtual void set_trainer_secret_id(
                uint16_t secret_id
            ) = 0;

            virtual void set_trainer_id(
                uint32_t id
            ) = 0;

            virtual std::string get_trainer_gender() = 0;

            virtual void set_trainer_gender(
                const std::string &trainer_gender
            ) = 0;

            virtual std::string get_ability() = 0;

            virtual std::string get_ball() = 0;

            virtual std::string get_location_caught() = 0;

            virtual void set_location_caught(
                const std::string &location
            ) = 0;

            virtual std::string get_original_game() = 0;

            virtual uint32_t get_personality() = 0;

            virtual int get_experience() = 0;

            virtual void set_experience(
                int experience
            ) = 0;

            virtual int get_level() = 0;

            virtual void set_level(
                int level
            ) = 0;

            virtual const std::map<std::string, bool>& get_markings() = 0;

            virtual const std::map<std::string, bool>& get_ribbons() = 0;

            virtual const pkmn::move_slots_t& get_moves() = 0;

            virtual const std::map<std::string, int>& get_EVs() = 0;

            virtual const std::map<std::string, int>& get_IVs() = 0;

            virtual const std::map<std::string, int>& get_stats() = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native_pc_data() = 0;
            virtual void* get_native_party_data() = 0;
            #endif
    };

    typedef std::vector<pokemon::sptr> pokemon_list_t;
}

#endif /* PKMN_POKEMON_HPP */
