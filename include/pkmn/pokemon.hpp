/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_HPP
#define PKMN_POKEMON_HPP

#include <pkmn/config.hpp>
#include <pkmn/move_slot.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/types/class_with_attributes.hpp>
#include <pkmn/types/shared_ptr.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(disable: 4275) // An exported class was derived from a class that was not exported.
#endif

namespace pkmn {

    class PKMN_API pokemon: public class_with_attributes
    {
        public:
            typedef pkmn::shared_ptr<pokemon> sptr;

            #ifndef __DOXYGEN__
            pokemon(): class_with_attributes() {}
            virtual ~pokemon () {}
            #endif

            static sptr make(
                const std::string &species,
                const std::string &game,
                const std::string &form,
                int level
            );

            static sptr from_file(
                const std::string &filepath
            );

            static const uint32_t DEFAULT_TRAINER_ID;

            static const std::string DEFAULT_TRAINER_NAME;

            virtual sptr to_game(
                const std::string& game
            ) = 0;

            virtual void export_to_file(
                const std::string& filepath
            ) = 0;

            virtual std::string get_species() = 0;

            virtual std::string get_game() = 0;

            virtual std::string get_form() = 0;

            virtual void set_form(
                const std::string &form
            ) = 0;

            virtual bool is_egg() = 0;

            virtual void set_is_egg(
                bool is_egg
            ) = 0;

            virtual const pkmn::database::pokemon_entry& get_database_entry() = 0;

            virtual std::string get_condition() = 0;

            virtual void set_condition(
                const std::string& condition
            ) = 0;

            virtual std::string get_nickname() = 0;

            virtual void set_nickname(
                const std::string &nickname
            ) = 0;

            virtual std::string get_gender() = 0;

            virtual void set_gender(
                const std::string &gender
            ) = 0;

            virtual bool is_shiny() = 0;

            virtual void set_shininess(
                bool value
            ) = 0;

            virtual std::string get_held_item() = 0;

            virtual void set_held_item(
                const std::string &held_item
            ) = 0;

            virtual int get_pokerus_duration() = 0;

            virtual void set_pokerus_duration(
                int diruation
            ) = 0;

            virtual std::string get_original_trainer_name() = 0;

            virtual void set_original_trainer_name(
                const std::string &trainer_name
            ) = 0;

            virtual uint16_t get_original_trainer_public_id() = 0;

            virtual uint16_t get_original_trainer_secret_id() = 0;

            virtual uint32_t get_original_trainer_id() = 0;

            virtual void set_original_trainer_public_id(
                uint16_t public_id
            ) = 0;

            virtual void set_original_trainer_secret_id(
                uint16_t secret_id
            ) = 0;

            virtual void set_original_trainer_id(
                uint32_t id
            ) = 0;

            virtual std::string get_original_trainer_gender() = 0;

            virtual void set_original_trainer_gender(
                const std::string &trainer_gender
            ) = 0;

            virtual int get_current_trainer_friendship() = 0;

            virtual void set_current_trainer_friendship(
                int friendship
            ) = 0;

            virtual std::string get_ability() = 0;

            virtual void set_ability(
                const std::string &ability
            ) = 0;

            virtual std::string get_ball() = 0;

            virtual void set_ball(
                const std::string &ball
            ) = 0;

            virtual int get_level_met() = 0;

            virtual void set_level_met(
                int level
            ) = 0;

            virtual std::string get_location_met(
                bool as_egg
            ) = 0;

            virtual void set_location_met(
                const std::string &location,
                bool as_egg
            ) = 0;

            virtual std::string get_original_game() = 0;

            virtual void set_original_game(
                const std::string &game
            ) = 0;

            virtual uint32_t get_personality() = 0;

            virtual void set_personality(
                uint32_t personality
            ) = 0;

            virtual int get_experience() = 0;

            virtual void set_experience(
                int experience
            ) = 0;

            virtual int get_level() = 0;

            virtual void set_level(
                int level
            ) = 0;

            virtual const std::map<std::string, bool>& get_markings() = 0;

            virtual void set_marking(
                const std::string &marking,
                bool value
            ) = 0;

            virtual const std::map<std::string, bool>& get_ribbons() = 0;

            virtual void set_ribbon(
                const std::string &ribbon,
                bool value
            ) = 0;

            virtual const std::map<std::string, int>& get_contest_stats() = 0;

            virtual void set_contest_stat(
                const std::string &stat,
                int value
            ) = 0;

            virtual const pkmn::move_slots_t& get_moves() = 0;

            virtual void set_move(
                const std::string &move,
                int index
            ) = 0;

            virtual void set_move_pp(
                int index,
                int pp
            ) = 0;

            virtual const std::map<std::string, int>& get_EVs() = 0;

            virtual void set_EV(
                const std::string &stat,
                int value
            ) = 0;

            virtual const std::map<std::string, int>& get_IVs() = 0;

            virtual void set_IV(
                const std::string &stat,
                int value
            ) = 0;

            virtual int get_current_hp() = 0;

            virtual void set_current_hp(
                int hp
            ) = 0;

            virtual const std::map<std::string, int>& get_stats() = 0;

            virtual std::string get_icon_filepath() = 0;

            virtual std::string get_sprite_filepath() = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native_pc_data() = 0;
            virtual void* get_native_party_data() = 0;
            #endif
    };

    typedef std::vector<pokemon::sptr> pokemon_list_t;
}

#ifdef PKMN_PLATFORM_WIN32
#    pragma warning(default: 4275)
#endif

#endif /* PKMN_POKEMON_HPP */
