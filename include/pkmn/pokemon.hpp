/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef INCLUDED_PKMN_POKEMON_HPP
#define INCLUDED_PKMN_POKEMON_HPP

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
            pokemon() {};
            virtual ~pokemon() {};
            #endif

            static sptr make(
                const std::string &species, const std::string &game,
                const int level,
                const std::string &move1, const std::string &move2,
                const std::string &move3, const std::string &move4
            );

            virtual std::string get_game() = 0;

            virtual std::string get_species() = 0;
            virtual std::string get_form() = 0;
            virtual std::string get_ability() = 0;
            virtual const pkmn::database::pokemon_entry& get_database_entry() = 0;

            virtual uint32_t get_personality() = 0;
            virtual int get_happiness() = 0;
            virtual std::string get_nature() = 0;
            virtual std::string get_gender() = 0;
            virtual bool is_egg() = 0;
            virtual bool is_shiny() = 0;

            virtual std::string get_nickname() = 0;
            virtual std::string get_trainer_name() = 0;
            virtual std::string get_trainer_gender() = 0;

            virtual uint32_t get_trainer_id() = 0;
            virtual uint16_t get_trainer_public_id() = 0;
            virtual uint16_t get_trainer_secret_id() = 0;

            virtual std::string get_original_game() = 0;
            virtual int get_level_met() = 0;
            virtual std::string get_location_met(
                bool as_egg
            ) = 0;
            virtual std::string get_ball() = 0;
            virtual std::string get_date_met(
                bool as_egg
            ) = 0;

            virtual int get_experience() = 0;
            virtual int get_level() = 0;

            virtual int get_pokerus_duration() = 0;

            virtual const std::map<std::string, int>& get_stats() = 0;
            virtual const std::map<std::string, int>& get_EVs() = 0;
            virtual const std::map<std::string, int>& get_IVs() = 0;
            virtual const std::map<std::string, int>& get_contest_stats() = 0;
            virtual const std::map<std::string, bool>& get_markings() = 0;
            virtual const std::map<std::string, bool>& get_ribbons() = 0;
            virtual const std::map<std::string, bool>& get_super_training_medals() = 0;

            virtual const pkmn::database::item_entry& get_held_item() = 0;
            virtual const pkmn::moveset_t& get_moves() = 0;

            virtual void set_form(
                const std::string &form
            ) = 0;

            virtual void set_ability(
                const std::string &ability
            ) = 0;

            virtual void set_personality(
                const uint32_t personality
            ) = 0;
            virtual void set_happiness(
                const int happiness
            ) = 0;
            virtual void set_nature(
                const std::string &nature
            ) = 0;
            virtual void set_gender(
                const std::string &gender
            ) = 0;
            virtual void set_shininess(
                bool value
            ) = 0;

            virtual void set_nickname(
                const std::string &nickname
            ) = 0;
            virtual void set_trainer_name(
                const std::string &trainer_name
            ) = 0;

            virtual void set_trainer_id(
                const uint32_t id
            ) = 0;
            virtual void set_trainer_public_id(
                const uint16_t public_id
            ) = 0;
            virtual void set_trainer_secret_id(
                const uint16_t secret_id
            ) = 0;
            virtual void set_trainer_gender(
                const std::string &gender
            ) = 0;

            virtual void set_original_game(
                const std::string &game
            ) = 0;
            virtual void set_level_met(
                const int level
            ) = 0;
            virtual void set_location_met(
                const std::string &location,
                bool as_egg
            ) = 0;
            virtual void set_ball(
                const std::string &ball
            ) = 0;
            virtual void set_date_met(
                const std::string &date,
                bool as_egg
            ) = 0;

            virtual void set_experience(
                const int experience
            ) = 0;
            virtual void set_level(
                const int level
            ) = 0;

            virtual void set_pokerus_duration(
                const int duration
            ) = 0;

            virtual void set_EV(
                const std::string &stat,
                const int EV
            ) = 0;
            virtual void set_IV(
                const std::string &stat,
                const int IV
            ) = 0;
            virtual void set_contest_stat(
                const std::string &stat,
                const int value
            ) = 0;
            virtual void set_marking(
                const std::string &marking,
                bool value
            ) = 0;
            virtual void set_ribbon(
                const std::string &ribbon,
                bool value
            ) = 0;
            virtual void set_super_training_medal(
                const std::string &super_training_medal,
                bool value
            ) = 0;

            virtual void set_held_item(
                const std::string &held_item
            ) = 0;
            virtual void set_move(
                const std::string &move,
                const int position
            ) = 0;

            #ifndef __DOXYGEN__
            virtual void* get_native_pc_pokemon() = 0;
            virtual void* get_native_party_data() = 0;
            #endif

            virtual const std::map<std::string, int>& get_attributes() = 0;
            virtual void set_attribute(
                const std::string &key,
                const int value
            ) = 0;
    };

    typedef std::vector<pkmn::pokemon::sptr> pokemon_list_t;
}
#endif /* INCLUDED_PKMN_POKEMON_HPP */
