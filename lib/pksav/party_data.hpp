/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_PARTY_DATA_HPP
#define INCLUDED_PKMN_PKSAV_PARTY_DATA_HPP

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>
#include <pksav/gba/pokemon.h>
#include <pksav/common/nds_pokemon.h>
#include <pksav/gen6/pokemon.h>

#include <pkmn/database/pokemon_entry.hpp>

namespace pksav {

    void gen1_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gen1_pc_pokemon_t* pc,
        pksav_gen1_pokemon_party_data_t* party_data_out
    );

    void gen2_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gen2_pc_pokemon_t* pc,
        pksav_gen2_pokemon_party_data_t* party_data_out
    );

    void gba_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gba_pc_pokemon_t* pc,
        pksav_gba_pokemon_party_data_t* party_data_out
    );

    void nds_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_nds_pc_pokemon_t* pc,
        pksav_nds_pokemon_party_data_t* party_data_out
    );

    void gen6_pc_pokemon_to_party_data(
        const pkmn::database::pokemon_entry &entry,
        const pksav_gen6_pc_pokemon_t* pc,
        pksav_gen6_pokemon_party_data_t* party_data_out
    );

}

#endif /* INCLUDED_PKMN_PKSAV_PARTY_DATA_HPP */
