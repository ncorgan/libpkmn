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
        int pokemon_index,
        int game_id
    ): pokemon_impl(pokemon_index, game_id)
    {
        _native_pc  = reinterpret_cast<void*>(new pksav_gen1_pc_pokemon_t);
        _our_pc_mem = true;

        _native_party = reinterpret_cast<void*>(new pksav_gen1_pokemon_party_data_t);
        _our_party_mem = true;
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
