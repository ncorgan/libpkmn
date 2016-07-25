/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKSAV_CONVERSIONS_HPP
#define INCLUDED_PKMN_PKSAV_CONVERSIONS_HPP

#include <pksav/gen1/pokemon.h>
#include <pksav/gen2/pokemon.h>

namespace pksav {

    void gen1_pc_pokemon_to_gen2(
        const pksav_gen1_pc_pokemon_t* from,
        pksav_gen2_pc_pokemon_t* to
    );

    void gen1_party_pokemon_to_gen2(
        const pksav_gen1_party_pokemon_t* from,
        pksav_gen2_party_pokemon_t* to
    );

}

#endif /* INCLUDED_PKMN_PKSAV_CONVERSIONS_HPP */
