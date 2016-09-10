/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "pokemon_gen1impl.hpp"

#include "pksav/party_data.hpp"

#include <pksav/common/stats.h>
#include <pksav/math/base256.h>
#include <pksav/math/endian.h>

#define GEN1_PC_RCAST    reinterpret_cast<pksav_gen1_pc_pokemon_t*>(_native_pc)
#define GEN1_PARTY_RCAST reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_native_party)

namespace pkmn {

    pokemon_gen1impl::pokemon_gen1impl(
        int pokemon_index, int game_id,
        int move1_id, int move2_id,
        int move3_id, int move4_id
    ): pokemon_impl(pokemon_index, game_id)
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen1_pc_pokemon_t);
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        _our_party_mem = true;

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
    }

    pokemon_gen1impl::pokemon_gen1impl(
        pksav_gen1_pc_pokemon_t* pc,
        int game_id
    ): pokemon_impl(pc->species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        pksav::gen1_pc_pokemon_to_party_data(
            reinterpret_cast<const pksav_gen1_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_our_pc_mem)
        );
        _our_party_mem = true;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    pokemon_gen1impl::pokemon_gen1impl(
        pksav_gen1_party_pokemon_t* party,
        int game_id
    ): pokemon_impl(party->pc.species, game_id)
    {
        _native_pc = reinterpret_cast<void*>(&party->pc);
        _our_pc_mem = false;

        _native_party = reinterpret_cast<void*>(&party->party_data);
        _our_party_mem = false;

        // Populate abstractions
        _update_EV_map();
        _update_IV_map();
        _update_stat_map();
        _update_moves(-1);
    }

    std::string pokemon_gen1impl::get_nickname() {
        return _nickname;
    }

    std::string pokemon_gen1impl::get_trainer_name() {
        return _trainer_name;
    }

    int pokemon_gen1impl::get_experience() {
        return int(pksav_from_base256(
                       GEN1_PC_RCAST->exp,
                       3
                   ));
    }

    int pokemon_gen1impl::get_level() {
        return int(GEN1_PARTY_RCAST->level);
    }

    void pokemon_gen1impl::_calculate_stats() {
        pksav::gen1_pc_pokemon_to_party_data(
            reinterpret_cast<const pksav_gen1_pc_pokemon_t*>(_native_pc),
            reinterpret_cast<pksav_gen1_pokemon_party_data_t*>(_our_pc_mem)
        );
    }

    void pokemon_gen1impl::_update_moves(
        int index
    ) {
        switch(index) {
            case 0:
            case 1:
            case 2:
            case 3:
                _moves[index] = pkmn::move_slot(
                    pkmn::database::move_entry(
                        GEN1_PC_RCAST->moves[index],
                        _database_entry.get_game_id()
                    ),
                    (GEN1_PC_RCAST->move_pps[index] & 0x3F) // PKSav TODO: define for this
                );
                break;

            default:
                for(size_t i = 0; i < 4; ++i) {
                    _update_moves(i);
                }
        }
    }

    void pokemon_gen1impl::_update_EV_map() {
        _EVs["HP"]      = int(pksav_bigendian16(GEN1_PC_RCAST->ev_hp));
        _EVs["Attack"]  = int(pksav_bigendian16(GEN1_PC_RCAST->ev_atk));
        _EVs["Defense"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_def));
        _EVs["Speed"]   = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spd));
        _EVs["Special"] = int(pksav_bigendian16(GEN1_PC_RCAST->ev_spcl));
    }

    void pokemon_gen1impl::_update_IV_map() {
        uint8_t IV = 0;

        pksav_get_gb_IV(
            &GEN1_PC_RCAST->iv_data,
            PKSAV_STAT_HP,
            &IV
        );
        _IVs["HP"] = int(IV);

        pksav_get_gb_IV(
            &GEN1_PC_RCAST->iv_data,
            PKSAV_STAT_ATTACK,
            &IV
        );
        _IVs["Attack"] = int(IV);

        pksav_get_gb_IV(
            &GEN1_PC_RCAST->iv_data,
            PKSAV_STAT_DEFENSE,
            &IV
        );
        _IVs["Defense"] = int(IV);

        pksav_get_gb_IV(
            &GEN1_PC_RCAST->iv_data,
            PKSAV_STAT_SPEED,
            &IV
        );
        _IVs["Speed"] = int(IV);

        pksav_get_gb_IV(
            &GEN1_PC_RCAST->iv_data,
            PKSAV_STAT_SPECIAL,
            &IV
        );
        _IVs["Special"] = int(IV);
    }

    void pokemon_gen1impl::_update_stat_map() {
        _stats["HP"]      = int(pksav_bigendian16(GEN1_PARTY_RCAST->max_hp));
        _stats["Attack"]  = int(pksav_bigendian16(GEN1_PARTY_RCAST->atk));
        _stats["Defense"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->def));
        _stats["Speed"]   = int(pksav_bigendian16(GEN1_PARTY_RCAST->spd));
        _stats["Special"] = int(pksav_bigendian16(GEN1_PARTY_RCAST->spcl));
    }
}
