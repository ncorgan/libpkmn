/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_CONVERSIONS_GEN5_CONVERSIONS_HPP
#define INCLUDED_PKMN_CONVERSIONS_GEN5_CONVERSIONS_HPP

#include <pksav/common/nds_pokemon.h>
#include <pksav/gen6/pokemon.h>

#include "libpkmgc_includes.hpp"

namespace pkmn { namespace conversions {

    void gen5_pc_pokemon_to_gen6(
        const pksav_nds_pc_pokemon_t* from,
        pksav_gen6_pc_pokemon_t* to
    );

    void gen5_party_pokemon_to_gen6(
        const pksav_nds_party_pokemon_t* from,
        pksav_gen6_party_pokemon_t* to
    );

}}

#endif /* INCLUDED_PKMN_CONVERSIONS_GEN5_CONVERSIONS_HPP */
