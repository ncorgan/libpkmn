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
#include <pksav/gen6/pokemon.h>

#include "libpkmgc_includes.hpp"

namespace pkmn { namespace conversions {

    // GBA

    void gba_pc_pokemon_to_gcn(
        const pksav_gba_pc_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    );

    void gba_party_pokemon_to_gcn(
        const pksav_gba_party_pokemon_t* from,
        LibPkmGC::GC::Pokemon* to
    );

    void gba_pc_pokemon_to_gen4(
        const pksav_gba_pc_pokemon_t* from,
        pksav_nds_pc_pokemon_t* to
    );

    void gba_party_pokemon_to_gen4(
        const pksav_gba_party_pokemon_t* from,
        pksav_nds_party_pokemon_t* to
    );

    void gba_pc_pokemon_to_gen5(
        const pksav_gba_pc_pokemon_t* from,
        pksav_nds_pc_pokemon_t* to
    );

    void gba_party_pokemon_to_gen5(
        const pksav_gba_party_pokemon_t* from,
        pksav_nds_party_pokemon_t* to
    );

    void gba_pc_pokemon_to_gen6(
        const pksav_gba_pc_pokemon_t* from,
        pksav_gen6_pc_pokemon_t* to
    );

    void gba_party_pokemon_to_gen6(
        const pksav_gba_party_pokemon_t* from,
        pksav_gen6_party_pokemon_t* to
    );

    // GCN

    void gcn_pokemon_to_gba_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_pc_pokemon_t* to
    );

    void gcn_pokemon_to_gba_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gba_party_pokemon_t* to
    );

    void gcn_pokemon_to_gen4_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_nds_pc_pokemon_t* to
    );

    void gcn_pokemon_to_gen4_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_nds_party_pokemon_t* to
    );

    void gcn_pokemon_to_gen5_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_nds_pc_pokemon_t* to
    );

    void gcn_pokemon_to_gen5_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_nds_party_pokemon_t* to
    );

    void gcn_pokemon_to_gen6_pc(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gen6_pc_pokemon_t* to
    );

    void gcn_pokemon_to_gen6_party(
        const LibPkmGC::GC::Pokemon* from,
        pksav_gen6_party_pokemon_t* to
    );

}}

#endif /* INCLUDED_PKMN_CONVERSIONS_GEN3_CONVERSIONS_HPP */
