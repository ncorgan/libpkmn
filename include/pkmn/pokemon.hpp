/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
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

#include <pkmn/enums/ability.hpp>
#include <pkmn/enums/ball.hpp>
#include <pkmn/enums/condition.hpp>
#include <pkmn/enums/contest_stat.hpp>
#include <pkmn/enums/game.hpp>
#include <pkmn/enums/gender.hpp>
#include <pkmn/enums/item.hpp>
#include <pkmn/enums/language.hpp>
#include <pkmn/enums/marking.hpp>
#include <pkmn/enums/move.hpp>
#include <pkmn/enums/nature.hpp>
#include <pkmn/enums/species.hpp>
#include <pkmn/enums/stat.hpp>

#include <cstdint>
#include <memory>
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
            typedef std::shared_ptr<pokemon> sptr;

            static sptr make(
                pkmn::e_species species,
                pkmn::e_game game,
                const std::string& form,
                int level
            );

            static sptr from_file(
                const std::string& filepath
            );

            static const uint32_t DEFAULT_TRAINER_ID;

            static const std::string DEFAULT_TRAINER_NAME;

            virtual sptr to_game(pkmn::e_game game) = 0;

            virtual void export_to_file(
                const std::string& filepath
            ) = 0;

            virtual pkmn::e_species get_species() = 0;

            virtual pkmn::e_game get_game() = 0;

            virtual std::string get_form() = 0;

            virtual void set_form(
                const std::string& form
            ) = 0;

            virtual bool is_egg() = 0;

            virtual void set_is_egg(
                bool is_egg
            ) = 0;

            virtual const pkmn::database::pokemon_entry& get_database_entry() = 0;

            virtual pkmn::e_condition get_condition() = 0;

            virtual void set_condition(
                pkmn::e_condition condition
            ) = 0;

            virtual std::string get_nickname() = 0;

            virtual void set_nickname(
                const std::string& nickname
            ) = 0;

            virtual pkmn::e_gender get_gender() = 0;

            virtual void set_gender(
                pkmn::e_gender gender
            ) = 0;

            virtual bool is_shiny() = 0;

            virtual void set_shininess(
                bool value
            ) = 0;

            virtual pkmn::e_item get_held_item() = 0;

            virtual void set_held_item(
                pkmn::e_item held_item
            ) = 0;

            virtual pkmn::e_nature get_nature() = 0;

            virtual void set_nature(
                pkmn::e_nature nature
            ) = 0;

            virtual int get_pokerus_duration() = 0;

            virtual void set_pokerus_duration(
                int duration
            ) = 0;

            virtual std::string get_original_trainer_name() = 0;

            virtual void set_original_trainer_name(
                const std::string& trainer_name
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

            virtual pkmn::e_gender get_original_trainer_gender() = 0;

            virtual void set_original_trainer_gender(
                pkmn::e_gender trainer_gender
            ) = 0;

            virtual pkmn::e_language get_language() = 0;

            virtual void set_language(
                pkmn::e_language language
            ) = 0;

            virtual int get_current_trainer_friendship() = 0;

            virtual void set_current_trainer_friendship(
                int friendship
            ) = 0;

            virtual pkmn::e_ability get_ability() = 0;

            virtual void set_ability(pkmn::e_ability ability) = 0;

            virtual pkmn::e_ball get_ball() = 0;

            virtual void set_ball(
                pkmn::e_ball ball
            ) = 0;

            virtual int get_level_met() = 0;

            virtual void set_level_met(
                int level
            ) = 0;

            virtual std::string get_location_met(
                bool as_egg
            ) = 0;

            virtual void set_location_met(
                const std::string& location,
                bool as_egg
            ) = 0;

            virtual pkmn::e_game get_original_game() = 0;

            virtual void set_original_game(pkmn::e_game game) = 0;

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

            virtual const std::map<pkmn::e_marking, bool>& get_markings() = 0;

            virtual void set_marking(
                pkmn::e_marking marking,
                bool value
            ) = 0;

            virtual const std::map<std::string, bool>& get_ribbons() = 0;

            virtual void set_ribbon(
                const std::string& ribbon,
                bool value
            ) = 0;

            virtual const std::map<pkmn::e_contest_stat, int>& get_contest_stats() = 0;

            virtual void set_contest_stat(
                pkmn::e_contest_stat stat,
                int value
            ) = 0;

            virtual const pkmn::move_slots_t& get_moves() = 0;

            virtual void set_move(
                pkmn::e_move move,
                int index
            ) = 0;

            virtual void set_move_pp(
                int index,
                int pp
            ) = 0;

            virtual const std::map<pkmn::e_stat, int>& get_EVs() = 0;

            virtual void set_EV(
                pkmn::e_stat stat,
                int value
            ) = 0;

            virtual const std::map<pkmn::e_stat, int>& get_IVs() = 0;

            virtual void set_IV(
                pkmn::e_stat stat,
                int value
            ) = 0;

            virtual int get_current_hp() = 0;

            virtual void set_current_hp(
                int hp
            ) = 0;

            virtual const std::map<pkmn::e_stat, int>& get_stats() = 0;

            virtual std::string get_icon_filepath() = 0;

            virtual std::string get_sprite_filepath() = 0;

            #ifndef __DOXYGEN__
            pokemon(): class_with_attributes() {}
            virtual ~pokemon () {}
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
