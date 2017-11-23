/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "misc_common.hpp"
#include "pokemon_gen6impl.hpp"
#include "pokemon_gcnimpl.hpp"

#include "database/database_common.hpp"
#include "database/id_to_string.hpp"
#include "database/index_to_string.hpp"

#include "pksav/enum_maps.hpp"
#include "pksav/party_data.hpp"
#include "pksav/pksav_call.hpp"

#include "types/rng.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/form.hpp>
#include <pkmn/calculations/gender.hpp>
#include <pkmn/calculations/shininess.hpp>
#include <pkmn/database/move_entry.hpp>
#include <pkmn/utils/paths.hpp>

#include <pkmn/database/item_entry.hpp>

#include <pksav/common/markings.h>
#include <pksav/common/stats.h>
#include <pksav/gen6/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <ctime>
#include <fstream>
#include <stdexcept>

#define GEN6_PC_RCAST    (reinterpret_cast<pksav_gen6_pc_pokemon_t*>(_native_pc))
#define GEN6_PARTY_RCAST (reinterpret_cast<pksav_gen6_pokemon_party_data_t*>(_native_party))

namespace fs = boost::filesystem;

namespace pkmn
{
    pokemon_gen6impl::pokemon_gen6impl(
        pkmn::database::pokemon_entry&& database_entry,
        int level
    ): pokemon_impl(std::move(database_entry))
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gen6_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gen6_pokemon_party_data_t));
        _our_party_mem = true;

        (void)level;

        (void)_blockA;
        (void)_blockB;
        (void)_blockC;
        (void)_blockD;
    }

    pokemon_gen6impl::pokemon_gen6impl(
        pksav_gen6_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(
           //pksav_littleendian16(pc->blocks.growth.species),
           0,
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;
    }

    pokemon_gen6impl::pokemon_gen6impl(
        pksav_gen6_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           //pksav_littleendian16(party->pc.blocks.growth.species),
           0,
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;
    }

    pokemon_gen6impl::pokemon_gen6impl(
        const pksav_gen6_pc_pokemon_t &pc,
        int game_id
    ): pokemon_impl(
           //pksav_littleendian16(pc.blocks.growth.species),
           0,
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        *GEN6_PC_RCAST = pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        _populate_party_data();
        _our_party_mem = true;
    }

    pokemon_gen6impl::pokemon_gen6impl(
        const pksav_gen6_party_pokemon_t &party,
        int game_id
    ): pokemon_impl(
           //pksav_littleendian16(party.pc.blocks.growth.species),
           0,
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(new pksav_gen6_pc_pokemon_t);
        *GEN6_PC_RCAST = party.pc;
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_data_t);
        *GEN6_PARTY_RCAST = party.party_data;
        _our_party_mem = true;
    }

    pokemon_gen6impl::~pokemon_gen6impl()
    {
        if(_our_pc_mem)
        {
            delete GEN6_PC_RCAST;
        }
        if(_our_party_mem)
        {
            delete GEN6_PARTY_RCAST;
        }
    }

    pokemon::sptr pokemon_gen6impl::to_game(
        const std::string& game
    )
    {
        (void)game;

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        pkmn::pokemon::sptr ret;

        return ret;
    }

    void pokemon_gen6impl::export_to_file(
        const std::string& filepath
    )
    {
        (void)filepath;
    }

    void pokemon_gen6impl::set_form(
        const std::string &form
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _database_entry.set_form(form);
    }

    bool pokemon_gen6impl::is_egg()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return false;
    }

    void pokemon_gen6impl::set_is_egg(
        bool is_egg
    )
    {
        (void)is_egg; 

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    std::string pokemon_gen6impl::get_condition()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret = "None";

        return ret;
    }

    void pokemon_gen6impl::set_condition(
        const std::string& condition
    )
    {
        (void)condition;
    }

    std::string pokemon_gen6impl::get_nickname()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_nickname(
        const std::string &nickname
    )
    {
        pkmn::enforce_string_length(
            "Nickname",
            nickname,
            1,
            10
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    std::string pokemon_gen6impl::get_gender()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_gender(
        const std::string &gender
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)gender;
    }

    bool pokemon_gen6impl::is_shiny()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return false;
    }

    void pokemon_gen6impl::set_shininess(
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)value;
    }

    std::string pokemon_gen6impl::get_held_item()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_held_item(
        const std::string &held_item
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        // Make sure item is valid and holdable
        pkmn::database::item_entry item(
            held_item,
            get_game()
        );
    }

    std::string pokemon_gen6impl::get_trainer_name()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_trainer_name(
        const std::string &trainer_name
    )
    {
        pkmn::enforce_string_length(
            "Trainer name",
            trainer_name,
            1,
            7
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    uint16_t pokemon_gen6impl::get_trainer_public_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    uint16_t pokemon_gen6impl::get_trainer_secret_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    uint32_t pokemon_gen6impl::get_trainer_id()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_trainer_public_id(
        uint16_t public_id
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)public_id;
    }

    void pokemon_gen6impl::set_trainer_secret_id(
        uint16_t secret_id
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)secret_id;
    }

    void pokemon_gen6impl::set_trainer_id(
        uint32_t id
    )
    {
        (void)id;
    }

    std::string pokemon_gen6impl::get_trainer_gender()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_trainer_gender(
        const std::string &gender
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)gender;
    }

    int pokemon_gen6impl::get_friendship()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_friendship(
        int friendship
    )
    {
        pkmn::enforce_bounds("Friendship", friendship, 0, 255);

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    std::string pokemon_gen6impl::get_ability()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret;

        return ret;
    }

    void pokemon_gen6impl::set_ability(
        const std::string &ability
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)ability;
    }

    std::string pokemon_gen6impl::get_ball()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return "";
    }

    void pokemon_gen6impl::set_ball(
        const std::string &ball
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        // Try and instantiate an item_entry to validate the ball.
        (void)pkmn::database::item_entry(ball, get_game());
    }


    int pokemon_gen6impl::get_level_met()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_level_met(
        int level
    )
    {
        pkmn::enforce_bounds("Level met", level, 0, 100);
    }

    std::string pokemon_gen6impl::get_location_met(
        bool as_egg
    )
    {
        (void)as_egg;
        return "";
    }

    void pokemon_gen6impl::set_location_met(
        const std::string &location,
        bool as_egg
    )
    {
        (void)location;
        (void)as_egg;
    }

    std::string pokemon_gen6impl::get_original_game()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        std::string ret;

        return ret;
    }

    void pokemon_gen6impl::set_original_game(
        const std::string &game
    )
    {
        (void)game;
    }

    uint32_t pokemon_gen6impl::get_personality()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_personality(
        uint32_t personality
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)personality;
    }

    int pokemon_gen6impl::get_experience()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_experience(
        int experience
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        int max_experience = _database_entry.get_experience_at_level(100);
        pkmn::enforce_bounds("Experience", experience, 0, max_experience);

        _populate_party_data();
        _update_stat_map();
    }

    int pokemon_gen6impl::get_level()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_level(
        int level
    )
    {
        pkmn::enforce_bounds("Level", level, 1, 100);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        _populate_party_data();
        _update_stat_map();
    }

    void pokemon_gen6impl::set_IV(
        const std::string &stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)stat;
        (void)value;
    }

    void pokemon_gen6impl::set_marking(
        const std::string &marking,
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)marking;
        (void)value;
    }

    void pokemon_gen6impl::set_ribbon(
        const std::string &ribbon,
        bool value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(_ribbons.find(ribbon) == _ribbons.end())
        {
            throw std::invalid_argument("Invalid ribbon.");
        }

        (void)value;
    }

    void pokemon_gen6impl::set_contest_stat(
        const std::string &stat,
        int value
    )
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        (void)stat;
        (void)value;
    }

    void pokemon_gen6impl::set_move(
        const std::string &move,
        int index
    )
    {
        pkmn::enforce_bounds("Move index", index, 0, 3);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        // This will throw an error if the move is invalid.
        pkmn::database::move_entry entry(
            move,
            get_game()
        );
        (void)entry;
    }

    void pokemon_gen6impl::set_EV(
        const std::string &stat,
        int value
    )
    {
        pkmn::enforce_value_in_vector(
            "Stat",
            stat,
            pkmn::MODERN_STATS
        );
        pkmn::enforce_EV_bounds(stat, value, true);

        boost::lock_guard<pokemon_gen6impl> lock(*this);

        if(stat == "HP")
        {
        }
        else if(stat == "Attack")
        {
        }
        else if(stat == "Defense")
        {
        }
        else if(stat == "Speed")
        {
        }
        else if(stat == "Special Attack")
        {
        }
        else
        {
        }

        _update_EV_map();
        _populate_party_data();
    }

    int pokemon_gen6impl::get_current_hp()
    {
        boost::lock_guard<pokemon_gen6impl> lock(*this);

        return 0;
    }

    void pokemon_gen6impl::set_current_hp(
        int hp
    )
    {
        pkmn::enforce_bounds(
            "Current HP",
            hp,
            0,
            _stats["HP"]
        );

        boost::lock_guard<pokemon_gen6impl> lock(*this);
    }

    void pokemon_gen6impl::_set_contest_ribbon(
        const std::string &ribbon,
        bool value
    )
    {
        std::vector<std::string> ribbon_parts;
        boost::split(ribbon_parts, ribbon, boost::is_any_of(" "));

        (void)value;

        _update_ribbons_map();
    }

    void pokemon_gen6impl::_populate_party_data()
    {
        _update_stat_map();
    }

    void pokemon_gen6impl::_update_moves(
        int index
    )
    {
        _moves.resize(4);
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                break;

            default:
                for(int i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen6impl::_update_markings_map()
    {
        /*_markings["Circle"]   = bool(GEN6_PC_RCAST->markings & PKSAV_MARKING_CIRCLE);
        _markings["Triangle"] = bool(GEN6_PC_RCAST->markings & PKSAV_MARKING_TRIANGLE);
        _markings["Square"]   = bool(GEN6_PC_RCAST->markings & PKSAV_MARKING_SQUARE);
        _markings["Heart"]    = bool(GEN6_PC_RCAST->markings & PKSAV_MARKING_HEART);*/
    }

    void pokemon_gen6impl::_update_ribbons_map()
    {
    }

    void pokemon_gen6impl::_update_EV_map()
    {
        /*_EVs["HP"]              = int(_effort->ev_hp);
        _EVs["Attack"]          = int(_effort->ev_atk);
        _EVs["Defense"]         = int(_effort->ev_def);
        _EVs["Speed"]           = int(_effort->ev_spd);
        _EVs["Special Attack"]  = int(_effort->ev_spatk);
        _EVs["Special Defense"] = int(_effort->ev_spdef);*/
    }

    void pokemon_gen6impl::_update_stat_map()
    {
        /*_stats["HP"]              = int(pksav_littleendian16(GEN6_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(GEN6_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(GEN6_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(GEN6_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(GEN6_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(GEN6_PARTY_RCAST->spdef));*/
    }
}
