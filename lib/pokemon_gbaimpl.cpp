/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gbaimpl.hpp"
#include "database/id_to_index.hpp"
#include "database/index_to_string.hpp"

#include "pksav/party_data.hpp"

#include <pksav/common/stats.h>
#include <pksav/gba/text.h>
#include <pksav/math/endian.h>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <cstring>
#include <ctime>
#include <random>
#include <stdexcept>

#define GBA_PC_RCAST    reinterpret_cast<pksav_gba_pc_pokemon_t*>(_native_pc)
#define GBA_PARTY_RCAST reinterpret_cast<pksav_gba_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    pokemon_gbaimpl::pokemon_gbaimpl(
        int pokemon_index, int game_id,
        int move1_id, int move2_id,
        int move3_id, int move4_id,
        int level
    ): pokemon_impl(pokemon_index, game_id)
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gba_pc_pokemon_t);
        std::memset(_native_pc, 0, sizeof(pksav_gba_pc_pokemon_t));
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gba_pokemon_party_data_t);
        std::memset(_native_party, 0, sizeof(pksav_gba_pokemon_party_data_t));
        _our_party_mem = true;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        /*
         * Since move IDs are manually passed in, manually create the move slots with
         * full PP.
         */
        pkmn::database::move_entry move1(move1_id, game_id);
        pkmn::database::move_entry move2(move2_id, game_id);
        pkmn::database::move_entry move3(move3_id, game_id);
        pkmn::database::move_entry move4(move4_id, game_id);

        _moves.emplace_back(
            pkmn::move_slot(
                (pkmn::database::move_entry&&)move1_id,
                move1.get_pp(0)
            )
        );
        _moves.emplace_back(
            pkmn::move_slot(
                (pkmn::database::move_entry&&)move2_id,
                move2.get_pp(0)
            )
        );
        _moves.emplace_back(
            pkmn::move_slot(
                (pkmn::database::move_entry&&)move3_id,
                move3.get_pp(0)
            )
        );
        _moves.emplace_back(
            pkmn::move_slot(
                (pkmn::database::move_entry&&)move4_id,
                move4.get_pp(0)
            )
        );

        // TODO: Use PKSav PRNG after refactor merged in
        std::srand(std::time(NULL));
        GBA_PC_RCAST->personality = uint32_t(std::rand());
        GBA_PC_RCAST->ot_id.id = pksav_littleendian32(LIBPKMN_OT_ID);

        pksav_text_to_gba(
            boost::algorithm::to_upper_copy(
                _database_entry.get_species()
            ).c_str(),
            GBA_PC_RCAST->nickname,
            11
        );

        // TODO: language (should be enum in PKSav)

        pksav_text_to_gba(
            LIBPKMN_OT_NAME.c_str(),
            GBA_PC_RCAST->otname,
            8
        );

        _growth->species = pksav_littleendian16(uint16_t(pokemon_index));
        _growth->exp = pksav_littleendian32(uint32_t(
                           _database_entry.get_experience_at_level(level)
                       ));
        _growth->friendship = uint8_t(_database_entry.get_base_happiness());

        for(size_t i = 0; i < 4; ++i) {
            _attacks->moves[i] = pksav_littleendian16(uint16_t(
                                     _moves[i].move.get_move_id()
                                 ));
            _attacks->move_pps[i] = uint8_t(_moves[i].pp);
        }

        // TODO: Use PKSav PRNG after refactor merged in
        _effort->ev_hp    = uint32_t(std::rand());
        _effort->ev_atk   = uint32_t(std::rand());
        _effort->ev_def   = uint32_t(std::rand());
        _effort->ev_spd   = uint32_t(std::rand());
        _effort->ev_spatk = uint32_t(std::rand());
        _effort->ev_spdef = uint32_t(std::rand());

        _misc->met_location = 0xFF; // Fateful encounter

        _misc->origin_info = uint16_t(level);
        uint16_t game_index = uint16_t(pkmn::database::game_id_to_index(game_id));
        _misc->origin_info |= (game_index << PKSAV_GBA_ORIGIN_GAME_OFFSET);
        // TODO: ball

        _misc->iv_egg_ability = uint32_t(std::rand());
        _misc->iv_egg_ability &= ~PKSAV_GBA_EGG_MASK;
        if(GBA_PC_RCAST->personality % 2) {
            _misc->iv_egg_ability |= PKSAV_GBA_ABILITY_MASK;
        } else {
            _misc->iv_egg_ability &= ~PKSAV_GBA_ABILITY_MASK;
        }

        _misc->ribbons_obedience |= PKSAV_GBA_OBEDIENCE_MASK;

        set_level(level);
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        pksav_gba_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(pc->blocks.growth.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gba_pokemon_party_data_t);
        pksav::gba_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gba_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gba_pokemon_party_data_t*>(_our_pc_mem)
        );
        _our_party_mem = true;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gbaimpl::pokemon_gbaimpl(
        pksav_gba_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(
           pksav_littleendian16(party->pc.blocks.growth.species),
           game_id
       )
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Set block pointers
        _growth  = &GBA_PC_RCAST->blocks.growth;
        _attacks = &GBA_PC_RCAST->blocks.attacks;
        _effort  = &GBA_PC_RCAST->blocks.effort;
        _misc    = &GBA_PC_RCAST->blocks.misc;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    std::string pokemon_gbaimpl::get_nickname() {
        char nickname[11];
        pksav_text_from_gba(
            GBA_PC_RCAST->nickname,
            nickname,
            10
        );

        return std::string(nickname);
    }

    void pokemon_gbaimpl::set_nickname(
        const std::string &nickname
    ) {
        if(nickname.size() < 1 or nickname.size() > 10) {
            throw std::invalid_argument(
                      "The nickname length must be 0-10."
                  );
        }

        pksav_text_to_gba(
            nickname.c_str(),
            GBA_PC_RCAST->nickname,
            10
        );
    }

    std::string pokemon_gbaimpl::get_trainer_name() {
        char otname[8];
        pksav_text_from_gba(
            GBA_PC_RCAST->otname,
            otname,
            7
        );

        return std::string(otname);
    }

    void pokemon_gbaimpl::set_trainer_name(
        const std::string &trainer_name
    ) {
        if(trainer_name.size() < 1 or trainer_name.size() > 7) {
            throw std::invalid_argument(
                      "The trainer name name length must be 0-7."
                  );
        }

        pksav_text_to_gba(
            trainer_name.c_str(),
            GBA_PC_RCAST->otname,
            7
        );
    }

    uint16_t pokemon_gbaimpl::get_trainer_public_id() {
        return pksav_littleendian16(GBA_PC_RCAST->ot_id.pid);
    }

    uint16_t pokemon_gbaimpl::get_trainer_secret_id() {
        return pksav_littleendian16(GBA_PC_RCAST->ot_id.sid);
    }

    uint32_t pokemon_gbaimpl::get_trainer_id() {
        return pksav_littleendian32(GBA_PC_RCAST->ot_id.id);
    }

    void pokemon_gbaimpl::set_trainer_public_id(
        uint16_t public_id
    ) {
        GBA_PC_RCAST->ot_id.pid = pksav_littleendian16(public_id);
    }

    void pokemon_gbaimpl::set_trainer_secret_id(
        uint16_t secret_id
    ) {
        GBA_PC_RCAST->ot_id.sid = pksav_littleendian16(secret_id);
    }

    void pokemon_gbaimpl::set_trainer_id(
        uint32_t id
    ) {
        GBA_PC_RCAST->ot_id.id = pksav_littleendian32(id);
    }

    std::string pokemon_gbaimpl::get_location_caught() {
        return pkmn::database::location_index_to_name(
                   _misc->met_location,
                   _database_entry.get_game_id()
               );
    }

    void pokemon_gbaimpl::set_location_caught(
        const std::string &location
    ) {
        _misc->met_location = uint8_t(pkmn::database::location_name_to_index(
                                          location,
                                          _database_entry.get_game_id()
                                      ));
    }

    int pokemon_gbaimpl::get_experience() {
        return int(_growth->exp);
    }

    void pokemon_gbaimpl::set_experience(
        int experience
    ) {
        int max_experience = _database_entry.get_experience_at_level(100);

        if(experience < 0 or experience > max_experience) {
            throw std::out_of_range(
                      str(boost::format(
                              "experience: valid range 0-%d"
                          ) % max_experience)
                  );
        }

        _growth->exp = uint32_t(experience);
        GBA_PARTY_RCAST->level = uint8_t(_database_entry.get_level_at_experience(experience));

        _calculate_stats();
        _update_stat_map();
    }

    int pokemon_gbaimpl::get_level() {
        return int(GBA_PARTY_RCAST->level);
    }

    void pokemon_gbaimpl::set_level(
        int level
    ) {
        if(level < 1 or level > 100) {
            throw std::out_of_range(
                      "level: valid range 1-100"
                  );
        }

        GBA_PARTY_RCAST->level = uint8_t(level);
        _growth->exp = uint32_t(_database_entry.get_experience_at_level(level));

        _calculate_stats();
        _update_stat_map();
    }

    void pokemon_gbaimpl::_calculate_stats() {
        pksav::gba_pc_pokemon_to_party_data(
            _database_entry,
            reinterpret_cast<const pksav_gba_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gba_pokemon_party_data_t*>(_our_pc_mem)
        );
    }

    void pokemon_gbaimpl::_update_moves(
        int index
    ) {
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        pksav_littleendian16(_attacks->moves[index]),
                        _database_entry.get_game_id()
                    ),
                    pksav_littleendian16(_attacks->move_pps[index])
                );
                break;

            default:
                for(size_t i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gbaimpl::_update_EV_map() {
        _EVs["HP"]              = int(_effort->ev_hp);
        _EVs["Attack"]          = int(_effort->ev_atk);
        _EVs["Defense"]         = int(_effort->ev_def);
        _EVs["Speed"]           = int(_effort->ev_spd);
        _EVs["Special Attack"]  = int(_effort->ev_spatk);
        _EVs["Special Defense"] = int(_effort->ev_spdef);
    }

    void pokemon_gbaimpl::_update_IV_map() {
        uint8_t IV = 0;

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_HP,
            &IV
        );
        _IVs["HP"] = int(IV);

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_ATTACK,
            &IV
        );
        _IVs["Attack"] = int(IV);

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_DEFENSE,
            &IV
        );
        _IVs["Defense"] = int(IV);

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_SPEED,
            &IV
        );
        _IVs["Speed"] = int(IV);

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_SPATK,
            &IV
        );
        _IVs["Special Attack"] = int(IV);

        pksav_get_IV(
            &_misc->iv_egg_ability,
            PKSAV_STAT_SPDEF,
            &IV
        );
        _IVs["Special Defense"] = int(IV);
    }

    void pokemon_gbaimpl::_update_stat_map() {
        _stats["HP"]              = int(pksav_littleendian16(GBA_PARTY_RCAST->max_hp));
        _stats["Attack"]          = int(pksav_littleendian16(GBA_PARTY_RCAST->atk));
        _stats["Defense"]         = int(pksav_littleendian16(GBA_PARTY_RCAST->def));
        _stats["Speed"]           = int(pksav_littleendian16(GBA_PARTY_RCAST->spd));
        _stats["Special Attack"]  = int(pksav_littleendian16(GBA_PARTY_RCAST->spatk));
        _stats["Special Defense"] = int(pksav_littleendian16(GBA_PARTY_RCAST->spdef));
    }
}
