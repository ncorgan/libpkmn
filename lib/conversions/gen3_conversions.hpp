/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_CONVERSIONS_GEN3_CONVERSIONS_HPP
#define INCLUDED_PKMN_CONVERSIONS_GEN3_CONVERSIONS_HPP

#include <pksav/gba/pokemon.h>
#include <pksav/common/nds_pokemon.h>

#include "libpkmgc_includes.hpp"

namespace pkmn { namespace conversions {

    void gba_pc_pokemon_to_gcn(
        const pksav_gba_pc_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    );

    void gba_party_pokemon_to_gcn(
        const pksav_gba_party_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    );

    void gcn_pokemon_to_gba_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_pc_pokemon_t* to
    );

    void gcn_pokemon_to_gba_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_party_pokemon_t* to
    );

    void gba_pc_pokemon_to_gen4(
        const pksav_gba_pc_pokemon_t* from,
        pksav_nds_pc_pokemon_t* to,
        int from_game_id,
        int to_game_id
    );

    void gba_party_pokemon_to_gen4(
        const pksav_gba_party_pokemon_t* from,
        pksav_nds_party_pokemon_t* to,
        int from_game_id,
        int to_game_id
    );

}}

#endif /* INCLUDED_PKMN_CONVERSIONS_GEN3_CONVERSIONS_HPP */
